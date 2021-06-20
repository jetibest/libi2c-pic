# libi2c-pic
Universal I2C library for Microchip PIC devices

## DISCLAIMER: NO WARRANTY, CODE IS UNTESTED, USE AT OWN RISK

In a Microchip PIC microcontroller, there is an MSSP module which is responsible for I2C communication (and possibly other serial communication such as SPI).
Multiple of such modules following naming convention: `SSP1`, `SSP2`, etc.

Every PIC device has its own header and source files, because they may have a different implementation.

Note: This library is designed to work with [Microchip's XC8 compiler](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers) (preferably use MPLAB X IDE as editor) `#include <xc.h>`.

This library is **work in progress**: only the slave has been implemented, and only for one specific device.

## Usage

The implementation of the following functions are mandatory, but the implementation itself is of course optional.
Note that while executing any of these functions, the I2C communication is being stalled, so returns from the function as fast as possible.

```c
#include "i2c_pic16f1614.h"

void SSP1_I2C_slave_begin(unsigned char address)
{
    // Do something before communication with master (startbit and address detected)
}
void SSP1_I2C_slave_read(unsigned char* data, size_t length)
{
    // `length` bytes have been put in the `data` array, do something with it here
}
void SSP1_I2C_slave_write(unsigned char* data)
{
    // set bytes in the `data` array to send to master (how many bytes? that is up to the protocol that is defined, only the master can control how many bytes it wants to read)
}
void SSP1_I2C_slave_end(void)
{
    // Do something after communication with master (stopbit detected), such as clearing the data buffer.
}
```

In your main program logic, you must initialize the I2C module like so:

```c
// Setup MSSP registers for I2C as slave (using a 7-bit address)
SSP1_I2C_slave_init(24); // parameter is I2C address and must be >7 and <120
```

Note on **interrupts**:
Global interrupts and peripheral interrupts must be enabled if the interrupt flags are handled by the interrupt handler.
Furthermore, your interrupt handler must include a call to `SSPx_I2C_slave_handle_interrupt();` in the interrupt handler.
However, if you choose to handle the interrupt in the main code, then the interrupts for the SSPx module must be disabled (`PIE1bits.SSP1IE = 0` and `PIE2bits.BCL1IE = 0` or simply disable any peripheral interrupt `INTCONbits.PEIE = 0`), after `SSP1_I2C_slave_init` was called.
The interrupt flags are set and can be used, regardless of whether actual interrupt calls are enabled or not.

Lastly, the pins to use with I2C must be configured:

 - Select input pins for the I2C module to use with the chosen pins (see `SSPCLKPPS` and `SSPDATPPS` for clock and data pins respectively).
 - Select output pins for the I2C module to use with the chosen pins (see `RxyPPS`, and set both pins to SCL and SDA functionality).
 - Set the chosen pins in the Tri-State register (see `TRISxy`) as input (tri-stated).
 - Enable Open-Drain on the chosen pins: (see `ODCONx` -> `ODxy`).
 

## Getting started with example code

See main.c for example code.
To download and compile<sup>[1](#compile-note)</sup> the example code:

```
cd ~/Downloads/
git clone https://github.com/jetibest/libi2c-pic
cd libi2c-pic/
make clean build
```

Get latest changes to repository using:

```
cd ~/Downloads/libi2c-pic/
git pull
make clean build
```

<a name="compile-note">1.</a>
You need to install `git` and `xc8`.
In order to install the XC8 compiler, [download the archive from Microchip's website](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers).
Ensure that the `xc8` executable is in the PATH environment variable (e.g. `/opt/microchip/xc8/v2.05/bin`).
