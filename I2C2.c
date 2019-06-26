#include "usbcom.h"
#include "I2C2.h"

//定义管脚
#define SCL GPIO_PIN_4
#define SDA GPIO_PIN_5
#define nop asm(" nop")

#define I2CPORT GPIO_PORTE_BASE
#define DEVICE_ADDR 0xE0
static int s_device_addr = DEVICE_ADDR;


void Delay_ms2(unsigned long nValue)//毫秒为单位，8MHz为主时钟
{
    unsigned long nCount;
    int i;
    unsigned long j;
    nCount = 2667;
    for(i = nValue;i > 0;i--)
    {
    	for(j = nCount;j > 0;j--);
    }
    return;
}
void Delay_us2(unsigned long nValue)//微秒为单位，8MHz为主时钟
{
    int nCount;
    int i;
    int j;
    nCount = 3;
    for(i = nValue;i > 0;i--)
    {
    	for(j = nCount;j > 0;j--);
    }
    return;
}


void I2C_Set_sda_high2( void )
{
    //将SDA设置为输出模式
    GPIOPinTypeGPIOOutput(I2CPORT, SDA);
    //SDA管脚输出为高电平
    GPIOPinWrite(I2CPORT, SDA, 0xFF);	
    
    nop;
    nop;
    return;
}
void I2C_Set_sda_low2 ( void )
{
    //将SDA设置为输出模式
    GPIOPinTypeGPIOOutput(I2CPORT, SDA);	
    //SDA管脚输出为低电平
    GPIOPinWrite(I2CPORT, SDA, 0x00);		
    
    nop;
    nop;
    return;
}
void I2C_Set_sck_high2( void )
{
    //将SCL设置为输出模式
    GPIOPinTypeGPIOOutput(I2CPORT, SCL);
    //SCL管脚输出为高电平
    GPIOPinWrite(I2CPORT, SCL, 0xFF);		
    
    nop;
    nop;
    return;
}
void I2C_Set_sck_low2 ( void )
{
    //将SCL设置为输出模式
    GPIOPinTypeGPIOOutput(I2CPORT, SCL);
    //SCL管脚输出为低电平
    GPIOPinWrite(I2CPORT, SCL, 0x00);			
    
    nop;
    nop;
    return;
}
int  I2C_GetACK2(void)
{
    int nTemp = 0;
    int j;
    
    nop;
    nop;
	    //将SDA设置为输入方向
    GPIOPinTypeGPIOInput(I2CPORT, SDA);	
    for(j = 5;j > 0;j--);
    I2C_Set_sck_low2();
	for(j = 5;j > 0;j--);


    I2C_Set_sck_high2();
    
    for(j = 10;j > 0;j--);
    //获得数据
    nTemp = (int)(GPIOPinRead(I2CPORT,SDA));	
    I2C_Set_sck_low2();
    return (nTemp & SDA);
}
void I2C_SetACK2(void)
{ 
    int j;
    I2C_Set_sck_low2();
	//for(j = 50;j > 0;j--);
    I2C_Set_sda_low2();
	for(j = 5;j > 0;j--);
    I2C_Set_sck_high2();
	for(j = 10;j > 0;j--);
    I2C_Set_sck_low2();
	//for(j = 200;j > 0;j--);
    return;
}
void I2C_SetNAk2(void)
{
    int j;
    I2C_Set_sck_low2();
	//for(j = 50;j > 0;j--);
    I2C_Set_sda_high2();
	for(j = 5;j > 0;j--);
    I2C_Set_sck_high2();
	for(j = 10;j > 0;j--);
    I2C_Set_sck_low2();
	//for(j = 200;j > 0;j--);
    return;
}
void I2C_START2(void)
{
    int i;
    
    I2C_Set_sda_high2();
    //for(i = 10;i > 0;i--);
    I2C_Set_sck_high2();
    for(i = 5;i > 0;i--);
    I2C_Set_sda_low2();
    for(i = 10;i > 0;i--);
    I2C_Set_sck_low2();
	//for(i = 10;i > 0;i--);
    return;
}
void I2C_STOP2(void)
{
    int i;
    
    I2C_Set_sda_low2();
    //for(i = 10;i > 0;i--);
    I2C_Set_sck_low2();
    for(i = 5;i > 0;i--);
    I2C_Set_sck_high2();
    for(i = 10;i > 0;i--);
    I2C_Set_sda_high2();
    for(i = 10;i > 0;i--);
    //I2C_Set_sck_low();
    //延迟一点时间
    //Delay_ms(10);		
    
    return;
}
void I2C_TxHToL2(int nValue)
{
    int i;
    int j;
    
    for(i = 0;i < 8;i++)
    {
        I2C_Set_sck_low2();
    	if(nValue & 0x80)
    	    I2C_Set_sda_high2();
    	else
    	    I2C_Set_sda_low2();
    	for(j = 5;j > 0;j--);
    	I2C_Set_sck_high2();
    	nValue <<= 1;
    	for(j = 10;j > 0;j--);
        
    }
    I2C_Set_sck_low2();
    return;
}
void I2C_TxLToH2(int nValue)
{
    int i;
    int j;
    
    for(i = 0;i < 8;i++)
    {
        I2C_Set_sck_low2();
    	if(nValue & 0x01)
    	    I2C_Set_sda_high2();
    	else
    	    I2C_Set_sda_low2();
    	for(j = 5;j > 0;j--);
    	I2C_Set_sck_high2();
    	nValue >>= 1;
    	for(j = 10;j > 0;j--);

    }
    I2C_Set_sck_low2();
    return;
}


void I2C_Initial2( void )
{
    // SCL:PA_0 ;  SDA:PA_1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	//weak pull-up
	GPIOPadConfigSet(I2CPORT, SCL|SDA,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_OD);
    //将SCL管脚设置为输出管脚
    GPIOPinTypeGPIOInput(I2CPORT, SCL|SDA);
    I2C_Set_sck_low2();
    I2C_STOP2();
    Delay_ms2(5);

    s_device_addr = DEVICE_ADDR;
    return;
}

/////////////////////////////////////////////
// 接收是从 LSB 到 MSB 的顺序
int  I2C_RxByte2(void)
{
    int nTemp = 0;
    int i;
    int j;
    
    //I2C_Set_sda_high();
    //将SDA管脚设置为输入方向
    GPIOPinTypeGPIOInput(I2CPORT, SDA);			
    nop;
    nop;
    nop;
    nop;
    for(i = 0;i < 8;i++)
    {
        I2C_Set_sck_low2();
		for(j = 5;j > 0;j--);
    	I2C_Set_sck_high2();
    	for(j = 10;j > 0;j--);
    	if(GPIOPinRead(I2CPORT,SDA))
    	{
    	    nTemp |= (0x01 << i);
    	}
    }
    I2C_Set_sck_low2();
    return nTemp;
}
/////////////////////////////////////////////
// 接收是从 MSB 到 LSB 的顺序
int  I2C_RxHToL2(void)
{
    int nTemp = 0;
    int i;
    int j;
    
    //I2C_Set_sda_high();
    //将SDA管脚设置为输入方向
    GPIOPinTypeGPIOInput(I2CPORT, SDA);				
    nop;
    nop;
    nop;
    nop;
    for(i = 0;i < 8;i++)
    {
        I2C_Set_sck_low2();
		for(j = 5;j > 0;j--);
    	I2C_Set_sck_high2();
    	for(j = 10;j > 0;j--);
    	if(GPIOPinRead(I2CPORT,SDA))
    	{
    	    nTemp |= (0x01 << (7 - i));
    	}

    }
    I2C_Set_sck_low2();
    return nTemp;
}

void setDevAddr(int addr)
{
	s_device_addr = addr;
}

int getDevAddr()
{
	return s_device_addr;
}

//设备码 0x70
char readByte2(int addr)
{
    char low;
    int j;
	int tmp = 0;
	char data = 0;
    //地址
    low = (char)(addr & 0xff);
    
    // 启动数据总线
    I2C_START2();
    // 发送写命令
    I2C_TxHToL2(s_device_addr);
    // 等待 ACK
    tmp = I2C_GetACK2();
    //发送高地址
    I2C_TxHToL2(low);
    // 等待 ACK
    tmp = tmp +I2C_GetACK2();
	I2C_STOP();
	for(j = 5;j > 0;j--);
    // 启动数据总线
    I2C_START2();
    // 发送读命令
    I2C_TxHToL2(s_device_addr+1);
    // 等待 ACK
    tmp = tmp +I2C_GetACK2();
    //读一个数据
    data = I2C_RxHToL2();
    //设置NAK
    I2C_SetNAk2();
    // 停止总线
    I2C_STOP2();
    //u1printf("ack %d\n\r",tmp);
    return data;
}


void writeByte2(int addr,char data)
{

    char low;
    int tmp = 0;
    //低地址
    low = (char)(addr & 0xff);
    
    // 启动数据总线
    I2C_START2();
    // 发送写命令
    I2C_TxHToL2(s_device_addr);
    // 等待 ACK
    tmp = I2C_GetACK2();
    //发送地址
    I2C_TxHToL2(low);
    // 等待 ACK
    tmp = tmp +I2C_GetACK2();
    //写入数据
    I2C_TxHToL2(data);
    // 等待 ACK
    tmp = tmp +I2C_GetACK2();
    // 停止总线
    I2C_STOP2();
    //u1printf("ack %d\n\r",tmp);
    return;
}
//70M 72
/*
void initclk()
{
	setDevAddr(0xD4);

	writeByte2(0x10,0x80);
	writeByte2(0x11,0x0C);
	writeByte2(0x12,0x81);
	writeByte2(0x13,0x80);
	writeByte2(0x14,0x00);
	writeByte2(0x15,0x03);
	writeByte2(0x16,0x8C);
	writeByte2(0x17,0x02);
	writeByte2(0x18,0xA0);
	writeByte2(0x19,0x00);
	writeByte2(0x1A,0x00);
	writeByte2(0x1B,0x00);
	writeByte2(0x1C,0x9F);
	writeByte2(0x1D,0xFF);
	writeByte2(0x1E,0xF0);
	writeByte2(0x1F,0x80);

	writeByte2(0x20,0x00);
	writeByte2(0x21,0x81);
	writeByte2(0x22,0x00);
	writeByte2(0x23,0x00);
	writeByte2(0x24,0x00);
	writeByte2(0x25,0x00);
	writeByte2(0x26,0x00);
	writeByte2(0x27,0x00);
	writeByte2(0x28,0x00);
	writeByte2(0x29,0x00);
	writeByte2(0x2A,0x04);
	writeByte2(0x2B,0x00);
	writeByte2(0x2C,0x10);
	writeByte2(0x2D,0x00);
	writeByte2(0x2E,0x60);
	writeByte2(0x2F,0x04); //64

	writeByte2(0x30,0x00);
	writeByte2(0x31,0x81);
	writeByte2(0x32,0x00);
	writeByte2(0x33,0x00);
	writeByte2(0x34,0x00);
	writeByte2(0x35,0x00);
	writeByte2(0x36,0x00);
	writeByte2(0x37,0x00);
	writeByte2(0x38,0x00);
	writeByte2(0x39,0x00);
	writeByte2(0x3A,0x04);
	writeByte2(0x3B,0x00);
	writeByte2(0x3C,0x10);
	writeByte2(0x3D,0x00);
	writeByte2(0x3E,0x60);
	writeByte2(0x3F,0x04); //64

	writeByte2(0x40,0x00);
	writeByte2(0x41,0x00);
	writeByte2(0x42,0x00);
	writeByte2(0x43,0x00);
	writeByte2(0x44,0x00);
	writeByte2(0x45,0x00);
	writeByte2(0x46,0x00);
	writeByte2(0x47,0x00);
	writeByte2(0x48,0x00);
	writeByte2(0x49,0x00);
	writeByte2(0x4A,0x04);
	writeByte2(0x4B,0x00);
	writeByte2(0x4C,0x00);
	writeByte2(0x4D,0x00);
	writeByte2(0x4E,0x00);
	writeByte2(0x4F,0x00);

	writeByte2(0x50,0x00);
	writeByte2(0x51,0x00);
	writeByte2(0x52,0x00);
	writeByte2(0x53,0x00);
	writeByte2(0x54,0x00);
	writeByte2(0x55,0x00);
	writeByte2(0x56,0x00);
	writeByte2(0x57,0x00);
	writeByte2(0x58,0x00);
	writeByte2(0x59,0x00);
	writeByte2(0x5A,0x04);
	writeByte2(0x5B,0x00);
	writeByte2(0x5C,0x00);
	writeByte2(0x5D,0x00);
	writeByte2(0x5E,0x00);
	writeByte2(0x5F,0x00);

	writeByte2(0x60,0x73);
	writeByte2(0x61,0x01);
	writeByte2(0x62,0x73);
	writeByte2(0x63,0x01);
	writeByte2(0x64,0xBB);
	writeByte2(0x65,0x00);
	writeByte2(0x66,0xBB);
	writeByte2(0x67,0x00);
	writeByte2(0x68,0x1E);
	writeByte2(0x69,0xE4);


}
*/

//66M 72
void initclk()
{
	setDevAddr(0xD4);

		writeByte2(0x10,0x44);
		writeByte2(0x11,0x0C);
		writeByte2(0x12,0x81);
		writeByte2(0x13,0x80);
		writeByte2(0x14,0x00);
		writeByte2(0x15,0x04);
		writeByte2(0x16,0x0C);
		writeByte2(0x17,0x0A);
		writeByte2(0x18,0x80);
		writeByte2(0x19,0x00);
		writeByte2(0x1A,0x00);
		writeByte2(0x1B,0x00);
		writeByte2(0x1C,0x9F);
		writeByte2(0x1D,0xFF);
		writeByte2(0x1E,0xD8);
		writeByte2(0x1F,0x80);

		writeByte2(0x20,0x00);
		writeByte2(0x21,0x0C);
		writeByte2(0x22,0x00);
		writeByte2(0x23,0x00);
		writeByte2(0x24,0x00);
		writeByte2(0x25,0x00);
		writeByte2(0x26,0x00);
		writeByte2(0x27,0x00);
		writeByte2(0x28,0x00);
		writeByte2(0x29,0x00);
		writeByte2(0x2A,0x04);
		writeByte2(0x2B,0x00);
		writeByte2(0x2C,0x00);
		writeByte2(0x2D,0x00);
		writeByte2(0x2E,0x60);
		writeByte2(0x2F,0x00); //64

		writeByte2(0x30,0x00);
		writeByte2(0x31,0x81);
		writeByte2(0x32,0x00);
		writeByte2(0x33,0x00);
		writeByte2(0x34,0x00);
		writeByte2(0x35,0x00);
		writeByte2(0x36,0x00);
		writeByte2(0x37,0x00);
		writeByte2(0x38,0x00);
		writeByte2(0x39,0x00);
		writeByte2(0x3A,0x04);
		writeByte2(0x3B,0x00);
		writeByte2(0x3C,0x70);
		writeByte2(0x3D,0x00);
		writeByte2(0x3E,0x60);
		writeByte2(0x3F,0x37); //64

		writeByte2(0x40,0x00);
		writeByte2(0x41,0x00);
		writeByte2(0x42,0x00);
		writeByte2(0x43,0x00);
		writeByte2(0x44,0x00);
		writeByte2(0x45,0x00);
		writeByte2(0x46,0x00);
		writeByte2(0x47,0x00);
		writeByte2(0x48,0x00);
		writeByte2(0x49,0x00);
		writeByte2(0x4A,0x04);
		writeByte2(0x4B,0x00);
		writeByte2(0x4C,0x00);
		writeByte2(0x4D,0x00);
		writeByte2(0x4E,0x00);
		writeByte2(0x4F,0x00);

		writeByte2(0x50,0x00);
		writeByte2(0x51,0x00);
		writeByte2(0x52,0x00);
		writeByte2(0x53,0x00);
		writeByte2(0x54,0x00);
		writeByte2(0x55,0x00);
		writeByte2(0x56,0x00);
		writeByte2(0x57,0x00);
		writeByte2(0x58,0x00);
		writeByte2(0x59,0x00);
		writeByte2(0x5A,0x04);
		writeByte2(0x5B,0x00);
		writeByte2(0x5C,0x00);
		writeByte2(0x5D,0x00);
		writeByte2(0x5E,0x00);
		writeByte2(0x5F,0x00);

		writeByte2(0x60,0x73);
		writeByte2(0x61,0x01);
		writeByte2(0x62,0x73);
		writeByte2(0x63,0x01);
		writeByte2(0x64,0xBB);
		writeByte2(0x65,0x00);
		writeByte2(0x66,0xBB);
		writeByte2(0x67,0x00);
		writeByte2(0x68,0x1E);
		writeByte2(0x69,0xE4);


}

void ResetFPGA()
{
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_7|GPIO_PIN_1);
	//GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPD);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1,0x00);
	Delay_ms(100);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1,0xFF);
}



	

