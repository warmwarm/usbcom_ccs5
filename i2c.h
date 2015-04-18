//¶¨Òåº¯Êý
void I2C_Initial(void);
void I2C_Set_sda_high(void);
void I2C_Set_sda_low (void);
void I2C_Set_sck_high(void);
void I2C_Set_sck_low (void);
int  I2C_GetACK(void);
void I2C_SetACK(void);
void I2C_SetNAk(void);
void I2C_START(void);
void I2C_STOP(void);
void I2C_TxHToL(int);
void I2C_TxLToH(int);
int  I2C_RxByte(void);
int  I2C_RxHToL(void);
void Delay_ms(unsigned long nValue);
void Delay_us(unsigned long nValue);

void test(void);

