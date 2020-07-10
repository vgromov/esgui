// s6e63d6x.cc file - SAMSUNG s6e63d6x AMOLED display controller driver implementation
//
#include <esfwxe/utils.h>

// color conversions to|from universal format
//
// native color type is r(5)-(3)-g(6)-(2)-b(5)-(3)

// entry mode control register 0x03:
// CLS  MDT1  MDT0  BGR X  X  X  SS X  X I/D1 I/D0 X  X  X  AM
// 15      : CLS This bit is used to define the color and interface bus format, When MDT0-1 = 00   
//          CLS = 0: 65K-color mode through 8-bit(Index address 24h) or 16-bit bus(Index address 23h) 
//          CLS = 1: 262K-color mode through 9-bit(Index address 24h) or 18-bit bus(Index address 23h) 
// 14     : MDT1 This bit is active on the 80-system of 8-bit bus and the data for 1-pixel is transported to the memory for 3 
//          write cycles. This bit is on the 80-system of 16-bit bus and the data for 1-pixel is transported to the memory 
//          for 2 write cycles. When the 80-system interface mode is not set in the 8-bit or16-bit mode, set MDT1 bit to 
//          be “0”. 
// 13      : MDT0 When 8-bit or16-bit 80 interface mode and MDT1 bit =1, MDT0 defines color depth for the IC. 
//          8-bit (80-system), MDT0 = 0: 262k-color mode (3 times of 6-bit data transfer to GRAM)   
//          8-bit (80-system), MDT0 = 1: 65k-color mode (5-bit, 6-bit, 5-bit data transfer to GRAM)   
//          16-bit (80-system), MDT0 = 0: 262k-color mode (16-bit, 2-bit data transfer to GRAM) 
//          16-bit (80-system), MDT0 = 1: 262k-color mode (2-bit, 16-bit data transfer to GRAM)
// 12      : BGR About writing 18-bit data to GRAM, it is changed <R><G><B> into <B><G><R>. 
//          BGR = 0; {DB[17:12], DB[11:6], DB[5:0]} is assigned to {R, G, B}. Actually the analog value that corresponds 
//          to DB[17:12] is output firstly at source output 
//          BGR = 1; {DB[17:12], DB[11:6], DB[5:0]} is assigned to {B, G, R}. Actually the analog value that corresponds 
//          to DB[5:0] is output firstly at source output. 
// 8      : SS Select the direction of the source driver channel in pixel unit. 
//          When user changes the value of SS, memory should be updated to apply the change. 
// 5      : I/D1 
// 4      : I/D0 When ID[1], ID[0]    = 1, the address counter (AC) is automatically increased by 1 after the data is written to 
//          the GRAM. When ID[1], ID[0] = 0, the AC is automatically decreased by 1 after the data is written to the 
//          GRAM. 
// 0      : AM Set the automatic update method of the AC after the data is written to GRAM. When AM = “0”, the data is 
//          continuously written in horizontally. When AM = “1”, the data is continuously written vertically. When window 
//          addresses are specified, the GRAM in the window range can be written to according to the ID[1:0] and AM. 

// set\get s6e63d6x screen  rotation
#pragma Otime
static void s6e63d6xSetScreenRotation(ESGUI_HDRV hdrv, ESGUI_SCREEN_ROTATION rotation)
{
  // preserve main part of entry mode
  // 65k, 8bit, pixel transmission is R-G-B
  esU16 entryMode = 0x6000;
  esguiScreenIoBatchBegin(hdrv);
  hdrv->rotation = rotation;
  switch( rotation )
  {
  case ESGUI_SCREEN_ROT_0:
    // horz inc & vert inc, mirrored horizontal driver, 
    // horizontal update
    entryMode |= 0x0130;
    break;
  case ESGUI_SCREEN_ROT_180:
    // horz inc & vert dec increment, normal pixel driver
    // horizontal update
    entryMode |= 0x0010;
    break;
  case ESGUI_SCREEN_ROT_90CW:
    // 90 cw horz & vert inc, normal pixel driver  
    // vertical update
    entryMode |= 0x0031;
    break;
  case ESGUI_SCREEN_ROT_90CCW:
    // 90 ccw: horz inc & vert dec, mirrored pixel driver,
    // vertical update
    entryMode |= 0x0111;
    break;
  }
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x03);
  esguiScreenBusWriteDataW(hdrv->hbus, entryMode);
  // optional parallel data sync
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x22);
  esguiScreenIoBatchEnd(hdrv);
}

// prepare coordinates depending on screen rotation
#pragma Otime
static __inline void s6e63d6xPreparePoint(ESGUI_HDRV hdrv, int* x, int* y)
{
  switch( hdrv->rotation )
  {
  case ESGUI_SCREEN_ROT_90CW:  
    esguiUtilsSwapInt(x, y);
    break;
  case ESGUI_SCREEN_ROT_90CCW:
    esguiUtilsSwapInt(x, y);
    *y = hdrv->height - *y -1;
    break;
  case ESGUI_SCREEN_ROT_180:
    *y = hdrv->height - *y -1;
    break;
  }  
}

// internal position setter
#pragma Otime
static __inline void internal_s6e63d6xSetPos(ESGUI_HDRV hdrv, int x, int y)
{
  // 0-7 address
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x20);
  esguiScreenBusWriteDataW(hdrv->hbus, x + s6e63d6x_OFFS);
  // 8-16 address
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x21);
  esguiScreenBusWriteDataW(hdrv->hbus, y );
  // optional parallel data sync
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x22);
}

// set s6e63d6x screen position. consecutive s6e63d6x memory writes
// will start at this position
#pragma Otime
static void s6e63d6xSetPos(ESGUI_HDRV hdrv, int x, int y)
{
  s6e63d6xPreparePoint(hdrv, &x, &y);
  internal_s6e63d6xSetPos(hdrv, x, y);
}

// internal window setter
#pragma Otime
static __inline void internal_s6e63d6xSetWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1)
{
  // set RAM window
  //
  // vertical start
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x35);
  esguiScreenBusWriteDataW(hdrv->hbus, y0);
  // vertical end
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x36);
  esguiScreenBusWriteDataW(hdrv->hbus, y1);
  // horizontal start-end
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x37);
  esguiScreenBusWriteDataW(hdrv->hbus, ((x0 + s6e63d6x_OFFS) << 8) | (x1 + s6e63d6x_OFFS) );
  // optional parallel data sync
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x22);
  // set starting position, depending on rotation  
  switch( hdrv->rotation )
  {
  case ESGUI_SCREEN_ROT_0:
  case ESGUI_SCREEN_ROT_90CW:
    internal_s6e63d6xSetPos(hdrv, x0, y0);
    break;
  case ESGUI_SCREEN_ROT_90CCW:
  case ESGUI_SCREEN_ROT_180:
    internal_s6e63d6xSetPos(hdrv, x0, y1);
    break;
  }
}

// set virtual window in which all s6e63d6x memory writes will go
#pragma Otime
static void s6e63d6xSetWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1)
{
  s6e63d6xPreparePoint(hdrv, &x0, &y0);
  s6e63d6xPreparePoint(hdrv, &x1, &y1);
  // normalize x & y
  if( x0 > x1 )
    esguiUtilsSwapInt(&x0, &x1);
  if( y0 > y1 )
    esguiUtilsSwapInt(&y0, &y1);

  internal_s6e63d6xSetWindow( hdrv, x0, y0, x1, y1);
}

// reset virtual window
#pragma Otime
static void s6e63d6xResetWindow(ESGUI_HDRV hdrv)
{
  internal_s6e63d6xSetWindow( hdrv, 0, 0, hdrv->width-1, hdrv->height-1);
}

// driver initializer
#pragma Otime
static esBL s6e63d6xInit(ESGUI_HDRV hdrv, ESGUI_COLOR_FORMAT fmt)
{
  if(!hdrv)
    return FALSE;

  // reset display hardware
  esguiScreenHwReset(hdrv);
  // begin display io batch
  esguiScreenIoBatchBegin(hdrv);
   // execute initialization commands
  //
  // interface selection: i80, 8-bit, parallel
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x24);
  // Check chip ID
  hdrv->chipId = s6e63d6xReadId(hdrv);
  if( !s6e63d6xIsValidId(hdrv) )
  {
    hdrv->chipId = 0;
    // end display io batch
    esguiScreenIoBatchEnd(hdrv);
    return FALSE;
  }
  
  // system interface, internal clock
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x02);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x0000);
   // panel interface control 1
  esguiScreenBusWriteCmdB(hdrv->hbus, 0xF8);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x000F); // VGL = +5V
  esguiScreenBusWriteCmdB(hdrv->hbus, 0xF9);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x0019); // VGH = -7V
  // adjust display gamma
   esguiScreenBusWriteCmdB(hdrv->hbus, 0x70);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x3480);
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x71);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x3900);
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x72);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x3C80);
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x73);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x291F);
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x74);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x2119);
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x75);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x3221);
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x76);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x211A);
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x77);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x2C22);
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x78);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x271D);
  // select maximum internal osc frequency
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x18);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x0028);
  // display duty control
  // FP3 = 1; BP3 = 1; (front porch = 8 back porch = 8 lines) NLn = (height, adjusted to greater multiple of 8) >> 3
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x01);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x8800 | 
    ((((hdrv->height - hdrv->height%8)+(hdrv->height%8 ? 8 : 0)) >> 3) & 0x003F) );
   // cancel stand-by mode
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x10);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x0000);
  // wait for some time, ~500ms
  msDelay(500);
   // switch display on
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x05);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x0001);
   // init screen rotation
  s6e63d6xSetScreenRotation(hdrv, ESGUI_SCREEN_ROT_0);
  // set initial memory window and position, leave display in data input ready mode 
  s6e63d6xResetWindow(hdrv);

  // end display io batch
  esguiScreenIoBatchEnd(hdrv);
  return TRUE;
}

// set pixel to color at current position
#pragma Otime
static __inline void s6e63d6xSetPixel(ESGUI_HDRV hdrv, ESGUI_Color_t clr)
{
  // Adjust color to 5-3 6-2 5-3
  clr = ((clr & 0xF800) << 8) + ((clr & 0x07E0) << 5) + ((clr & 0x001F) << 3);
  
  // by our design display's always ready to receive data
  esguiScreenBusWriteDataW(hdrv->hbus, (esU16)(clr >> 8 & 0xFFFF) );
  esguiScreenBusWriteDataB(hdrv->hbus, (esU8)(clr & 0xFF));
}

// internal window filler
#pragma Otime
static void internal_s6e63d6xFillWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1, ESGUI_Color_t clr)
{
  int iTmp = x0;
  // perform fill operation
  while(y0 <= y1)
  {
    while(x0 <= x1)
    {
      s6e63d6xSetPixel(hdrv, clr);
      ++x0;
    }

    x0 = iTmp;
    ++y0;
  }
}

// fill current s6e63d6x window area with specified color
#pragma Otime
static void s6e63d6xFillWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1, ESGUI_Color_t clr)
{
  esguiScreenSetWindow( hdrv, x0, y0, x1, y1 );
  internal_s6e63d6xFillWindow( hdrv, x0, y0, x1, y1, clr );
}

// window clear 
#pragma Otime
static void s6e63d6xClear(ESGUI_HDRV hdrv, ESGUI_Color_t clr)
{
  s6e63d6xResetWindow( hdrv );
  internal_s6e63d6xFillWindow( hdrv, 0, 0, hdrv->width-1, hdrv->height-1, clr );  
}

// driver setup
#pragma Otime
void s6e63d6xScreenDriverSetup(ESGUI_HDRV hdrv, int extx, int exty)
{
  if( hdrv )
  {
    esguiScreenDriverHandleInit(hdrv, sizeof(ESGUI_DRV));
    // set native size of driven screen
    hdrv->width = extx;
    hdrv->height = exty;
    hdrv->init = s6e63d6xInit;
    hdrv->setRotation = s6e63d6xSetScreenRotation;
    hdrv->setPos = s6e63d6xSetPos;
    hdrv->setWindow = s6e63d6xSetWindow;
    hdrv->resetWindow = s6e63d6xResetWindow;
    hdrv->setPixel = s6e63d6xSetPixel;
    hdrv->fillWindow = s6e63d6xFillWindow;
    hdrv->clear = s6e63d6xClear;
  }
}

// misc controller - specific services
#pragma Otime
esU16 s6e63d6xReadId(ESGUI_HDRV hdrv)
{
  esU16 result = 0;
  esguiScreenIoBatchBegin(hdrv);
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x0F);
  esguiScreenBusReadDataW(hdrv->hbus, &result);
  // optional parallel data sync
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x22);
  esguiScreenIoBatchEnd(hdrv);
  return result;
}

#pragma Otime
esBL s6e63d6xIsValidId(ESGUI_HDRV hdrv)
{
  return 0x63D6 == hdrv->chipId;
}
