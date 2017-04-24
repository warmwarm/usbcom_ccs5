#include "i2c.h"

void InitALS(void)
{
	I2C_Initial();
	
    writeByte(1,0x00);//Disable and Power down
	writeByte(2,0x00);//Clear all interrupt flag
	writeByte(0x0e,0x00);//Initialize reset registers

	writeByte(5,0x00);//disable ALS interrupt
	writeByte(6,0xf0);
	writeByte(7,0xff);
	writeByte(1,0x06);//enable ALS
}

float GetALSValue(void)
{
    float value = 0;
	char tmp = 0;
	tmp= readByte(0x0a);
	value = tmp;
	tmp = readByte(0x09);

	value = value * 0x100 + tmp;
	value = value/4095 * 2000;
	return value;
}

void GetALSRawData(char * pbuf)
{
	if(0 == pbuf)
	{
	    return ;
	}
	pbuf[0] = readByte(0x09);
	pbuf[1] = readByte(0x0a);
	//pbuf[1] = (pbuf[1]&0x0F);
	pbuf[2] = readByte(0x00);
	pbuf[3] = 0x0A;
	return;
}

void ALTest(void)
{

	u1printf("ALS %f\n\r",GetALSValue());
}

