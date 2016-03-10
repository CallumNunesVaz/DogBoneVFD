/* -------------------------------------------------------------------------- *
 * File: menu_System.h
 * 
 * Author: Callum Nunes-Vaz
 * 
 * Date: 26th Feb 2016
 * 
 * Description:
 * A library for managing the menu system of a wristwatch that draws letters,
 * digits, and symbols on the 4 7-segment sections. This library also manages 
 * the button presses and their actions.
 * -------------------------------------------------------------------------- */

#ifndef MENU_SYSTEM_H
#define	MENU_SYSTEM_H

#include <xc.h>
#include "data.h"
#include "display.h"
#include "data.h"
#include "mma8451.h"
#include "ds3231.h"

/* ------------------------- D E F I N I T I O N S ------------------------- */

// Fosc  frequency for _delay()  library
#define _XTAL_FREQ 16000000   

#define byte unsigned char

#define GRID1 0
#define GRID2 1
#define GRID3 2
#define GRID4 3

// Menu system definitions 
#define TIME 0 
#define MINS 1
#define HOURS 2
#define DAYS 3
#define MONTHS 4
#define YEAR 5 
#define MAX_BRIGHT 6 
#define VIEW_ANGLE 7
#define ANGLE_ERROR 8
#define HOUR_MODE 9
#define TIMEOUT 10
#define ORIENT 11
#define LIGHT_SCALING 12
#define ANGLE_LIVE 13
#define DATE 14

typedef enum HR_MODE {
    MODE_12H, MODE_24H
} hr_Mode;

/* Orientation modes for handling user-selected orientation of display */
typedef enum ORIENTATION_MODE {
    LEFT_HANDED, RIGHT_HANDED
} orientation_Mode;

/* ------------------- G L O B A L     V A R I A B L E S  ------------------ */

hr_Mode HR_MODE;
orientation_Mode POS;

const byte menu_Letters_GRID1[] = {19,19,19,3,10,18,1,0,4,6,16,19,9,0,3};
const byte menu_Letters_GRID2[] = {19,19,19,19,10,14,14,10,14,14,11,6,19,19,3};

/* --------------------------- P R O T O T Y P E S ------------------------- */
byte button_State(void);
void display_Current_Menu(void);
void init_Menu_System(void);
void init_Timers(void);
void load_Brightness(void);
void load_Brightness(void);
void load_Orientation(void);
void load_Hr_Mode(void);
void next_Menu(void);
void load_Menu(byte count);
void perform_Switch2_Action(void);
void reset_Menu(void);
int shift_Segments(int code_In, signed char dir);
void update_Local_Settings(void);

#endif

