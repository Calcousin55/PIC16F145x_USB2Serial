
#ifndef IO_MAPPING_H
#define IO_MAPPING_H

    /*********************************************************************************************************/
    /******** Compile Time Defines  **************************************************************************/
    /*********************************************************************************************************/

    #define INPUT_PIN  1
    #define OUTPUT_PIN 0

    // Compile Time Token-pasting to Generate LATxy
    #define  WRITE(port, bit)  _LAT(port,bit)
    #define _LAT(port, bit)     LAT##port##bit  // LATA0

    // Compile Time Token-pasting to Generate Rxy
    #define  READ(port, bit)   _PORT(port,bit)
    #define _PORT(port, bit)    R##port##bit    // RA0

    // Compile Time Token-pasting to Generate TRISxy
    #define  DIR(port, bit)    _TRIS(port,bit)
    #define _TRIS(port, bit)    TRIS##port##bit // TRISA0

    /******************************************************************************/
    /******** USB stack hardware selection options ********************************/
    /******************************************************************************/
    //
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
    //  sense feature by making sure "USE_USB_BUS_SENSE_IO" has been defined below.
    //
    //  If "USE_USB_BUS_SENSE_IO" is commented out then assuming the +5V VBUS is 
    //  always present (like it would be in a bus powered only application)
    //
    /******************************************************************************/

    //#define USE_USB_BUS_SENSE_IO
    #if defined(USE_USB_BUS_SENSE_IO)

        #define USB_BUS_SENSE_PORT  A
        #define USB_BUS_SENSE_PIN   3

        #define tris_usb_bus_sense  DIR( USB_BUS_SENSE_PORT, USB_BUS_SENSE_PIN)
        #define USB_BUS_SENSE       READ(USB_BUS_SENSE_PORT, USB_BUS_SENSE_PIN)
    #endif

    /*********************************************************************************************************/
    /******** UART optional Pins *****************************************************************************/
    /*********************************************************************************************************/
    //
    // Comment/Uncomment the optional (define USB_CDC_SUPPORT_x) UART signals as desired for application usage.
    // These options are not required for basic Rx/Tx communication.
    // If an option is used below make sure the polarity and pin definitions are set correctly.
    //
    /**********************************************************************************************************/

    // RTS (Ready to Send) - An signal from the USB/CDC host to indicate that its Rx buffer space is available and that the UART device  is free to send data to the USB/CDC host
    // CTS (Clear to Send) - An signal from the UART device  to indicate that its RX buffer space is available and that the USB/CDC host is free to send data to the UART device
    //#define USB_CDC_SUPPORT_HARDWARE_FLOW_CONTROL

    // DSR (Data Set Ready) - A signal from the UART device to the USB/CDC host that indicates that the UART device is ready to receive/send data.
    //#define USB_CDC_SUPPORT_DSR_REPORTING

    // DTR (Data Terminal Ready) - A signal from the USB/CDC host to the UART device that indicates that the USB/CDC host is ready to receive/send data.
    #define USB_CDC_SUPPORT_DTR_SIGNALING

    /** I/O pin Configuration **************************************************************************/

    // Update the pin configuration defines for the GPIO pins that are wanted for the optional UART signals when the respected define above are used.
    #define UART_RTS_PORT  A
    #define UART_RTS_PIN   5

    #define UART_CTS_PORT  A
    #define UART_CTS_PIN   4

    #define UART_DSR_PORT  C
    #define UART_DSR_PIN   2

    #define UART_DTR_PORT  C
    #define UART_DTR_PIN   3

    // Define the logic level for the "active" state.  Setting is only relevant if the respective function is enabled.
    // 1 = Active state logic level is Vdd
    // 0 = Active state logic level is Vss
    #define USB_CDC_RTS_ACTIVE_LEVEL    1
    #define USB_CDC_CTS_ACTIVE_LEVEL    1
    #define USB_CDC_DSR_ACTIVE_LEVEL    1
    #define USB_CDC_DTR_ACTIVE_LEVEL    1

    /** I/O pin definitions **************************************************************************/

    // Set Pin Direction
    #define UART_TRIS_RTS  DIR(UART_RTS_PORT, UART_RTS_PIN)
    #define UART_TRIS_CTS  DIR(UART_CTS_PORT, UART_CTS_PIN)
    #define UART_TRIS_DSR  DIR(UART_DSR_PORT, UART_DSR_PIN)
    #define UART_TRIS_DTR  DIR(UART_DTR_PORT, UART_DTR_PIN)

    // Used by the USB driver to read/write the optional UART signals
    #define UART_RTS       WRITE(UART_RTS_PORT, UART_RTS_PIN) // Set  Output (LAT)
    #define UART_CTS       READ( UART_CTS_PORT, UART_CTS_PIN) // Read Input  (PORT)
    #define UART_DSR       READ( UART_DSR_PORT, UART_DSR_PIN) // Read Input  (PORT)
    #define UART_DTR       WRITE(UART_DTR_PORT, UART_DTR_PIN) // Set  Output (LAT)

    // Used to initialize the pins when the desired optional UART signals are used.
    #define mInitRTSPin() {UART_TRIS_RTS = OUTPUT_PIN;}  //Configure RTS as a digital output.
    #define mInitCTSPin() {UART_TRIS_CTS = INPUT_PIN;}   //Configure CTS as a digital input.
    #define mInitDSRPin() {UART_TRIS_DSR = INPUT_PIN;}   //Configure DSR as a digital input.
    #define mInitDTRPin() {UART_TRIS_DTR = OUTPUT_PIN;}  //Configure DTR as a digital output.

    /*********************************************************************************************************/
    /******** LEDs *******************************************************************************************/
    /*********************************************************************************************************/
    //
    // GPIO pins are used to sinking current through the LEDs by driving the output of the LEDs low.
    // Changing the output to input to not pull current when the LEDs are powered off of a lower supply voltage.
    // Such as if the PIC is powered off of 3.3V but the LEDs are powered off 5V.
    // Note) The LATx for the pin should be 0 for this to work which is so when changing to an output the pin is low.
    // LED Off with Pin as Input (No current draw)  USB/Rx/Tx LEDs
    // LED On with Pin as Output Low (Current draw) USB/Rx/Tx LEDs
    //
    //      ___ Power Supply (VDD, 5V, 3.3V, or desired voltage as long as it doesn't exceed VDD supplied to the PIC)
    //       |
    //       |
    //       /
    //       \  Resistor
    //       /
    //       |
    //      _|_ LED
    //      \ / -->
    //      _V_ -->
    //       |
    //       |
    //      PIN
    //
    /*********************************************************************************************************/

    /** I/O Pin Configuration ********************************************************************************/

    #define LED_Rx_PORT  C
    #define LED_Rx_PIN   0

    #define LED_Tx_PORT  C
    #define LED_Tx_PIN   1

    /** I/O pin definitions **************************************************************************/

    // Set Pin Output Value
    #define LED_LAT_Rx          WRITE(LED_Rx_PORT,  LED_Rx_PIN)
    #define LED_LAT_Tx          WRITE(LED_Tx_PORT,  LED_Tx_PIN)

    // Set Pin Direction
    #define LED_TRIS_Rx         DIR(LED_Rx_PORT,  LED_Rx_PIN)
    #define LED_TRIS_Tx         DIR(LED_Tx_PORT,  LED_Tx_PIN)

    // All LEDs initialized in the off state. Setting driving state to 0 and changing them to inputs
    #define mInitAllLEDs()      LED_LAT_Rx   = 0;         \
                                LED_LAT_Tx   = 0;         \
                                LED_TRIS_Rx  = INPUT_PIN; \
                                LED_TRIS_Tx  = INPUT_PIN;

    #define mGetLED_Rx()        LED_TRIS_Rx
    #define mGetLED_Tx()        LED_TRIS_Tx

    #define LED_On_Value        0
    #define LED_Off_Value       1

    #define mLED_Rx_On()        LED_TRIS_Rx  = LED_On_Value;
    #define mLED_Tx_On()        LED_TRIS_Tx  = LED_On_Value;

    #define mLED_Rx_Off()       LED_TRIS_Rx  = LED_Off_Value;
    #define mLED_Tx_Off()       LED_TRIS_Tx  = LED_Off_Value;

#endif  //IO_MAPPING_H
