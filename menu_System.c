/*
 * File:   menu_System.c
 * Author: Callum
 *
 * Created on 1 February 2016, 3:22 PM
 */

#include "menu_System.h"

/* ------------------- G L O B A L     V A R I A B L E S  ------------------ */

byte CURRENT_MENU; // the currently selected menu, either time or settings
int menu[4];

// list of functions for acting on switch 2 press for menu index of 1 to 12
void (*switch2_func_ptr[12])(void) = {inc_Minutes, inc_Hours, inc_Days, 
                                inc_Months, inc_Years, inc_Brightness_EEPROM, 
                                inc_Tilt_Angle_EEPROM, inc_Tilt_Range_EEPROM, 
                                toggle_Hr_Mode_EEPROM, inc_Timeout_EEPROM, 
                                toggle_Orientation_EEPROM, toggle_Light_Sensing_EEPROM};

/* ------------------------- S U B R O U T I N E S ------------------------- */

/* Check if buttons were pressed if at all using extremely simple de-bouncing routine
 * 0=none, 1=switch1, 2=switch2, 3=both */
byte button_State(void) {
    // declare variables
    byte state = 0, i, s1_States = 0, s2_States = 0;
    // every 50us get the states of the switches, 15 times
    for (i = 0; i < 20; i++) {
        s1_States += SWITCH1;
        s2_States += SWITCH2;
        __delay_us(50);
    }
    // if a button was registered low enough times, say it was pressed (low)
    if (s1_States < 10) state += 1;
    if (s2_States < 10) state += 2;
    if (state & 2) __delay_ms(250);
    return (state & 3);
}

void display_Animation(void) {
    byte i;
    char count = 0;
    
    close_Display();
    int menu_Prev[4], menu_Now[4], render[4];
    for (i = 0; i < 4; i++)
        menu_Prev[i] = menu[i]; // grab old menu
    next_Menu(); // go to next menu
    load_Menu(0); // render it for global access
    for (i = 0; i < 4; i++)
        menu_Now[i] = menu[i]; // grab new menu
    
    for (i = 0; i < 4; i++)
                render[i] = shift_Segments(menu_Prev[i], 0 - (count))
                + shift_Segments(menu_Now[i], 4 - (count));
    open_Display();
    while (count <= 4) { // count increments every 0.125s
        for (i = 0; i < 4; i++) send_Code(render[i]+(grid_Codes[POS])[i]);
        // check the timer
        if (PIR1bits.TMR1IF) {
            count++;
            PIR1bits.TMR1IF = 0;
            for (i = 0; i < 4; i++)
                render[i] = shift_Segments(menu_Prev[i], 0 - (count))
                + shift_Segments(menu_Now[i], 4 - (count));
        }
        
    }
}

/* Display the current menu system, and handle the timeout for exiting this 
 * function. */
void display_Current_Menu(void) {
    // declare variables
    byte button, count_Max, count, i;
    /* set the max count for the iterations of timer1 from the user-set 
     * variable in eeprom. */
    count = 0;
    count_Max = (bcd_To_Byte(get_Timeout_EEPROM()) / 5)*4;
    // set up the display
    open_Display();
    // enter loop for 1-6 seconds (depending on settings)
    while (count <= count_Max) { // count increments every 0.125s
        // check the buttons
        if ((!SWITCH1 || !SWITCH2) && count) { // if switches have been pressed
            close_Display(); // stop the last digit drawn from displaying
            button = button_State(); // get the current state of the buttons
            switch (button) {
                case 1:
                    while (!SWITCH1); // wait till switch 1 is free
                    open_Display(); // re-enable anode and cathode driving
                    if (CURRENT_MENU >= 2) display_Animation();
                    else next_Menu();
                    break;
                case 2:
                    perform_Switch2_Action();                    
                    break;
            }
            count = 0; // reset the count variable
            open_Display(); // re-enable anode and cathode driving
        }
        // check the timer
        if (PIR1bits.TMR1IF) {
            count++;
            PIR1bits.TMR1IF = 0;
            TMR1 = 3036; /// remaining time = 0.125s
            load_Menu(count & 0x04); // reload current menu
        }
        // display menu
        for (i = 0; i < 4; i++) send_Code(menu[i]+(grid_Codes[POS])[i]);
        if (CURRENT_MENU <= 2) display_Dots();
    }
    close_Display();
    reset_Menu(); // Reset current menu to time for next call of function
}

/* Run the initialisation for the menu system and its dependencies */
void init_Menu_System(void) {
    init_Timers();
    init_I2C();
    init_mma8451q();
    reset_Menu();
    init_Filament_Driver();
}

void init_Timers(void) {
    /* Timer 1 configured for 0.131072-second overflow, this is altered where it 
     * is used to make 0.125 seconds */
    T1CON = 0b00110000; // clk src is Fosc/4 (4MHz), 1:8 PRE, Do not sync, OFF
    T1GCON = 0; // not using gate enable
    PIR1bits.TMR1IF = 0;

    T2CON = 2; // 1:1 POST, TMR2 OFF, 1:16 PRE
    PR2 = 249; // 0.001 timing (1kHz)
    PIR1bits.TMR2IF = 0;

    T4CON = 3; // 1:1 POST, TMR4 OFF, 1:64 PRE
    PR4 = 249; // 0.004 timing (250Hz)
    PIR3bits.TMR4IF = 0;

    T6CON = 2; // 1:1 POST, TMR6 OFF, 1:16 PRE
    PR6 = 1; // 0.001 timing (1kHz)
    PIR3bits.TMR6IF = 0;
}

/* THIS USES 30% OF PROGRAM MEMORY!!!*/
void load_Menu(byte flash) {
    signed char i;
    byte temp;
    // reset Array
    for (i = 3; i >= 0; i--) menu[i] = 0;
    // assign new contents
    if (CURRENT_MENU <= 2) { // if displaying time or time-correction
        for (i = 3; i >= 1; i--)
            menu[i] = (num_Codes[POS])[(get_Time()>>(4 * (3 - i)))&0x0F];
        // special case for first digit in 12H mode (it sets AM/PM and 12H bits)
        menu[0] = (num_Codes[POS])[(get_Hours()>>4)&(1+(get_Hr_Mode_EEPROM()>>5)<<1)];
        if (menu[0] == (num_Codes[POS])[0]) // remove leading zero's
            menu[0] = 0;
        if (flash && CURRENT_MENU) { // if correcting, flash portion to correct
            for (i = (5 - 2 * CURRENT_MENU); i >= (4 - 2 * CURRENT_MENU); i--)
                menu[i] = (seg_Codes[POS])[SEGMENT_D];
        }
    } else { // else display settings/data stored either in DS3231 or EEPROM
        // gather data
        if (CURRENT_MENU <= 5) temp = read_DS3231_byte(CURRENT_MENU + 1);
        if (CURRENT_MENU == 6) temp = read_MMA8451Q_Byte(WHO_AM_I) - 0x88;
        if (CURRENT_MENU >= 7) temp = eeprom_read(CURRENT_MENU);
        if (CURRENT_MENU == ANGLE_LIVE) temp = get_Tilt();
        // display constant letters
        menu[0] = (letter_Codes[POS])[menu_Letters_GRID1[CURRENT_MENU]];
        menu[1] = (letter_Codes[POS])[menu_Letters_GRID2[CURRENT_MENU]];
        // display variable letters and numbers for each menu
        if (CURRENT_MENU <= TIMEOUT || CURRENT_MENU == ANGLE_LIVE) {
            menu[2] = (num_Codes[POS])[temp >> 4];
            menu[3] = (num_Codes[POS])[temp & 0x0F];
        }
        if (CURRENT_MENU == ORIENT)
            menu[0] = (letter_Codes[POS])[LETTER_L + temp * 5];
        if (CURRENT_MENU == LIGHT_SCALING) {
            menu[2] = (letter_Codes[POS])[LETTER_o];
            menu[3] = (letter_Codes[POS])[LETTER_F + temp * 5];
        }
        if (CURRENT_MENU == DATE) {
            menu[0] = (num_Codes[POS])[get_Day() >> 4];
            menu[1] = (num_Codes[POS])[get_Day()&0x0F];
            menu[2] = (num_Codes[POS])[get_Month() >> 4];
            menu[3] = (num_Codes[POS])[get_Month()&0x0F];
        }
    }
}

/* Go to the next menu in the sequence, retreive data for it */
void next_Menu(void) {
    // go to next menu in sequence
    if (CURRENT_MENU >= ANGLE_LIVE) CURRENT_MENU = TIME; //back to start
    else CURRENT_MENU++; // set menu to next in list
    // load the working byte global variable
    load_Menu(0);
}

/* Switch 2 will perform a different role depending on the menu it is in */
void perform_Switch2_Action(void) {
    byte temp;
    if (CURRENT_MENU == TIME || CURRENT_MENU == DATE) return;
    (*switch2_func_ptr[CURRENT_MENU - 1])();
    load_Menu(0); // load the working byte global variable
}

/* Reset the menu to the main menu (the time) */
void reset_Menu(void) {
    CURRENT_MENU = TIME;
    load_Menu(0);
}
