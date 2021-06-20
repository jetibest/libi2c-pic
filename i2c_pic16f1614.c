#include "i2c_pic16f1614.h"

// Note: address must be within range 8-119 (inclusive)
void SSP1_I2C_slave_init(unsigned char address)
{
    // PIE1 [PERIPHERAL INTERRUPT ENABLE REGISTER 1] (pp. 98)
    
    // Synchronous Serial Port (MSSP) Interrupt Enable bit
    PIE1bits.SSP1IE = 1;
    
    
    // PIR1 [PERIPHERAL INTERRUPT REQUEST REGISTER 1] (pp. 103)
    
    // Synchronous Serial Port (MSSP) Interrupt Flag bit
    PIR1bits.SSP1IF = 0; // 0: Interrupt is not pending
    
    
    // PIE2 [PERIPHERAL INTERRUPT ENABLE REGISTER 2] (pp. 99)
    
    // MSSP Bus Collision Interrupt Enable bit
    PIE2bits.BCL1IE = 1; // 1: Enables the MSSP Bus Collision Interrupt
    
    
    // PIR2 [PERIPHERAL INTERRUPT REQUEST REGISTER 2] (pp. 104)
    
    // BCL1IF: MSSP Bus Collision Interrupt Flag bit
    PIR2bits.BCL1IF = 0; // 0: Interrupt is not pending
    
    
    // SSP1ADD [MSSP ADDRESS AND BAUD RATE REGISTER] (pp. 312)
    
    // Address bits at 1-7, 0th bit is unused and ignored
    SSP1ADD = (unsigned char) (address << 1); // 7-bit address
     
    
    // SSP1MSK [SSP MASK REGISTER] (pp. 312)
    
    // Mask bits
    SSP1MSK = 0b11111111;
    
    
    // SSP1STAT [SSP STATUS REGISTER] (pp. 308)
    
    // Slew rate control
    SSP1STATbits.SMP = 1; // 1: Slew rate control disabled; for Standard Speed mode (100 kHz and 1 MHz)
    // SMBus specification
    SSP1STATbits.CKE = 1; // 0: Disable SMBus specific inputs, 1: Enable input logic so that thresholds are compliant with SMBus specification
    
    
    // SSP1CON1 [SSP CONTROL REGISTER 1] (pp. 309)
    
    // Write Collision Detect bit
    SSP1CON1bits.WCOL = 0; // 0: No collision
    // Receive Overflow Indicator bit
    SSP1CON1bits.SSPOV = 0; // 0: No overflow
    // Synchronous Serial Port Enable bit
     SSP1CON1bits.SSPEN = 1; // 1: Enables the serial port and configures the SDA and SCL pins as the source of the serial port pins
    // Clock Polarity Select bit
    SSP1CON1bits.CKP = 1; // 1: Enable clock
    // Synchronous Serial Port Mode Select bits
    SSP1CON1bits.SSPM = 0b0110; // I2C Slave mode, 7-bit address
    
    
    // SSP1CON2 [SSP CONTROL REGISTER 2] (pp. 310)
    
    // Stretch Enable bit
     SSP1CON2bits.SEN = 1; // 1: Clock stretching is enabled for both slave transmit and slave receive (stretch enabled)
    
    
    // SSP1CON3 [SSP CONTROL REGISTER 3] (pp. 311)
    
    // Stop Condition Interrupt Enable bit
    SSP1CON3bits.PCIE = 1; // 1: Enable interrupt on detection of Stop condition
    // Start Condition Interrupt Enable bit
    SSP1CON3bits.SCIE = 1; // 1: Enable interrupt on detection of Start or Reset conditions
    // SDA Hold Time Selection bit [hint: set to 300ns on buses with large capacitance]
    SSP1CON3bits.SDAHT = 1; // 1: Minimum of 300 ns hold time on SDA after the falling edge of SCL
    // Slave Mode Bus Collision Detect Enable bit; upon collision, PIR2bits.BCL1IF is set, and bus goes idle
    SSP1CON3bits.SBCDE = 1; // 1: Enable slave bus collision interrupts
}

// Slave reception protocol (pp. 277)
void SSP1_I2C_slave_handle_interrupt()
{
    // BCL1IF: MSSP Bus Collision Interrupt Flag bit
    if(PIR2bits.BCL1IF == 1) // 1: Interrupt is pending
    {
        // read the previous value to clear the buffer
        __SSP1_I2C_slave_null = SSP1BUF;
        
        // Release the clock line
        SSP1CON1bits.CKP = 1;
        
        // clear the interrupt flag
        PIR2bits.BCL1IF = 0;
    }
    
    // SSP1IF: Synchronous Serial Port (MSSP) Interrupt Flag bit
    if(PIR1bits.SSP1IF == 1) // 1: Interrupt is pending
    {
        // Hold clock (clock stretching must be enabled: SSP1CON2bits.SEN == 1)
        // This is redundant in both AHEN/DHEN=1, and SEN=1 (two separate cases, see pp. 279 vs pp. 280 vs pp. 281)
        // SSP1CON1bits.CKP = 0;
        
        // Error: Handle overflow or collision
        if(SSP1CON1bits.SSPOV == 1 || SSP1CON1bits.WCOL == 1)
        {
            // read the previous value to clear the buffer
            __SSP1_I2C_slave_null = SSP1BUF;
            
            // clear the overflow flag
            SSP1CON1bits.SSPOV = 0;
            
            // clear the collision bit
            SSP1CON1bits.WCOL = 0;
        }
        // Check if start bit was set
        else if(SSP1STATbits.S == 1)
        {
            if(SSP1STATbits.D_nA == 0) // 0: Indicates that the last byte received or transmitted was address
            {
                // Wait for SSP1BUF to be transferred (redundant, use if no interrupt, but polling)
                // while(SSP1STATbits.BF == 0); // 0: Receive not complete, SSP1BUF empty
                
                // read the previous value to clear the buffer, this is the address
                // the address is already matched by the module, otherwise we were not interrupted
                __SSP1_I2C_slave_address = SSP1BUF; // maybe this address is 7 bits, but does that mean the 0th bit is included or not?
                
                // opportunity to reset the buffer to zero-filled, or do something else upon getting address
                SSP1_I2C_slave_begin(__SSP1_I2C_slave_address);
                
                // reset index (for reading or writing buffer data)
                __SSP1_I2C_slave_buffer_index = 0;
            }
            else // 1: Indicates that the last byte received or transmitted was data
            {
                if(SSP1STATbits.R_nW == 0) // 0: Write (master will write, slave will read)
                {
                    // buffer was full already, what are we going to do?
                    if(__SSP1_I2C_slave_buffer_index >= SSP1_I2C_SLAVE_MAX_BUFFER_LENGTH)
                    {
#ifdef SSP1_I2C_SLAVE_FLAG_OVERFLOW_OVERWRITE
                        __SSP1_I2C_slave_buffer_index = 0;
#else
#ifndef SSP1_I2C_SLAVE_FLAG_OVERFLOW_IGNORE
                        // by default, trigger a slave read, when buffer is full, and then start back at zero
                        SSP1_I2C_slave_read(__SSP1_I2C_slave_buffer_data, __SSP1_I2C_slave_buffer_index);
                        __SSP1_I2C_slave_buffer_index = 0;
#endif /* SSP1_I2C_SLAVE_FLAG_OVERFLOW_IGNORE */
#endif /* SSP1_I2C_SLAVE_FLAG_OVERFLOW_OVERWRITE */
                    }
                    
                    // Wait for SSP1BUF to be transferred (redundant, use if no interrupt, but polling)
                    // while(SSP1STATbits.BF == 0); // 0: Receive not complete, SSP1BUF empty
                    
                    // continue reading more bytes into buffer, clearing the BF flag
                    __SSP1_I2C_slave_buffer_data[__SSP1_I2C_slave_buffer_index++] = SSP1BUF;
                }
            }
            
            if(SSP1STATbits.R_nW == 1) // 1: Read (master will read, slave will write)
            {
                if(__SSP1_I2C_slave_buffer_index == 0)
                {
                    // we need to write, so gather up all the data in advance at this moment
                    SSP1_I2C_slave_write(__SSP1_I2C_slave_buffer_data); // first let the slave write to __I2C_slave_buffer_data
                }
                
                // Wait for SSP1BUF to be cleared (redundant, use if no interrupt, but polling)
                // while(SSP1STATbits.BF == 0); // 0: Data transmit complete (does not include the nACK and Stop bits), SSP1BUF is empty
                
                // prepare a byte to write to master
                SSP1BUF = __SSP1_I2C_slave_buffer_data[__SSP1_I2C_slave_buffer_index++];
            }
        }
        // Check if stop bit was set
        else if(SSP1STATbits.P == 1)
        {
            if(SSP1STATbits.R_nW == 0)
            {
                // all the bytes have been received, now we trigger the read function
                SSP1_I2C_slave_read(__SSP1_I2C_slave_buffer_data, __SSP1_I2C_slave_buffer_index);
            }
            
            SSP1_I2C_slave_end();
        }
        
        // Release the clock line
        SSP1CON1bits.CKP = 1;
        
        // Reset interrupt flag here, this avoids unwanted interrupts during processing of data.
        // SSP1IF: Synchronous Serial Port (MSSP) Interrupt Flag bit
        PIR1bits.SSP1IF = 0; // 0: No interrupt is pending
    }
}
