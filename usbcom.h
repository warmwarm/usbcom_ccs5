#ifndef USBCOM_H_
#define USBCOM_H_
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/timer.h"
 
  
#include "inc/hw_nvic.h" 
 
 
#include "inc/hw_ssi.h"
 
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/ssi.h"
#include "utils/softssi.h"
#include "driverlib/usb.h"
#include "grlib/grlib.h"
#include "usblib/usblib.h"
#include "usblib/usbhid.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdhid.h"
#include "usblib/device/usbdhidmouse.h"
 
#include "utils/uartstdio.h"
#include "drivers/buttons.h"
#include "drivers/formike128x128x16.h"

#define  MCU_VERSION_STRING "2013-02-24"
 
 
 
typedef unsigned char       Uchar;
#define LOW(U16)                ((Uchar)U16)
#define HIGH(U16)               ((Uchar)(U16>>8))	
typedef unsigned int        Uint16; 
typedef unsigned long        Uint32; 

 
#include <string.h> 
#define VENDOR_ID             0x242C        /* Atmel vendor ID = 0x242C */
#define PRODUCT_ID            0x1006        /* Product ID for LCD USB : 0x1003 */

#define GPIO_PINS GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7

#define USE_UART0
#define USE_GPIO_FOR_FPGA

//for calibration
extern int calib_status;

#define CALST_NIL 0 
#define CALST_INIT 1
#define CALST_STABLIZATION 2
#define CALST_BL_CTRL 3
#define CALST_BL_CTRL_LOW 4
#define CALST_BL_CTRL_HIGH 5
#define CALST_CALIBRATION 6


//mcu op mode
#define MCU_OP_MODE_GEN1  1
#define MCU_OP_MODE_GEN2  2

#define GEN_MAX_LUM 2047 // for test we use 12336, will change to 2048
#define GEN_MIN_LUM 0 // for test we use 12593, will change to 2048

#define DEFAULT_BACKLIGHT_CONTROL_LEVEL 250

#define BACKLIGHT_CONTROL_UPPER_BOND 180
#define BACKLIGHT_CONTROL_LOWER_BOND 160

//define message type from USB device to USB host
#define DEC_TO_HOST_MSG_LENGTH 34
#define DEC_TO_HOST_INT_LENGTH 16

#define AUTO_CALIBRATION_ACK 1
#define AUTO_CALIBRATION_NACK 2
#define AUTO_CALIBRATION_ERROR 3
#define AUTO_CALIBRATION_COMP 4
#define LUT_ENTRY              5
#define LUT_ENTRY_WITH_LUMIN   6


//Protocol V2
//host->MCU
#define WRITE_LUT_REQ 0
#define CAL_GEN1_REQ 1
#define WRITE_LUT_DEFAULT_REQ 2
#define RESTORE_LUT_REQ 3
#define ACTIVE_GEN1_REQ 4
#define ACTIVE_GEN2_REQ 5
#define GOTO_BOOTLOADER_REQ 6
#define READ_EEPROM_REQ 7
#define CAL_GEN2_REQ 8
#define SET_LIFETIME_REQ 9
#define GET_LIFETIME_REQ 10
#define SET_BACKLIGHT_REQ 11
#define GET_BACKLIGHT_REQ 12
#define SET_DICOM_STATUS_REQ 13
#define GET_DICOM_STATUS_REQ  14
#define SET_POWER_STATUS_REQ  15
#define GET_POWER_STATUS_REQ  16
#define GET_MCU_VERSION_REQ  17
#define GET_FPGA_VERSION_REQ  18
#define GET_LC_VERSION_REQ  19
#define GET_HW_VERSION_REQ  20


#define WRITE_LUT_RSP 100
#define CAL_GEN1_RSP 101
#define WRITE_LUT_DEFAULT_RSP 102
#define RESTORE_LUT_RSP 103
#define ACTIVE_GEN1_RSP 104
#define ACTIVE_GEN2_RSP 105
#define GOTO_BOOTLOADER_RSP 106
#define READ_EEPROM_RSP 107
#define CAL_GEN2_RSP 108
#define SET_LIFETIME_RSP 109
#define GET_LIFETIME_RSP 110
#define SET_BACKLIGHT_RSP 111
#define GET_BACKLIGHT_RSP 112
#define SET_DICOM_STATUS_RSP 113
#define GET_DICOM_STATUS_RSP 114
#define SET_POWER_STATUS_RSP 115
#define GET_POWER_STATUS_RSP 116
#define GET_MCU_VERSION_RSP  117
#define GET_FPGA_VERSION_RSP  118
#define GET_LC_VERSION_RSP  119
#define GET_HW_VERSION_RSP  120


#define DICOM_EVENT_REPORT 150
#define CAL_EVENT_REPORT 151
#define POWER_EVENT_REPORT 152
#define ERROR_EVENT_REPORT 153


unsigned long  SendDatatoHost(Uchar *ptr, int len);
unsigned long SendCalibrationAckMsg();
unsigned long SendCalibrationNAckMsg();
unsigned long SendCalibrationErrorMsg();
unsigned long SendAutoCalibrationCompleteMsg();
unsigned long SendLUTEntryMsg(Uchar ddl, unsigned long index);
unsigned long SendLUTEntryWithLuminMsg(Uchar ddl, unsigned long index, float lumin);
unsigned long SendLutToHost(unsigned short index,Uchar lux1,Uchar lux2,Uchar lux3);
unsigned long SendLutToRsp(Uchar cmd,Uchar state,Uchar function);



//UART

void InitUART0();
void InitUART1();
void TST_UART1_IntHandler(void);
void MTR_UART0_IntHandler(void);
void printc(char c);
void print(const unsigned char *pucBuffer);
int u1printf(const char* aStrToPrint, ...);
void TST_UART1_Send(const unsigned char *pucBuffer, unsigned int ulCount);
void MTR_UART0_Send(const unsigned char *pucBuffer, unsigned int ulCount);
void SendRequestToLuxmeter(unsigned char index0, unsigned char index1);
void ProcessDataFromLuxmeter(unsigned char *pdata, unsigned int count);
void SendMaxToLuxmeter(unsigned int lux);
void SendMinToLuxmeter(unsigned int lux);
unsigned char CRC8_Tab(unsigned char *p, char counter);


//extern    unsigned char rvcBuf[10];
//extern    int rcvN;
//extern unsigned long uart1ruan;
//GPIO
void InitGPIO();
void EnableTristate();
void DisableTristate();
void SelectEEPROM();
void DeselectEEPROM();
void InitGPIOPB3();
void GPIOPB3IntHandler(void);
//FPGA
void InfomFPGABeginCalibration();
void InfomFPGAGenGivenGrayScale(unsigned char lo, unsigned char hi);
void InfomFPGAEndCalibration();
void InfomFPGASetMaxBackgroundLevel(unsigned char percentage);
void InformFPGASetGen1Mode();
void InformFPGASetGen2Mode();
void InformFPGAReleaseEEPROM();
void InformFPGATakeEEPROM();

long ReadFPGAStatusRegister();
long DICOMButtonPressed();
long CALButtonPressed();
long PoweronPressed();
extern volatile long FPGACtrlBitSet;
//
void WriteCalibrationStatusMsg0();
void WriteCalibrationStatusMsg1();
void WriteCalibrationStatusMsg2();
void ClearCalibrationStatusMsg();



//SPI
long SPIWriteByte(unsigned long HiAddr,unsigned long LoAddr, Uchar data);
long SPIReadByte(unsigned long HiAddr,unsigned long LoAddr, Uchar *pdata);
long CheckEepromWriteDone();
void InitSPI();
void SSI0Int_IntHandler(void);
void WriteLutIndex(unsigned char ddl,unsigned long index);
void saveMaxBacklightLevel(unsigned char level);
void getMaxBacklightLevel();
long SPIWriteNByte(unsigned long HiAddr,unsigned long LoAddr, Uchar * pdata,int len);

//usb
void JumpToDFU();
#define USB_MUX_GPIO_PERIPH     SYSCTL_PERIPH_GPIOH
#define USB_MUX_GPIO_BASE       GPIO_PORTH_BASE
#define USB_MUX_GPIO_PIN        GPIO_PIN_2
#define USB_MUX_SEL_DEVICE      USB_MUX_GPIO_PIN
#define USB_MUX_SEL_HOST        0

void InitAmedicomGen2USBDevice();

void usbRxHandler(unsigned char*rev_buf);

extern Uchar tx[34];
extern tUSBDHIDDevice g_sHIDAmedicomGen2Device;
//system
void InitAmedicomGen2System();
void EnterInfiniteLoop();











//utilities
void itoa(char *p, int i);
void ftoa( char *p, float f);


//calibration
float JND_to_LUMINANCE(float JND);
float LUMINANCE_to_JND(float LUMINANCE);

extern unsigned char backlightControlLevel;
void Delay(unsigned long ulSeconds);

extern long DICOMbuttonState;
extern long CALbuttonState;
extern long PowerbuttonState;

void ClearExistingCalibration();
//timer

void InitTimer();
void Timer0IntHandler(void);





//auto calibration from calispector
extern long AutoCalibrationFromCalispector;


//timer to record monitor life
void increase_time_duration();
void reset_life_time();
unsigned long get_live_time();

void PostSPI();

#endif /*USBCOM_H_*/
