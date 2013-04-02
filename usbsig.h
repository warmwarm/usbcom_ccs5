#ifndef USBSIG_H_
#define USBSIG_H_

#define USBSIG_PUT_LUT_TO_EEPROM              0

#define USBSIG_CALIBRATE                      1
#define USBSIG_PUT_DEFAULT_LUT_TO_EEPROM      2
#define USBSIG_RESTORE_LUT_FROM_DEFAULT_LUT   3

#define USBSIG_GET_LUT                        4

#define USBSIG_GO_TO_BOOTLOADER               5

#define USBSIG_TESTING_1                      6

#define USBSIG_TESTING_2                      7

#define USBSIG_ACTIVATE_GEN1_FUNC             8

#define USBSIG_SET_LIFE_TIME_COMP             9
#define USBSIG_GET_LIFT_TIME_ACK              10


union t_auto_calibration_resp_msg//this structure used for ACK, NACK, and ERROR
{
  Uchar msg_type;
  Uchar msg[DEC_TO_HOST_MSG_LENGTH];	
	
};

union t_lut_entry_msg
{
	struct
	{
	  Uchar msg_type;
	  Uchar ddl;//a value from 0 to 255
	  unsigned long index;
	} msg_layout;
	
	Uchar msg[DEC_TO_HOST_MSG_LENGTH];
	 
	
	
};

union t_lut_entry_with_lumin_msg
{
	struct
	{
	  Uchar msg_type;
	  Uchar ddl;//a value from 0 to 255
	  unsigned long index;//a value from 0 to 2048
	  float lumin;
	} msg_layout;
	
	Uchar msg[DEC_TO_HOST_MSG_LENGTH];
};


union t_usb_cmd
{
	struct
	{
	      Uchar cmd;
		  Uchar state;
		  unsigned short data[DEC_TO_HOST_INT_LENGTH];
	} msg;
	struct
	{
	      Uchar cmd;
		  Uchar state;
		  unsigned short data;
		  float data2;
	} msg2;
	struct
	{
	      Uchar cmd;
		  Uchar state;
		  Uchar str[DEC_TO_HOST_MSG_LENGTH - 2];
	} msgstr;
	  Uchar raw[DEC_TO_HOST_MSG_LENGTH];
};

#endif /*USBSIG_H_*/
