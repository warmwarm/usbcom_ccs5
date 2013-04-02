#include "usbcom.h"
 
extern tBoolean jumpToBootloaderSignaled;

extern Uchar FPGA_CTL;
extern long FPGAStatus;

 
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

		
	   /*
        else
	    if(strcmp(cmd,"lux")==0)
	 	{
	 	   u1printf("lux index %d lux %f\n\r",index,lux);
	 	   lux = lux*100;
	 	   u1buf[0] = index % 256;
		   u1buf[1] = index / 256;
		   u1buf[4] = (char)((int)lux%256);
		   u1buf[3] = (char)((int)lux/256%256);
		   u1buf[2] = (char)((int)lux/256/256);
		   ProcessDataFromLuxmeter(u1buf,6);
	 	}
        else
        	{
        	    index = 0;
				lux = 100;
        	   	 	   lux = lux*100;
	 	   u1buf[0] = index % 256;
		   u1buf[1] = index / 256;
		   u1buf[4] = (char)((int)lux%256);
		   u1buf[3] = (char)((int)lux/256%256);
		   u1buf[2] = (char)((int)lux/256/256);
		   ProcessDataFromLuxmeter(u1buf,6);
        	}
	*/
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
    static unsigned char rvcBuf[100] = {0}; //Jimmy: change1: from 5 to 100.
    unsigned char c;
    static int rcvN=0;
    int cmd = 0;
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
    
    
    
    while(UARTCharsAvail(UART1_BASE))
    {
    	
        //
        // Read the next character from the UART and write it back to the UART.
        //
        //ROM_UARTCharPutNonBlocking(UART1_BASE,ROM_UARTCharGetNonBlocking(UART1_BASE));
        
        c = UARTCharGetNonBlocking(UART1_BASE);
        /*if (c == '\n')
        {
           rvcBuf[0]='A';
           rvcBuf[1]='r';
           rvcBuf[2]='m';
           rvcBuf[3]='s';
           rvcBuf[4]='t';
           rvcBuf[5]='e';
           rvcBuf[6]='l';
           rvcBuf[7]='>';
           rcvN = 8;
        }*/

        rvcBuf[rcvN] = c;
        rcvN++;
        u1printf("%c",c);
        if(c == 13)
        {
        	u1printf("\n\r");
        	cmd = 1;
           break;
        }

    }
    

   /*
    unsigned char DataToMtr[6];
    
    DataToMtr[0]=0;
    DataToMtr[1]=0;
    DataToMtr[2]=0;
    DataToMtr[3]=0;
    DataToMtr[4]=0;
    DataToMtr[5]=0;
    
    MTR_UART0_Send(DataToMtr,6);
      
    */
    //MTR_UART0_Send("data sent to meter",12);
    //TST_UART1_Send(rvcBuf,rcvN);
  
     
   //TST_UART1_Send(rvcBuf,rcvN);  //Jimmy: change3: from commented to uncommented.
   if(1 == cmd)
   	{
   	    cmdprocess(rvcBuf);
		memset(rvcBuf,0,sizeof(rvcBuf));
		rcvN = 0;
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
    DataToMtr[5]=0;
    
    MTR_UART0_Send(DataToMtr,6);	
	
	
}


