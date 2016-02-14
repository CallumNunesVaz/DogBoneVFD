/* Slight modification of hi-tech c i2c library found at:
 * <http://www.8051projects.net/wiki/I2C_Implementation_on_PIC>
 * 
 * For use with XC8 compiler, and pic16f1828
 */ 

#include "i2c.h"

// Initialize I2C in master mode, Sets the required baudrate
void init_I2C(void) {
	TRISBbits.TRISB4 = 1;     // SDA as input
	TRISBbits.TRISB6 = 1;     // SCL as input
	SSP1STATbits.SMP = 1;     // slew rate disabled
    SSP1CON1bits.SSPEN = 1;   // enable RB6 and RB4 as SCL and SDA (disable I/O)
	SSP1CON1bits.SSPM = 0x08; // i2c master mode where clock = FOSC/(4*(SSPADD+1))
	SSP1ADD = 0x09;           // 400kHz baud rate for 16MHz Fosc
    PIE1bits.SSP1IE = 0;      // disable MSSP interrupt for i2c
}
 
// Send a start condition on I2C Bus
void I2CStartBit() {
	SSP1CON2bits.SEN = 1;     // Start condition enabled 
	while(SSP1CON2bits.SEN);  // automatically cleared by hardware
}
 
// Send a stop condition on I2C Bus
void I2CStopBit() {
	SSP1CON2bits.PEN = 1;     // Stop condition enabled 
	while(SSP1CON2bits.PEN);  // automatically cleared by hardware
}
 
// Sends a repeated start condition on I2C Bus
void I2CRestartBit() {
	SSP1CON2bits.RSEN = 1;    // Repeated start enabled 
	while(SSP1CON2bits.RSEN); // wait for condition to finish 
}
 
// Generates acknowledge for a transfer
void I2CAckBit() {
	SSP1CON2bits.ACKDT = 0;   // Acknowledge data bit, 0 = ACK
	SSP1CON2bits.ACKEN = 1;   // Ack data enabled
	while(SSP1CON2bits.ACKEN);// wait for ack data to send on bus
}
 
// Generates Not-acknowledge for a transfer
void I2CNackBit() {
	SSP1CON2bits.ACKDT = 1;   // Acknowledge data bit, 1 = NAK 
	SSP1CON2bits.ACKEN = 1;   // Ack data enabled
	while(SSP1CON2bits.ACKEN);// wait for ack data to send on bus
}
 
// Wait for a transfer to finish
void I2CWait(){
	while ((SSP1CON2&0x1F)||(SSP1STAT&0x04));
}
 
// Send a byte on I2C bus, can be address or data
void I2CSend(unsigned char data) {
	SSP1BUF = data;          // Move data to SSPBUF
	while(SSP1STATbits.BF);  // wait till complete data is sent from buffer 
	I2CWait();               // wait for any pending transfer
}

// read byte of data from I2C bus
unsigned char I2CRead(void) {
	unsigned char temp;
	SSP1CON2bits.RCEN = 1;   // Enable data reception 
	while(!SSP1STATbits.BF); // wait for buffer full 
	temp = SSP1BUF;          // Read serial buffer and store in temp register 
	I2CWait();               // wait to check any pending transfer 
	return temp;             // Return the read data from bus 
}