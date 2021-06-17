#pragma config WDTE = OFF   // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF  // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF  // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF    // Low-Voltage In-Circuit Serial Programming Enable bit
#pragma config WRT = OFF    // Flash Program Memory Write Enable bits
#pragma config CP = OFF     // Flash Program Memory Code Protection bit
#pragma config MCLRE = OFF

#define _XTAL_FREQ 8000000

#include <xc.h>
#include <pic16f1614.h>

#include "i2c_pic16f1614.h"

// careful, device addresses below 8 are reserved
// device addresses above 119
#define SSP1_I2C_DEVICE_ADDRESS 8

// Uncomment to use interrupt instead of while-loop in main()
// #define USE_INTERRUPT

// Setup RC0 and RC1 pins for use with I2C (RC0 is clock, RC1 is data)
void pins_init(void)
{
    // OPTION_REG [OPTION REGISTER] (pp. 223)
    
    // negated Weak Pull-Up Enable bit
    OPTION_REGbits.nWPUEN = 1; // 0: Weak pull-ups are enabled by individual WPUx latch values
    
    
    // WPUC [WEAK PULL-UP PORTC REGISTER] (pp. 167)
    
    // Weak Pull-up Register bits (Port C 0-5)
    WPUC = 0; // 0: disable pull-ups on all ports
    WPUCbits.WPUC0 = 0; // 1: Pull-up enabled
    WPUCbits.WPUC1 = 0; // 1: Pull-up enabled
    
    
    // xxxPPS [PERIPHERAL XXX INPUT SELECTION] (pp. 172)
    
    // set SCK/SCLK as RC0, and SDA/SDAT as RC1
    SSPCLKPPS = 0b10000; // 10: Peripheral input is PORTC, 000: Peripheral input is from PORTx Bit 0 (Rx0)
    SSPDATPPS = 0b10001; // 10: Peripheral input is PORTC, 001: Peripheral input is from PORTx Bit 1 (Rx1)
    
    
    // RxyPPS [PIN Rxy OUTPUT SOURCE SELECTION REGISTER] (pp. 172)
    
    // Pin Rxy Output Source Selection bits
    RC0PPS = 0b10000; // 10000: SCL
    RC1PPS = 0b10001; // 10001: SDA
    
    
    // Reset A-ports (to output, and set value to 1 for stability)
    TRISA = 0; // 0: output (default is 1: input/tri-stated)
    PORTA = 1; // set to 1 for stability
    
    
    // TRISC [PORTC TRI-STATE REGISTER] (pp. 165)
    
    // PORTC Tri-State Control bits
    TRISC0 = 1; // 1: PORTC pin configured as an input (tri-stated)
    TRISC1 = 1; // 1: PORTC pin configured as an input (tri-stated)
    
    
    // Reset C-ports (to output, and set value to 1 for stability)
    // C0 is used for SSP1
    // C1 is used for SSP1
    TRISCbits.TRISC2 = 0; // 0: output (default is 1: input/tri-stated)
    TRISCbits.TRISC3 = 0; // 0: output (default is 1: input/tri-stated)
    TRISCbits.TRISC4 = 0; // 0: output (default is 1: input/tri-stated)
    TRISCbits.TRISC5 = 0; // 0: output (default is 1: input/tri-stated)
    // C0 is used for SSP1
    // C1 is used for SSP1
    PORTCbits.RC2 = 1; // set to 1 for stability
    PORTCbits.RC3 = 1; // set to 1 for stability
    PORTCbits.RC4 = 1; // set to 1 for stability
    PORTCbits.RC5 = 1; // set to 1 for stability
    
    
    // ODCONC [PORTC OPEN-DRAIN CONTROL REGISTER] (pp. 167)
    
    // PORTC Open-Drain Enable bits
    ODCONCbits.ODC0 = 1; // 1: Port pin operates as open-drain drive (sink current only)
    ODCONCbits.ODC1 = 1; // 1: Port pin operates as open-drain drive (sink current only)
}

// Handle interrupts (alternative to while-loop in main)
void __interrupt() interrupt_handler(void)
{
    if(INTCONbits.PEIE == 1)
    {
#ifdef USE_INTERRUPT
        if(PIE1bits.SSP1IE == 1 && PIR1bits.SSP1IF == 1)
        {
            // I2C_slave_handle_interrupt() calls either I2C_slave_read, I2C_slave_write, or nothing (in case of error etc.)
            SSP1_I2C_slave_handle_interrupt();
        }
#endif /* USE_INTERRUPT */
    }
}

void SSP1_I2C_slave_begin(unsigned char address)
{
    // startbit detected, and we received the address
    // due to the configuration, the hardware already filters out any address that does not match our specified I2C_DEVICE_ADDRESS
    // so address must be equal to I2C_DEVICE_ADDRESS, and does not need to be tested again
}

// When reading from master, we wait for the end of transmission until we call this function with the gathered data
void SSP1_I2C_slave_read(unsigned char* data, size_t length)
{
    // by doing nothing, we keep the data stored in the buffer
}

// When writing to master, we prepare the whole buffer before any transmission, but the master decides how many bytes to read from it (I2C slave does not know in advance)
void SSP1_I2C_slave_write(unsigned char* data)
{
    // by doing nothing, the same buffer is written back
}

void SSP1_I2C_slave_end(void)
{
    // stopbit detected, we could do some cleanup, for instance, emptying the buffer etc.
}

void main(void)
{
    // Setup RC0 and RC1 pins for use with I2C (RC0 is clock, RC1 is data)
    pins_init();
    
    
    // INTCON [INTERRUPT CONTROL REGISTER] (pp. 97)
    
    // Global Interrupt Enable bit
    INTCONbits.GIE = 1; // 1: Enables all active interrupts
    // Peripheral Interrupt Enable bit
    INTCONbits.PEIE = 1; // 1: Enables all active peripheral interrupts
    
    
    // Setup MSSP registers for I2C as slave (using a 7-bit address)
    SSP1_I2C_slave_init(SSP1_I2C_DEVICE_ADDRESS);
    
    while(1)
    {
#ifndef USE_INTERRUPT
        
        // Poll for I2C message flag (or alternatively define USE_INTERRUPT in order to use the interrupt handler)
        if(PIE1bits.SSP1IE == 1 && PIR1bits.SSP1IF == 1)
        {
            // I2C_slave_handle_interrupt() calls either I2C_slave_read, I2C_slave_write, or nothing (in case of error etc.)
            SSP1_I2C_slave_handle_interrupt();
        }
        
#endif /* USE_INTERRUPT */
    }
    
    
    return;
}
