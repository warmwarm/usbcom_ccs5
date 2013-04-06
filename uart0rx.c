#include "usbcom.h"

int bl_low_tested_num=0;
int bl_high_tested_num=0;

float LuxMin;
float LuxMax;
float JNDMin;
float JNDMax;
float JNDInterval;

float stableLuxUpper=0;
float stableLuxLower=0;

float PreMeasuredLux=0;

unsigned long CurrMeasuredIndex=0;

unsigned char CurrCalibratingDDL=0;
unsigned char backlightControlLevel;

int MCUOpMode=MCU_OP_MODE_GEN1;

static float preBacklightBright=0;
static int bkStableCount=0;

extern int gUsbGetLux;

extern long PowerUpState;
extern long PowerbuttonState;
long usbIsPluged=0;
volatile long FPGACtrlBitSet=false;
void ClearExistingCalibration()
{
  calib_status=CALST_INIT;
  bl_low_tested_num=0;
  bl_high_tested_num=0;	
  
  //stableLuxUpper=0;      ///changed by minghao
  //stableLuxLower=0;	   ///changed by minghao
  PreMeasuredLux=0;
  CurrMeasuredIndex=0;

  CurrCalibratingDDL=0;
  //backlightControlLevel=255;	  ///changed by minghao
  preBacklightBright=0;
  bkStableCount=0;


  //DICOMbuttonState=0;    ///changed by minghao
  CALbuttonState=0;
  
  //if (FPGACtrlBitSet)    ///changed by minghao
  InfomFPGAEndCalibration();
  ClearCalibrationStatusMsg();
  print("existing cal canceled");
  
}

void ProcessDataFromLuxmeter(unsigned char *pdata, unsigned int count)
{
	unsigned char RxData[6];
	unsigned long index;
	float lux;
	memcpy(RxData,pdata,count);
	if (count==6)
	{
	   index=RxData[0]+RxData[1]*256;
	
	   lux=( RxData[2]*65536 +RxData[3]*256+RxData[4])*0.01;
	   u1printf("lut value[%f] gUsbGetLux[%d]\n\r",lux,gUsbGetLux);
	   if(gUsbGetLux >=0)
	   	{
	   	  u1printf("send lut index[%d]\n\r",gUsbGetLux);
	   	  SendLutToHost(gUsbGetLux,RxData[2],RxData[3],RxData[4]);
		  gUsbGetLux = -1;
	   	}
	
	   //u1printf("index[%d] lux[%f]\n\r",index,lux);
       ///add by minghao 
	   if(MCUOpMode==MCU_OP_MODE_GEN1)
	   {
          print("Gen 2, meter connected");
	      InformFPGASetGen2Mode();
	   }
	   //send data to windows:
	   
	   //SendDatatoHost(RxData,6);
	   
	   
	}
	else if(count==1)
	{   if (calib_status==CALST_INIT)
		{
			print("Gen 1, no meter.\n");  //Update by Jimmy.
			//MCUOpMode=MCU_OP_MODE_GEN1;
		    InformFPGASetGen1Mode();
		    return;
		}
		else
		{
			print("Armstel\\>status error\n");
			return;
		}
	}
	else
	{
		//print("unexpected data length from meter");
        return;
		
	}
    
	//read buttons status
    ReadFPGAStatusRegister(); 

    if (DICOMButtonPressed())
       DICOMbuttonState=1;
    else
       DICOMbuttonState=0;
    
    if (CALButtonPressed())
    {
        if(0 == CALbuttonState)
        {
        	InformFPGASetGen2Mode();
        }
    	CALbuttonState=1;
		
    }
	else
	{
	    CALbuttonState=0;
	}
    
    if(PoweronPressed())
       PowerbuttonState=1;
    else
       PowerbuttonState=0;	

	if(calib_status==CALST_STABLIZATION
	||calib_status==CALST_BL_CTRL
	||calib_status==CALST_BL_CTRL_LOW
	||calib_status==CALST_CALIBRATION
	)
	{
		if(PowerbuttonState==0)
		{
			
			ClearExistingCalibration();
			 
			return;
			
		}
		if (DICOMbuttonState==0&&CALbuttonState==0)
		{
			ClearExistingCalibration();
			
			return;
			
		}
	}
	switch (calib_status)
	{
		case CALST_NIL:
		{
			//error		
			print("error:data received when calibration is not running");
		    break;	
		}
		case CALST_INIT:
		{

            ///edit by minghao
    	    ///when power up will come here
    		if(calib_status==CALST_INIT)
		    {
		      if ((CALbuttonState)||DICOMbuttonState)
		      {
		    	calib_status=CALST_STABLIZATION;
		    	//tell lux meter to collect 	    	
				if(CALbuttonState)
			    {
			       WriteCalibrationStatusMsg0();
				   InfomFPGABeginCalibration();
				   FPGACtrlBitSet=1;
			    }
			    InfomFPGASetMaxBackgroundLevel(backlightControlLevel);
				Delay(3);
		    	InfomFPGAGenGivenGrayScale(255,7);
				Delay(3);
				if((0 != stableLuxUpper)&&(0 != stableLuxLower))
				{
				   calib_status=CALST_BL_CTRL;
				}
				//backlightControlLevel=255;

				SendRequestToLuxmeter(255,7);
		    	u1printf("start a whole process\n\r");
		      }   	 	
		    }
    		return;
 
		}	
		
		case CALST_STABLIZATION:
		{
            ///edit by minghao
	        u1printf("stab lux is:%f count[%d]\n\r",lux,bkStableCount);
            if(0 == preBacklightBright)
          	{
          	  preBacklightBright = lux;
          	}
	        //sprintf(str_lux,"%4.2f",lux-preBacklightBright);;
	        //print(str_lux);
	        if (bkStableCount<3)
	        {
	      	   bkStableCount++;
	      	   Delay(3);
	      	   SendRequestToLuxmeter(255,7);
	      	   return;
	        }	

	        if (fabs(lux-preBacklightBright)<0.9)
	        {
	      	  //it's stable
	      	 
	      	  stableLuxUpper=lux*0.96;
	      	  stableLuxLower=lux*0.94;
	      	  u1printf("stabled up[%f] down[%f]\n\r",stableLuxUpper,stableLuxLower);
	      	
	      	  //now stabled, check if in DICOM status, if yes, keep monitor backligh
	      	  calib_status=CALST_BL_CTRL;

	      	  //backlightControlLevel=255*0.95;
	      	  //InfomFPGASetMaxBackgroundLevel(backlightControlLevel);
	      	  Delay(3);
	      	  SendRequestToLuxmeter(255,7);	      	
	      	  return;	      	
	        }	
	        else
	        {
		      	bkStableCount=0;
		      	preBacklightBright=lux;
		      	
		      	Delay(3);
		      	SendRequestToLuxmeter(255,7);
		      	
		      	return;	
		      	
   	        }   
			//ROM_TimerDisable(TIMER0_BASE, TIMER_A);		  
		}		
		
		
		case CALST_BL_CTRL:
		{
			print("begin backlight contrl ");
			if (lux>stableLuxUpper)
			{   bl_high_tested_num=0;
				print(" bigger ");
				if(backlightControlLevel > 5) /// speed up 
				{
					backlightControlLevel = backlightControlLevel-5;
				}
				InfomFPGASetMaxBackgroundLevel(backlightControlLevel);
				Delay(3);
				SendRequestToLuxmeter(255,7);
				return;
				
			}
			else if (lux<stableLuxLower)
			{   bl_high_tested_num=0;
				u1printf(" smaller backlightControlLevel[%d]",backlightControlLevel);
				if (backlightControlLevel<255)
				{
					InfomFPGASetMaxBackgroundLevel(++backlightControlLevel);
					Delay(3);
					SendRequestToLuxmeter(255,7);
					return;
				}
				else
				{
					//we reached max
					//warning;
					///minghao error ,exit calibration
					stableLuxUpper=0;
                    stableLuxLower=0;	
					backlightControlLevel = 255;
                    ClearExistingCalibration();
					return;
				}	
			}
			else
			{   
				print( " == ");
				
				if (bl_high_tested_num>3)
				{
					print("Really stable");
					//really stable, do calibration
					///minghao  judge in cal mode or dicom mode
					if(CALbuttonState)
					{
                        /// step to cal status
						print("begin CAL");
						//do calibraion
						WriteCalibrationStatusMsg1();
    		            LuxMax=lux;
    		            bl_low_tested_num=0;
    		            calib_status=CALST_BL_CTRL_LOW;
    		        	
    		            //tell FPGA to generate min lux
    		            InfomFPGAGenGivenGrayScale(0,0);
    		        	//tell lux meter get it
    		        	Delay(3);
    		            SendRequestToLuxmeter(0,0);				
				
				        return;							
					}
					else if(DICOMbuttonState)
					{
                           /// continue to monitor the backlight
					       bl_high_tested_num=4;
						   Delay(20);
    		               SendRequestToLuxmeter(255,7);	
						   return;
					}
					/*
					else if (DICOMbuttonState)
					{
					   //Delay(10);
					   //	ClearExistingCalibration();
					   print("no CAL, continue MON");
					   bl_high_tested_num=4;
				       Delay(10);
				       SendRequestToLuxmeter(255,7);
				       return;						
						
					}*/
					else 
					   return;
					
					
					
				}
				else
				{
					bl_high_tested_num++;
					print("not really stable");
					Delay(3);
					SendRequestToLuxmeter(255,7);	
					print("sent again");
				}
				
			 
				
				
			}
			 
		    break;	
		}		
				
		
			
 
		
		case CALST_BL_CTRL_HIGH:
		{
			
           /*
    		
    		if (index==2047)//every thing is good
    		{
    			
    			if (bl_high_tested_num<3)//not stable, wait
    		    {
    		    	print("hi:less than 3 times, resend");
    		    	bl_high_tested_num++;
    		    	SendRequestToLuxmeter(255,7);
    			    return;
    			
    		    }
    		    //bl_high_tested_num==3 ok, get the lux
    		    else
    		    {
    		    	print(" Hi: 3 times, ");
    		   
    		        LuxMax=lux;
    		        if (LuxMax>BACKLIGHT_CONTROL_UPPER_BOND)
    		        {
    		        	print(" Hi:larger than range ");
    		        	backlightControlLevel--;
    		        	
    		        	InfomFPGASetMaxBackgroundLevel(backlightControlLevel);
    		        	InfomFPGAGenGivenGrayScale(255,7);
    		        	;
    		        	bl_high_tested_num=0;//reset counter
    		        	
    		        	//informs lux meter
    		        	SendRequestToLuxmeter(255,7);
    		        	
    		        	
    		        }
    		        else if (LuxMax<BACKLIGHT_CONTROL_LOWER_BOND)
    		        {
    		        	print(" Hi:smaller than range ");
    		        	if (backlightControlLevel>=255)//we reached max
    		        	{
     		        	   print("reached max");
    		        	   print("  lux is:");
	                       print(str_lux);
	                       
	                       saveMaxBacklightLevel(255);
    		        	   LuxMax=lux;
    		        	   bl_low_tested_num=0;
    		        	   calib_status=CALST_BL_CTRL_LOW;
    		        	
    		        	   //tell FPGA to generate min lux
    		        	   InfomFPGAGenGivenGrayScale(0,0);
    		        	   //tell lux meter get it
    		        	
    		        	   SendRequestToLuxmeter(0,0);   		        		
    		        	}
    		        	else
    		        	{
    		        	   print("Hi: smaller than range,add  ");
    		        	   backlightControlLevel++;
    		        	   InfomFPGASetMaxBackgroundLevel(backlightControlLevel);
    		        	   InfomFPGAGenGivenGrayScale(255,7);
    		        	    
    		        	   bl_high_tested_num=0;//reset counter
    		        	
    		        	   //informs lux meter
    		        	   SendRequestToLuxmeter(255,7);	
    		        	}	        	
    		        	
    		        }
    		        else // within our range, go to next stage for min_lux
    		        {
    		        	print("Hi:within range");
    		        	print("  lux is:");
    		        	saveMaxBacklightLevel(backlightControlLevel);
	                    print(str_lux);
    		        	LuxMax=lux;
    		        	bl_low_tested_num=0;
    		        	calib_status=CALST_BL_CTRL_LOW;
    		        	
    		        	//tell FPGA to generate min lux
    		        	InfomFPGAGenGivenGrayScale(0,0);
    		        	//tell lux meter get it
    		        	
    		        	SendRequestToLuxmeter(0,0);
   		            }
    		        
    		        
    		    
    		    }
    		    
    		    
    		    
    		    
    			
    		}
    		else //error received
    		{
    		   InfomFPGAGenGivenGrayScale(255,7);
    		   SendRequestToLuxmeter(255,7);
    		}
    					
			
			
			
			
		    */
		    break;	
		}		
		
		case CALST_BL_CTRL_LOW:
		{
			u1printf(" begin testing lower end\n\r");

    		if (index==0)//every thing is good
    		{
    			
    			
    			if (bl_low_tested_num<3)//not stable, wait
    		    {
    		    	//print("Lo:less than 3 times, resend");
    		    	bl_low_tested_num++;
    		    	SendRequestToLuxmeter(0,0);
    			    return;
    			
    		    }
    		    //bl_high_tested_num==3 ok, get the lux
    		    else
    		    {
    		    	
 		    	
    		    	//print("Lo: 3 times.");
    		    	LuxMin=lux;
    		    	calib_status=CALST_CALIBRATION;
    		    	
    		    	//we get max and min, we can generate target lux table for 256.
    		    	
    		    	 JNDMin=LUMINANCE_to_JND(LuxMin);
    		    	 JNDMax=LUMINANCE_to_JND(LuxMax);
    		    	 
    		    	 JNDInterval=(JNDMax-JNDMin)/255.000;
    		    	 
    		    	 print("write lut 0");
    		    	 
    		    	 //init calibration...
    		    	 WriteLutIndex(0,0);
    		    	 print("write lut 0 done");
    		    	 
    		    	 PreMeasuredLux=LuxMin;
    		    	  
    		    	 CurrMeasuredIndex=1;
    		    	 CurrCalibratingDDL=1;
    		    	 InfomFPGAGenGivenGrayScale(LOW(CurrMeasuredIndex),HIGH(CurrMeasuredIndex));
    		    	 
    		    	 
    		    	 SendRequestToLuxmeter(LOW(CurrMeasuredIndex),HIGH(CurrMeasuredIndex));
    		    	 //print("begin calibration");

    		    
    		    }
    		        		      			
    		}
    		else //error received
    		{
    		   SendRequestToLuxmeter(0,0);
    		}

		    break;	
		}		
		
		
		case CALST_CALIBRATION:
		{
			//index = CurrMeasuredIndex;
			//lux = sfLux[index];
			u1printf("mesure ddl[%03d] index[%04d] lux %f\n\r",CurrCalibratingDDL,index,lux);
			float CurrTargetLux=JND_to_LUMINANCE(JNDMin+CurrCalibratingDDL*JNDInterval);
			//u1printf("CurrTargetLux %f  PreMeasuredLux[%f]\n\r",CurrTargetLux,PreMeasuredLux);
			if (CurrCalibratingDDL<256)//
			{
			   if (lux >= CurrTargetLux) ///minghao ,change the condition
			   {//good, we got one, the index of current ddl is either current one or pre one
                  ///minghao ,change the condition
                  if  (fabs(lux-CurrTargetLux)<fabs(CurrTargetLux-PreMeasuredLux))
				  {
					//write index to CurrCalibratingDDL position
					WriteLutIndex(CurrCalibratingDDL,index);
					
					//report 
				//	if (AutoCalibrationFromCalispector)
				//	   SendLUTEntryWithLuminMsg(CurrCalibratingDDL, index, lux);
					
					u1printf("writelut nor ddl[%03d] index[%04d] lux %f\n\r",CurrCalibratingDDL,index,lux);
						
				  }
				  else
				  {
					WriteLutIndex(CurrCalibratingDDL,index-1);
					//
					//report 
				    //	if (AutoCalibrationFromCalispector)
				    //	   SendLUTEntryWithLuminMsg(CurrCalibratingDDL, index, PreMeasuredLux);					
					u1printf("writelut pre ddl[%03d] index[%04d] lux %f\n\r",CurrCalibratingDDL,index-1,PreMeasuredLux);
									
				  }
				  

				
				  if (CurrCalibratingDDL>=255||index>=2047)//
				  {
				  	 //last item, we don't need to calibrate it, just write it
				  	 WriteLutIndex(255,2047);
				  	 
				  	 if (AutoCalibrationFromCalispector)
				  	  	 SendAutoCalibrationCompleteMsg();
				  	  	 
				  	  	 
				  	 print("we are done with calibarion");
				  	 

					ClearExistingCalibration();
                    
                    //to see if we need keep monitor
                    
                    //ReadFPGAStatusRegister();
                    if(DICOMbuttonState)
                    {
                       /// dicom mode continue to monitor the backlight
 	      	           calib_status=CALST_BL_CTRL;
 	      	           bl_high_tested_num=0;
	      	           //Delay(3);
	      	           SendRequestToLuxmeter(255,7);	                   
                    }
                      	     
				     return;		  	
				  }
					
				  PreMeasuredLux=lux;
				
				  CurrCalibratingDDL++;
				  CurrMeasuredIndex++;
				
				  InfomFPGAGenGivenGrayScale(LOW(CurrMeasuredIndex),HIGH(CurrMeasuredIndex));
				  Delay(1);
					
				  //perform next
				  SendRequestToLuxmeter(LOW(CurrMeasuredIndex),HIGH(CurrMeasuredIndex));

			   }
			   else
			   {

				  if (CurrCalibratingDDL>=255||index>=2047)//
				  {
				  	 //last item, we don't need to calibrate it, just write it
				  	 WriteLutIndex(255,2047);
				  	 
				  	 if (AutoCalibrationFromCalispector)
				  	  	 SendAutoCalibrationCompleteMsg();
				  	  	 
				  	  	 
				  	 print("we are done with calibarion");
				  	 
				  	 
					 ClearExistingCalibration();

                    
                    //ReadFPGAStatusRegister();
                    if(DICOMbuttonState)
                    {
 	      	           calib_status=CALST_BL_CTRL;
 	      	           bl_high_tested_num=0;
	      	 
	      	
	      	           Delay(3);
	      	           SendRequestToLuxmeter(255,7);	                      	                	
                    }		
                    return;
				  }//simon

				    PreMeasuredLux=lux;
					CurrMeasuredIndex++;
					InfomFPGAGenGivenGrayScale(LOW(CurrMeasuredIndex),HIGH(CurrMeasuredIndex));
					Delay(1);
					SendRequestToLuxmeter(LOW(CurrMeasuredIndex),HIGH(CurrMeasuredIndex));

					
			   }
			}
			else//we are done with calibration
			{
				ClearExistingCalibration();
				if(DICOMbuttonState)
                {
                       /// dicom mode continue to monitor the backlight
 	      	           calib_status=CALST_BL_CTRL;
 	      	           bl_high_tested_num=0;      	
	      	           Delay(3);
	      	           SendRequestToLuxmeter(255,7);	                      	                	
                }
				//Delay(10);
                //ClearExistingCalibration();
				return;
				
			}
		break;	
		}
		default:
		{
			
			print("error:data received in invalid state!");
		    return;				
			
		}			
		
	}
	
	
	
}

