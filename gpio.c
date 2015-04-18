#include "usbcom.h"

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
    //end

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_3);
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_RISING_EDGE);
    GPIOPinIntEnable(GPIO_PORTB_BASE, GPIO_PIN_3);
    IntEnable(INT_GPIOB);
    
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x00);
    
   	
	
}

int GetHardwareVesion()
{
   //default vesion is 3(11);pad is 2(10)
   int version = 3;
   version =  GPIOPinRead(GPIO_PORTG_BASE,GPIO_PIN_4|GPIO_PIN_5);
   version = version>>4;
   return version;
}
	

void SelectEEPROM()
{
//	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3,0x0);	
	
}
void DeselectEEPROM()
{
	//GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3,0x08);	
	
}
void EnableTristate()
{
   // tell FPGA to release EEPROM so MCU can access it
   //GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3,0x08);	
	
}

void DisableTristate()
{
	
   //let FPGA to access EEPROM
   //GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3,0x0);
	
}


