/* -------------------------------------------------------------------------- *
 * File: mma8451.c
 * 
 * Author: Callum Nunes-Vaz
 * 
 * Date: 26th Feb 2016
 * 
 * Description:
 * A simple library for accessing the bare minimum from the mma8451
 * accelerometer for tilt detection purposes. Originally created for a 
 * wristwatch.
 * -------------------------------------------------------------------------- */

#include "mma8451.h"

byte prev_Angle;

byte data_XYZ[3];

void init_mma8451q(void) {
    write_MMA8451Q_Byte(REG_MMA8451_F_SETUP, 0x00); // FIFO DISABLED
    write_MMA8451Q_Byte(REG_MMA8451_TRIG_CFG, 0x00); // trigger is used by fifo so turn off
    write_MMA8451Q_Byte(REG_MMA8451_XYZ_DATA_CFG, 0x10); // output high-pass filtered, range of 2g's
    write_MMA8451Q_Byte(REG_MMA8451_HP_FILTER_CUTOFF, 0x20); // enable hpf, disable lpf, hpf = 0.5Hz with 1.56 wake and date rate in LPRN mode
    write_MMA8451Q_Byte(REG_MMA8451_PL_CFG, 0x80); // Portrait/Landscape off, Debounce counter mode Default
    write_MMA8451Q_Byte(REG_MMA8451_PL_COUNT, 0x00); // Debounce Count value. Default value.
    write_MMA8451Q_Byte(REG_MMA8451_PL_BF_ZCOMP, 0x44); // Z-Lock & Back/Front Trip Angle Threshold. defaults.
    write_MMA8451Q_Byte(REG_MMA8451_PL_THS_REG, 0x84); // Portrait/Landscape angle, Added angle. defaults
    write_MMA8451Q_Byte(REG_MMA8451_FF_MT_CFG, 0x00); //  Freefall detection DISABLED/DEFAULTS
    write_MMA8451Q_Byte(REG_MMA8451_FF_MT_THS, 0x00); // counter mode selection, Threshold. DISABLED/DEFAULTS
    write_MMA8451Q_Byte(REG_MMA8451_FF_MT_COUNT, 0x00); // debounce sample counts, DISABLED/DEFAULTS
    write_MMA8451Q_Byte(REG_MMA8451_TRANSIENT_CFG, 0x00); // DISABLED/DEFAULTS
    write_MMA8451Q_Byte(REG_MMA8451_TRANSIENT_THS, 0x00); // DISABLED/DEFAULTS
    write_MMA8451Q_Byte(REG_MMA8451_TRANSIENT_COUNT, 0x00); // DISABLED/DEFAULTS
    write_MMA8451Q_Byte(REG_MMA8451_PULSE_CFG, 0x00); // DISABLED/DEFAULTS
    write_MMA8451Q_Byte(REG_MMA8451_PULSE_THSX, 0x00); // DISABLED/DEFAULTS
    write_MMA8451Q_Byte(REG_MMA8451_PULSE_THSY, 0x00); // DISABLED/DEFAULTS
    write_MMA8451Q_Byte(REG_MMA8451_PULSE_THSZ, 0x00); // DISABLED/DEFAULTS
    write_MMA8451Q_Byte(REG_MMA8451_PULSE_TMLT, 0x00); // DISABLED/DEFAULTS
    write_MMA8451Q_Byte(REG_MMA8451_PULSE_LTCY, 0x00); // DISABLED/DEFAULTS
    write_MMA8451Q_Byte(REG_MMA8451_PULSE_WIND, 0x00); // DISABLED/DEFAULTS
    write_MMA8451Q_Byte(REG_MMA8451_ASLP_COUNT, 0x00); // DISABLED/DEFAULTS
    write_MMA8451Q_Byte(REG_MMA8451_CTRL_REG1, 0xFD); // SLEEP Mode Rate=1.56Hz, Output Data Rate=1.56Hz, Reduced Noise mode, ACTIVE
    write_MMA8451Q_Byte(REG_MMA8451_CTRL_REG2, 0x0D); // no self-test or soft reset, low noise low power in both wake and sleep, autosleep enabled
    write_MMA8451Q_Byte(REG_MMA8451_CTRL_REG3, 0x00); // all features disabled
    write_MMA8451Q_Byte(REG_MMA8451_CTRL_REG4, 0x00); // disable all interrupt
    write_MMA8451Q_Byte(REG_MMA8451_CTRL_REG5, 0x00); // disable int pins
    write_MMA8451Q_Byte(REG_MMA8451_OFF_X, 0x00); // no offset
    write_MMA8451Q_Byte(REG_MMA8451_OFF_Y, 0x00); // no offset
    write_MMA8451Q_Byte(REG_MMA8451_OFF_Z, 0x00); // no offset 
}

void write_MMA8451Q_Byte(byte data, byte reg) {
    I2CStartBit();
    I2CSend(MMA8451_ADDR + I2C_WRITE);
    I2CSend(reg);
    I2CSend(data);
    I2CStopBit();
    __delay_us(10);
}

byte read_MMA8451Q_Byte(byte reg) {
    byte data;
    I2CStartBit();
    I2CSend(MMA8451_ADDR + I2C_WRITE);
    I2CSend(reg);
    I2CRestartBit();
    I2CSend(MMA8451_ADDR + I2C_READ);
    data = I2CRead();
    I2CNackBit();
    I2CStopBit();
    return data;
}

void load_mma8451q_Data(void) {
    byte i;
    I2CStartBit();
    I2CSend(MMA8451_ADDR + I2C_WRITE);
    I2CSend(0x01);
    I2CRestartBit();
    I2CSend(MMA8451_ADDR + I2C_READ);
    for (i = 0; i <= 2; i++) {
        data_XYZ[i] = I2CRead(); // load msb
        I2CAckBit(); // ack the read
    }
    I2CRead(); // dummy read
    I2CNackBit(); // nack for no more reads
    I2CStopBit(); // stop communications
}

byte new_mma8451q_Data_Ready(void) {
    return (read_MMA8451Q_Byte(STATUS)&0x08);
}

/* return angle in 4-bit BCD from data in data_XYZ[3]; 
 * X pos = top, Y pos = left, Z pos = out of display*/
byte get_Tilt(void) {
    byte mid, low = 0, high = 89;
    unsigned int diff;
    float signed_XYZ[3];
    if (new_mma8451q_Data_Ready()) {
    load_mma8451q_Data();
    // convert from 2c to signed int
    for (mid = 0; mid <= 2; mid++) {
        signed_XYZ[mid] = data_XYZ[mid];
        if (data_XYZ[mid] >= 128) // if negative (because is unsigned int 2c)
            signed_XYZ[mid] = !(data_XYZ[mid] - 1); // invert
    }

    // quick checks
    if (accel_x > 0) return 0x00; // if display tilted away from user
    if (accel_z > 0) return 0x90; // if display facing user's chest
    if (accel_y > 7 || accel_y < 7) return 0x00; // ~(>1 || <1)m/s^2

    // there is now 90 degrees to work with, Binary search for right index
    diff = (unsigned int)((accel_z / accel_x)*1000);

    while (low <= high) { // while low and high haven't closed in on result
        mid = (low + high) / 2; // find new mid index
        if (diff < arctan[mid]) // if lower than mid, redefine high
            high = mid - 1;
        else {
            if (diff > arctan[mid]) low = mid + 1; // if higher than mid, redefine low
            else break; // if neither higher or lower, index has been found
        }
    }

    mid = byte_To_BCD(mid);
    prev_Angle = mid;
    return mid;
    } else
        return prev_Angle;
}