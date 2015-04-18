#include "usbcom.h"
#include "I2C.h"

//����ܽ�
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
    //��SCL�ܽ�����Ϊ����ܽ�
    GPIOPinTypeGPIOOutput(I2CPORT, SCL|SDA);		
    I2C_Set_sck_low();
    I2C_STOP();
    Delay_ms(10);
    return;
}
void I2C_Set_sda_high( void )
{
    //��SDA����Ϊ���ģʽ
    GPIOPinTypeGPIOOutput(I2CPORT, SDA);
    //SDA�ܽ����Ϊ�ߵ�ƽ
    GPIOPinWrite(I2CPORT, SDA, 0xFF);	
    
    nop;
    nop;
    return;
}
void I2C_Set_sda_low ( void )
{
    //��SDA����Ϊ���ģʽ
    GPIOPinTypeGPIOOutput(I2CPORT, SDA);	
    //SDA�ܽ����Ϊ�͵�ƽ
    GPIOPinWrite(I2CPORT, SDA, 0x00);		
    
    nop;
    nop;
    return;
}
void I2C_Set_sck_high( void )
{
    //��SCL����Ϊ���ģʽ
    //GPIOPinTypeGPIOOutput(I2CPORT, SCL);	
    //SCL�ܽ����Ϊ�ߵ�ƽ
    GPIOPinWrite(I2CPORT, SCL, 0xFF);		
    
    nop;
    nop;
    return;
}
void I2C_Set_sck_low ( void )
{
    //��SCL����Ϊ���ģʽ
    //GPIOPinTypeGPIOOutput(I2CPORT, SCL);		
    //SCL�ܽ����Ϊ�͵�ƽ
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
	    //��SDA����Ϊ���뷽��
    GPIOPinTypeGPIOInput(I2CPORT, SDA);	
    for(j = 5;j > 0;j--);
    I2C_Set_sck_low();
	for(j = 5;j > 0;j--);


    I2C_Set_sck_high();
    
    for(j = 20;j > 0;j--);
    //�������
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
    //�ӳ�һ��ʱ��
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
// �����Ǵ� LSB �� MSB ��˳��
int  I2C_RxByte(void)
{
    int nTemp = 0;
    int i;
    int j;
    
    //I2C_Set_sda_high();
    //��SDA�ܽ�����Ϊ���뷽��
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
// �����Ǵ� MSB �� LSB ��˳��
int  I2C_RxHToL(void)
{
    int nTemp = 0;
    int i;
    int j;
    
    //I2C_Set_sda_high();
    //��SDA�ܽ�����Ϊ���뷽��
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

void Delay_ms(unsigned long nValue)//����Ϊ��λ��8MHzΪ��ʱ��
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
void Delay_us(unsigned long nValue)//΢��Ϊ��λ��8MHzΪ��ʱ��
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

//�豸�� 0x48
void readByte(int addr,char pBuf[])
{
    char low;
    int j;
	int tmp = 0;
    //��ַ
    low = (char)(addr & 0xff);
    
    // ������������
    I2C_START();			
    // ����д����
    I2C_TxHToL(0x48);			
    // �ȴ� ACK
    tmp = I2C_GetACK();	
    //���͸ߵ�ַ
    I2C_TxHToL(low);	
    // �ȴ� ACK
    I2C_GetACK();    
	I2C_STOP();
	for(j = 50;j > 0;j--);
    // ������������
    I2C_START();	
    // ���Ͷ�����
    I2C_TxHToL(0x49);	
    // �ȴ� ACK
    I2C_GetACK();	
    //��һ������
    pBuf[0] = I2C_RxHToL();
    //����NAK
    I2C_SetNAk();
    // ֹͣ����
    I2C_STOP();
    u1printf("ack %d\n\r",tmp);
    return;
}

void readPage(int addr,int count,char pBuf[])
{
    char low;
    int j;
	int tmp = 0;
    //��ַ
    low = (char)(addr & 0xff);
    
    // ������������
    I2C_START();			
    // ����д����
    I2C_TxHToL(0x48);			
    // �ȴ� ACK
    tmp = I2C_GetACK();	
    //���͸ߵ�ַ
    I2C_TxHToL(low);	
    // �ȴ� ACK
    I2C_GetACK();    
	I2C_STOP();
	for(j = 50;j > 0;j--);
    // ������������
    I2C_START();	
    // ���Ͷ�����
    I2C_TxHToL(0x49);	
    // �ȴ� ACK
    I2C_GetACK();	
	for(j=0;j<count;j++)
	{
      //��һ������
      pBuf[j] = I2C_RxHToL();
      //����NAK
      if(j<count -1)
      	{
      	  I2C_SetACK();
      	}
	  else
	  	{
	      I2C_SetNAk();
	  	}
	}
    // ֹͣ����
    I2C_STOP();
    u1printf("ack %d\n\r",tmp);
    return;
}

void writeByte(int addr,char pBuf[])
{

    char low;
    int tmp = 0;
    //�͵�ַ
    low = (char)(addr & 0xff);
    
    // ������������
    I2C_START();			
    // ����д����
    I2C_TxHToL(0x48);			
    // �ȴ� ACK
    tmp = I2C_GetACK();	
    //���͵�ַ
    I2C_TxHToL(low);	
    // �ȴ� ACK
    I2C_GetACK();
    //д������
    I2C_TxHToL(pBuf[0]);
    // �ȴ� ACK
    I2C_GetACK();
    // ֹͣ����
    I2C_STOP();	
    u1printf("ack %d\n\r",tmp);
    return;
}


void test(void)
{
    char puf[8] = {0};
    int i= 0;
	/*
    for(i=0;i<50;i++)
    	{
    	
	I2C_Set_sda_high();
	I2C_Set_sck_high();
	Delay(1);
	I2C_Set_sda_low();
	I2C_Set_sck_low();
	Delay(1);
    	}
    	*/
    //writeByte(0x0e,0x55);
	//Delay_ms(100);
	readByte(0x00,puf);
	u1printf("i2c test 0x%x\n\r",puf[0]);
	readByte(0x01,puf);
	u1printf("i2c test 0x%x\n\r",puf[0]);
}
	

