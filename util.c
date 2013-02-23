#include "usbcom.h"
void itoa(char *p, int i)
{
	int len;
	int j;
	char t;
	char *a=p;
	if (i==0)
	{
		*p='0';
		*(p+1)='\0';
		return;
	}	
	while(i)
	{
		*a=(i%10)+'0';
		a++;
		i/=10;
	}

	*a='\0';
	
	a=p;
	len =strlen(a);
	
	
	for(j=0;j<len/2;j++)
	{
		t=a[j];
		*(a+j)=a[len-1-j];
		*(a+len-1-j)=t;
	}
}

void ftoa( char *p, float f)
{
	 
	int in;
	 
	char *a;
	int len;
	int count=0;
	
	itoa(p,f);
	
	len=strlen(p);
	a=p+len;
	*a='.';
	a++;
	
	f-=(int)(f);
	
	while(count<8)
	{
		in=(f*10);
		*a=in+'0';
		f=f*10-in;
		a++;
		count++;
		
		
	}
	*a='\0';
}

float JND_to_LUMINANCE(float JND)
{

	float a = (float)-1.3011877, 
		  b = (float)-2.5840191E-2, 
		  c = (float) 8.0242636E-2, 
		  d = (float)-1.0320229E-1, 
		  e = (float) 1.3646699E-1, 
		  f = (float)2.8745620E-2, 
		  g = (float)-2.5468404E-2, 
		  h = (float)-3.1978977E-3,  
		  k =(float) 1.2992634E-4, 
		  m = (float)1.3635334E-3;
  double lnJND=log(JND);

return (float)pow(10,
		  (a+c*lnJND+e*pow(lnJND,2)+g*pow(lnJND,3)+m*pow(lnJND,4))
           /
	      (1+b*pow(lnJND,1)+d*pow(lnJND,2)+f*pow(lnJND,3)+h*pow(lnJND,4)+k*pow(lnJND,5))
		  );
}

float LUMINANCE_to_JND(float LUMINANCE)
{
float A =(float) 71.498068, 
      B = (float)94.593053, 
	  C = (float)41.912053, 
	  D = (float)9.8247004, 
	  E = (float)0.28175407, 
	  F = (float)-1.1878455, 
	  G = (float)-0.18014349, 
	  H = (float)0.14710899, 
	  I = (float)- 0.017046845;
float ten=10.0;

double ln_LUM,ln_10,log10LUM;

	if (LUMINANCE<=0)
		LUMINANCE=0.1;

ln_LUM=(log(LUMINANCE));
ln_10=(log(ten));
log10LUM=ln_LUM/ln_10;

return (float)(A+B*pow(log10LUM,1)+C*pow(log10LUM,2)+D*pow(log10LUM,3)+E*pow(log10LUM,4)
        +F*pow(log10LUM,5)+G*pow(log10LUM,6)+H*pow(log10LUM,7)+I*pow(log10LUM,8));
}
extern tUSBDHIDDevice g_sHIDAmedicomGen2Device;
void JumpToDFU()
{
	    //
    // Shut down the current USB device and get it off the bus.
    //
    USBDHIDTerm(&g_sHIDAmedicomGen2Device);

    //
    // Disable all interrupts.
    //
    IntMasterDisable();

    //
    // We must make sure we turn off SysTick and its interrupt
    // before entering the boot loader!
    //
    SysTickIntDisable();
    SysTickDisable();

    //
    // Disable all processor interrupts.  Instead of disabling them
    // one at a time, a direct write to NVIC is done to disable all
    // peripheral interrupts.
    //
    HWREG(NVIC_DIS0) = 0xffffffff;
    HWREG(NVIC_DIS1) = 0xffffffff;

    //
    // Reset the USB peripheral
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_USB0);
    SysCtlPeripheralReset(SYSCTL_PERIPH_USB0);
    SysCtlPeripheralDisable(SYSCTL_PERIPH_USB0);




    //
    // Re-enable all interrupts.
    //
    IntMasterEnable();

    //
    // Return control to the boot loader.  This is a call to the SVC
    // handler in the boot loader.
    //
    (*((void (*)(void))(*(unsigned long *)0x2c)))();
	
}
void
Delay(unsigned long ulSeconds)
{
    //
    // Loop while there are more seconds to wait.
    //
    while(ulSeconds--)
    {
        //
        // Wait until the SysTick value is less than 1000.
        //
        while(ROM_SysTickValueGet() > 1000)
        {
        }

        //
        // Wait until the SysTick value is greater than 1000.
        //
        while(ROM_SysTickValueGet() < 1000)
        {
        }
    }
}



void increase_time_duration()
{//we have 3 registers: V1(1024-1025);V2(1026-1027);V3(1028-1029)
	
            	 
    Uint16 Addr;
    Uchar value_high_byte;	
    Uchar value_low_byte;		
	unsigned long V1,V2,V3;
	
	unsigned long newV;

    Addr=1024;                       
    SPIReadByte(HIGH(Addr),LOW(Addr),&value_high_byte);
    
    Addr=1025;                       
    SPIReadByte(HIGH(Addr),LOW(Addr),&value_low_byte);
    
    V1=value_high_byte*65535+value_low_byte;
    
    
    Addr=1026;                       
    SPIReadByte(HIGH(Addr),LOW(Addr),&value_high_byte);
    
    Addr=1027;                       
    SPIReadByte(HIGH(Addr),LOW(Addr),&value_low_byte);
    
    V2=value_high_byte*65535+value_low_byte;
    
    Addr=1028;                       
    SPIReadByte(HIGH(Addr),LOW(Addr),&value_high_byte);
    
    Addr=1029;                       
    SPIReadByte(HIGH(Addr),LOW(Addr),&value_low_byte);
    
    V3=value_high_byte*65535+value_low_byte;
        
 
 //WE got all three register values,
 
   if ((V1==V2)&&(V2==V3)) //ALL good  
   {
   	newV=V1+1;
   	
   	
   	
   }
   else if (V1==V2)//v3 crash
   {
   	newV=V1+1;
   	
   	
   }
   else if (V2==V3)//v1 crash
   {
   	newV=V2+1;
   	
   	
   }
   else //V1!=V2!=V3, v2 crash
   {
   	newV=V1+1;
   	
   	
   }
   
   value_high_byte=HIGH(newV);
   value_low_byte=LOW(newV);   
   
   //write newV to V1,V2,V3
   
   //write V1
   Addr=1024;      
   SPIWriteByte(HIGH(Addr),LOW(Addr), value_high_byte);
   
   Addr=1025;      
   SPIWriteByte(HIGH(Addr),LOW(Addr), value_low_byte);
    
   
   
   
   
   //write V2
    Addr=1026;      
   SPIWriteByte(HIGH(Addr),LOW(Addr), value_high_byte);
   
   Addr=1027;      
   SPIWriteByte(HIGH(Addr),LOW(Addr), value_low_byte);
    
   
     
   
   
   
   
   //write V3
   
    Addr=1028;      
   SPIWriteByte(HIGH(Addr),LOW(Addr), value_high_byte);
   
   Addr=1029;      
   SPIWriteByte(HIGH(Addr),LOW(Addr), value_low_byte);
    
  
        
                               	
	
}

void reset_life_time()
{
   Uint16 Addr;
   Uchar value_high_byte=0;	
   Uchar value_low_byte=0;		  //write V1
   
   Addr=1024;      
   SPIWriteByte(HIGH(Addr),LOW(Addr), value_high_byte);
   
   Addr=1025;      
   SPIWriteByte(HIGH(Addr),LOW(Addr), value_low_byte);
    
   
   
   
   
   //write V2
    Addr=1026;      
   SPIWriteByte(HIGH(Addr),LOW(Addr), value_high_byte);
   
   Addr=1027;      
   SPIWriteByte(HIGH(Addr),LOW(Addr), value_low_byte);
    
   
     
   
   
   
   
   //write V3
   
    Addr=1028;      
   SPIWriteByte(HIGH(Addr),LOW(Addr), value_high_byte);
   
   Addr=1029;      
   SPIWriteByte(HIGH(Addr),LOW(Addr), value_low_byte);
    
  	
	
	
	
}




unsigned long get_live_time()
{//we have 3 registers: V1(1024-1025);V2(1026-1027);V3(1028-1029)
	
            	 
    Uint16 Addr;
    Uchar value_high_byte;	
    Uchar value_low_byte;		
	unsigned long V1,V2,V3;
	
	unsigned long newV;

    Addr=1024;                       
    SPIReadByte(HIGH(Addr),LOW(Addr),&value_high_byte);
    
    Addr=1025;                       
    SPIReadByte(HIGH(Addr),LOW(Addr),&value_low_byte);
    
    V1=value_high_byte*65535+value_low_byte;
    
    
    Addr=1026;                       
    SPIReadByte(HIGH(Addr),LOW(Addr),&value_high_byte);
    
    Addr=1027;                       
    SPIReadByte(HIGH(Addr),LOW(Addr),&value_low_byte);
    
    V2=value_high_byte*65535+value_low_byte;
    
    Addr=1028;                       
    SPIReadByte(HIGH(Addr),LOW(Addr),&value_high_byte);
    
    Addr=1029;                       
    SPIReadByte(HIGH(Addr),LOW(Addr),&value_low_byte);
    
    V3=value_high_byte*65535+value_low_byte;
        
 
 //WE got all three register values,
 
   if ((V1==V2)&&(V2==V3)) //ALL good  
   {
   	newV=V1+1;
   	
   	
   	
   }
   else if (V1==V2)//v3 crash
   {
   	newV=V1+1;
   	
   	
   }
   else if (V2==V3)//v1 crash
   {
   	newV=V2+1;
   	
   	
   }
   else //V1!=V2!=V3, v2 crash
   {
   	newV=V1+1;
   	
   	
   }
   
   return newV;
                               	
	
}










