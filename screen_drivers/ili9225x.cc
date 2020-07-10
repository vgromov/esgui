// ili9225x driver interface implementation
#ifndef _ili_9225_x_cc_
#define _ili_9225_x_cc_

// ili925x.cc file - ILITEK a-Si TFT LCD Single Chip Driver
//
#include <esfwxe/utils.h>

#define ili9225xCmdWrite(regAddr, reg) \
  esguiScreenBusWriteCmdB(hdrv->hbus, (regAddr)); \
  esguiScreenBusWriteDataW(hdrv->hbus, (reg))  

// Entry Mode (R03h)
// R/W RS D15 D14 D13 D12 D11 D10 D9   D8   D7 D6 D5  D4  D3 D2 D1 D0
// W    1 0   0   0   BGR 0   0   MDT1 MDT0 0   0 ID1 ID0 AM 0  0  0
// AM Control the GRAM update direction. When AM = “0”, the address is updated in horizontal writing direction.
// When AM = “1”, the address is updated in vertical writing direction. When a window area is set by
// registers R36h/R37h and R38h/R39h, only the addressed GRAM area is updated based on I/D[1:0] and
// AM bits setting.
// I/D[1:0] Control the address counter (AC) to automatically increase or decrease by 1 when update one pixel
// display data.
//
// AM   I/D[1:0]     Register R20/R21 Start Address
//        00          R20 00AFh R21 00DBh       
//        01          R20 0000h R21 00DBh
// 0/1    10          R20 00Afh R21 0000h
//        11          R20 0000h R21 0000h
//
// MDT1: This bit is active on the 80-system of 8-bit bus and the data for 1-pixel is transported to 
// the memory for 3 write cycles. 
// This bit is on the 80-system of 16-bit bus, and the data for 1-pixel is transported 
// to the memory for 2 write cycles. When the 80-system interface mode is not set in the 8-bit 
// or 16-bit mode, set MDT1 bit to be “0”.
// MDT0: When 8-bit or 16-bit 80 interface mode and MDT1 bit=1, MDT0 defines color depth for the IC. 0 = 262K, 1 = 65K
// 
// BGR Swap the R and B order of written data.
//
// set\get ili9225x screen  rotation
#pragma Otime
static void ili9225xSetScreenRotation(ESGUI_HDRV hdrv, ESGUI_SCREEN_ROTATION rotation)
{
  // Preserve main part of output control
  // SS = 0, SM,GS = 0
  // We also set the line count in accordance to the display physical height
  esU16 outCtl = ((hdrv->height >= 220) ? 0x001C : hdrv->height / 8);
  // Preserve main part of entry mode
  // 65k, 8bit, pixel transmission is B-G-R
  esU16 entryMode = 0x1300;
  // NB! SS bit value should be paired with RGB|BGR bit value to maintain current color order
  esguiScreenIoBatchBegin(hdrv);
  hdrv->rotation = rotation;
  switch( rotation )
  {
  case ESGUI_SCREEN_ROT_0:
    // GS = 0, SS = 1
    outCtl |= 0x0100;
    // update AM = 0, ID = 11
    // horizontal increment, vertical increment
    entryMode |= 0x0030;
    break;
  case ESGUI_SCREEN_ROT_180:
    // GS = 0, SS = 1
    outCtl |= 0x0100;
    // update AM = 0, ID = 00
    // horizontal decrement, vertical decrement
    break;
  case ESGUI_SCREEN_ROT_90CCW:
    // GS = 0, SS = 1
    outCtl |= 0x0100;
    // update AM = 1, ID = 01
    // horizontal increment, vertical decrement
    entryMode |= 0x0018;
    break;
  case ESGUI_SCREEN_ROT_90CW:
    // GS = 0, SS = 1
    outCtl |= 0x0100;
    // update AM = 1, ID = 10
    // horizontal decrement, vertical increment
    entryMode |= 0x0028;
    break;
  }
  // Driver output control setup
  ili9225xCmdWrite(0x01, outCtl);
  ili9225xCmdWrite(0x03, entryMode);
  // optional parallel data sync
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x22);
  esguiScreenIoBatchEnd(hdrv);
}

// prepare coordinates depending on screen rotation
#pragma Otime
static __inline void ili9225xPreparePoint(ESGUI_HDRV hdrv, int* x, int* y)
{
  switch( hdrv->rotation )
  {
  case ESGUI_SCREEN_ROT_90CCW:  
    esguiUtilsSwapInt(x, y);
    *y = hdrv->height - *y -1;    
    break;
  case ESGUI_SCREEN_ROT_90CW:
    esguiUtilsSwapInt(x, y);
    *x = hdrv->width - *x -1;
    break;
  case ESGUI_SCREEN_ROT_180:
    *x = hdrv->width - *x -1;
    *y = hdrv->height - *y -1;
    break;
  }  
}

// internal position setter
#pragma Otime
static __inline void internal_ili9225xSetPos(ESGUI_HDRV hdrv, int x, int y)
{
  // 0-7 address
  ili9225xCmdWrite(0x20, x);
  // 8-16 address
  ili9225xCmdWrite(0x21, y);
  // optional parallel data sync
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x22);
}

// set ili9225x screen position. consecutive ili9225x memory writes
// will start at this position
#pragma Otime
static void ili9225xSetPos(ESGUI_HDRV hdrv, int x, int y)
{
  ili9225xPreparePoint(hdrv, &x, &y);
  internal_ili9225xSetPos(hdrv, x, y);
}

// internal window setter
#pragma Otime
static __inline void internal_ili9225xSetWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1)
{
  // set RAM window
  //
  // horizontal start
  ili9225xCmdWrite(0x36, x1);
  // horizontal end
  ili9225xCmdWrite(0x37, x0);
  // vertical start
  ili9225xCmdWrite(0x38, y1);
  // vertical end
  ili9225xCmdWrite(0x39, y0);
  // optional parallel data sync
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x22);
  // set starting position, depending on rotation  
  switch( hdrv->rotation )
  {
  case ESGUI_SCREEN_ROT_0:
    internal_ili9225xSetPos(hdrv, x0, y0);
    break;
  case ESGUI_SCREEN_ROT_180:
    internal_ili9225xSetPos(hdrv, x1, y1);
    break;
  case ESGUI_SCREEN_ROT_90CCW:
    internal_ili9225xSetPos(hdrv, x0, y1);
    break;
  case ESGUI_SCREEN_ROT_90CW:
    internal_ili9225xSetPos(hdrv, x1, y0);
    break;
  }
}

// set virtual window in which all ili9225x memory writes will go
#pragma Otime
static void ili9225xSetWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1)
{
  ili9225xPreparePoint(hdrv, &x0, &y0);
  ili9225xPreparePoint(hdrv, &x1, &y1);
  // normalize x & y
  if( x0 > x1 )
    esguiUtilsSwapInt(&x0, &x1);
  if( y0 > y1 )
    esguiUtilsSwapInt(&y0, &y1);

  internal_ili9225xSetWindow( hdrv, x0, y0, x1, y1);
}

// reset virtual window
#pragma Otime
static void ili9225xResetWindow(ESGUI_HDRV hdrv)
{
  internal_ili9225xSetWindow( hdrv, 0, 0, hdrv->width-1, hdrv->height-1);
}

// set pixel to color at current position
#pragma Otime
static __inline void ili9225xSetPixel(ESGUI_HDRV hdrv, ESGUI_Color_t clr)
{
  // Adjust color to 5-3 6-2 5-3
  clr = ((clr & 0xF800) << 8) + ((clr & 0x07E0) << 5) + ((clr & 0x001F) << 3);
  
  // by our design display's always ready to receive data
  esguiScreenBusWriteDataW(hdrv->hbus, (esU16)(clr >> 8 & 0xFFFF) );
  esguiScreenBusWriteDataB(hdrv->hbus, (esU8)(clr & 0xFF));
}

// internal window filler
#pragma Otime
static void internal_ili9225xFillWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1, ESGUI_Color_t clr)
{
  int iTmp = x0;
  // perform fill operation
  while(y0 <= y1)
  {
    while(x0 <= x1)
    {
      ili9225xSetPixel(hdrv, clr);
      ++x0;
    }

    x0 = iTmp;
    ++y0;
  }
}

// fill current ili9225x window area with specified color
#pragma Otime
static void ili9225xFillWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1, ESGUI_Color_t clr)
{
  esguiScreenSetWindow( hdrv, x0, y0, x1, y1 );
  internal_ili9225xFillWindow( hdrv, x0, y0, x1, y1, clr );
}

// window clear 
#pragma Otime
static void ili9225xClear(ESGUI_HDRV hdrv, ESGUI_Color_t clr)
{
  ili9225xResetWindow( hdrv );
  internal_ili9225xFillWindow( hdrv, 0, 0, hdrv->width-1, hdrv->height-1, clr );  
}

// Control screen power mode
#pragma Otime
static void ili9225xPowerModeSet(ESGUI_HDRV hdrv, ESGUI_SCREEN_PWRMODE mode)
{
  // begin display io batch
  esguiScreenIoBatchBegin(hdrv);

  switch(mode)
  {
  case ESGUI_SCREEN_PWRON:
    // Start oscillator
    ili9225xCmdWrite(0x0F, 0x0701);  
    msDelay(50); //< wait ~50ms
    ili9225xCmdWrite(0x11, 0x0018);                 //Set APON,PON,AON,VCI1EN,VC
    ili9225xCmdWrite(0x12, 0x3121);
    ili9225xCmdWrite(0x13, 0x006B);
    ili9225xCmdWrite(0x14, 0x3C4B);     
    // Get out of standby
    ili9225xCmdWrite(0x10, 0x0200);
    msDelay(100); //< wait ~100ms
    // Start Auto booster circuit
    ili9225xCmdWrite(0x11, 0x1041);   
    msDelay(50); //< wait ~50ms
    // Powert ctl3 setup (Step-up circuit amplification)
    ili9225xCmdWrite(0x12, 0x2012);
    msDelay(50); //< wait ~50ms
    // Power ctl4 setup (Amplifying factor for gamma voltage)
    ili9225xCmdWrite(0x13, 0x0033);  
    // Power ctl5 setup (VCOM supply settings)
    ili9225xCmdWrite(0x14, 0x5169);
    // Setup gamma curve voltage
    ili9225xCmdWrite(0x50, 0x0000); 
    ili9225xCmdWrite(0x51, 0x0C05); 
    ili9225xCmdWrite(0x52, 0x0302); 
    ili9225xCmdWrite(0x53, 0x0007);
    ili9225xCmdWrite(0x54, 0x0302);
    ili9225xCmdWrite(0x55, 0x050C);
    ili9225xCmdWrite(0x56, 0x0000);
    ili9225xCmdWrite(0x57, 0x0700);
    ili9225xCmdWrite(0x58, 0x1C00);          
    ili9225xCmdWrite(0x59, 0x001C);
    
    // LCD Driving waveform control
    ili9225xCmdWrite(0x02, 0x0300); // No inversion, active with positive polarity 
    
    // Setup display control for off-screen operations
    ili9225xCmdWrite(0x07, 0x0016);
    
    // Back and front porch (front porch = 8 back porch = 8 lines)
    ili9225xCmdWrite(0x08, 0x0808);
    // Set-up display frame cycle
    ili9225xCmdWrite(0x0B, 0x3100);
    // Interface control - disable RGB interface settings
    ili9225xCmdWrite(0x0C, 0x0001); 
    
    // Set-up gate scan control
    ili9225xCmdWrite(0x30, 0x0000);

    // Disable vertical scroll
    ili9225xCmdWrite(0x31, hdrv->height-1);
    ili9225xCmdWrite(0x32, 0x0000);
    ili9225xCmdWrite(0x33, 0x0000);

    // Screen driving set-up
    ili9225xCmdWrite(0x34, hdrv->height-1);
    ili9225xCmdWrite(0x35, 0x0000);  

     // Initialize screen rotation
    if(ESGUI_SCREEN_ROT_INVALID == hdrv->rotation)
      ili9225xSetScreenRotation(hdrv, ESGUI_SCREEN_ROT_0);
    else 
      ili9225xSetScreenRotation(hdrv, hdrv->rotation);
      
    // set initial memory window and position, clear display RAM
    ili9225xClear(hdrv, 0);

    // Switch offscreen operations to onscreen mode
    ili9225xCmdWrite(0x07, 0x0017);
    
    // Prepare for data command
    esguiScreenBusWriteCmdB(hdrv->hbus, 0x22);    
    break;
  case ESGUI_SCREEN_PWRSTDBY:
  case ESGUI_SCREEN_PWROFF:
    // Turn off display
    ili9225xCmdWrite(0x07, 0x0000); 
    msDelay(50); //< wait ~50ms
    
    // Stop Auto booster circuit
    ili9225xCmdWrite(0x11, 0x0041); 
    msDelay(50); //< wait ~50ms
    
    // Enter standby
    ili9225xCmdWrite(0x10, 0x0201);
    break;
  }
  
  // end display io batch
  esguiScreenIoBatchEnd(hdrv);
}

// driver initializer
#pragma Otime
static esBL ili9225xInit(ESGUI_HDRV hdrv, ESGUI_COLOR_FORMAT fmt)
{
  if(!hdrv)
    return FALSE;
  
  // reset display hardware
  esguiScreenHwReset(hdrv);
  hdrv->chipId = ili9225xIdRead(hdrv);
  // read chip id
  if( !ili9225isValidId(hdrv) )
  {
    hdrv->chipId = 0;
    return FALSE;
  }
    
   // execute initialization commands
  //
  // Turn off LCD first
  ili9225xPowerModeSet(hdrv, ESGUI_SCREEN_PWROFF);
  // Power display on, performing necessary initial configurations
  ili9225xPowerModeSet(hdrv, ESGUI_SCREEN_PWRON);

  return TRUE;
}

// misc controller - specific services
#pragma Otime
esU16 ili9225xIdRead(ESGUI_HDRV hdrv)
{
  esU16 result = 0;
  esguiScreenIoBatchBegin(hdrv);
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x00);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x0001);
  esguiScreenBusReadDataW(hdrv->hbus, &result);
  // optional parallel data sync
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x22);
  esguiScreenIoBatchEnd(hdrv);
  return result;
}

// driver setup
#pragma Otime
void ili9225xScreenDriverSetup(ESGUI_HDRV hdrv, int extx, int exty)
{
  if( hdrv )
  {
    esguiScreenDriverHandleInit(hdrv, sizeof(ESGUI_DRV));
    // set native size of driven screen
    hdrv->width = extx;
    hdrv->height = exty;
    hdrv->init = ili9225xInit;
    hdrv->setPowerMode = ili9225xPowerModeSet;
    hdrv->setRotation = ili9225xSetScreenRotation;
    hdrv->setPos = ili9225xSetPos;
    hdrv->setWindow = ili9225xSetWindow;
    hdrv->resetWindow = ili9225xResetWindow;
    hdrv->setPixel = ili9225xSetPixel;
    hdrv->fillWindow = ili9225xFillWindow;
    hdrv->clear = ili9225xClear;
  }
}

// Chip ID validity check
// Some chip modifications may return slightly different ID codes
#pragma Otime
esBL ili9225isValidId(ESGUI_HDRV hdrv)
{
  return 0x9224 <= hdrv->chipId && 0x9227 >= hdrv->chipId;
}

#ifdef ESGUI_USE_ILI9225X_DRIVER_TEST
// Window fill for driver testing
void ili9225xWindowFillTest(ESGUI_HDRV hdrv)
{
  int x0 = 7, x1 = 15, y0 = 19, y1 = 33;
  esguiScreenIoBatchBegin(hdrv);
  
  ili9225xPreparePoint(hdrv, &x0, &y0);
  ili9225xPreparePoint(hdrv, &x1, &y1);

  if( x0 > x1 )
    esguiUtilsSwapInt(&x0, &x1);
  if( y0 > y1 )
    esguiUtilsSwapInt(&y0, &y1);

  internal_ili9225xSetWindow( hdrv, x0, y0, x1, y1 );
  internal_ili9225xFillWindow( hdrv, x0, y0, x1, y1, 0xFFFFFFFF );
  
  esguiScreenIoBatchEnd(hdrv);
}
#endif

#endif // _ili_9225_x_cc_
