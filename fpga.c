#include "usbcom.h"
extern int MCUOpMode;   ///add by Minghao

Uchar FPGA_CTL=0x00;
long FPGAStatus=0;
long ReadFPGAStatusRegister()
{

  long u=3;

  GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PINS );


  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x84);
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0xC4);

  //read data
  u=  GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2);
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x00);

  GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PINS );

  //TST_UART1_Send(&u,2);
  FPGAStatus=u;
  if(FPGAStatus > 0)
  	{
  		u1printf("FPGAStatus :%x\n\r",FPGAStatus);
  	}
  
  return 1;
}



long DICOMButtonPressed()
{
  return FPGAStatus&0x01;
}

long CALButtonPressed()
{
  return FPGAStatus&0x02;
}

long PoweronPressed()
{
  return FPGAStatus&0x04;
}

void InfomFPGABeginCalibration()
{
u1printf("InfomFPGABeginCalibration\n\r");
  //P0=0;
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x00);

  FPGA_CTL = FPGA_CTL | 0x01;

  //P2=1;
  GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PINS, FPGA_CTL);

  //P0_6=1;
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0xFF);

  //P0_6=0;
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0x00);
}


void InfomFPGAGenGivenGrayScale(unsigned char lo, unsigned char hi)
{
u1printf("InfomFPGAGenGivenGrayScale\n\r");
  // P0=1;
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x01);

  // P2=drive_level_lo_byte;
  GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PINS,lo);

  //P0_6=1;
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0xFF);
   
  //P0_6=0;
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0x00);
  
  // P0=2;
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x02);

  // P2=drive_level_hi_byte;
  GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PINS,hi);

  //P0_6=1;
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0xFF);

  //P0_6=0;
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0x00);

}

void InfomFPGAEndCalibration()
{
u1printf("InfomFPGAEndCalibration\n\r");
   FPGA_CTL = FPGA_CTL & 0xFE;
   //P0=0;
   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x0);

   //P2=1;
   GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PINS, FPGA_CTL);

   //P0_6=1;
   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0xFF);

   //P0_6=0;
   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0x0);
}

// percentage: set background light to the percentage of existing back lignt.
// for example if existing max limuinance is 400, we want to decrease it to 320,
// then we call InfomFPGASetMaxBackgroundLevel(80) because 4000%=320.
// Acceptable input value of percentage is from 0 to 200.
void InfomFPGASetMaxBackgroundLevel(unsigned char level)
{
u1printf("InfomFPGASetMaxBackgroundLevel\n\r");
  //Ray, Pease finish this function implementation
  // P0=2;
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x03);

  // P2=drive_level_hi_byte;
  GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PINS,level);

  //P0_6=1;
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0xFF);

  //P0_6=0;
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0x00);

}

//after all calibration done, inform FPGA to release the button,
//so user can push it to calibrate again

void InformFPGASetGen1Mode()
{   
	  u1printf("V1 mode\n\r");
	  MCUOpMode=MCU_OP_MODE_GEN1;  ///add by Minghao  change MCU mode flag
	  FPGA_CTL=FPGA_CTL & 0xFD;
    //P0=0;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x00);

    //P2=1;
    GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PINS, FPGA_CTL);

    //P0_6=1;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0xFF);

    //P0_6=0;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0x00);
}

void InformFPGASetGen2Mode()
{
   u1printf("V2 mode\n\r");
	MCUOpMode=MCU_OP_MODE_GEN2;  ///add by Minghao change MCU mode flag
    FPGA_CTL=FPGA_CTL |  0x02;
    //P0=0;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x00);
    
    //P2=1;
    GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PINS, FPGA_CTL);
    
    //P0_6=1;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0xFF);
    
    
    //P0_6=0;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0x00);
}

void InformFPGAReleaseEEPROM()
{
u1printf("InformFPGAReleaseEEPROM\n\r");
   FPGA_CTL=FPGA_CTL |  0x04;

   //P0=0;
   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x00);

   //P2=1;
   GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PINS, FPGA_CTL);

   //P0_6=1;
   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0xFF);

   //P0_6=0;
   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0x00);
}

void InformFPGATakeEEPROM()
{
u1printf("InformFPGATakeEEPROM\n\r");
   FPGA_CTL=FPGA_CTL &  0xFB;
   //P0=0;
   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x00);

   //P2=1;
   GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PINS, FPGA_CTL);

   //P0_6=1;
   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0xFF);

   //P0_6=0;
   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0x00);
}


void InformFPGAReleasePushbutton()
{

}

//below function are used to control the screen message display
void WriteCalibrationStatusMsg0()
{
u1printf("WriteCalibrationStatusMsg0\n\r");
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x05);
  
  GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PINS,0xFB);

  //P0_6=1;
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0xFF);

  //P0_6=0;
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0x00);

}

void WriteCalibrationStatusMsg1()
{
 u1printf("WriteCalibrationStatusMsg1\n\r");
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x05);

  GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PINS,0xFE);
  //P0_6=1;
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0xFF);
  
  //P0_6=0;
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0x00);
}

void WriteCalibrationStatusMsg2()
{ 
u1printf("WriteCalibrationStatusMsg2\n\r");
   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x05);

   GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PINS,0xFD);

   //P0_6=1;
   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0xFF);

   //P0_6=0;
   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0x00);
}

void ClearCalibrationStatusMsg()
{
u1printf("ClearCalibrationStatusMsg\n\r");
   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PINS,0x05);

   GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PINS,0xFF);

   //P0_6=1;
   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0xFF);
   
   //P0_6=0;
   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, 0x00);
}


void WriteFPGAData(char da)
{

   GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PINS,da);
}


