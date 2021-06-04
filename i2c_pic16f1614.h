// Device: PIC16F1614
// Datasheet: http://ww1.microchip.com/downloads/en/devicedoc/40001769c.pdf

#ifndef I2C_PIC16F1614_H
#define	I2C_PIC16F1614_H

// indicate that the SSP1 module is available:
#define SSP1_I2C

// specify a suitable buffer length, depending on available PIC memory
#ifndef SSP1_I2C_SLAVE_MAX_BUFFER_LENGTH
#define SSP1_I2C_SLAVE_MAX_BUFFER_LENGTH 256
#endif

#include "i2c.h"


#endif	/* I2C_PIC16F1614_H */

