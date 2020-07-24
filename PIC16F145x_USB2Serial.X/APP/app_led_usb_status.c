/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license),
please contact mla_licensing@microchip.com
*******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include "../system.h"
#include "../io_mapping.h"
#include "../USB/usb_device.h"
#include "../UART/eusart.h"


// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Constants
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data Types
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Macros or Functions
// *****************************************************************************
// *****************************************************************************

void APP_LEDUpdateUSBStatus(void)
{
    static uint16_t msCount  = 0;

    if(USBIsDeviceSuspended() == true)
    {
        mLED_Tx_Off();
        mLED_Rx_Off();
        return;
    }

    // For more info on the states look at the USB_DEVICE_STATE enum on line 74 in the usb_device.h file
    switch(USBGetDeviceState())
    {
        case CONFIGURED_STATE:
            // Every 50 ms update the UART Tx/ Rx LEDs
            if (msCount >= 50)
            {
                msCount = 0;

                // Rx LED
                if (mGetLED_Rx() == LED_On_Value)
                {
                    mLED_Rx_Off();
                    receiving = 0;
                }
                else if (receiving)
                {
                    mLED_Rx_On();
                }

                // Tx LED
                if (mGetLED_Tx() == LED_On_Value)
                {
                    mLED_Tx_Off();
                    sending = 0;
                }
                else if (sending)
                {
                    mLED_Tx_On();
                }
            }
            break;
            
//        case DETACHED_STATE:
//            break;
            
//        case ATTACHED_STATE:
//            break;
            
//        case POWERED_STATE:
//            break;
            
//        case DEFAULT_STATE:
//            break;
            
//        case ADR_PENDING_STATE:
//            break;
            
//        case ADDRESS_STATE:
//            break;
            
        default:
            break;
    }
    
    /* Increment the millisecond counter. */
    msCount++;
}

/*******************************************************************************
 End of File
*/
