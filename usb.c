#include "usbcom.h"


//*****************************************************************************
//
// The languages supported by this device.
//
//*****************************************************************************
const unsigned char g_pLangDescriptor[] =
{
4,
USB_DTYPE_STRING,
USBShort(USB_LANG_EN_US)
};

//*****************************************************************************
//
// The manufacturer string.
//
//*****************************************************************************
const unsigned char g_pManufacturerString[] =
{
    (17 + 1) * 2,
    USB_DTYPE_STRING,
    'A', 0, 'r', 0, 'm', 0, 's', 0, 't', 0, 'e', 0, 'l', 0, ' ', 0, 'A', 0,
    'm', 0, 'e', 0, 'd', 0, 'i', 0, 'c', 0, 'o', 0, 'm', 0, '.', 0,
};


//*****************************************************************************
//
// The product string.
//
//*****************************************************************************
const unsigned char g_pProductString[] =
{
    (13 + 1) * 2,
    USB_DTYPE_STRING,
    'A', 0, 'e', 0, 'd', 0, 'i', 0, 'c', 0, 'o', 0, 'm', 0, ' ', 0, 'G', 0,
    'e', 0, 'n', 0, '2', 0, '.', 0
};

//*****************************************************************************
//
// The serial number string.
//
//*****************************************************************************
const unsigned char g_pSerialNumberString[] =
{
    (8 + 1) * 2,
    USB_DTYPE_STRING,
    '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '2', 0
};

//*****************************************************************************
//
// The interface description string.
//
//*****************************************************************************
const unsigned char g_pHIDInterfaceString[] =
{
    (13 + 1) * 2,
    USB_DTYPE_STRING,
    'H', 0, 'I', 0, 'D', 0, ' ', 0, 'I', 0, 'n', 0, 't', 0, 'e', 0, 'r', 0, 'f', 0,
    'a', 0, 'c', 0, 'e', 0
};

//*****************************************************************************
//
// The configuration description string.
//
//*****************************************************************************
const unsigned char g_pConfigString[] =
{
    (16 + 1) * 2,
    USB_DTYPE_STRING,
    'H', 0, 'I', 0, 'D', 0, 'C', 0, 'o', 0, 'n', 0, 'f', 0, 'i', 0, 'g', 0,
    'u', 0, 'r', 0, 'a', 0, 't', 0, 'i', 0, 'o', 0, 'n', 0
};

//*****************************************************************************
//
// The descriptor string table.
//
//*****************************************************************************
const unsigned char * const g_pStringDescriptors[] =
{
g_pLangDescriptor,
g_pManufacturerString,
g_pProductString,
g_pSerialNumberString,

g_pHIDInterfaceString,
g_pConfigString
};
#define NUM_STRING_DESCRIPTORS (sizeof(g_pStringDescriptors) / sizeof(unsigned char *))

//*****************************************************************************
//
// The HID device private data.
//
//*****************************************************************************
tHIDInstance g_sHIDInstance;

static const unsigned char g_pucAmedicomGen2ReportDescriptor[]=
{
    0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined Page 1)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
                                   // ---- common globals ----
    0x15, 0x00,                    //   LOGICAL_MINIMUM (-128)
    0x25, 0xFF,                    //   LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //   REPORT_SIZE (8)

                                   // ---- input report ----
    0x95, 0x22,                    //   REPORT_COUNT (8)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs) 

                                   // ---- output report ----
    0x95, 0x22,                    //   REPORT_COUNT (34)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0x91, 0x00,                    //   OUTPUT (Data,Var,Abs) 
    0xc0                           // END_COLLECTION
};




 /*

static const unsigned char g_pucAmedicomGen2ReportDescriptor[]=
{
    0x05, 0x01, //0xff,               UsagePage(USB_HID_GENERIC_DESKTOP),
    0x09, 0x00,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
   // 0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (-128)
    0x25, 0xFF,                    //   LOGICAL_MAXIMUM (127)
    0x19, 0x01,                    //     USAGE_MINIMUM (1)
    0x29, 0x08,                    //     USAGE_MAXIMUM (8)    
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x02,                    //   INPUT (Data,Ary,Abs)
  
    0x19, 0x01,                    //     USAGE_MINIMUM (1)
 
 
     0x29, 0x08,                   //     USAGE_MAXIMUM (8) 
 
    0x91, 0x02,                    //   OUTPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};
 */
//*****************************************************************************
//
// The HID class descriptor table.  For the mouse class, we have only a single
// report descriptor.
//
//*****************************************************************************
static const unsigned char * const g_pAmedicomGen2ClassDescriptors[] =
{
    g_pucAmedicomGen2ReportDescriptor
};

//*****************************************************************************
//
// The HID descriptor for the mouse device.
//
//*****************************************************************************
static const tHIDDescriptor g_sAmedicomGen2HIDDescriptor =
{
    9,                                 // bLength
    USB_HID_DTYPE_HID,                 // bDescriptorType
    0x110,                             // bcdHID (version 1.11 compliant)
    0,                                 // bCountryCode (not localized)
    1,                                 // bNumDescriptors
    {
        {
            USB_HID_DTYPE_REPORT,                  // Report descriptor
            sizeof(g_pucAmedicomGen2ReportDescriptor)     // Size of report descriptor
        }
    }
};

tHIDReportIdle g_psReportIdle[1] =
{
{ 125, 0, 0, 0 }//, // Report 0 polled every 500mS (4 * 125).
 
};


extern long usbIsPluged;

static unsigned long
HIDAmedicomGen2TxHandler(void *pvCBData, unsigned long ulEvent,
                  unsigned long ulMsgData, void *pvMsgData)
{
	
    switch (ulEvent)
    {
    	case USB_EVENT_TX_COMPLETE:
        {
        	//print("usb tx: sent complete");
        	//USBDHIDReportWrite(&g_sHIDAmedicomGen2Device,tx,8,0);
        	//Delay(10);
        	break;
        }
    	
    }	
    return 0;
}

unsigned char rev_buf[35];  //use 35 to avoid memory access violation.

static unsigned long
HIDAmedicomGen2RxHandler(void *pvCBData, unsigned long ulEvent,
                  unsigned long ulMsgData, void *pvMsgData)
{
   //tHIDMouseInstance *psInst;
   // tUSBDHIDMouseDevice *psDevice;

    //
    // Make sure we didn't get a NULL pointer.
    //
	if(NULL == pvCBData)
	{
		return 0;
	}
    //ASSERT(pvCBData);

    //
    // Get a pointer to our instance data
    //
   // psDevice = (tUSBDHIDMouseDevice *)pvCBData;
   // psInst = psDevice->psPrivateHIDMouseData;

    //
    // Which event were we sent?
    //
    switch (ulEvent)
    {
        //
        // The host has connected to us and configured the device.
        //
        case USB_EVENT_CONNECTED:
        {
          //  psInst->ucUSBConfigured = true;
           
            print("usb connected!");
		    usbIsPluged=1;
            break;
        }

        //
        // The host has disconnected from us.
        //
        case USB_EVENT_DISCONNECTED:
        {
          //  psInst->ucUSBConfigured = false;
          //JumpToDFU();
          //(*((void (*)(void))(*(unsigned long *)0x2c)))();

          print("usb disconnected!");  

          break;
        }

        //
        // The host is polling us for a particular report and the HID driver
        // is asking for the latest version to transmit.
        //
        case USBD_HID_EVENT_IDLE_TIMEOUT:
        case USBD_HID_EVENT_GET_REPORT:
        {
            //
            // We only support a single input report so we don't need to check
            // the ulMsgValue parameter in this case.  Set the report pointer
            // in *pvMsgData and return the length of the report in bytes.
            //
            //print(" GET_REPORT ");
            *(unsigned char **)pvMsgData = tx;
            return (8);
        }

        //
        // The device class driver has completed sending a report to the
        // host in response to a Get_Report request.
        //
        case USBD_HID_EVENT_REPORT_SENT:
        {
            //
            // We have nothing to do here.
            //
            //print(" REPORT_SENT ");
            break;
        }
        
        case USB_EVENT_TX_COMPLETE:
        {
        	//print("sent complete");
        	
        	break;
        	
        	
        	
        }

        //
        // This event is sent in response to a host Set_Report request.  The
        // mouse device has no output reports so we return a NULL pointer and
        // zero length to cause this request to be stalled.
        //
 

        //
        // The host is asking us to set either boot or report protocol (not
        // that it makes any difference to this particular mouse).
        //
        case USBD_HID_EVENT_SET_PROTOCOL:
        {
        //    psInst->ucProtocol = ulMsgData;
            print("set protocol."); 
            break;
        }

        //
        // The host is asking us to tell it which protocol we are currently
        // using, boot or request.
        //
        case USBD_HID_EVENT_GET_PROTOCOL:
        {
           // return (psInst->ucProtocol);
            print("get protocol."); 
            break; 
        }

        //
        // Pass ERROR, SUSPEND and RESUME to the client unchanged.
        //
        case USB_EVENT_ERROR:
        {
            print("USB_EVENT_ERROR event");
            break;
        }
        case USB_EVENT_SUSPEND:
        {
        	print("USB_EVENT_SUSPEND event");
			usbIsPluged=0;  ///add by minghao .indicate usb pluging status
        	break;
        }
        case USB_EVENT_RESUME:
        {
        	print("USB_EVENT_RESUME event.");
        	break;

        }
        case USB_EVENT_RX_AVAILABLE:
        {
        	 
        	//TST_UART1_Send((unsigned char *)"something ", 10);
            print("rx available."); 
        	break;
        	
        }
        case USBD_HID_EVENT_GET_REPORT_BUFFER:
        {
            //
            // We are being asked for a report that does not exist for
            // this device.
            //
           
           
            //UARTSend((unsigned char *)"get buf:", 8);
           
            
            //
            
            print("usb get report buffer."); 
            //*(unsigned char **)pvMsgData = (void *)rev_buf;
            return (unsigned long)rev_buf;
        }
        
        case USBD_HID_EVENT_SET_REPORT:
        {
        	/*
        	 * Jimmy: for debugging.
        	 * int i;
        	char p[5];
        	print("usb rx handler called start.."); 
        	for (i=0; i<34; i++)
        	{ 
        	   itoa(p,  rev_buf[i]);
        	   print(p);
        	}*/
        	usbRxHandler(rev_buf);
        }

        //
        // We ignore all other events.
        //
        default:
        {
            break;
        }
    }
    return (0);
}
	
                  	
const tUSBDHIDDevice g_sHIDAmedicomGen2Device =
{
//
// The Vendor ID you have been assigned by USB-IF.
//
    VENDOR_ID,
 
//
// The product ID you have assigned for this device.
//
 
    PRODUCT_ID,
//
// The power consumption of your device in milliamps.
//
0,//POWER_CONSUMPTION_mA,
//
// The value to be passed to the host in the USB configuration descriptor’s
// bmAttributes field.
//
USB_CONF_ATTR_SELF_PWR,   //change to self powered type
//
// This mouse supports the boot subclass.
//
USB_HID_SCLASS_NONE,//USB_HID_SCLASS_BOOT,
//
// This device supports the BIOS mouse report protocol.
//
USB_HID_PROTOCOL_NONE,//USB_HID_PROTOCOL_MOUSE,
//
// The device has a single input report.
//
1,
//
// A pointer to our array of tHIDReportIdle structures. For this device,
// the array must have 1 element (matching the value of the previous field).
//
g_psReportIdle,
//
// A pointer to your receive callback event handler.
//
HIDAmedicomGen2RxHandler,
//
// A value that you want passed to the receive callback alongside every
// event.
//
(void *)&g_sHIDInstance,
//
// A pointer to your transmit callback event handler.
//
HIDAmedicomGen2TxHandler,
//
// A value that you want passed to the transmit callback alongside every
// event.
//
(void *)&g_sHIDInstance,

//
// This device does not want to use a dedicated interrupt OUT endpoint
// since there are no output or feature reports required.
//
false,
//
// A pointer to the HID descriptor for the device.
//
&g_sAmedicomGen2HIDDescriptor,
//
// A pointer to the array of HID class descriptor pointers for this device.
// The number of elements in this array and their order must match the
// information in the HID descriptor provided above.
//
g_pAmedicomGen2ClassDescriptors,
//
// A pointer to your string table.
//
g_pStringDescriptors,
//
// The number of entries in your string table. This must equal
// (1 + (5 + (num HID strings)) * (num languages)).
//
NUM_STRING_DESCRIPTORS,
//
// A pointer to the private instance data allocated for the class driver to
// use.
//
&g_sHIDInstance
};

void InitAmedicomGen2USBDevice()
{
	
    //USB
    // Enable the USB mux GPIO.
    //
    SysCtlPeripheralEnable(USB_MUX_GPIO_PERIPH);

    //
    // The LM3S3748 board uses a USB mux that must be switched to use the
    // host connector and not the device connecter.
    //
    GPIOPinTypeGPIOOutput(USB_MUX_GPIO_BASE, USB_MUX_GPIO_PIN);
    GPIOPinWrite(USB_MUX_GPIO_BASE, USB_MUX_GPIO_PIN, USB_MUX_SEL_DEVICE);
   
 
   
   USBDHIDInit(0, &g_sHIDAmedicomGen2Device);   
   
	
};


