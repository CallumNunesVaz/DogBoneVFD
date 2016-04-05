# DogBoneVFD

The VFD display (IVL2-7/5) is a piece of soviet new-old-stock (NOS), and has a unique look to it. Making a watch out of it is cooler still.

Inspiration for this watch is credited to Johngineer for his IVL2-7/5 based watch 'ChronodeVFD'. At the time of writing his project can be found on his blog at http://www.johngineer.com/blog/?p=1595. You should check it out, it’s gorgeous.

Current Checklist:
  * Enable sleep mode (will need to replace pull-up resistor on board to do this).
  * Enable accelorometer usage. The PIC is having trouble communicating with the MMA8451 via I2C, the accelorometer does not respond to the calls. Using a raspberry pi with the board, the accelorometer worked just fine, so the problem is the way the pic is handling the i2c.
  * Perform code optimisations in menu_System.c to reduce code to under 4KB. Alternatively replace PIC16F1828 with PIC16F1819 to bring program mmemory up to 8KB.
