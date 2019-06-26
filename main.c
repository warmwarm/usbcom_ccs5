
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
#include "i2c.h"


int calib_status=CALST_NIL;
int usb_cal=0;

volatile tBoolean jumpToBootloaderSignaled=false;
extern int MCUOpMode;
extern long usbIsPluged;
volatile extern int gUsbGetLux;

int main(void)
{
	Delay_ms(150);
	gUsbGetLux = -1;
   ROM_SysTickPeriodSet(ROM_SysCtlClockGet());
   ROM_SysTickEnable();	
	
   calib_status=CALST_INIT;
   InitAmedicomGen2System();

   u1printf("Armstel firmware bootup\n\r");

   //SendRequestToLuxmeter(0,0);

   u1printf("hardware %d\n\r",GetHardwareVesion());
   
   InformFPGASetGen1Mode();
   
   ///add by Minghao
   backlightControlLevel = 255;

   InformFPGAReleaseEEPROM();
   EnableTristate();
   Delay_ms(2000);//delay for pad to power up
   DisableTristate();
   InformFPGATakeEEPROM();
   while(!jumpToBootloaderSignaled)
   {
	 //Delay(5);
	 CheckDICOMButton();
	 //Uart1_ProcessCmd();
   }
   u1printf("jump to boot loader\n\r");
   Delay(50);
   JumpToDFU();

   EnterInfiniteLoop();	
}


 







