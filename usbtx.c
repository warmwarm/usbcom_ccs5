#include "usbcom.h"
#include "usbsig.h"


long AutoCalibrationFromCalispector=false;

//Uchar tx[34];
unsigned long  SendDatatoHost(Uchar *ptr, int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		tx[i]=ptr[i];
	}
	
	return USBDHIDReportWrite(&g_sHIDAmedicomGen2Device,tx,DEC_TO_HOST_MSG_LENGTH,0);	
	
}

unsigned long SendCalibrationAckMsg()
{
	union t_auto_calibration_resp_msg msg;
	
	msg.msg_type=AUTO_CALIBRATION_ACK;
	
	return SendDatatoHost(msg.msg, DEC_TO_HOST_MSG_LENGTH);
	
	
	
}

unsigned long SendCalibrationNAckMsg()
{
	union t_auto_calibration_resp_msg msg;
	
	msg.msg_type=AUTO_CALIBRATION_NACK;
	
	return SendDatatoHost(msg.msg, DEC_TO_HOST_MSG_LENGTH);
	
	
	
}
unsigned long SendCalibrationErrorMsg()
{
	union t_auto_calibration_resp_msg msg;
	
	msg.msg_type=AUTO_CALIBRATION_ERROR;
	
	return SendDatatoHost(msg.msg, DEC_TO_HOST_MSG_LENGTH);
	
	
	
}
unsigned long SendAutoCalibrationCompleteMsg()
{
	union t_auto_calibration_resp_msg msg;
	
	msg.msg_type=AUTO_CALIBRATION_COMP;
	
	return SendDatatoHost(msg.msg, DEC_TO_HOST_MSG_LENGTH);
	
	
	
}
unsigned long SendLUTEntryMsg(Uchar ddl, unsigned long index)
{
	union t_lut_entry_msg msg;
	
	msg.msg_layout.msg_type=LUT_ENTRY;
	msg.msg_layout.ddl=ddl;
	msg.msg_layout.index=index;
	
	return SendDatatoHost(msg.msg, DEC_TO_HOST_MSG_LENGTH);
	
	
	
}



unsigned long SendLUTEntryWithLuminMsg(Uchar ddl, unsigned long index, float lumin)
{
	union t_lut_entry_with_lumin_msg msg;
	
	msg.msg_layout.msg_type=LUT_ENTRY;
	msg.msg_layout.ddl=ddl;
	msg.msg_layout.index=index;
	msg.msg_layout.lumin=lumin;
	
	return SendDatatoHost(msg.msg, DEC_TO_HOST_MSG_LENGTH);
	
	
	
}

unsigned long SendLutToHost(Uint16 index,float lux)
{
	union t_usb_cmd rsp;
	rsp.msg.cmd = CAL_GEN2_REQ + 100;
	rsp.msg.state = 1;
	rsp.msg2.data = index;
	rsp.msg2.data2 = lux;
	return SendDatatoHost(rsp.raw, DEC_TO_HOST_MSG_LENGTH);
}















