#include "usbcom.h"

void InitGPIO()
{
    //GPIOPortIntRegister(GPIO_PORTB_BASE, GPIOPB3IntHandler);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PINS );
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PINS );


    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_3);
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_RISING_EDGE);
    GPIOPinIntEnable(GPIO_PORTB_BASE, GPIO_PIN_3);
    IntEnable(INT_GPIOB);
    
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x00);
    
   	
	
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


