/* -------------------------------------------------------------------------- *
 * File: i2c.h
 * 
 * Author: Callum Nunes-Vaz
 * 
 * Date: 26th Feb 2016
 * 
 * Description:
 * Simple Library for managing the i2C functions on the PIC16F1828
 * 
 * Slight modification of hi-tech c i2c library found at:
 * <http://www.8051projects.net/wiki/I2C_Implementation_on_PIC>
 * -------------------------------------------------------------------------- */

#ifndef I2C_H
#define	I2C_H

#include <xc.h>

/* ------------------------- D E F I N I T I O N S ------------------------- */

// definitions for code simplification 
#define byte unsigned char
#define I2C_WRITE 0
#define I2C_READ 1


/* --------------------------- P R O T O T Y P E S ------------------------- */

// Initialize I2C in master mode, Sets the required baudrate
void init_I2C(void);
 
// Send a start condition on I2C Bus
void I2CStartBit(void);
 
// Send a stop condition on I2C Bus
void I2CStopBit(void);
 
// Sends a repeated start condition on I2C Bus
void I2CRestartBit(void);
 
// Generates acknowledge for a transfer
void I2CAckBit(void);
 
// Generates Not-acknowledge for a transfer
void I2CNackBit(void);

// Wait for a transfer to finish
void I2CWait(void);

// Send a byte on I2C bus, can be address or data
void I2CSend(unsigned char data);

// read byte of data from I2C bus
unsigned char I2CRead(void);

#endif	