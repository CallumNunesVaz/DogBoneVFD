/* -------------------------------------------------------------------------- *
 * File: display.c
 * 
 * Author: Callum Nunes-Vaz
 * 
 * Date: 26th Feb 2016
 * 
 * Description:
 * A library for operating the IVL2-7/5 vacuum fluorescent display via the 
 * MAX6920 display driver as laid out on a custom PCB.  
 * -------------------------------------------------------------------------- */

#include "display.h"

// Fosc  frequency for _delay()  library
#define _XTAL_FREQ 16000000

/* ------------------------- S U B R O U T I N E S ------------------------- */

/* Set up the display for other functions to manipulate it */
void open_Display(void) {
    VREG_EN = 1; // enable 2.5V and 20V voltage regulators
    MAX6920_BLANK = 0; // allow the anode driver to power the anodes
    // reset the timer values
    TMR1 = 0;
    TMR2 = 0;
    TMR4 = 0;
    TMR6 = 0;
    // Turn on the timers
    T1CONbits.TMR1ON = 1; // turn on  0.131072s timer for display-timeout (see below)
    T2CONbits.TMR2ON = 1; // turn on timer for send_Code() call
    T4CONbits.TMR4ON = 1; // Turn on timer that handles H-Bridge switching
    T6CONbits.TMR6ON = 1; // turn on matching timer of T2 for Vreg control
}

/* Perform closing functions to stop functions using the display */
void close_Display(void) {
    T1CONbits.TMR1ON = 0; // turn off 0.131072s timer for display-timeout (see below)
    T2CONbits.TMR2ON = 0; // turn off 1kHz timer for display_Digits/Dots/Letters() calls 
    T4CONbits.TMR4ON = 0; // Turn off timer that handles H-Bridge switching
    T6CONbits.TMR6ON = 0; // turn on matching timer of T2 for Vreg control
    MAX6920_BLANK = 1; // disable the anode driver from powering the anodes
    VREG_EN = 0; // disable 2.5V and 20V voltage regulators
}

/* send code to MAX6920 to display the two dots in the middle of the display 
 * This takes approximately 1ms to complete */
void display_Dots(void) {
    // send code to MAX6920
    send_Code(128);
}

/* The Filament is a H-bridge driven by the CCP2 module in half-bridge PWM mode
 * on pins RA4 and RA5. This allows the brightness across the display to be 
 * more even. */
void init_Filament_Driver(void) {
    byte temp;
    APFCON1bits.CCP2SEL = 1; // CCP2/P2A module tied to RA5
    APFCON1bits.P2BSEL = 1; // P2B module tied to RA4
    CCP2CON = 0b10001100; // half-bridge-PWM-mode, PxA and PxB active-high
    temp = CCPTMRS0;
    CCPTMRS0 = (temp & 243) + 4; // CCP2 mapped to timer 4
    CCP2AS = 0; // Auto-shutdown disabled
    PWM2CON = 0x0F; // Restart disabled, 15*(Fosc/4) delay between pins
    PSTR2CON = 0; // PWM steering disabled  (UNUSED)
    CCPR2L = 0b01111101; // with LSB in CCP2CON this makes 500 (2*PR4=50% duty)
    CCPR2H = 0;
}

/* Routine for sending 12-bit numbers to the MAX6920 shift register, According 
 * to the datasheet the MAX6920 min width of pulses in 100ns at max (~11MHz) so 
 * Fosc/4 = 4MHz should be fine without nops for MAX6920 logic inputs. 
 * Additionally this routine will wait for 1ms between calls by polling timer 2 
 * and timer 6 which are configured for 1kHz operation. 
 */
void send_Code(unsigned int code) {
    // declare variables
    byte i;
    // push code to MAX6920 shift register bit by bit
    for (i = 0; i < 12; i++) {
        MAX6920_CLK = 0; // clock low
        MAX6920_DIN = code & 1; // set data input as current LSB of code
        MAX6920_CLK = 1; // clock high to add data to MAX6920
        code >>= 1; // shift code bits down, ready to set next bit
    }
    // Latch code to display
    MAX6920_LOAD = 1;
    NOP();
    MAX6920_LOAD = 0;
    
    // timer flag polling for timing between calls
    while (!PIR3bits.TMR6IF);
    PIR3bits.TMR6IF = 0; // reset flag
    MAX6920_BLANK = 1; // disable anode controller
    // wait till 1ms has passed between calls of this function
    while (!PIR1bits.TMR2IF);
    PIR1bits.TMR2IF = 0; // reset flag
    // re-enable display drivers...
    MAX6920_BLANK = 0; // enable anode controller
    TMR6 = 0;
    T6CONbits.TMR6ON = 1; // turn on timer 6
}