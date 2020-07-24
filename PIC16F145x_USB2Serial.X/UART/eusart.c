/**
  EUSART Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart.c

  @Summary
    This is the generated driver implementation file for the EUSART driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for EUSART.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.3
        Device            :  PIC16F1454
        Driver Version    :  2.1.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.20 and above
        MPLAB 	          :  MPLAB X 5.40
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

/**
  Section: Included Files
*/
#include "eusart.h"

/**
  Section: EUSART APIs
*/

void EUSART_Initialize(void)
{
    // Baud Rate Control Register
  //BAUDCONbits.ABDOVF (Read only) Auto-Baud Detect Overflow --- (Async only mode. Auto-baud timer overflowed or did not overflow)
  //BAUDCONbits.RCIDL  (Read only) Receive Idle Flag ----------- (Async only mode. Receiver is idle (1)/ Start bit has been received and receiving (0))
    BAUDCONbits.SCKP   = 0; // Synchronous Clock Polarity ------ (Sync mode (Clock data on rising(1)/falling(0) edge of clock) / Async mode (Transmit inverted(1)/non-inverted(0) data))
    BAUDCONbits.BRG16  = 1; // 16-bit Baud Rate Generator ------ (16-bit or 8-bit baud rate generator)
    BAUDCONbits.WUE    = 0; // Wake-up Enable ------------------ (Async only mode. (1) Receiver is waiting for a falling edge... / (0) Receiver is operating normally)
    BAUDCONbits.ABDEN  = 1; // Auto-Baud Detect Enable --------- (Async only mode. Enable/Disable (clears when auto-baud is complete)

    // Receive Status and Control Register
    RCSTAbits.SPEN  = 1; // Serial Port Enable ----------------- (Enable (configures Rx/Tx pins) or Disable (held in reset) serial port)
    RCSTAbits.RX9   = 0; // 9-bit Receive Enable --------------- (9-bit or 8-bit reception)
    RCSTAbits.SREN  = 0; // Single Receive Enable -------------- (Sync mode only. Enable/Disable single receive)
    RCSTAbits.CREN  = 1; // Continuous Receive Enable ---------- (Enable/Disable receiver (In sync mode CREN overrides SREN))
    RCSTAbits.ADDEN = 0; // Address Detect Enable -------------- (Async mode only and with RX9=1. Enable/Disable)
  //RCSTAbits.FERR  (Read only) Framing Error ------------------ (Error or no Error (Cleared by reading RCREG register and receive next valid byte))
  //RCSTAbits.OERR  (Read only) Overrun Error ------------------ (Error or no Error (Cleared by clearing CREN))
  //RCSTAbits.RX9D  (Read only) 9th bit of Received Data ------- (Can be address/data/parity bit received)

    // Transmit Status and Control Register
    TXSTAbits.CSRC  = 0; // Clock Source ----------------------- (Sync mode only. Clock generated internally (0) or from external source (1))
    TXSTAbits.TX9   = 0; // 9-bit transmission ----------------- (9-bit/8-bit)
    TXSTAbits.TXEN  = 1; // Transmit Enable -------------------- (Enable/Disable)
    TXSTAbits.SYNC  = 0; // EUSART Mode ------------------------ (Async/Sync) 
    TXSTAbits.SENDB = 0; // Send Break Character --------------- (Async mode only. Set to 1 to send sync break on next transmission)
    TXSTAbits.BRGH  = 1; // High Baud Rate --------------------- (Async mode only. High/Low speed)
  //TXSTAbits.TRMT  (Read only) Transmit Shift Register Status - (Empty or Full)
    TXSTAbits.TX9D  = 0; // 9th bit of Transmit Data ----------- (Can be address/data/parity bit to transmit)

    // Baud Rate Generator High/Low bytes (initialize to 9600 but will be change by CDC)
    SPBRGL = 0xE1; // SPBRGL 225;
    SPBRGH = 0x04; // SPBRGH 4;

}

bool EUSART_is_tx_ready(void)
{
    return (bool)(PIR1bits.TXIF && TXSTAbits.TXEN);
}

bool EUSART_is_rx_ready(void)
{
    return (bool)(PIR1bits.RCIF);
}

bool EUSART_is_tx_done(void)
{
    return TXSTAbits.TRMT;
}

uint8_t EUSART_Read(void)
{
    // FIXME maybe remove similar to other usage
    while(!PIR1bits.RCIF){}
    
    if(1 == RCSTAbits.OERR)
    {
        // EUSART error - restart

        RCSTAbits.CREN = 0; 
        RCSTAbits.CREN = 1; 
    }
    receiving = 1;
    return RCREG;
}

void EUSART_Write(uint8_t txData)
{
    // FIXME maybe remove similar to other usage
    while(0 == PIR1bits.TXIF){}

    sending = 1;
    TXREG = txData;    // Write the data byte to the USART.
}

/**
  End of File
*/
