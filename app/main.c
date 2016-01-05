
/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : August 5, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR LPC2478-SK board.
 *  It implements USB CDC (Communication Device Class) device and install
 * it like a Virtual COM port. The UART0 is used for physical implementation
 * of the RS232 port.
 *
 * Jumpers:
 *  EXT/JLINK  - depending of power source
 *  ISP_E      - unfilled
 *  RST_E      - unfilled
 *  BDS_E      - unfilled
 *  C/SC       - SC
 *
 * Note:
 *  After power-up the controller gets it's clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 * The LCD shares pins with Trace port. If ETM is enabled the LCD will not work.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#define NONPROT 0xFFFFFFFF
#define CRP1  	0x12345678
#define CRP2  	0x87654321
/*If CRP3 is selected, no future factory testing can be performed on the device*/
#define CRP3  	0x43218765

#ifndef SDRAM_DEBUG
#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned crp = NONPROT;
#endif

#define LCD_VRAM_BASE_ADDR ((Int32U)&SDRAM_BASE_ADDR)

extern Int32U SDRAM_BASE_ADDR;
extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
int main(void)
{
Int8U Buffer[100];
pInt8U pBuffer;
Int32U Size,TranSize;
int i,j;

Boolean CdcConfigureStateHold;

#if CDC_DEVICE_SUPPORT_LINE_CODING > 0
CDC_LineCoding_t CDC_LineCoding;
UartLineCoding_t UartLineCoding;
#endif // CDC_DEVICE_SUPPORT_LINE_CODING > 0

#if CDC_DEVICE_SUPPORT_LINE_STATE > 0
UartLineEvents_t      UartLineEvents;

SerialState_t   SerialState;
#endif // CDC_DEVICE_SUPPORT_LINE_STATE > 0 

  GLCD_Ctrl (FALSE);
  // Init GPIO
  GpioInit();
#ifndef SDRAM_DEBUG
  // MAM init
  MAMCR_bit.MODECTRL = 0;
  MAMTIM_bit.CYCLES  = 3;   // FCLK > 40 MHz
  MAMCR_bit.MODECTRL = 2;   // MAM functions fully enabled
  // Init clock
  InitClock();
  // SDRAM Init
  SDRAM_Init();
#endif // SDRAM_DEBUG
  // Init VIC
  VIC_Init();
  // GLCD init
  GLCD_Init (NULL, NULL);
  // Disable Hardware cursor
  GLCD_Cursor_Dis(0);
  // Init UART 0
  UartInit(UART_0,4,NORM);
  // Init USB
 // UsbCdcInit();

  __enable_interrupt();

  // Soft connection enable
 // USB_ConnectRes(TRUE);
  // Enable GLCD
  GLCD_Ctrl (TRUE);

  GLCD_SetFont(&Terminal_18_24_12,0x0000FF,0x000cd4ff);
  GLCD_SetWindow(95,10,255,33);
  GLCD_TextSetPos(0,0);
  GLCD_print("\Hello PENIS");

  GLCD_SetFont(&Terminal_9_12_6,0x0000FF,0x000cd4ff);
  GLCD_SetWindow(0,100,320,140);
  GLCD_TextSetPos(0,0);
  //GLCD_print("\fCommunication device class Skid Skid");

 // CdcConfigureStateHold = !IsUsbCdcConfigure();
  
 // GLCD_SetWindow(100,150,200,200);
 // GLCD_TextSetPos(300,300);
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     
      // Update the baud rate
      UartLineCoding.dwDTERate = 115200;
      // Update the stop bits number
      UartLineCoding.bStopBitsFormat = UART_ONE_STOP_BIT;
      // Update the parity type
      UartLineCoding.bParityType = UART_NO_PARITY;
      // Update the word width
      UartLineCoding.bDataBits = (UartWordWidth_t)(3);
      //Description: Init UART Baud rate, Word width, Stop bits, Parity type
      UartSetLineCoding(UART_0,UartLineCoding);
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     
  
  while(1)
  {
    /*for(i=0;i<10;i++){
      GLCD_print("%d\n",i);
      GLCD_TextSetPos(0,0);
      
      for(j=0;j<2000;j++){};     
  }*/
  
  Size = UartRead(UART_0,Buffer,sizeof(Buffer)-1);

  GLCD_print("%s\n",Buffer);
  GLCD_TextSetPos(0,0);
  
  
}
}