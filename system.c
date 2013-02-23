#include "usbcom.h"
#include "setlut.h" 
 
 
void InitAmedicomGen2System()
{
    // Set the clocking to run directly from the crystal.
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_12MHZ);
                   
   InitUART1();         

         
   InitUART0();
   
 

    InitGPIO();
  InitGPIOPB3();

  InitSPI();

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

