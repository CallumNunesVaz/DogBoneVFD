#ifndef MMA8451Q_H
#define	MMA8451Q_H

#include <xc.h>
#include "i2c.h"
#include "data.h"

// Fosc  frequency for _delay()  library
#define _XTAL_FREQ 16000000   

#define byte unsigned char
#define accel_x signed_XYZ[0]
#define accel_y signed_XYZ[1]
#define accel_z signed_XYZ[2]

// i2C Address of the device
#define MMA8451Q_ADDR 0x1C

// register names and corresponding addresses
#define STATUS 0x00
#define OUT_X_MSB 0x01
#define OUT_X_LSB 0x02
#define OUT_Y_MSB 0x03
#define OUT_Y_LSB 0x04
#define OUT_Z_MSB 0x05
#define OUT_Z_LSB 0x06
#define F_SETUP 0x09
#define TRIG_CFG 0x0A
#define SYSMOD 0x0B
#define INT_SOURCE 0x0C
#define WHO_AM_I 0x0D
#define XYZ_DATA_CFG 0x0E
#define HP_FILTER_CUTOFF 0x0F
#define PL_STATUS 0x10
#define PL_CFG 0x11
#define PL_COUNT 0x12
#define PL_BF_ZCOMP 0x13
#define PL_THS_REG 0x14
#define FF_MT_CFG 0x15
#define FF_MT_SRC 0x16
#define FF_MT_THS 0x17
#define FF_MT_COUNT 0x18
#define TRANSIENT_CFG 0x1D
#define TRANSIENT_SRC 0x1E
#define TRANSIENT_THS 0x1F
#define TRANSIENT_COUNT 0x20
#define PULSE_CFG 0x21
#define PULSE_SRC 0x22
#define PULSE_THSX 0x23
#define PULSE_THSY 0x24
#define PULSE_THSZ 0x25
#define PULSE_TMLT 0x26
#define PULSE_LTCY 0x27
#define PULSE_WIND 0x28
#define ASLP_COUNT 0x29
#define CTRL_REG1 0x2A
#define CTRL_REG2 0x2B
#define CTRL_REG3 0x2C
#define CTRL_REG4 0x2D
#define CTRL_REG5 0x2E
#define OFF_X 0x2F
#define OFF_Y 0x30
#define OFF_Z 0x31

/* angles 0 through to 88 in steps of 2deg // arctan angles *1000
const unsigned int arctan[] = {
    0, 34, 69, 105, 140, 176, 212, 249,
    286, 324, 363, 404, 445, 487, 531, 577,
    624, 674, 726, 781, 839, 900, 965, 1035,
    1110, 1191, 1279, 1376, 1482, 1600, 1732, 1880,
    2050, 2246, 2475, 2747, 3077, 3487, 4010, 4704,
    5671, 7115, 9514, 14300, 28636
};*/

// angles 0 through to 89 in steps of 1deg
const unsigned int arctan[] = {
    0, 17, 34, 52, 69, 87, 105, 122,
    140, 158, 176, 194, 212, 230, 249, 267,
    286, 305, 324, 344, 363, 383, 404, 424,
    445, 466, 487, 509, 531, 554, 577, 600,
    624, 649, 674, 700, 726, 753, 781, 809,
    839, 869, 900, 932, 965, 1000, 1035, 1072,
    1110, 1150, 1191, 1234, 1279, 1327, 1376, 1428,
    1482, 1539, 1600, 1664, 1732, 1804, 1880, 1962,
    2050, 2144, 2246, 2355, 2475, 2605, 2747, 2904,
    3077, 3270, 3487, 3732, 4010, 4331, 4704, 5144,
    5671, 6313, 7115, 8144, 9514, 11430, 14300, 19081,
    28636, 57289
};

void init_mma8451q(void);
void write_MMA8451Q_Byte(byte data, byte reg);
byte read_MMA8451Q_Byte(byte reg);
void load_mma8451q_Data(void);
unsigned int* get_MMA8451Q_Data_Addr(void);
byte new_mma8451q_Data_Ready(void);
byte get_Tilt(void);

#endif

