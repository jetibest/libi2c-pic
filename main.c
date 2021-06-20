#include "config.h"

#define _XTAL_FREQ 4000000 // 4 MHz

#include <xc.h>
#include <pic16f1614.h>

#include "i2c_pic16f1614.h"


// do not use device addresses below 8 or above 119 (these are reserved)
#define SSP1_I2C_DEVICE_ADDRESS 0x18

// Comment to use while-loop in main() instead of interrupt
#define USE_INTERRUPT

// Comment to use MFINTOSC oscillator to generate the same frequency for these ambiguous frequencies: 500kHz, 250kHz, 125kHz, 50kHz, 31.25kHz
#define INTOSC_HF


// Automatically set OSCCONbits.IRCF: Initialize frequency of internal oscillator based on value of _XTAL_FREQ
void intosc_init()
{
#if _XTAL_FREQ == 16000000
    OSCCONbits.IRCF = 0b1111; // = 16MHz HF
#elif _XTAL_FREQ == 8000000
    OSCCONbits.IRCF = 0b1110; // = 8MHz HF
#elif _XTAL_FREQ == 4000000
    OSCCONbits.IRCF = 0b1101; // = 4MHz HF
#elif _XTAL_FREQ == 2000000
    OSCCONbits.IRCF = 0b1100; // = 2MHz HF
#elif _XTAL_FREQ == 1000000
    OSCCONbits.IRCF = 0b1011; // = 1MHz HF
#elif _XTAL_FREQ == 500000 && defined INTOSC_HF
    OSCCONbits.IRCF = 0b1010; // = 500kHz HF
#elif _XTAL_FREQ == 250000 && defined INTOSC_HF
    OSCCONbits.IRCF = 0b1001; // = 250kHz HF
#elif _XTAL_FREQ == 125000 && defined INTOSC_HF
    OSCCONbits.IRCF = 0b1000; // = 125kHz HF
#elif _XTAL_FREQ == 500000
    OSCCONbits.IRCF = 0b0111; // = 500kHz MF (default upon Reset)
#elif _XTAL_FREQ == 250000
    OSCCONbits.IRCF = 0b0110; // = 250kHz MF
#elif _XTAL_FREQ == 125000
    OSCCONbits.IRCF = 0b0101; // = 125kHz MF
#elif _XTAL_FREQ == 62500
    OSCCONbits.IRCF = 0b0100; // = 62.5kHz MF
#elif _XTAL_FREQ == 31250 && defined INTOSC_HF
    OSCCONbits.IRCF = 0b0011; // = 31.25kHz HF
#elif _XTAL_FREQ == 31250
    OSCCONbits.IRCF = 0b0010; // = 31.25kHz MF
#elif _XTAL_FREQ == 31000
    OSCCONbits.IRCF = 0b0001; // = 31kHz LF
#endif
}

// Setup RC0 and RC1 pins for use with I2C (RC0 is clock, RC1 is data)
void pins_init(void)
{
    // OPTION_REG [OPTION REGISTER] (pp. 223)
    
    // negated Weak Pull-Up Enable bit
    // [default] OPTION_REGbits.nWPUEN = 1; // 0: Weak pull-ups are enabled by individual WPUx latch values, 1: All weak pull-ups are disabled (except MCLR, if it is enabled)
    
    
    // xxxPPS [PERIPHERAL XXX INPUT SELECTION] (pp. 172)
    
    // set SCK/SCLK as RC0, and SDA/SDAT as RC1
    SSPCLKPPS = 0b10000; // 10: Peripheral input is PORTC, 000: Peripheral input is from PORTx Bit 0 (Rx0)
    SSPDATPPS = 0b10001; // 10: Peripheral input is PORTC, 001: Peripheral input is from PORTx Bit 1 (Rx1)
    
    
    // RxyPPS [PIN Rxy OUTPUT SOURCE SELECTION REGISTER] (pp. 172)
    
    // Pin Rxy Output Source Selection bits
    RC0PPS = 0b10000; // 10000: SCL
    RC1PPS = 0b10001; // 10001: SDA
    
    
    // ANSELA [PORTA ANALOG SELECT REGISTER] (pp. 152)
    
    // ANSA: Analog Select between Analog or Digital Function on Pins RA<0:2,4>, respectively
    //  - 0: Digital I/O. Pin is assigned to port or digital special function.
    //  - 1: Analog input. Pin is assigned as analog input (When setting a pin to an analog input, the corresponding TRIS bit must be set to Input mode to allow external control of the voltage on the pin). Digital input buffer disabled.
    ANSELAbits.ANSA0 = 0;
    ANSELAbits.ANSA1 = 0;
    ANSELAbits.ANSA2 = 0;
    ANSELAbits.ANSA4 = 0;
    
    
    // TRISA [PORTA TRI-STATE REGISTER] (pp. 151)
    
    // PORTA Tri-State Control bit
    //  - 0: PORTA pin is configured as an output.
    //  - 1: PORTA pin is configured as an input (tri-stated).
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA3 = 0;
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA5 = 0;
    
    
    // WPUA [WEAK PULL-UP PORTA REGISTER] (pp. 153) (ignored if OPTION_REGbits.nWPUEN == 1)
    
    // Weak Pull-Up Register bits (Port A 0-5)
    //  - 0: Pull-up disabled
    //  - 1: Pull-up enabled
    WPUAbits.WPUA0 = 0;
    WPUAbits.WPUA1 = 0;
    WPUAbits.WPUA2 = 0;
    WPUAbits.WPUA3 = 0;
    WPUAbits.WPUA4 = 0;
    WPUAbits.WPUA5 = 0;
    
    
    // PORTA [PORTA REGISTER] (pp. 151)
    
    // PORTA I/O Value bits (Writes to PORTA are actually written to corresponding LATA register. Reads from PORTA register is return of actual I/O pin values)
    //  - 0: Port pin is lte V_IL.
    //  - 1: Port pin is gte V_IH.
    PORTAbits.RA0 = 1;
    PORTAbits.RA1 = 1;
    PORTAbits.RA2 = 1;
    PORTAbits.RA3 = 1;
    PORTAbits.RA4 = 1;
    PORTAbits.RA5 = 1;
    
    
    // ANSELC [PORTC ANALOG SELECT REGISTER] (pp. 166)
    
    // ANSA: Analog Select between Analog or Digital Function on Pins RC<0:3>, respectively
    //  - 0: Digital I/O. Pin is assigned to port or digital special function.
    //  - 1: Analog input. Pin is assigned as analog input (When setting a pin to an analog input, the corresponding TRIS bit must be set to Input mode to allow external control of the voltage on the pin). Digital input buffer disabled.
    ANSELCbits.ANSC0 = 0;
    ANSELCbits.ANSC1 = 0;
    ANSELCbits.ANSC2 = 0;
    ANSELCbits.ANSC3 = 0;
    
    
    // TRISC [PORTC TRI-STATE REGISTER] (pp. 165)
    
    // PORTC Tri-State Control bits
    TRISCbits.TRISC0 = 1; // 1: PORTC pin configured as an input (tri-stated)
    TRISCbits.TRISC1 = 1; // 1: PORTC pin configured as an input (tri-stated)
    TRISCbits.TRISC2 = 0; // 0: output (default is 1: input/tri-stated)
    TRISCbits.TRISC3 = 0; // 0: output (default is 1: input/tri-stated)
    TRISCbits.TRISC4 = 0; // 0: output (default is 1: input/tri-stated)
    TRISCbits.TRISC5 = 0; // 0: output (default is 1: input/tri-stated)
    
    
    // WPUC [WEAK PULL-UP PORTC REGISTER] (pp. 167) (ignored if OPTION_REGbits.nWPUEN == 1)
    
    // Weak Pull-up Register bits (Port C 0-5)
    //  - 0: Pull-up disabled
    //  - 1: Pull-up enabled
    WPUCbits.WPUC0 = 0;
    WPUCbits.WPUC1 = 0;
    WPUCbits.WPUC2 = 0;
    WPUCbits.WPUC3 = 0;
    WPUCbits.WPUC4 = 0;
    WPUCbits.WPUC5 = 0;
    
    
    // ODCONC [PORTC OPEN-DRAIN CONTROL REGISTER] (pp. 167)
    
    // PORTC Open-Drain Enable bits
    ODCONCbits.ODC0 = 1; // 1: Port pin operates as open-drain drive (sink current only)
    ODCONCbits.ODC1 = 1; // 1: Port pin operates as open-drain drive (sink current only)
    
    
    // PORTC [PORTC REGISTER] (pp. 165)
    
    // PORTC I/O Value bits (Writes to PORTC are actually written to corresponding LATC register. Reads from PORTC register is return of actual I/O pin values)
    // PORTCbits.RC0 = 0;  // 1: Port pin is gte V_IH, 0: Port pin is lte V_IL
    // PORTCbits.RC1 = 0;  // 1: Port pin is gte V_IH, 0: Port pin is lte V_IL
    PORTCbits.RC2 = 0; // 1: Port pin is gte V_IH, 0: Port pin is lte V_IL
    PORTCbits.RC3 = 1; // 1: Port pin is gte V_IH, 0: Port pin is lte V_IL
    PORTCbits.RC4 = 1; // 1: Port pin is gte V_IH, 0: Port pin is lte V_IL
    PORTCbits.RC5 = 1; // 1: Port pin is gte V_IH, 0: Port pin is lte V_IL
}

// Handle interrupts (alternative to while-loop in main)
void __interrupt() interrupt_handler(void)
{
    if(INTCONbits.PEIE == 1)
    {
#ifdef USE_INTERRUPT
        // I2C_slave_handle_interrupt() calls either I2C_slave_read, I2C_slave_write, or nothing (in case of error etc.)
        SSP1_I2C_slave_handle_interrupt();
#endif /* USE_INTERRUPT */
    }
}

void SSP1_I2C_slave_begin(unsigned char address)
{
    // startbit detected, and we received the address
    // due to the configuration, the hardware already filters out any address that does not match our specified I2C_DEVICE_ADDRESS
    // so address must be equal to I2C_DEVICE_ADDRESS, and does not need to be tested again
    
    PORTCbits.RC2 = 1;
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
    
    PORTCbits.RC2 = 0;
}

void main(void)
{
    // Initialize internal oscillator to the right crystal frequency as defined by _XTAL_FREQ
    intosc_init();
    
    // Setup RC0 and RC1 pins for use with I2C (RC0 is clock, RC1 is data)
    // Setup RC2 pin as digital output (initialize on 0) to indicate I2C communication activity
    pins_init();
    
#ifdef USE_INTERRUPT
    // INTCON [INTERRUPT CONTROL REGISTER] (pp. 97)
    
    // Global Interrupt Enable bit
    INTCONbits.GIE = 1; // 1: Enables all active interrupts
    // Peripheral Interrupt Enable bit
    INTCONbits.PEIE = 1; // 1: Enables all active peripheral interrupts
#endif
    
    // Setup MSSP registers for I2C as slave (using a 7-bit address)
    SSP1_I2C_slave_init(SSP1_I2C_DEVICE_ADDRESS);
    
    while(1)
    {
#ifndef USE_INTERRUPT
        
        // Poll for I2C message flag (or alternatively define USE_INTERRUPT in order to use the interrupt handler)
        // I2C_slave_handle_interrupt() calls either I2C_slave_read, I2C_slave_write, or nothing (in case of error etc.)
        SSP1_I2C_slave_handle_interrupt();
        
#endif /* USE_INTERRUPT */
    }
    
    
    return;
}
