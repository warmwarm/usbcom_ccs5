#include "usbcom.h"

void InitTimer()
{
    //
    // Enable the peripherals used by this example.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
 
    IntMasterEnable();
    // Configure the two 32-bit periodic timers.
    //
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);
   
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()*60*10*3);//set timer to half hour
    
    
 

    //
    // Setup the interrupts for the timer timeouts.
    //
    ROM_IntEnable(INT_TIMER0A);
 
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
 

    //
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);
    //
    
 
	
	
	
}

void Timer0IntHandler(void)
{
    //
    // Clear the timer interrupt.
    //
    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Toggle the flag for the first timer.
    //
    

    //
    // Update the interrupt status on the display.
    //
    IntMasterDisable();
    
    
    
    
    //InformFPGASetGen2Mode();
    print("timer pops");
    
    increase_time_duration();
    // if it's dicom mode, then calculate the timer
   // InfomFPGABeginCalibration(); 
     
    //InfomFPGASetMaxBackgroundLevel(255);
    //InfomFPGAGenGivenGrayScale(255,7);
    	    
     
    //inform lux meter to calibrate
    //SendRequestToLuxmeter(0,0);
    
    
    IntMasterEnable();
}



