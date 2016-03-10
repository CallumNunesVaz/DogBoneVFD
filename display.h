/* -------------------------------------------------------------------------- *
 * File: display.h
 * 
 * Author: Callum Nunes-Vaz
 * 
 * Date: 26th Feb 2016
 * 
 * Description:
 * A library for operating the IVL2-7/5 vacuum fluorescent display via the 
 * MAX6920 display driver.  
 * -------------------------------------------------------------------------- */


#ifndef DISPLAY_H
#define	DISPLAY_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "data.h"

/* ------------------------- D E F I N I T I O N S ------------------------- */

// Fosc  frequency for _delay()  library
#define _XTAL_FREQ 16000000   

// definitions for code simplification 
#define byte unsigned char
#define DISPLAY_NOTHING send_Code(0)

// Letter addresses for Letter_Codes[]
#define LETTER_A 0
#define LETTER_b 1
#define LETTER_C 2
#define LETTER_d 3
#define LETTER_E 4
#define LETTER_F 5
#define LETTER_H 6
#define LETTER_i 7
#define LETTER_J 8
#define LETTER_L 9
#define LETTER_n 10
#define LETTER_o 11
#define LETTER_O 12
#define LETTER_P 13
#define LETTER_r 14
#define LETTER_S 15
#define LETTER_t 16
#define LETTER_U 17
#define LETTER_y 18
#define NOTHING 19

// Segment Addresses for Segment_Codes[]
#define SEGMENT_A 0
#define SEGMENT_B 1
#define SEGMENT_C 2
#define SEGMENT_D 3
#define SEGMENT_E 4
#define SEGMENT_F 5
#define SEGMENT_G 6

// definitions for display_Time() function parameters
#define COMPLETE 0
#define ONLY_HOURS 1
#define ONLY_MINS 2

// Pin definitions
#define MAX6920_CLK LATCbits.LATC2
#define MAX6920_BLANK LATCbits.LATC4
#define MAX6920_DIN LATCbits.LATC0
#define MAX6920_LOAD LATBbits.LATB5
#define HBridge_A LATAbits.LATA5
#define HBridge_B LATAbits.LATA4
#define VREG_EN LATCbits.LATC5
#define SWITCH2 PORTCbits.RC6
#define SWITCH1 PORTAbits.RA0

/* ---------------------- S E G M E N T    C O D E S ----------------------- */

const unsigned int seg_Codes_RH[] = {1, 512, 8, 16, 4, 2048, 256};
const unsigned int seg_Codes_LH[] = {16, 4, 2048, 1, 512, 8, 256};

const unsigned int grid_Codes_RH[] = {32, 2, 1024, 64};
const unsigned int grid_Codes_LH[] = {64, 1024, 2, 32};

const unsigned int num_Codes_RH[] = {2589,520,789,793,2824,2329,2333,521,2845,2841};
const unsigned int num_Codes_LH[] = {2589,2052,789,2325,2316,2329,2841,2068,2845,2333};

const unsigned int letter_Codes_RH[] = {2829, 2332, 2069, 796, 2325, 2309, 2828, 
            8, 536, 2068, 268, 284, 2589, 2821, 260, 2329, 2324, 2588, 2840, 0};
const unsigned int letter_Codes_LH[] = {2844, 2825, 537, 2821, 793, 792, 2828, 
         2048, 2053, 521, 2816, 2817, 2589, 796, 768, 2329, 777, 2573, 2317, 0};

const unsigned int shift_UpRH_DownLH[] = {0,2,2048,512,256,32,64,128,1,0,1024,0};     
const unsigned int shift_DownRH_UpLH[] = {256,2,0,0,0,32,64,128,16,8,1024,4};
               
const unsigned int *seg_Codes[] = {seg_Codes_LH, seg_Codes_RH};
const unsigned int *grid_Codes[] = {grid_Codes_LH, grid_Codes_RH};
const unsigned int *num_Codes[] = {num_Codes_LH, num_Codes_RH};
const unsigned int *letter_Codes[] = {letter_Codes_LH, letter_Codes_RH};
const unsigned int *shift_Codes[] = {shift_UpRH_DownLH, shift_DownRH_UpLH};


/* --------------------------- P R O T O T Y P E S ------------------------- */

void open_Display(void);

void close_Display(void);

void display_Current_Menu(void);

void display_Dots(void);

void display_Time(unsigned int time, byte mode);

void init_Filament_Driver(void);

void send_Code(unsigned int code);

#endif

