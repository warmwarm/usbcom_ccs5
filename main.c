
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ssi.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/ssi.h"
#include "utils/softssi.h"
#include "driverlib/usb.h"
#include "grlib/grlib.h"
#include "usblib/usblib.h"
#include "usblib/usbhid.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdhid.h"
#include "usblib/device/usbdhidmouse.h"
#include "utils/uartstdio.h"
#include "drivers/buttons.h"
#include "drivers/formike128x128x16.h"
#include "usbcom.h"

int calib_status=CALST_NIL;
volatile tBoolean jumpToBootloaderSignaled=false;
extern int MCUOpMode;
extern long usbIsPluged;
extern int gUsbGetLux;

int main(void)
{
	unsigned int mindex = 0;
	gUsbGetLux = -1;
   ROM_SysTickPeriodSet(ROM_SysCtlClockGet());
   ROM_SysTickEnable();	
	
   calib_status=CALST_INIT;
   InitAmedicomGen2System();
   u1printf("Start1\n\r");
  
   Delay(1);
   u1printf("Start2\n\r");
   Delay(1);

   SendRequestToLuxmeter(0,0);
   Delay(1);
   u1printf("Start3\n\r");

   ///add by Minghao
   backlightControlLevel = 255;
   while(!jumpToBootloaderSignaled)
   {

     ///when usb pluged,change mode to Gen1
	 if(usbIsPluged&&(MCUOpMode==MCU_OP_MODE_GEN2))
	 {
	    ClearExistingCalibration();
		InformFPGASetGen1Mode();
	 }
	 Delay(5);
   }
   u1printf("jump to boot loader\n\r");
   JumpToDFU();

   EnterInfiniteLoop();	
}


 







