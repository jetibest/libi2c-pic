#ifndef CONFIG_H
#define	CONFIG_H

// CONFIG1: CONFIGURATION WORD 1 (pp. 67)

// !CLKOUTEN: Clock Out Enable bit
//  - 0b1: OFF = CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin.
//  - 0b0: ON = CLKOUT function is enabled on the CLKOUT pin.
// [default] #pragma config CLCKOUTEN = OFF

// BOREN: Brown-Out Reset Enable bits (Enabling Brown-out Reset does not automatically enable Power-up Timer)
//  - 0b11: OFF = BOR disabled.
//  - 0b10: SBODEN = BOR controlled by SBOREN bit of the BORCON register.
//  - 0b01: NSLEEP = BOR enabled during operation and disabled in Sleep.
//  - 0b00: ON = BOR enabled.
// [default] #pragma config BOREN = OFF

// !CP: Code Protection bit (Once enabled, code-protect can only be disabled by bulk erasing device)
//  - 0b1: OFF = Program memory code protection is disabled.
//  - 0b0: ON = Program memory code protection is enabled.
// [default] #pragma config CP = OFF

// MCLRE: !MCLR/V_PP Pin Function Select bit (only if LVP = OFF, otherwise this value is ignored)
//  - 0b1: ON = !MCLR/V_PP pin function is !MCLR; Weak pull-up enabled.
//  - 0b0: OFF = !MCLR/V_PP pin function is digital input; !MCLR internally disabled; Weak pull-up under control of WPUA3 bit.
#pragma config MCLRE = OFF

// !PWRTE: Power-Up Timer Enable bit
//  - 0b1: OFF = PWRT disabled.
//  - 0b0: ON = PWRT enabled.
// [default] #pragma config PWRTE = OFF

// FOSC: Oscillator Selection bits
//  - 0b11: ECH = External clock, High-Power mode: on CLKIN pin.
//  - 0b10: ECM = External clock, Medium-Power mode: on CLKIN pin.
//  - 0b01: ECL = External clock, Low-Power mode: on CLKIN pin.
//  - 0b00: INTOSC = oscillator I/O function on CLKIN pin.
#pragma config FOSC = INTOSC


// CONFIG2: CONFIGURATION WORD 2 (pp. 68)

// LVP: Low-Voltage Programming Enable bit (The LVP bit cannot be programmed to '0' when Programming mode is entered via LVP)
//  - 0b1: ON = Low-voltage programming enabled
//  - 0b0: OFF = High-voltage on !MCLR must be used for programming
#pragma config LVP = OFF

// !DEBUG: In-Circuit Debugger Mode bit (The !DEBUG bit in Configuration Words is managed automatically by device development tools including debuggers and programmers. For normal device operation, this bit should be maintained as a '1'.)
//  - 0b1: OFF = In-Circuit Debugger disabled, ICSPCLK and ICSPDAT are general purpose I/O pins
//  - 0b0: ON = In-Circuit Debugger enabled, ICSPCLK and ICSPDAT are dedicated to the debugger
// [default] #pragma config DEBUG = OFF

// !LPBOR: Low-Power BOR Enable bit
//  - 0b1: OFF = Low-Power Brown-out Reset is disabled
//  - 0b0: ON = Low-Power Brown-out Reset is enabled
#pragma config LPBOR = OFF

// BORV: Brown-Out Reset Voltage Selection bit (See V_BOR parameter for specific trip point voltages)
//  - 0b1: LO = Brown-out Reset Voltage (V_BOR), low trip point selected
//  - 0b0: HI = Brown-out Reset Voltage (V_BOR), high trip point selected
// [default] #pragma config BORV = LO

// STVREN: Stack Overflow/Underflow Reset Enable bit
//  - 0b1: ON = Stack Overflow or Underflow will cause a Reset
//  - 0b0: OFF = Stack Overflow or Underflow will not cause a Reset
// [default] #pragma config STVREN = ON

// PLLEN: PLL Enable bit
//  - 0b1: ON = 4xPLL enabled
//  - 0b0: OFF = 4xPLL disabled
// [default] #pragma config PLLEN = ON

// !ZCD: ZCD Disable bit
//  - 0b1: OFF = ZCD disabled. ZCD can be enabled by setting the ZCD1EN bit of ZDC1CON
//  - 0b0: ON = ZCD always enabled
// [default] #pragma config ZCD = OFF

// PPS1WAY: PPSLOCK Bit One-Way Set Enable bit
//  - 0b1: ON = The PPSLOCK bit can only be set once after an unlocking sequence is executed; once PPSLOCK is set, all future changes to PPS registers are prevented
//  - 0b0: OFF = The PPSLOCK bit can be set and cleared as needed (provided an unlocking sequence is executed)
// [default] #pragma config PPS1WAY = ON

// WRT: Flash Memory Self-Write Protection bits
// 4kW Flash memory (PIC16(L)F1614/8):
//  - 0b11: OFF = Write protection off
//  - 0b10: BOOT = 000h to 1FFh write-protected, 200h to FFFh may be modified by PMCON control
//  - 0b01: HALF = 000h to 7FFh write-protected, 800h to FFFh may be modified by PMCON control
//  - 0b00: ALL = 000h to FFFh write-protected, no addresses may be modified by PMCON control
// [default] #pragma config WRT = OFF


// CONFIG3: CONFIGURATION WORD 3 (pp. 69)

// WDTCCS: WDT Configuration Clock Select bits
//  - 0b111: SWC = Software Control; WDT clock selected by CS<2:0>
//  - 0b110 -> 0b010: Reserved
//  - 0b001: MFINTOSC = WDT reference clock is MFINTOSC, 31.25 kHz (default value)
//  - 0b000: LFINTOSC = WDT reference clock is LFINTOSC, 31.00 kHz output
// [default] #pragma config WDTCCS = SWC

// WDTCWS: WDT Configuration Window Select bits
//  - 0b111: WDTCWSSW = Software WDT window size control (WDTWS bits)
//  - 0b110: WDTCW100 = 100 percent window open time (Legacy WDT)
//  - 0b101: WDTCW75 = 75 percent window open time
//  - 0b100: WDTCW625 = 62.5 percent window open time
//  - 0b011: WDTCW50 = 50 percent window open time
//  - 0b010: WDTCW375 = 37.5 percent window open time
//  - 0b001: WDTCW25 = 25 percent window open time
//  - 0b000: WDTCW125 = 12.5 percent window open time (A window delay of 12.5% is only available in Software Control mode via the WDTCON1 register)
// [default] #pragma config WDTCWS = WDTCWSSW

// WDTE: Watchdog Timer Enable bits
//  - 0b11: ON = WDT enabled in all modes, the SEN bit in the WDTCON0 register is ignored
//  - 0b10: NSLEEP = WDT enabled while running and disabled in Sleep
//  - 0b01: SWDTEN = WDT controlled by the SEN bit in the WDTCON0 register
//  - 0b00: OFF = WDT disabled
#pragma config WDTE = OFF

// WDTCPS: WDT Configuration Period Select bits
//  - 0b11111: WDTCPS1F = Software Control (WDTPS)
//  - 0b10011 -> 0b11110: WDTCPS0 = 1:32 (1ms period)
//  - 0b11010: WDTCPS12 = 1:8388608 (256s period)
//  - 0b11001: WDTCPS11 = 1:4194304 (128s period)
//  - 0b10000: WDTCPS10 = 1:2097152 (64s period)
//  - 0b01111: WDTCPSf = 1:1048576 (32s period)
//  - 0b01110: WDTCPSe = 1:524299 (16s period)
//  - 0b01101: WDTCPSd = 1:262144 (8s period)
//  - 0b01100: WDTCPSc = 1:131072 (4s period)
//  - 0b01011: WDTCPSb = 1:65536 (2s period)
//  - 0b01010: WDTCPSa = 1:32768 (1s period)
//  - 0b01001: WDTCPS9 = 1:16384 (512ms period)
//  - 0b01000: WDTCPS8 = 1:8192 (256ms period)
//  - 0b00111: WDTCPS7 = 1:4096 (128ms period)
//  - 0b00110: WDTCPS6 = 1:2048 (64ms period)
//  - 0b00101: WDTCPS5 = 1:1024 (32ms period)
//  - 0b00100: WDTCPS4 = 1:512 (16ms period)
//  - 0b00011: WDTCPS3 = 1:256 (8ms period)
//  - 0b00010: WDTCPS2 = 1:128 (4ms period)
//  - 0b00001: WDTCPS1 = 1:64 (2ms period)
//  - 0b00000: WDTCPS0 = 1:32 (1ms period)
// [default] #pragma config WDTCPS = WDTCPS1F

#endif	/* CONFIG_H */

