#ifndef DS32231_H
#define	DS32231_H

#include <xc.h> 
#include "i2c.h"
#include "data.h"

#define byte unsigned char

#define DS3231_ADDR 0xD0

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
byte read_DS3231_byte(byte reg);
void set_Minutes(byte mins);
void set_Days(byte days);
void set_Hours(byte hours);
void set_Month(byte months);
void set_Year(byte years);
void write_DS3231_byte(byte data, byte reg);

#endif

