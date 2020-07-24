/**
  @Generated PIC10 / PIC12 / PIC16 / PIC18 MCUs Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    mcc.c

  @Summary:
    This content generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.3
        Device            :  PIC16F1454
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.20 and above or later
        MPLAB             :  MPLAB X 5.40
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include <xc.h>

/** CONFIGURATION Bits **********************************************/

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover Mode (Internal/External Switchover Mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config CPUDIV = NOCLKDIV// CPU System Clock Selection Bit (NO CPU system divide)
#pragma config USBLSCLK = 48MHz // USB Low SPeed Clock Selection bit (System clock expects 48 MHz, FS/LS USB CLKENs divide-by is set to 8.)
#pragma config PLLMULT = 3x     // PLL Multipler Selection Bit (3x Output Frequency Selected)
#pragma config PLLEN = ENABLED  // PLL Enable Bit (3x or 4x PLL Enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include "system.h"
#include "io_mapping.h"
/*********************************************************************
* Function: void SYSTEM_Initialize(void)
*
* Overview: Initializes the system.
*
* PreCondition: None
*
* Input:  None
*
* Output: None
*
********************************************************************/
void SYSTEM_Initialize(void)
{
    PIN_MANAGER_Initialize();
    OSCILLATOR_Initialize();
    mInitAllLEDs();

    //  The USB specifications require that USB peripheral devices must never source
    //  current onto the Vbus pin.  Additionally, USB peripherals should not source
    //  current on D+ or D- when the host/hub is not actively powering the Vbus line.
    //  When designing a self powered (as opposed to bus powered) USB peripheral
    //  device, the firmware should make sure not to turn on the USB module and D+
    //  or D- pull up resistor unless Vbus is actively powered.  Therefore, the
    //  firmware needs some means to detect when Vbus is being powered by the host.
    //  A 5V tolerant I/O pin can be connected to Vbus (through a resistor), and
    //  can be used to detect when Vbus is high (host actively powering), or low
    //  (host is shut down or otherwise not supplying power).  The USB firmware
    //  can then periodically poll this I/O pin to know when it is okay to turn on
    //  the USB module/D+/D- pull up resistor.  When designing a purely bus powered
    //  peripheral device, it is not possible to source current on D+ or D- when the
    //  host is not actively providing power on Vbus. Therefore, implementing this
    //  bus sense feature is optional.  This firmware can be made to use this bus
    //  sense feature by making sure "USE_USB_BUS_SENSE_IO" has been defined in the
    //  io_mapping.h file.
    #if defined(USE_USB_BUS_SENSE_IO)
        tris_usb_bus_sense = INPUT_PIN; // See io_mapping.h
    #endif
}

/*********************************************************************
* Function: void PIN_MANAGER_Initialize(void)
*
* Overview: GPIO and peripheral I/O initialization.
*
* PreCondition: None
*
* Input:  None
*
* Output: None
*
********************************************************************/
void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */
    LATA = 0x00;
    LATC = 0x00;

    /**
    TRISx registers
    */
    TRISA = 0x38;
    TRISC = 0x2F;

    /**
    ANSELx registers
    */
    ANSELC = 0x00;
    ANSELA = 0x00;

    /**
    WPUx registers
    */
    WPUA = 0x00;
    OPTION_REGbits.nWPUEN = 1;

    /**
    APFCONx registers
    */
    APFCON = 0x00;
}

/*********************************************************************
* Function: void OSCILLATOR_Initialize(void)
*
* Overview: Oscillator initialization.
*
* PreCondition: None
*
* Input:  None
*
* Output: None
*
********************************************************************/
void OSCILLATOR_Initialize(void)
{
    // SCS FOSC; SPLLMULT 3xPLL; SPLLEN disabled; IRCF 16MHz_HF; 
    OSCCON = 0x7C;
    // TUN 0; 
    OSCTUNE = 0x00;
    // ACTSRC SOSC; ACTUD enabled; ACTSRC Fll-speed; ACTEN enabled; 
    ACTCON = 0x90;
    // SBOREN disabled; BORFS disabled; 
    BORCON = 0x00;
    // Wait for PLL to stabilize
    while(PLLRDY == 0){}
}
