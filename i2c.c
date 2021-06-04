#include "i2c.h"

/* 
 * To be implemented by the device specific source file:
 *   void SSP1_I2C_slave_init(unsigned char address);
 *   void SSP1_I2C_slave_handle_interrupt(void);
 * 
 * 
 * To be implemented by the source code using this library:
 *   void SSP1_I2C_slave_begin(unsigned char address);
 *   void SSP1_I2C_slave_read(unsigned char* data, size_t length);
 *   void SSP1_I2C_slave_write(unsigned char* data);
 *   void SSP1_I2C_slave_end(void);
 * 
 */
