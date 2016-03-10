/* -------------------------------------------------------------------------- *
 * File: ds3231.c
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

#include "ds3231.h"

/* ------------------------- S U B R O U T I N E S ------------------------- */

/* Get the current date from the DS3231 clock */
byte get_Day(void) {
    return read_Ds3231_Byte(REG_DS3231_DAY);
}

/* Retrieve the hours portion (byte) of the time from the DS3231M real time 
 * clock over i2C and don't assign to the 'time' variable */
byte get_Hours(void) {
    return read_Ds3231_Byte(REG_DS3231_HOURS);
}

/* retrieve minutes byte from clock */
byte get_Minutes(void) {
    return read_Ds3231_Byte(REG_DS3231_MINUTES);
}

/* Get the current date from the DS3231 clock */
byte get_Month(void) {
    return read_Ds3231_Byte(REG_DS3231_MONTH_CENTURY);
}

/* retrieve time over I2C in BCD from the DS3231M real-time clock module */
int get_Time(void) {
    int time;
    time = get_Hours();
    time <<= 8;
    time += get_Minutes();
    return time;
}

/* retrieve minutes byte from clock */
byte get_Year(void) {
    return read_Ds3231_Byte(REG_DS3231_YEAR);
}

/* increment the hours on the DS3231M Clock */
void inc_Days(void) {
    byte day, month, year, lim;
    year = get_Year();
    month = get_Month();
    day = bcd_To_Byte(get_Day());
    if (month % 2)
        lim = 31;
    else {
        if (month == 2) {
            if (year % 4) lim = 28;
            else lim = 29;
        } else {
            lim = 30;
        }
    }
    if (day < lim) day++;
    else day = 1;
    set_Days(byte_To_BCD(day));
}

/* Increment the hours on the DS3231M Clock */
void inc_Hours(void) {
    byte temp;
    temp = get_Hours();
    if (temp & 0x40) { // if in 12H mode increment 1-12
        if ((temp & 0x1F) <= 0x11) temp++; // increment if 1-11
        else temp = (temp & 0xE0) + 1;  // if 12 then clear 5lsb, and add 1
    } 
    else { // if in 24H mode increment 0-23
        if ((temp & 0x3F) <= 0x22) temp++; // increment if 0-22
        else temp = temp & 0xC0; // if 23 then clear 6lsb
    }
    if ((temp & 0x0F) >= 0x0A) temp = (temp & 0xF0) + 0x10; // if lower nibble too high
    set_Hours(temp);
}

/* Increment the minutes on the DS3231M Clock (for user-adjustment of time etc.) */
void inc_Minutes(void) {
    byte temp;
    temp = get_Minutes();
    if (temp <= 0x58) {
        temp++;
        if ((temp & 0x0F) >= 0x0A) temp = (temp & 0xF0) + 0x10;
    } else {
        temp = 0;
    }
    set_Minutes(temp);
}

/* Increment the months on the DS3231M Clock (for user-adjustment of time etc.) */
void inc_Months(void) {
    byte temp;
    temp = get_Month();
    if (temp <= 0x11) {
        temp++;
        if ((temp & 0x0F) >= 0x0A) temp = (temp & 0xF0) + 0x10;
    } else {
        temp = 1;
    }
    set_Month(temp);
}

/* Increment the years on the DS3231M Clock (for user-adjustment of time etc.) */
void inc_Years(void) {
    byte temp;
    temp = get_Year();
    if (temp <= 0x98) {
        temp++;
        if ((temp & 0x0F) >= 0x0A) temp = (temp & 0xF0) + 0x10;
    } else {
        temp = 0;
    }
    set_Year(temp);
}

/* Initialise the DS3231M RTC for use in the circuit */
void init_ds3231 (void) {
    write_Ds3231_Byte(0x00, REG_DS3231_CONTROL);
    write_Ds3231_Byte(0x00, REG_DS3231_STATUS);
}

/* Because of the numerous single byte write routines for the clock, a general
 * routine provides a simpler interface of doing so */
byte read_Ds3231_Byte(byte reg) {
    byte temp;
    I2CStartBit(); // start communication
    I2CSend(DS3231_ADDR + I2C_WRITE);
    I2CSend(reg);
    I2CRestartBit();
    I2CSend(DS3231_ADDR + I2C_READ);
    temp = I2CRead();
    I2CNackBit();
    I2CStopBit(); // stop transmission
    return temp;
}

/* Set the minutes on the DS3231M Clock */
void set_Minutes(byte mins) {
    write_Ds3231_Byte(mins, REG_DS3231_MINUTES);
}

/* set the days on the DS3231 clock */
void set_Days(byte days) {
    write_Ds3231_Byte(days, REG_DS3231_DAY);
}

/* Set the hours on the DS3231M Clock */
void set_Hours(byte hours) {
    write_Ds3231_Byte(hours, REG_DS3231_HOURS);
}

/* Set the months on the DS3231M Clock */
void set_Month(byte months) {
    write_Ds3231_Byte(months, REG_DS3231_MONTH_CENTURY);
}

/* Set the Years on the DS3231M Clock */
void set_Year(byte years) {
    write_Ds3231_Byte(years, REG_DS3231_YEAR);
}

/* Because of the numerous single byte write routines for the clock, a general
 * routine provides a simpler interface of doing so */
void write_Ds3231_Byte(byte data, byte reg) {
    I2CStartBit();
    I2CSend(DS3231_ADDR + I2C_WRITE);
    I2CSend(reg);
    I2CSend(data);
    I2CStopBit();
}
