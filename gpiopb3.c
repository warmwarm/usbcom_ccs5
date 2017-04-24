#include "usbcom.h"

//PIN B7 is used to trigger the calibration
void InitGPIOPB3()
{

     
}

 
long needCalibration=0;

extern int bl_low_tested_num;
extern int bl_high_tested_num;
extern volatile long FPGACtrlBitSet;
extern float stableLuxUpper;
extern float stableLuxLower;
extern long usbIsPluged;

long DICOMbuttonState=0;
long CALbuttonState=0;
long PowerbuttonState=0;




void GPIOPB3IntHandler(void)
{
    //
    // Clear the GPIO interrupt.
 
    GPIOPinIntClear(GPIO_PORTB_BASE, GPIO_PIN_7);
    //GPIOPinIntDisable(GPIO_PORTB_BASE, GPIO_PIN_3);

    u1printf("GPIOPB3IntHandler: \n\r");
    
    ReadFPGAStatusRegister(); 
    
    if (DICOMButtonPressed())
    	DICOMbuttonState=1;
    else
    	DICOMbuttonState=0;
    
    CALbuttonState=0;
    /*
    if (CALButtonPressed())
    {
    	CALbuttonState=1;
    	//print("CAL: write fpga ctrl bit");
    	//InfomFPGABeginCalibration();
    }
	else
	{
	    CALbuttonState=0;      //add by Minghao
	}*/
    
    if(PoweronPressed())
    {
    	PowerbuttonState=1;      
    }
    else
    {
        stableLuxUpper=0;     //add by Minghao
        stableLuxLower=0;	    //when powerup  clear Lux refrence
		backlightControlLevel=255;	
    	PowerbuttonState=0;
    }
        
    //get interrupt, check currect status
    if(calib_status==CALST_INIT)
    {
      if ((CALbuttonState))
      {
        InformFPGASetGen2Mode();
    	calib_status=CALST_STABLIZATION;
        ///add by Minghao
        ///cal mode should inform FPGA to start calibration
        ///else can not control the backlight
		if(CALbuttonState)         
	    {  
	       WriteCalibrationStatusMsg0();
		   InfomFPGABeginCalibration();
		   FPGACtrlBitSet=1;
	    }
        ///reset the background level
	    InfomFPGASetMaxBackgroundLevel(backlightControlLevel);
		Delay(3);
    	InfomFPGAGenGivenGrayScale(255,7);
		Delay(3);
        ///judge if already have the LUX reference
		if((0 != stableLuxUpper)&&(0 != stableLuxLower))
		{
           ///jump to backlight control
		   calib_status=CALST_BL_CTRL;
		}
		//backlightControlLevel=255;

		SendRequestToLuxmeter(255,7);
    	u1printf("start a whole process\n\r");
    	return;
      }   	 	
    }
    else
    {
    	//already runing something
    	///add by Minghao
        if(PowerbuttonState==0)
		{
			
			ClearExistingCalibration();			
			//tell fpga to enter gen1 mode
			//if (usbIsPluged==1)
			//    InformFPGASetGen1Mode();
			
		}
		if (DICOMbuttonState==0&&CALbuttonState==0)
		{
			ClearExistingCalibration();
		}   	
    }
    

  
}
