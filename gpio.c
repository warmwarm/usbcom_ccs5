#include "usbcom.h"
#include "i2c2.h"

void InitGPIO()
{
    //GPIOPortIntRegister(GPIO_PORTB_BASE, GPIOPB3IntHandler);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PINS );
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PINS );

	//add by minghao 2015-04-18
	//get hardware version
	//Pin40 PortG_5; Pin41 PortG_4   
	//Pin41 for version_0; Pin40 fot version_1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, GPIO_PIN_4|GPIO_PIN_5);
	//weak pull-up
	GPIOPadConfigSet(GPIO_PORTG_BASE, GPIO_PIN_4|GPIO_PIN_5,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);
    //end 2015-04-18

	//add by minghao 2015-06-10
	//for pad ,the dicom button changed
    //pin95 PE2 for DICOM button input
    //pin1 PE7 is the FPGA's DICOM signal
    //pin74 PE0 is the FPGA's DICOM state
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2|GPIO_PIN_0);
	//GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6);
	//GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_7,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_OD);
	//GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_6,0xFF);

	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_6);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_6,0xFF);



	//PIN6 is cal in pad
	//end 2015-06-10

	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2|GPIO_PIN_0,0xFF);

    //GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_3);
    //GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_RISING_EDGE);
    //GPIOPinIntEnable(GPIO_PORTB_BASE, GPIO_PIN_3);
    //IntEnable(INT_GPIOB);
    
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x00);
    
    //Version3 need to reset FPGA
    if(3 == GetHardwareVesion())
    {
    	u1printf("init clk\n\r");
    	//GPIOPinConfigure(GPIO_PF1_T0CCP1);

    	//GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_1);
    	//GPIODirModeSet(GPIO_PORTE_BASE,GPIO_PIN_1,GPIO_DIR_MODE_OUT);
    	//GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1,0x00);
    	GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);
    	GPIODirModeSet(GPIO_PORTG_BASE,GPIO_PIN_2,GPIO_DIR_MODE_OUT);
    	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2,0x00);

    	Delay_ms(50);
    	//GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1,0xFF);
    	initclk();
    	Delay_ms(50);
    	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2,0xFF);

    }

}



// 1 for no DICOM
//0 for DICOM
int GetDICOMButton()
{

     int state = 1;
	 state = GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_2);
	 if(state > 0)
	 	{
	 		state = 1;
	 	}
	 return state;
}

//0 for FPGA in DICOM mode
// 1 for FPGA in nomal mode
int GetFPGA_DICOM()
{
    int state = 1;
	state = GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_0);
	if(state > 0)
		{
		   state = 1;
		}
	return state;
}


void ToggleFPGA_DICOM()
{
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_7);

	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7,0x00);
     Delay_ms(5);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_7,0xFF);

    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_7);
}

void DICOM_ON()
{
    int i = 0;
   	for(i = 0;i <5;i++)
   	{
	  if(GetFPGA_DICOM() > 0)
	  {
	   ToggleFPGA_DICOM();
	  }
	  else
	  {
	   break;
	  }
	  Delay_ms(1);
   	}
}

void DICOM_OFF()
{
    int i = 0;
   	for(i = 0;i <5;i++)
   	{
	  if(GetFPGA_DICOM() == 0)
	  {
	    ToggleFPGA_DICOM();
	  }
	  else
	  {
	    break;
	  }
	  Delay_ms(1);
   	}
}


void CheckDICOMButton()
{
    static int oldbutton = 1;
	int button = GetDICOMButton();
	if(oldbutton > button)
		{
		   //normal -> DICOM
		   DICOM_ON();
		   //InfomFPGABeginCalibration();
		}
    if(button > oldbutton)
    	{
    	   //DICOM -> normal
    	   DICOM_OFF();
    	   //InfomFPGAEndCalibration();
    	}
	oldbutton = button;
}


int GetHardwareVesion()
{
   //default vesion is 3(11);pad is 2(10)
   int version = 3;
   version =  GPIOPinRead(GPIO_PORTG_BASE,GPIO_PIN_4|GPIO_PIN_5);
   version = version>>4;
   return version;
}
	




