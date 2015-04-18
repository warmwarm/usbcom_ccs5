#include "usbcom.h"
#include "setlut.h" 
#include "i2c.h"
 
 
void InitAmedicomGen2System()
{
    // Set the clocking to run directly from the crystal.
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_12MHZ);
                   
   InitUART1();         

   InitGPIO();
   InitGPIOPB3();
   InitSPI();

   if(ARMSTEL_HARDWARE_DISPLAY_BOARD == GetHardwareVesion())
   	{
   	   //InitUART0();  for test
	   I2C_Initial();
   	}
   else
   	{
   	   I2C_Initial();
   	}

   
   InitAmedicomGen2USBDevice();
   
 // InitTimer();
   
   
   
   IntMasterEnable();


}

void EnterInfiniteLoop()
{
   
   while (1)
   {
   	;
   };    	
	
	
}

