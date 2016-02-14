/*
 * File:   data.c
 * Author: Callum
 *
 * Created on 1 February 2016, 4:39 PM
 */

#include "data.h"
#include "ds3231.h"

/* ------------------------- S U B R O U T I N E S ------------------------- */


/* converts BCD coded byte to standard byte for numbers less than 16 */
byte bcd_To_Byte(byte data) {
    byte high, low, out;
    low = data & 0x0F; // retrieve low bcd
    high = (data >> 4)&0x0F; // retrieve high bcd
    out = high * 10 + low; // convert to two digit number
    return out;
}

/* converts standard byte to BCD coded byte for numbers less than 16 */
byte byte_To_BCD(byte data) {
    byte out;
    out = data / 10; // find high side of BCD
    out <<= 4; // move high BCD to high side of byte
    out += data % 10; // find low side of byte and add it
    return out;
}

/* get the pre-specified range in which the tilt is registered from EEPROM*/
byte get_Timeout_EEPROM(void) {
    return eeprom_read(TIMEOUT_EEPROM_ADDR);
}

/* set the pre-specified hour-mode of the display (2-states)*/
byte get_Hr_Mode_EEPROM(void) {
    return eeprom_read(HR_MODE_EEPROM_ADDR);
}

/* get the pre-specified orientation of the display (2-states)*/
byte get_Orientation_EEPROM(void) {
    return eeprom_read(ORIENTATION_EEPROM_ADDR);
}

/* set the pre-specified max brightness of the display in EEPROM*/
void inc_Brightness_EEPROM(void) {
    byte temp;
    temp = eeprom_read(MAX_BRIGHT_EEPROM_ADDR);
    if (temp >= 3) temp = 1;
    else temp++;
    eeprom_write(MAX_BRIGHT_EEPROM_ADDR, temp);
}

/* set the pre-specified angle in which the tilt is registered in EEPROM*/
void inc_Tilt_Angle_EEPROM(void) {
    byte temp;
    temp = bcd_To_Byte(eeprom_read(TILT_ANG_EEPROM_ADDR));
    if (temp < 65) temp += 5;
    else temp = 25;
    eeprom_write(TILT_ANG_EEPROM_ADDR, byte_To_BCD(temp));
}

/* set the pre-specified range in which the tilt is registered in EEPROM*/
void inc_Tilt_Range_EEPROM(void) {
    byte temp;
    temp = bcd_To_Byte(eeprom_read(TILT_ERR_EEPROM_ADDR));
    if (temp < 30) temp += 5;
    else temp = 5;
    eeprom_write(TILT_ERR_EEPROM_ADDR, byte_To_BCD(temp));
}

/* Increment the timeout of the display in approximate 0.5 second steps */
void inc_Timeout_EEPROM(void) {
    /* Timer 1 is modified such that each iteration of display_Current_Menu() 
     * takes 0.125 seconds. The timer is modified by the user set variable which 
     * is in steps of 0.5 seconds, so a value of 35 indicates 3.5 seconds. */
    byte temp;
    temp = bcd_To_Byte(eeprom_read(TIMEOUT_EEPROM_ADDR));
    if (temp < 60) temp += 5;
    else temp = 10;
    eeprom_write(TIMEOUT_EEPROM_ADDR, byte_To_BCD(temp));
}

/* toggle the the display of 12 or 24 Hour time  */
void toggle_Hr_Mode_EEPROM(void) {
    byte temp;
    temp = get_Hours();
    // Toggle and store in EEPROM
    if (eeprom_read(HR_MODE_EEPROM_ADDR) == 0x24) { // 24 in BCD
        eeprom_write(HR_MODE_EEPROM_ADDR, 0x12); // 12 in BCD
        if (temp >= 0x13) temp += 0x0E; // take 0x12 and add 0x20 (pm-bit)
        if (!temp) temp = 0x12; // if 00am, correct formatting to 12am
        temp += 0x40; // set 12/!24 bit 
    } else {
        eeprom_write(HR_MODE_EEPROM_ADDR, 0x24);
        if (temp & 0x20) temp += 0x12; // add 12 to time if in PM
        temp &= 0x3F;  // mask for only 6 lsb
        if (temp == 0x24) temp = 0;  // correct formatting
    }
    set_Hours(temp);
}

/* Turn on or off the scaling of light, store in EEPROM */
void toggle_Light_Sensing_EEPROM(void) {
    if (eeprom_read(LIGHT_SCALE_EEPROM_ADDR))
        eeprom_write(LIGHT_SCALE_EEPROM_ADDR, 0);
    else
        eeprom_write(LIGHT_SCALE_EEPROM_ADDR, 1);
}

/* toggle the orientation of the display (left vs. right handed) */
void toggle_Orientation_EEPROM(void) {
    // toggle
    if (eeprom_read(ORIENTATION_EEPROM_ADDR))
        eeprom_write(ORIENTATION_EEPROM_ADDR, 0);
    else
        eeprom_write(ORIENTATION_EEPROM_ADDR, 1);
}
