/* -------------------------------------------------------------------------- *
 * File: main.c
 * 
 * Author: Callum Nunes-Vaz
 * 
 * Date: 26th Feb 2016
 * 
 * Description:
 * Main routine for the VFD wristwatch.
 * -------------------------------------------------------------------------- */

/* ------------ C O N F I G U R A T I O N    R E G I S T E R S ------------- */

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)


/* ---------------------------- I N C L U D E S ---------------------------- */

#include <xc.h>
#include "i2c.h"
#include "menu_System.h"


/* ------------------------- D E F I N I T I O N S ------------------------- */
// Fosc  frequency for _delay()  library
#define _XTAL_FREQ 16000000   

#define byte unsigned char

// Pin definitions
#define BATTERY_VDIV PORTCbits.RC3
#define SWITCH2 PORTCbits.RC6
#define SWITCH1 PORTAbits.RA0
#define LDR PORTCbits.RC7 
#define LDR_EN LATBbits.LATB7
#define LED LATAbits.LATA1
#define INT PORTAbits.RA2
#define DS3231M_RST LATCbits.LATC1
#define SDA PORTBbits.RB4
#define SCL PORTBbits.RB6

/* --------------------------- P R O T O T Y P E S ------------------------- */

/* Main initilisation routine that handles simple configuration and calls 
 * other initilisation routines */
void init(void);
/* The Filament is a H-bridge driven by the CCP2 module in half-bridge PWM mode
 * on pins RA4 and RA5. This allows the brightness across the display to be 
 * more even. */
void init_Filament_Driver(void);
/* Initialise the PICs i2c module */
void init_I2C(void);
/* Initialising the three timers used to manage various functions of the watch:
 * Timer 1: The ~2-second display timeout
 * Timer 2: The delay between sending symbols to VFD (1kHz)
 * Timer 4: The filament H-bridge Switcher, and vreg enable pin CCP PWM timer (250 Hz) 
 * {Fosc/4 = 4MHz} */
void init_Timers(void);
/* Flash the LED twice. Duration of 150ms */
void flash_LED(void);
/* Analogue read of light level from LDR , and return level between 1 and 100 of 
 * light level */
byte light_Level_Percent(void);

/* ----------------------- M A I N     R O U T I N E ----------------------- */

int main(void) {
    init();
    while(SWITCH1);
    while (1) {
        if (!SWITCH1) {
            while (!SWITCH1);
            display_Current_Menu();
        }
    }
}

/* ------------------------- S U B R O U T I N E S ------------------------- */

/* Main initilisation routine that handles simple configuration and calls 
 * other initilisation routines */
void init(void) {
    // configure internal oscillator
    OSCCON = 0b01111000; // 4x PLL disabled, 16 MHz clock, Clock determined by Fosc<2:0> word
    
    OPTION_REGbits.INTEDG = 1; // set external interrupt for rising edge

    // I/O
    TRISA = 0b00000101; // 1Hz input, switch 1 input
    TRISB = 0;
    TRISC = 0b01000000; // switch 2 input

    // A/D
    ANSELA = 0; // all digital
    ANSELB = 0; //...
    ANSELC = 0b10001000; // LDR and Battery measurements

    // interrupts
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    //INTCONbits.IOCIE = 1; // enable interrupt-on-change (for button presses while on)
    //INTCONbits.INTE = 1; // enable external interrupt on 'int' pin (to wake device)

    // Other intialisation routines
    init_Menu_System();
}