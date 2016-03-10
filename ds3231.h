/* -------------------------------------------------------------------------- *
 * File: ds3231.h
 * 
 * Author: Callum Nunes-Vaz
 * 
 * Date: 26th Feb 2016
 * 
 * Description:
 * A simple library for accessing the data from the ds3231 real-time clock, as
 * well as setting it. Included are functions to increment the units for each 
 * register and toggle between 12 and 24 hour modes.
 * -------------------------------------------------------------------------- */

#ifndef DS32231_H
#define	DS32231_H

#include <xc.h> 
#include "i2c.h" // custom i2c library for pic16F1828
#include "data.h" // custom library for data conversion

#define byte unsigned char

#define DS3231_ADDR 0xD0

#define REG_DS3231_SECONDS 0x00
#define REG_DS3231_MINUTES 0x01
#define REG_DS3231_HOURS 0x02
#define REG_DS3231_DAY 0x03
#define REG_DS3231_DATE 0x04
#define REG_DS3231_MONTH_CENTURY 0x05
#define REG_DS3231_YEAR 0x06
#define REG_DS3231_A1_SECONDS 0x07
#define REG_DS3231_A1_MINUTES 0x08
#define REG_DS3231_A1_HOURS 0x09
#define REG_DS3231_A1_DAY_DATE 0x0A
#define REG_DS3231_A2_MINUTES 0x0B
#define REG_DS3231_A2_HOURS 0x0C
#define REG_DS3231_A2_DAY_DATE 0x0D
#define REG_DS3231_CONTROL 0x0E
#define REG_DS3231_STATUS 0x0F
#define REG_DS3231_AGING_OFFSET 0x10
#define REG_DS3231_TEMP_H 0x11
#define REG_DS3231_TEMP_L 0x12


byte get_Day(void);
byte get_Hours(void);
byte get_Minutes(void);
byte get_Month(void);
int get_Time(void);
byte get_Year(void);
void inc_Days(void);
void inc_Hours(void);
void inc_Minutes(void);
void inc_Months(void);
void inc_Years(void);
void init_ds3231(void);
byte read_Ds3231_Byte(byte reg);
void set_Minutes(byte mins);
void set_Days(byte days);
void set_Hours(byte hours);
void set_Month(byte months);
void set_Year(byte years);
void write_Ds3231_Byte(byte data, byte reg);

#endif

