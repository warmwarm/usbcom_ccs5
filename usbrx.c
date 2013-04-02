#include "usbcom.h"
#include "usbsig.h"

extern int calib_status;
extern int bl_low_tested_num;
extern int bl_high_tested_num;
extern volatile tBoolean jumpToBootloaderSignaled;
extern long usbIsPluged;
extern int MCUOpMode;
int gUsbGetLux;

Uchar tx[34];
unsigned char wbuff[513];
// function to write LUT table
// 0 == using LUT table
// 1 == default LUT table
void WriteLUT(int type)
{

	unsigned int i;
	unsigned long LoAdd;
	unsigned long HiAdd;
	
    if(16 == wbuff[512])
    {


     //InfomFPGABeginCalibration();
	 InformFPGAReleaseEEPROM();

	 EnableTristate();
	 for (i=0;i<512;i++)
	 {
	     //itoa(valuechar, wbuff[i]);
	     //print(valuechar);
         HiAdd=HIGH(i+type*512);
         LoAdd=LOW(i+type*512);

	     if(!SPIWriteByte(HiAdd, LoAdd,wbuff[i]))
	     {
	        print("write fail");
	     }

	  }
	  DisableTristate();
	  InformFPGATakeEEPROM();
      wbuff[512] = 0;
    }
}

void usbRxHandler(unsigned char*rev_buf)

{
        print("data received in usbRxHandler."); 
        //UARTSend((unsigned char *)"received data:", 14);
        Uchar * RxData = rev_buf;
        //memcpy(RxData,rev_buf,sizeof(RxData));
        //TST_UART1_Send(RxData,34);
        
        //  AppUpdaterUSB()	;
         
 
        //parse what we received
        Uchar signal = RxData[0];
        Uchar state = RxData[1];
        union t_usb_cmd *rxcmd = (union t_usb_cmd *)rev_buf;
        u1printf("signal %d\n\r",signal);
        union t_usb_cmd rsp;
	    memset(&rsp,0,sizeof(rsp));
		rsp.msg.cmd = signal + 100;
		rsp.msg.state = 1;
            switch (signal)
            {
            	case WRITE_LUT_REQ:  //write  LUT to EEPROM
            	{
            		u1printf("setting lut0 index[%d]\n\r",state);
            		Uchar batch_num=state;
            	    if(0 == batch_num)
            	    {
            	    	wbuff[512] = 0;
            	    }
                    if(wbuff[512] < 16)
                    {

                    	wbuff[512]++;
                    	memcpy(wbuff+batch_num*32,RxData+2,32);
                    }

                    if(wbuff[512] == 16)
                    {
                    	WriteLUT(0);
						print("write LUT done.");
                    }

            		break;
            	}
            	
            	case CAL_GEN1_REQ://calibration, tell FPGA to generate give grayscal
            	{
            		
            		print("calibration in uxbRxHandler.");
            		// pc signals to generate.
            		switch (state)
            		{
            			case 0: //begin calibration
            			{
            				InfomFPGAEndCalibration();
            				InfomFPGABeginCalibration();
            				break;
            			}
            			
            			
            			
            			case 1: // in calibration
             			{
             				//UARTSend((unsigned char *)"IN    calibration", 17);
             				TST_UART1_Send(RxData,34);
             				TST_UART1_Send(rev_buf,34);
             				 
             				
             				unsigned char lo, hi;
             				lo=RxData[2];
             				hi=RxData[3];
             				
                            InfomFPGAGenGivenGrayScale(lo,hi);
    
            				
            				return;
            			}           			
            			
            			case 2: //end calibration
             			{
            				
            				InfomFPGAEndCalibration(); 
             				
            				break;
            			}           			
            			
   
            		}
            		
            		
            		
            		break;      		
            		
            	}
     
            	
            	case WRITE_LUT_DEFAULT_REQ://write  LUT to default EEPROM
            	{
            	            		//print("setting lut2");
            	            		Uchar batch_num=state;
            	            		//itoa(valuechar, state);
            	            	    //print(valuechar);
            	            	    if(0 == batch_num)
            	            	    {
            	            	    	wbuff[512] = 0;
            	            	    }
            	                    if(wbuff[512] < 16)
            	                    {

            	                    	wbuff[512]++;
            	                    	memcpy(wbuff+batch_num*32,RxData+2,32);
            	                    }

            	                    if(wbuff[512] == 16)
            	                    {
            	                    	WriteLUT(1);
            	                    	print("write LUT to default done.");
            	                    }
            	    break;
            	}
            	case RESTORE_LUT_REQ://restored LUT 
            	{
            		
            		Uint16 ii=0;
            		Uint16 OrigAddr,DestAddr;
            		Uchar value;
            		
            		print("restored LUT in uxbRxHandler.");
            		
            		InformFPGAReleaseEEPROM();
            		EnableTristate();   

       	   		    for(ii=0;ii<512;ii++)
       	   		    {
					  //read from i+512 write it to i

                      OrigAddr=ii+512;
                      DestAddr=ii;
                               
                      SPIReadByte(HIGH(OrigAddr),LOW(OrigAddr),&value);
                               
                      SPIWriteByte(HIGH(DestAddr),LOW(DestAddr),value);
                    }
           		
            	   DisableTristate();      
                   InformFPGATakeEEPROM();
	          	   break;
            	}
            	
            	case ACTIVE_GEN1_REQ: // activate Gen1 mode
            	{   print("usb pluged");
            		/*
            		usbIsPluged=1;
            		
            		if (calib_status==CALST_INIT)
            		if(MCUOpMode==MCU_OP_MODE_GEN2)
            		InformFPGASetGen1Mode();  
            	    */	          		
            		break;
            	}
				
            	case ACTIVE_GEN2_REQ: // Deactivate Gen1 mode
            	{   /*
            		usbIsPluged=0;
            		print("usb unpluged");
            		if(MCUOpMode==MCU_OP_MODE_GEN2)
            		InformFPGASetGen2Mode();
            	    */	
            		break;           		
            	}
            	            	
            	
            	case GOTO_BOOTLOADER_REQ: //go to boot loader
            	{
            		print("go to bootloader");
            		//ROM_UpdateUART();
            		//AppUpdaterUSB();
            		//USBDevDisconnect(USB0_BASE);
            		
            		print("signal main");
            		jumpToBootloaderSignaled=true;
            		//(*((void (*)(void))(*(unsigned long *)0x2c)))();
            		//USBDDFUUpdateBegin();

            		break;
            	}            	
            	
            	case READ_EEPROM_REQ:  //To read the current EEPROM data, refer to readLUTFromEEPROM()
            	{
            		 int i,j;
            		 Uchar value;
            		 unsigned long LoAdd, HiAdd;
					 int start;
					 start = rxcmd->msg.state * 512;
					 j = rxcmd->msgstr.str[0];
            		 InformFPGAReleaseEEPROM();
            		 EnableTristate();

            		 u1printf("read LUT Table begin:[%d] start[%d]\n\r",j,start);
					 
					 rsp.msg.state = j;
	            		 for(i=start;i<16+start;i++)
	            		 {
	            		    
	            		    HiAdd = HIGH((j*16+i)*2);
	            		    LoAdd = LOW((j*16+i)*2);
	            		    if (!SPIReadByte(HiAdd, LoAdd, &value))
	            		    {
	            		        print("read LUT fail.");

	            		        DisableTristate();
	            		        InformFPGATakeEEPROM();
								rsp.msg.state = 255;
	            		        break;
	            		    }
							u1printf("%v1[%d]\n\r",value);
							rsp.msg.data[i] = value;
						    HiAdd = HIGH((j*16+i)*2 + 1);
	            		    LoAdd = LOW((j*16+i)*2 + 1);
	            		    if (!SPIReadByte(HiAdd, LoAdd, &value))
	            		    {
	            		        print("read LUT fail.");

	            		        DisableTristate();
	            		        InformFPGATakeEEPROM();
								rsp.msg.state = 255;
	            		        break;
	            		    }
							u1printf("%v2[%d]\n\r",value);
							rsp.msg.data[i] += value*0x100;
							
	            		    u1printf("%d\n\r",rsp.msg.data[i]);
							//SendDatatoHost(rsp.raw, DEC_TO_HOST_MSG_LENGTH);
					     }

            		print("read LUT done.");
            		DisableTristate();
            		InformFPGATakeEEPROM();
					break;
            	}            	
            	
            	
            	case CAL_GEN2_REQ://auto, start a auto calibration from calispector
            	{
            		//
            		//first set mode to gen2 mode.
                    
                    print("auto calibration data received ");  
                    //Delay(1);
                    switch(rxcmd->msg.state)
                    {
                        case 0:
	                    // to check mode, if its has imbedded lux meter then, 
	                    // start a calibration and respond to host
	                    // else, send host a warning.
	                    {
	    	               InfomFPGABeginCalibration();
	    	               FPGACtrlBitSet=1;
	    	               CALbuttonState=1;
	    	               InfomFPGASetMaxBackgroundLevel(255);
	    	               InfomFPGAGenGivenGrayScale(255,7);
	    	               calib_status=CALST_STABLIZATION;
	    	        
	    	               Delay(3);
	    	               //tell lux meter to collect 
	    	               SendRequestToLuxmeter(255,7);
	    	               //Delay(3);
	    	               AutoCalibrationFromCalispector=true;
	                      }
						break;
						case 1:
							{
							  gUsbGetLux = rxcmd->msg.data[0];
							  InfomFPGAGenGivenGrayScale(LOW(rxcmd->msg.data[0]),HIGH(rxcmd->msg.data[0]));
							  SendRequestToLuxmeter(LOW(rxcmd->msg.data[0]),HIGH(rxcmd->msg.data[0]));
							  return;
							}
							break;
						case 2:
							ClearExistingCalibration();
							break;
						case 3:
							rsp.msg.state = calib_status;
							break;
                    }
            		//print("data sent ");           		
            		break;
            	}
            	
            	case SET_LIFETIME_REQ://set life time
            	{
            		//set V1,V2,V3 to 0
            		
            		reset_life_time();
            		
            		break;
            	}    
            	
            	case GET_LIFETIME_REQ://get life time
            	{
            		//read V1 and send back to host
            		unsigned long life_time= get_live_time();
            		
            		//send this data 
            		break;
	            }    
				case SET_BACKLIGHT_REQ:
            	{
					InfomFPGASetMaxBackgroundLevel(rxcmd->msg.state);
                    rsp.msg.state = 1;
            		break;
	            }  
				case GET_BACKLIGHT_REQ:
            	{
					
            		break;
	            }  
				case SET_DICOM_STATUS_REQ:
            	{
					//need to add FPGA command
            		break;
	            }  
				case GET_DICOM_STATUS_REQ:
            	{
					rsp.msg.state = DICOMButtonPressed();
            		break;
	            }
				case SET_POWER_STATUS_REQ:
            	{
            		break;
	            }  
				case GET_POWER_STATUS_REQ:
            	{
					rsp.msg.state = PoweronPressed();
            		break;
	            } 
				case GET_MCU_VERSION_REQ:
            	{
					u1printf("GET_MCU_VERSION_REQ\n\r");
					rsp.msgstr.state = 1;
					snprintf(rsp.msgstr.str,sizeof(rsp.msgstr.str),"%s",MCU_VERSION_STRING);
            		break;
	            }
				case GET_FPGA_VERSION_REQ:
            	{
					rsp.msgstr.state = 1;
					snprintf(rsp.msgstr.str,sizeof(rsp.msgstr.str),"%s","FPGA Version");
            		break;
	            }  
				case GET_LC_VERSION_REQ:
            	{
					snprintf(rsp.msgstr.str,sizeof(rsp.msgstr.str),"%s","LCD Controller Version");
            		break;
	            }
				case GET_HW_VERSION_REQ:
            	{
					snprintf(rsp.msgstr.str,sizeof(rsp.msgstr.str),"%s","HW Version");
            		break;
	            }  
				//send feedback to host
				
	          }
			u1printf("Send rsp\n\r");
			SendDatatoHost(rsp.raw, DEC_TO_HOST_MSG_LENGTH);
 }































