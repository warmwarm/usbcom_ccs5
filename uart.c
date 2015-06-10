#include "usbcom.h"
#include "ALSensor.h"

 
extern tBoolean jumpToBootloaderSignaled;

extern Uchar FPGA_CTL;
extern long FPGAStatus;

#define UART1_BUFF_LENGTH 128
static int Uart1_indicator;
static int Uart1_rcvN;
static unsigned char Uart1_rvcBuf[UART1_BUFF_LENGTH];

const unsigned char  CRC8_Table[256] = 
{
 0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83,  //8
 0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41, //16
 0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, //24
 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc, //
 0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 
 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62, 
 0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 
 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff, 
 0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 
 0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07, 
 0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 
 0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a, 
 0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 
 0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24, 
 0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b, 
 0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9, 
 0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 
 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd, 
 0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 
 0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50, 
 0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 
 0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee, 
 0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 
 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73, 
 0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 
 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b, 
 0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 
 0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16, 
 0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 
 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8, 
 0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 
 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35 
};
  
  
 
unsigned char CRC8_Tab(unsigned char *p, char counter)  
{
	unsigned char crc8 = 0; 
	char i = 0;
	for(i = 0; i < counter;i++)
	{   
    	crc8 = CRC8_Table[crc8^*p]; //查表得到CRC码  
  		p++; 
	}
	return crc8; 
 
} 


/// connect to sensor
void InitUART0()
{
	

    //MTR_UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);                         
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);                         
                                                                             
    //                                                                       
    // Enable processor interrupts.                                          
    //                                                                       
    // IntMasterEnable();                                                       
                                                                         
    //                                                                       
    // Set GPIO A0 and A1 as UART pins.                                      
    //                                                                       
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);            
                                                                             
    //                                                                       
    // Configure the UART for 115,200, 8-N-1 operation.                      
    //                                                                       
    UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 1200,//115200,
                             (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |     
                             UART_CONFIG_PAR_NONE));                         
                                                                             
    //                                                                       
    // Enable the UART interrupt.                                            
    //                                                                       
              
    
    //TST_UART1_Send((unsigned char *)"UART 0: configured", 18);     
                                                                          
    //MTR_UART0_Send((unsigned char *)"UART 0: testing sending", 23); 
    
    IntEnable(INT_UART0);                                                
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);      
     //MTR_UART0    
    	
	
}
 
 
void InitUART1()
{
	
	//MTR_UART1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);                         
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);                         
                                                                             
    //                                                                       
    // Enable processor interrupts.                                          
    //                                                                       
    // IntMasterEnable();                                                       
                                                                         
    //                                                                       
    // Set GPIO A0 and A1 as UART pins.                                      
    //                                                                       
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);            
                                                                             
    //                                                                       
    // Configure the UART for 115,200, 8-N-1 operation.                      
    //                                                                       
    UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), 115200,//115200,   //Jimmy: change1: from 1200 to 9600.
                             (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |     
                             UART_CONFIG_PAR_NONE));                         
                                                                             
    //                                                                       
    // Enable the UART interrupt.                                            
    //                                                                       
 //   IntEnable(INT_UART1);                                                
 //   UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);                
                                                                             
    //TST_UART1_Send((unsigned char *)"UART 1: configured", 18); 
    
    IntEnable(INT_UART1);                                                
    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);      
    
     //MTR_UART1  
	
	Uart1_indicator = 0;
	Uart1_rcvN = 0;
	memset(Uart1_rvcBuf,0,sizeof(Uart1_rvcBuf));
}

 
void MTR_UART0_Send(const unsigned char *pucBuffer, unsigned int ulCount)
{
 
    while(ulCount--)
        UARTCharPut(UART0_BASE, *pucBuffer++);
}


void TST_UART1_Send(const unsigned char *pucBuffer, unsigned int ulCount)
{
 
    while(ulCount--)
        UARTCharPut(UART1_BASE, *pucBuffer++);
};

 

///add by minghao
///print string to uart1
int u1printf(const char* aStrToPrint, ...)
{
   char buffer[1024];
   int ret = 0;
   va_list args;
   va_start(args, aStrToPrint);

   ret = vsnprintf(buffer,sizeof(buffer),aStrToPrint,args);
   TST_UART1_Send(buffer, (unsigned int)strlen((char *)buffer));
   return ret;
}
 

void print(const unsigned char *pucBuffer)
{

	u1printf("Armstel\\>");
	u1printf("%s\n\r",pucBuffer);
}
void printc(char c)
{
	u1printf("%c",c);

}

/// process debug cmd from uart1
/// just for debug
void cmdprocess(const unsigned char *pucBuffercc)
{
     char cmd[20] = {0};
	 char u1buf[8] = {0};
	 float lux = 0;
	 int index = 0;
	 int i = 0,j=0;
	 int value = 0;
     int addr = 0;
	 sscanf(pucBuffercc,"%s %d %f",cmd,&index,&lux);
	 if(strcmp(cmd,"dicom")==0)
	 	{
	 	   u1printf("dicom %d\n\r",index);
	 	   DICOMbuttonState = index;  
	 	}
      else
	  if(strcmp(cmd,"cal")==0)
	 	{
	 	   u1printf("cal %d\n\r",index);
	 	   CALbuttonState = index;
	 	}
	  else
	   if(strcmp(cmd,"power")==0)
	 	{
	 	   u1printf("power %d\n\r",index);
	 	   PowerbuttonState = index;
	 	}
       else
	   if(strcmp(cmd,"reboot")==0)
	   	 	{
	   	 	   u1printf("reboot!\n\r",index);
	   	 	jumpToBootloaderSignaled = 1;
	   	 	}
	   else
	   	if(strcmp(cmd,"eepread")==0)
	   	{
	   	       InformFPGAReleaseEEPROM();
	   	       EnableTristate(); 
               for(i=0;i<16;i++)
               {
                  //u1printf("\n\r[%04d] ",i*16);
                  for(j=0;j<16;j++)
                  {
                     addr = i*32+j*2;
                     SPIReadByte(HIGH(addr),LOW(addr),&u1buf[0]);
					 SPIReadByte(HIGH(addr+1),LOW(addr+1),&u1buf[1]);
					 value = u1buf[1]*256+u1buf[0];
					 u1printf("%04d\n\r",value);
                  }
               }
			   DisableTristate();
			   InformFPGATakeEEPROM();
			   u1printf("\n\r");
	   	}
	    else
	   	if(strcmp(cmd,"fbegin")==0)
	   	{
	   	       InfomFPGABeginCalibration();
	   	}
		else
	   	if(strcmp(cmd,"fend")==0)
	   	{
	   	      InfomFPGAEndCalibration();
			  ClearCalibrationStatusMsg();
	   	}
		else
	   	if(strcmp(cmd,"fcmsg")==0)
	   	{
	   	       ClearCalibrationStatusMsg();
	   	}
		else
	   	if(strcmp(cmd,"fmsg0")==0)
	   	{
	   	       InfomFPGAGenGivenGrayScale(0,0);
	   	}
		else
	   	if(strcmp(cmd,"fmsg1")==0)
	   	{
	   	       WriteCalibrationStatusMsg1();
	   	}
		else
	   	if(strcmp(cmd,"fmsg2")==0)
	   	{
	   	       WriteCalibrationStatusMsg2();
	   	}
		else
	   	if(strcmp(cmd,"version")==0)
	   	{
	   	       u1printf("MCU Version:[%s]\n\r",MCU_VERSION_STRING);
	   	}
		else
	   	if(strcmp(cmd,"reg")==0)
	   	{
	   	       ReadFPGAStatusRegister();
	   	       u1printf("register:[%x] ctl[%x]\n\r",FPGAStatus,FPGA_CTL);
	   	}
		else
	    if(strcmp(cmd,"cal1")==0)
	   	{
	   	       InformFPGASetGen2Mode();
	    	               InfomFPGABeginCalibration();
	    	               InfomFPGASetMaxBackgroundLevel(255);
	   	}
		else
	    if(strcmp(cmd,"cal2")==0)
	   	{
	   	       SendMaxToLuxmeter(100);
			   u1printf("cal2\n\r");
	   	}
				else
	    if(strcmp(cmd,"cal3")==0)
	   	{
	   	       ClearExistingCalibration();
			   u1printf("cal3\n\r");
	   	}
	    if(strcmp(cmd,"i2c")==0)
	   	{
	   	       ALTest();
	   	}

}
 
//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
 
void
MTR_UART0_IntHandler(void)
{
 
unsigned char rvcBuf[10];
int rcvN=0;
	//print("UART0 Rx");
	 
    unsigned long ulStatus;
	 
 

    //
    // Get the interrupt status.
    //
    ulStatus = UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    UARTIntClear(UART0_BASE, ulStatus);
    
    //IntMasterDisable();
    
    //GrStringDraw(&g_sContext, "UARTIntHandler",        -1, 12, 64, 0);

    //
    // Loop while there are characters in the receive FIFO.
    //
    
    
    
    while(UARTCharsAvail(UART0_BASE))
    {
    	
        //
        // Read the next character from the UART and write it back to the UART.
        //
        //ROM_UARTCharPutNonBlocking(UART1_BASE,ROM_UARTCharGetNonBlocking(UART1_BASE));
        
        rvcBuf[rcvN]=UARTCharGetNonBlocking(UART0_BASE);
        rcvN++;
        
        //print("one byte ");
        
        
    }
    ProcessDataFromLuxmeter(rvcBuf,rcvN);

  /* 
    if (DetermineGen1Gen2Phase==1)
    {
    	DetermineGen1Gen2Phase=0;
    	if (rcvN==1)
    	{
    	  print("Gen1");	
    	  //inform FPGA it's Gen1
    	  InformGPGASetGen1Mode();
    	  return;
    	}
    	else if (rcvN==6)
    	{
    		print("Gen2");
    		//inform FPGA it's Gen2
    		//InformGPGASetGen2Mode();
    		
    		//it's gen 2, we need to control back light
    		 
    		
    		//set timer
    	//	InitTimer();
    		
    		calib_status=CALST_STABLIZATION;
    		

  
    	 	
    		
    		
    		return;
    		 
    	}
    	else
    	   print("Gen Error");
    	
    	
    	
    	
    }
    */
    
  /*  
    lux=(rvcBuf[2]*65536+rvcBuf[3]*256+rvcBuf[4])/100.000;
	ftoa(str_lux,lux);
	print(str_lux); 
 */  
 
   // TST_UART1_Send(rvcBuf,rcvN);
    
     
    
    //ProcessDataFromLuxmeter(rvcBuf,rcvN);
    
  /* 
    //TST_UART1_Send(rvcBuf,rcvN);
    //uart1ruan=1;
   // TST_UART1_Send("uart0 handler called",12);
   
   float lux=(rvcBuf[2]*65536+rvcBuf[3]*256+rvcBuf[4])/100.00;
   
   unsigned char temp[10];
  
   
    
    ftoa(temp,lux);
   TST_UART1_Send(temp,8);
   
   
   
    MTR_UART0_Send(rvcBuf,rcvN);
   
   
   
   */
   
   
  //IntMasterEnable();
    
} 

void
TST_UART1_IntHandler(void)
{
    unsigned long ulStatus;
    unsigned char c;
    //
    // Get the interrupt status.
    //
    ulStatus = UARTIntStatus(UART1_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    ROM_UARTIntClear(UART1_BASE, ulStatus);
    
    //GrStringDraw(&g_sContext, "UARTIntHandler",        -1, 12, 64, 0);

    //
    // Loop while there are characters in the receive FIFO.
    //
    
    if(0 != Uart1_indicator)
    	{
    		return;
    	}
    
    while(UARTCharsAvail(UART1_BASE))
    {
    	
        //
        // Read the next character from the UART and write it back to the UART.
        //
        //ROM_UARTCharPutNonBlocking(UART1_BASE,ROM_UARTCharGetNonBlocking(UART1_BASE));
        if(Uart1_rcvN >= UART1_BUFF_LENGTH)
        {
        	u1printf("\n\r");
        	Uart1_indicator = 1;
        	break;
        }
        c = UARTCharGetNonBlocking(UART1_BASE);
		
        Uart1_rvcBuf[Uart1_rcvN] = c;
        Uart1_rcvN++;
        u1printf("%c",c);
        if(c == 13)
        {
        	u1printf("\n\r");
        	Uart1_indicator = 1;
           break;
        }

    }
    
    //Uart1_ProcessCmd();

	
	
}  

void Uart1_ProcessCmd(void)
{
	if(1 == Uart1_indicator)
   	{
   	    cmdprocess(Uart1_rvcBuf);
		memset(Uart1_rvcBuf,0,sizeof(Uart1_rvcBuf));
		Uart1_rcvN = 0;
		Uart1_indicator = 0;
   	}
	
}


//
void SendRequestToLuxmeter(unsigned char lo, unsigned char hi)
{
    unsigned char DataToMtr[6];
    
    DataToMtr[0]=lo;
    DataToMtr[1]=hi;
    DataToMtr[2]=0;
    DataToMtr[3]=0;
    DataToMtr[4]=0;
    DataToMtr[5]=CRC8_Tab(DataToMtr,5);
    
    MTR_UART0_Send(DataToMtr,6);	
	
	
}

void SendMaxToLuxmeter(unsigned int lux)
{
    unsigned char DataToMtr[6];
    
    DataToMtr[0]=0xFF;
    DataToMtr[1]=0xFF;
    DataToMtr[2]=0x01;
    DataToMtr[3]=0x2C;
    DataToMtr[4]=0x00;
    DataToMtr[5]=CRC8_Tab(DataToMtr,5);
    
    MTR_UART0_Send(DataToMtr,6);	
	
	
}


void SendMinToLuxmeter(unsigned int lux)
{
    unsigned char DataToMtr[6];
    
    DataToMtr[0]=0xFF;
    DataToMtr[1]=0xFE;
    DataToMtr[2]=0x00;
    DataToMtr[3]=0x00;
    DataToMtr[4]=0x00;
    DataToMtr[5]=CRC8_Tab(DataToMtr,5);
    
    MTR_UART0_Send(DataToMtr,6);	
	
	
}



