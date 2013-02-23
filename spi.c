#include "usbcom.h"

void EnableSPInCS();
void DisableSPInCS();


void InitSPI()
{
   SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);   
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
   

     
   GPIOPinConfigure(GPIO_PA2_SSI0CLK);
   GPIOPinConfigure(GPIO_PA3_SSI0FSS);
   GPIOPinConfigure(GPIO_PA4_SSI0RX);
   GPIOPinConfigure(GPIO_PA5_SSI0TX);
   
   
   
   GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3|GPIO_PIN_2);
   
   
   SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 1000000, 8);  // 1MBit/sec

   SSIEnable(SSI0_BASE);
 
	
}
 
 
long CheckEepromWriteDone()
{
  unsigned long *SPIRxData;
  unsigned long EepromStatusRegister;
  SPIRxData=&EepromStatusRegister;
  

   // First, make sure no SSI data is being sent (TX buffer empty).
    while(SSIBusy(SSI0_BASE));

    // Then make sure the RX buffer is empty by reading it until empty
    while(SSIDataGetNonBlocking(SSI0_BASE, SPIRxData));

    // Send 8-bit Read Status Register command
    SSIDataPutNonBlocking(SSI0_BASE, 0x05);

    // Send dummy Byte command. Sending a "dummy byte" to EEPROM, will allow
    // the EEPROM to clock out the requested data on the MISO line (RX).
    SSIDataPutNonBlocking(SSI0_BASE, 0x00);

    // Wait until TX finished sending (TX buffer empty)
    while(SSIBusy(SSI0_BASE));

    // Read the RX buffer until it is empty. Save the last value at SPIRxData.
    while(SSIDataGetNonBlocking(SSI0_BASE, SPIRxData))
    {
      ;//EepromStatusRegister = *SPIRxData;
    }
    if (EepromStatusRegister == 0x00)
    {
      return true;    // Return true since the EEPROM has finished interal write cycle.
    }
    return false;    // Return false since the EEPROM has NOT finished interal write cycle.

}
 
 
 
 
 
 long SPIWriteByte(unsigned long HiAddr,unsigned long LoAddr, Uchar data)
{
 
	unsigned long SPIClearData;
    unsigned long SIPTxData=data;
	
    //while(SSIDataGetNonBlocking(SSI0_BASE, &SPIClearData)); 
	
    while(SSIBusy(SSI0_BASE));
    // Then make sure the RX buffer is empty by reading it until empty
    while(SSIDataGetNonBlocking(SSI0_BASE, &SPIClearData)); 
	
   if(!SSIDataPutNonBlocking(SSI0_BASE, 0x06))//// Send WREN (Enable Write Operations)
   {
   	 print("send WREN Fail!");
   	 return false;
   	 
   	 
   }
   
   while(SSIBusy(SSI0_BASE));
     // Then make sure the RX buffer is empty by reading it until empty
    while(SSIDataGetNonBlocking(SSI0_BASE, &SPIClearData)); 

    if(!SSIDataPutNonBlocking(SSI0_BASE, 0x02))// Send Byte Write command
   {
   	 print("send Byte Write command Fail!");
   	 return false;
   }
     if (!SSIDataPutNonBlocking(SSI0_BASE, HiAddr))  // Send High Byte of 16-bit address
    {
   	 print("send High Byte of 16-bit address Fail!");
   	 return false;
   }    
     
     if(!SSIDataPutNonBlocking(SSI0_BASE, LoAddr))  	// Send Low Byte of 16-bit address
    {
   	 print("send low Byte of 16-bit address Fail!");
   	 return false;
   }       
     if(!SSIDataPutNonBlocking(SSI0_BASE, SIPTxData)) 	// send byte to be written
     {
   	 print("send data Fail!");
   	 return false;
   }      
   
   while(SSIBusy(SSI0_BASE));

     // Then make sure the RX buffer is empty by reading it until empty
   while(SSIDataGetNonBlocking(SSI0_BASE, &SPIClearData)); 
     
   if(!SSIDataPutNonBlocking(SSI0_BASE, 0x04))   // Send WRDI (Disable Write Operations)
      {
   	 print("send WRDI Fail!");
   	 return false;
   }   	
   
   while(SSIBusy(SSI0_BASE));
    // Then make sure the RX buffer is empty by reading it until empty
   while(SSIDataGetNonBlocking(SSI0_BASE, &SPIClearData)); 
   
   while (!CheckEepromWriteDone());
   return true;
	
	
}
///Function to write n bytes to EEPROM 
///not used
 long SPIWriteNByte(unsigned long HiAddr,unsigned long LoAddr, Uchar * pdata,int len)
 {

 	unsigned long SPIClearData;
 	int i = 0;
     //while(SSIDataGetNonBlocking(SSI0_BASE, &SPIClearData));

     while(SSIBusy(SSI0_BASE));
     // Then make sure the RX buffer is empty by reading it until empty
     while(SSIDataGetNonBlocking(SSI0_BASE, &SPIClearData));

    if(!SSIDataPutNonBlocking(SSI0_BASE, 0x06))//// Send WREN (Enable Write Operations)
    {
    	 print("send WREN Fail!");
    	 return false;


    }

    while(SSIBusy(SSI0_BASE));
      // Then make sure the RX buffer is empty by reading it until empty
     while(SSIDataGetNonBlocking(SSI0_BASE, &SPIClearData));

     if(!SSIDataPutNonBlocking(SSI0_BASE, 0x02))// Send Byte Write command
    {
    	 print("send Byte Write command Fail!");
    	 return false;
    }
      if (!SSIDataPutNonBlocking(SSI0_BASE, HiAddr))  // Send High Byte of 16-bit address
     {
    	 print("send High Byte of 16-bit address Fail!");
    	 return false;
    }

      if(!SSIDataPutNonBlocking(SSI0_BASE, LoAddr))  	// Send Low Byte of 16-bit address
     {
    	 print("send low Byte of 16-bit address Fail!");
    	 return false;
    }
    for(i = 0;i<len;i++)
    {
      if(!SSIDataPutNonBlocking(SSI0_BASE, (unsigned long)*(pdata+i))) 	// send byte to be written
      {
    	 print("send data Fail!");
    	 return false;
      }
    }

    while(SSIBusy(SSI0_BASE));

      // Then make sure the RX buffer is empty by reading it until empty
    while(SSIDataGetNonBlocking(SSI0_BASE, &SPIClearData));

    if(!SSIDataPutNonBlocking(SSI0_BASE, 0x04))   // Send WRDI (Disable Write Operations)
       {
    	 print("send WRDI Fail!");
    	 return false;
    }

    while(SSIBusy(SSI0_BASE));
     // Then make sure the RX buffer is empty by reading it until empty
    while(SSIDataGetNonBlocking(SSI0_BASE, &SPIClearData));

    while (!CheckEepromWriteDone());
    return true;


 }
  
long SPIReadByte(unsigned long HiAddr,unsigned long LoAddr, Uchar *pdata)
{
	 
     unsigned long SPIRxData;

    // First, make sure no SSI data is being sent (TX buffer empty).
    while(SSIBusy(SSI0_BASE));

    // Then make sure the RX buffer is empty by reading it until empty
    while(SSIDataGetNonBlocking(SSI0_BASE, (unsigned long *)pdata));


    // Send 8-bit READ command
    if(!SSIDataPutNonBlocking(SSI0_BASE, 0x03))
    {
      print("Send Byte read command Fail!");
      return false;
    }

    // Send High Byte of 16-bit address
    if(!SSIDataPutNonBlocking(SSI0_BASE, HiAddr))
    {
      print("Send High Byte of 16-bit address Fail!");
      return false;
    }

    // Send Low Byte of 16-bit address
    if(!SSIDataPutNonBlocking(SSI0_BASE, LoAddr))
    {
      print("Send Low Byte of 16-bit address Fail!");
      return false;
    }

    // Send dummy Byte command. Sending a "dummy byte" to EEPROM, will allow
    // the EEPROM to clock out the requested data on the MISO line (RX).
    if(!SSIDataPutNonBlocking(SSI0_BASE, 0x00))
    {
      print("Send Dummy Byte command Fail!");
      return false;
    }

    // Wait until TX finished sending (TX buffer empty)
    while(SSIBusy(SSI0_BASE));

    // Read the RX buffer until it is empty. Save the last value in pdata.
    while(SSIDataGetNonBlocking(SSI0_BASE, &SPIRxData))
    {
      *pdata = SPIRxData;
    }

    //print("read data is:");
    //TST_UART1_Send((unsigned char*)pdata,2);

    return true;
}


void WriteLutIndex(unsigned char ddl,unsigned long index)
{
	  InformFPGAReleaseEEPROM();
      unsigned long LoAdd;
      unsigned long HiAdd;	
      unsigned long Addr16;
      Uchar value;
      
      Addr16=ddl*2;
      value=LOW(index);      
      
      HiAdd=HIGH(Addr16);
      LoAdd=LOW(Addr16);
      if(!SPIWriteByte(HiAdd, LoAdd,value))
      {
          print("write fail");
                              	
      }	
      
      
      Addr16=ddl*2+1;
      value=HIGH(index);      
      
      HiAdd=HIGH(Addr16);
      LoAdd=LOW(Addr16);
      if(!SPIWriteByte(HiAdd, LoAdd,value))
      {
          print("write fail");
                              	
      }	
      
     InformFPGATakeEEPROM() ;
           
      
	
}


/*********************************
*
*This function is to test the accessibilty of SPI.
*
*
**********************************/

 void PostSPI()
 {

    Uchar value = 0;
	
    InformFPGAReleaseEEPROM();
	
    if(!SPIWriteByte(0, 0, 1))
    {
        print("write value 1 fail"); 
        InformFPGATakeEEPROM();
        return;
    }

    if(!SPIReadByte(0, 0, &value))	
    {
        print("read fail"); 
        InformFPGATakeEEPROM();
        return;
    }

    if (value ==1)
    {
    	print("write and read from (0) success!.");
    }
    else
    {
    	print("read data not equal to the writtten data(0).");
	    InformFPGATakeEEPROM();
	    return;
    }

    if(!SPIWriteByte(0, 100, 1))   //HiAddr=0, LoAddr=100,
    {
        print("write value 1 fail");
        InformFPGATakeEEPROM();
        return;
    }

    if(!SPIReadByte(0, 100, &value))		
    {
        print("read fail"); 
        InformFPGATakeEEPROM();
        return;
	}

    if (value ==1)
    {
    	print("write and read from (100) success!.");
    }
    else
    {
    	print("read data not equal to the writtten data(100).");
	    InformFPGATakeEEPROM();
	    return;
    }

    if(!SPIWriteByte(1, 0, 1))   //HiAddr=1, LoAddr=0, that is 256.
    {
        print("write value 1 fail"); 
        InformFPGATakeEEPROM();
	 return;	
	}

    if(!SPIReadByte(1, 0, &value))		
     {
        print("read fail"); 
        InformFPGATakeEEPROM();
	 return;	
	}

    if (value ==1)
     {
    	print("write and read from (256) success!.");
     }
    else
    {
    	print("read data not equal to the writtten data(256).");
	    InformFPGATakeEEPROM();
	    return;
    }

     InformFPGATakeEEPROM();

 }


