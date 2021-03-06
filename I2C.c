#include "usbcom.h"
#include "I2C.h"

//定义管脚
#define SCL GPIO_PIN_0
#define SDA GPIO_PIN_1
#define nop asm(" nop")

#define I2CPORT GPIO_PORTA_BASE

void I2C_Initial( void )
{
    // SCL:PA_0 ;  SDA:PA_1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	//weak pull-up
	GPIOPadConfigSet(I2CPORT, SCL|SDA,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);
    //将SCL管脚设置为输出管脚
    GPIOPinTypeGPIOOutput(I2CPORT, SCL|SDA);		
    I2C_Set_sck_low();
    I2C_STOP();
    Delay_ms(10);
    return;
}
void I2C_Set_sda_high( void )
{
    //将SDA设置为输出模式
    GPIOPinTypeGPIOOutput(I2CPORT, SDA);
    //SDA管脚输出为高电平
    GPIOPinWrite(I2CPORT, SDA, 0xFF);	
    
    nop;
    nop;
    return;
}
void I2C_Set_sda_low ( void )
{
    //将SDA设置为输出模式
    GPIOPinTypeGPIOOutput(I2CPORT, SDA);	
    //SDA管脚输出为低电平
    GPIOPinWrite(I2CPORT, SDA, 0x00);		
    
    nop;
    nop;
    return;
}
void I2C_Set_sck_high( void )
{
    //将SCL设置为输出模式
    //GPIOPinTypeGPIOOutput(I2CPORT, SCL);	
    //SCL管脚输出为高电平
    GPIOPinWrite(I2CPORT, SCL, 0xFF);		
    
    nop;
    nop;
    return;
}
void I2C_Set_sck_low ( void )
{
    //将SCL设置为输出模式
    //GPIOPinTypeGPIOOutput(I2CPORT, SCL);		
    //SCL管脚输出为低电平
    GPIOPinWrite(I2CPORT, SCL, 0x00);			
    
    nop;
    nop;
    return;
}
int  I2C_GetACK(void)
{
    int nTemp = 0;
    int j;
    
    nop;
    nop;
	    //将SDA设置为输入方向
    GPIOPinTypeGPIOInput(I2CPORT, SDA);	
    for(j = 5;j > 0;j--);
    I2C_Set_sck_low();
	for(j = 5;j > 0;j--);


    I2C_Set_sck_high();
    
    for(j = 20;j > 0;j--);
    //获得数据
    nTemp = (int)(GPIOPinRead(I2CPORT,SDA));	
    I2C_Set_sck_low();
    return (nTemp & SDA);
}
void I2C_SetACK(void)
{ 
    int j;
    I2C_Set_sck_low();
	//for(j = 50;j > 0;j--);
    I2C_Set_sda_low();
	for(j = 5;j > 0;j--);
    I2C_Set_sck_high();
	for(j = 20;j > 0;j--);
    I2C_Set_sck_low();
	//for(j = 200;j > 0;j--);
    return;
}
void I2C_SetNAk(void)
{
    int j;
    I2C_Set_sck_low();
	//for(j = 50;j > 0;j--);
    I2C_Set_sda_high();
	for(j = 5;j > 0;j--);
    I2C_Set_sck_high();
	for(j = 20;j > 0;j--);
    I2C_Set_sck_low();
	//for(j = 200;j > 0;j--);
    return;
}
void I2C_START(void)
{
    int i;
    
    I2C_Set_sda_high();
    //for(i = 10;i > 0;i--);
    I2C_Set_sck_high();
    for(i = 5;i > 0;i--);
    I2C_Set_sda_low();
    for(i = 20;i > 0;i--);
    I2C_Set_sck_low();
	//for(i = 10;i > 0;i--);
    return;
}
void I2C_STOP(void)
{
    int i;
    
    I2C_Set_sda_low();
    //for(i = 10;i > 0;i--);
    I2C_Set_sck_low();
    for(i = 5;i > 0;i--);
    I2C_Set_sck_high();
    for(i = 20;i > 0;i--);
    I2C_Set_sda_high();
    for(i = 20;i > 0;i--);
    //I2C_Set_sck_low();
    //延迟一点时间
    //Delay_ms(10);		
    
    return;
}
void I2C_TxHToL(int nValue)
{
    int i;
    int j;
    
    for(i = 0;i < 8;i++)
    {
        I2C_Set_sck_low();
    	if(nValue & 0x80)
    	    I2C_Set_sda_high();
    	else
    	    I2C_Set_sda_low();
    	for(j = 5;j > 0;j--);
    	I2C_Set_sck_high();
    	nValue <<= 1;
    	for(j = 20;j > 0;j--);
        
    }
    I2C_Set_sck_low();
    return;
}
void I2C_TxLToH(int nValue)
{
    int i;
    int j;
    
    for(i = 0;i < 8;i++)
    {
        I2C_Set_sck_low();
    	if(nValue & 0x01)
    	    I2C_Set_sda_high();
    	else
    	    I2C_Set_sda_low();
    	for(j = 5;j > 0;j--);
    	I2C_Set_sck_high();
    	nValue >>= 1;
    	for(j = 20;j > 0;j--);

    }
    I2C_Set_sck_low();
    return;
}
/////////////////////////////////////////////
// 接收是从 LSB 到 MSB 的顺序
int  I2C_RxByte(void)
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
        I2C_Set_sck_low();
		for(j = 5;j > 0;j--);
    	I2C_Set_sck_high();
    	for(j = 20;j > 0;j--);
    	if(GPIOPinRead(I2CPORT,SDA))
    	{
    	    nTemp |= (0x01 << i);
    	}
    }
    I2C_Set_sck_low();
    return nTemp;
}
/////////////////////////////////////////////
// 接收是从 MSB 到 LSB 的顺序
int  I2C_RxHToL(void)
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
        I2C_Set_sck_low();
		for(j = 5;j > 0;j--);
    	I2C_Set_sck_high();
    	for(j = 20;j > 0;j--);
    	if(GPIOPinRead(I2CPORT,SDA))
    	{
    	    nTemp |= (0x01 << (7 - i));
    	}

    }
    I2C_Set_sck_low();
    return nTemp;
}

void Delay_ms(unsigned long nValue)//毫秒为单位，8MHz为主时钟
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
void Delay_us(unsigned long nValue)//微秒为单位，8MHz为主时钟
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

//设备码 0x48
char readByte(int addr)
{
    char low;
    int j;
	int tmp = 0;
	char data = 0;
    //地址
    low = (char)(addr & 0xff);
    
    // 启动数据总线
    I2C_START();			
    // 发送写命令
    I2C_TxHToL(0x48);			
    // 等待 ACK
    tmp = I2C_GetACK();	
    //发送高地址
    I2C_TxHToL(low);	
    // 等待 ACK
    tmp = tmp +I2C_GetACK();    
	I2C_STOP();
	for(j = 10;j > 0;j--);
    // 启动数据总线
    I2C_START();	
    // 发送读命令
    I2C_TxHToL(0x49);	
    // 等待 ACK
    tmp = tmp +I2C_GetACK();	
    //读一个数据
    data = I2C_RxHToL();
    //设置NAK
    I2C_SetNAk();
    // 停止总线
    I2C_STOP();
    //u1printf("ack %d\n\r",tmp);
    return data;
}


void writeByte(int addr,char data)
{

    char low;
    int tmp = 0;
    //低地址
    low = (char)(addr & 0xff);
    
    // 启动数据总线
    I2C_START();			
    // 发送写命令
    I2C_TxHToL(0x48);			
    // 等待 ACK
    tmp = I2C_GetACK();	
    //发送地址
    I2C_TxHToL(low);	
    // 等待 ACK
    tmp = tmp +I2C_GetACK();
    //写入数据
    I2C_TxHToL(data);
    // 等待 ACK
    tmp = tmp +I2C_GetACK();
    // 停止总线
    I2C_STOP();	
    //u1printf("ack %d\n\r",tmp);
    return;
}


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

void test(void)
{

	u1printf("ALS %f\n\r",GetALSValue());
}

	

