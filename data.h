/* -------------------------------------------------------------------------- *
 * File: data.h
 * 
 * Author: Callum Nunes-Vaz
 * 
 * Date: 26th Feb 2016
 * 
 * Description:
 * Simple Library for managing the data that the wristwatch needs to operate.
 * This includes get and set methods for the EEPROM-stored, as well as standard 
 * byte to 4-bit BCD byte conversion.
 * -------------------------------------------------------------------------- */

#ifndef DATA_H
#define	DATA_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "i2c.h"

/* ------------------------- D E F I N I T I O N S ------------------------- */

// definitions for code simplification 
#define byte unsigned char

// EEPROM Addresses
#define MAX_BRIGHT_EEPROM_ADDR 0x06
#define TILT_ANG_EEPROM_ADDR 0x07
#define TILT_ERR_EEPROM_ADDR 0x08
#define HR_MODE_EEPROM_ADDR 0x09
#define TIMEOUT_EEPROM_ADDR 0x0A
#define ORIENTATION_EEPROM_ADDR 0x0B
#define LIGHT_SCALE_EEPROM_ADDR 0x0C

#define LDR_EN LATBbits.LATB7


/* --------------------------- P R O T O T Y P E S ------------------------- */

int battery_Level(void);

byte bcd_To_Byte(byte data);

byte byte_To_BCD(byte data);

byte read_Ds3231_Byte(byte reg);

byte get_Timeout_EEPROM(void);

byte get_Hr_Mode_EEPROM(void);

byte get_Orientation_EEPROM(void);

void inc_Brightness_EEPROM(void);

void inc_Tilt_Angle_EEPROM(void);

void inc_Tilt_Range_EEPROM(void);

void inc_Timeout_EEPROM(void);

void toggle_Hr_Mode_EEPROM(void);

void toggle_Light_Sensing_EEPROM(void);

void toggle_Orientation_EEPROM(void);

#endif

