#ifndef I2C_H
#define	I2C_H

#include <xc.h>

#ifdef SSP1_I2C

// it's possible to set a custom maximum buffer length, but must be set before the i2c.h is included
#ifndef SSP1_I2C_SLAVE_MAX_BUFFER_LENGTH
#define SSP1_I2C_SLAVE_MAX_BUFFER_LENGTH 256
#endif

// By default, an additional I2C_slave_read is called when the buffer is full (within a single transmission)
// #define I2C_SLAVE_FLAG_OVERFLOW_IGNORE        : ignore any additional bytes
// #define I2C_SLAVE_FLAG_OVERFLOW_OVERWRITE     : overwrite from beginning (without triggering a slave read)

unsigned char __SSP1_I2C_slave_null;
unsigned char __SSP1_I2C_slave_address;
size_t __SSP1_I2C_slave_buffer_index = 0;
unsigned char __SSP1_I2C_slave_buffer_data[SSP1_I2C_SLAVE_MAX_BUFFER_LENGTH];

// see also pp. 306
void SSP1_I2C_slave_init(unsigned char address);

// handle interrupt (must be called from the __interrupt() handler)
void SSP1_I2C_slave_handle_interrupt(void);


// interface for start of transmission from master (startbit), after address has been read
void SSP1_I2C_slave_begin(unsigned char address);

// interface for reading a byte (see I2C_SLAVE_FLAG_OVERFLOW_... for different ways of handling `length` > I2C_SLAVE_MAX_BUFFER_LENGTH)
void SSP1_I2C_slave_read(unsigned char* data, size_t length);

// interface for writing bytes (the master decides when to stop, this must be part of the higher order protocol)
void SSP1_I2C_slave_write(unsigned char* data);

// interface for end of transmission from master (stopbit)
void SSP1_I2C_slave_end(void);


#endif /* SSP1_I2C */


#ifdef SSP2_I2C

#ifndef SSP2_I2C_SLAVE_MAX_BUFFER_LENGTH
#define SSP2_I2C_SLAVE_MAX_BUFFER_LENGTH 256
#endif

unsigned char __SSP2_I2C_slave_null;
unsigned char __SSP2_I2C_slave_address;
size_t __SSP2_I2C_slave_buffer_index = 0;
unsigned char __SSP2_I2C_slave_buffer_data[I2C_SLAVE_MAX_BUFFER_LENGTH];

void SSP2_I2C_slave_init(unsigned char address);
void SSP2_I2C_slave_handle_interrupt(void);
void SSP2_I2C_slave_begin(unsigned char address);
void SSP2_I2C_slave_read(unsigned char* data, size_t length);
void SSP2_I2C_slave_write(unsigned char* data);
void SSP2_I2C_slave_end(void);

#endif /* SSP2_I2C */


#ifdef SSP3_I2C

#ifndef SSP3_I2C_SLAVE_MAX_BUFFER_LENGTH
#define SSP3_I2C_SLAVE_MAX_BUFFER_LENGTH 256
#endif

unsigned char __SSP3_I2C_slave_null;
unsigned char __SSP3_I2C_slave_address;
size_t __SSP3_I2C_slave_buffer_index = 0;
unsigned char __SSP3_I2C_slave_buffer_data[I2C_SLAVE_MAX_BUFFER_LENGTH];

void SSP3_I2C_slave_init(unsigned char address);
void SSP3_I2C_slave_handle_interrupt(void);
void SSP3_I2C_slave_begin(unsigned char address);
void SSP3_I2C_slave_read(unsigned char* data, size_t length);
void SSP3_I2C_slave_write(unsigned char* data);
void SSP3_I2C_slave_end(void);

#endif /* SSP3_I2C */


// ... add more SSP modules here when necessary ...


#endif	/* I2C_H */

