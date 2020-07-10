// hx5051.cc file - HIMAX HX5051 AMOLED driver interface implementation
//
#include <esfwxe/utils.h>

// entry mode control register 0x0005:
// 15      : TRI (1) - 8bits x 3 packets GRAM transfer mode  (18 bits over 8 or 16 bit interface)
// 14-13   : DFM1-0 specifies data format. when TRI == 1, 10 - 262k colors, 11 - 65k colors
//                                        when TRI == 0, 00 - 65k colors, 16 bit x 1 packet
// 12      : RGB order. if set to (1),  pixel data are reversed from RGB to BGR before writing to GRAM
// 9      : HWM, if set to 1 enables high speed write mode i.e. successful data transfer should come in 4 words at a time
// 8      : LHWM, (low power high speed write mode)
// 5      : ID1 - vertical (y) AC increment (1) or decrement (0)
// 4      : ID0 - horizontal (x) AC  increment (1)  or decrement (0)
// 3      : AM - GRAM data update direction - horizontal (0) or vertical (1)
// 2-0    : LG2-0  - logical compare operation
//                  000: replace
//                  110: replace data if match
//                  111: replace data if not match

// display control register 2 0x0007:  (!!! Note: himax manual contains error - SS bit declared 8, not 10)
// 14      : VSPL - polarity of vsync pin (0)-low active, (1)-high active
// 13      : HSPL - polarity of hsync pin (same meaning as vsync)
// 12      : DPL   - polarity od DOTCLK pin (0) - on rising edge, (1) - on falling edge
// 11      : EPL   - polarity of DE pin in RGB interface mode
// 10      : SS   - segment driver output shift direction (0) - 1-176; (1) - 176-1
// 9      : GS   - shift direction of the common driver (0) - 1-220; (1) - 220-1
// 2-1    : SM1-0 - scan mode 1(0) to 3(10), value of (11) is reserved
// 0      : BANK - (0) - 3 bank/line; (1) - 6 bank/line

// set\get hx5051 screen  rotation
#pragma Otime
static void hx5051SetScreenRotation(ESGUI_HDRV hdrv, ESGUI_SCREEN_ROTATION rotation)
{
  esU16 entryMode = 0, dispCtrl = 0;
  esguiScreenIoBatchBegin(hdrv);
  hdrv->rotation = rotation;
  switch( rotation )
  {
  case ESGUI_SCREEN_ROT_0:
    // scan mode 2, EPL 1, GS 0, SS 0 
    dispCtrl = 0x0804;
    // set h and v increment, horizontal orientation
    entryMode = 0x0030;
    break;
  case ESGUI_SCREEN_ROT_180:
    // scan mode 2, EPL 1, GS 1, SS 1 
    dispCtrl = 0x0E04;
    // set h and v increment, horizontal orientation
    entryMode = 0x0030;
    break;
  case ESGUI_SCREEN_ROT_90CW:
    // 90 cw: scan mode 2, EPL 1, GS 0, SS 1 
    dispCtrl = 0x0A04;
    // set h and v increment, vertical orientation
    entryMode = 0x0038;
    break; 
  case ESGUI_SCREEN_ROT_90CCW:
    // 90 ccw: scan mode 2, EPL 1, GS 1, SS 0 
    dispCtrl = 0x0C04;
    // set h and v increment, vertical orientation
    entryMode = 0x0038;
    break;
  }
   esguiScreenBusWriteCmdW(hdrv->hbus, 0x0007);
  esguiScreenBusWriteDataW(hdrv->hbus, dispCtrl);
  esguiScreenBusWriteCmdW(hdrv->hbus, 0x0005);
  esguiScreenBusWriteDataW(hdrv->hbus, entryMode);
  // ready for data & optional sync
  esguiScreenBusWriteCmdW(hdrv->hbus, 0x0022);
  esguiScreenIoBatchEnd(hdrv);
}

// prepare coordinates depending on screen rotation
#pragma Otime
static __inline void prepareCoords(ESGUI_HDRV hdrv, int* x, int* y)
{
  switch( hdrv->rotation )
  {
  case ESGUI_SCREEN_ROT_90CW:  
    esguiUtilsSwapInt(x, y);
    break;
  case ESGUI_SCREEN_ROT_90CCW:
    esguiUtilsSwapInt(x, y);
    break; 
  }  
}

// internal display position setter
#pragma Otime
static __inline void internal_hx5051SetPos(ESGUI_HDRV hdrv, int x, int y)
{
  esguiScreenBusWriteCmdW(hdrv->hbus, 0x0021);
  esguiScreenBusWriteDataW(hdrv->hbus, (y << 8) + x);
  // ready for data input, optional sync
  esguiScreenBusWriteCmdW(hdrv->hbus, 0x0022);  
  ((hx5051_DRV*)hdrv)->pos.x = x;
  ((hx5051_DRV*)hdrv)->pos.y = y;
}

// set hx5051 screen position. consecutive hx5051 memory writes
// will start at this position
#pragma Otime
static void hx5051SetPos(ESGUI_HDRV hdrv, int x, int y)
{
  prepareCoords(hdrv, &x, &y);
  internal_hx5051SetPos(hdrv, x, y);
}

// internal set window implementation
#pragma Otime
static __inline void internal_hx5051SetWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1)
{
  esguiRectSet_(
    x0, 
    y0, 
    x1, 
    y1, 
    &((hx5051_DRV*)hdrv)->wnd
  );
  // set position
  internal_hx5051SetPos(hdrv, x0, y0);
}

// set virtual window in which all hx5051 memory writes will go
#pragma Otime
static void hx5051SetWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1)
{
  prepareCoords(hdrv, &x0, &y0);
  prepareCoords(hdrv, &x1, &y1);
  // normalize x & y
  if( x0 > x1 )
    esguiUtilsSwapInt(&x0, &x1);
  if( y0 > y1 )
    esguiUtilsSwapInt(&y0, &y1);

  // set window
  internal_hx5051SetWindow(hdrv, x0, y0, x1, y1);
}

// reset window
#pragma Otime
static void hx5051ResetWindow(ESGUI_HDRV hdrv)
{
  // set window
  internal_hx5051SetWindow(
    hdrv, 
    0, 
    0, 
    hdrv->width-1, 
    hdrv->height-1
  );
}

// set pixel to color at current position
#pragma Otime
static void hx5051SetPixel(ESGUI_HDRV hdrv, ESGUI_Color_t clr)
{
  hx5051_DRV* ourDrv = (hx5051_DRV*)hdrv;
  // display should be ready for data here
  esguiScreenBusWriteDataW(hdrv->hbus, (esU16)clr);
  // keep track of pixel position, correcting it to the window if needed
  switch( hdrv->rotation )
  {
  case ESGUI_SCREEN_ROT_0:
  case ESGUI_SCREEN_ROT_180:
    if(++ourDrv->pos.x > ourDrv->wnd.edge.x)
    {
      ourDrv->pos.x = ourDrv->wnd.org.x;
      if(++ourDrv->pos.y > ourDrv->wnd.edge.y)
        ourDrv->pos.y = ourDrv->wnd.org.y;
      internal_hx5051SetPos(hdrv, ourDrv->pos.x, ourDrv->pos.y);
    }
    break;
  case ESGUI_SCREEN_ROT_90CW:
  case ESGUI_SCREEN_ROT_90CCW:
    if(++ourDrv->pos.y > ourDrv->wnd.edge.y)
    {
      ourDrv->pos.y = ourDrv->wnd.org.y;
      if(++ourDrv->pos.x > ourDrv->wnd.edge.x)
        ourDrv->pos.x = ourDrv->wnd.org.x;
      internal_hx5051SetPos(hdrv, ourDrv->pos.x, ourDrv->pos.y);        
    }
    break;
  }
}

// internal window filler
#pragma Otime
static void internal_hx5051FillWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1, ESGUI_Color_t clr)
{
  int iTmp = x0;

  while(y0 <= y1)
  {
    while(x0 <= x1)
    {
      hx5051SetPixel(hdrv, clr);
      ++x0;
    }

    x0 = iTmp;
    ++y0;
  }    
}

// fill current hx5051 window area with specified color
#pragma Otime
static void hx5051FillWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1, ESGUI_Color_t clr)
{
  hx5051SetWindow( hdrv, x0, y0, x1, y1 );
  internal_hx5051FillWindow( hdrv, x0, y0, x1, y1, clr );  
}

// clear screen
#pragma Otime
static void  hx5051Clear(ESGUI_HDRV hdrv, ESGUI_Color_t clr)
{
  hx5051ResetWindow(hdrv);
  internal_hx5051FillWindow( hdrv, 0, 0, hdrv->width-1, hdrv->height-1, clr );
}

// screen initializer
#pragma Otime
static esBL hx5051Init(ESGUI_HDRV hdrv, ESGUI_COLOR_FORMAT fmt)
{
  if(!hdrv)
    return FALSE;
  
  // reset display
  esguiScreenHwReset(hdrv);
  // start display io batch
  esguiScreenIoBatchBegin(hdrv);
  // initialization batch
  //
  // PON = 1; VRH2 = 1; VRH0 = 1
  esguiScreenBusWriteCmdW(hdrv->hbus, 0x0003);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x0015);
  // power control (1)
  // SAP2 = 1; BT2 = 1; BT1 = 1; BT0 = 1; AP2 = 1
  esguiScreenBusWriteCmdW(hdrv->hbus, 0x0001);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x4740);
  // power control (2) 
  // DC01 = 1; DC00 = 1; VC2 = 1; VC0 = 1
  esguiScreenBusWriteCmdW(hdrv->hbus, 0x0002);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x0305);
  // display control (4)
  // D1 = 1; D0 = 1
  esguiScreenBusWriteCmdW(hdrv->hbus, 0x0009);
  esguiScreenBusWriteDataW(hdrv->hbus, 0x0003);
  // end display io batch
  esguiScreenIoBatchEnd(hdrv);

  // set initial display rotation
  hx5051SetScreenRotation(hdrv, ESGUI_SCREEN_ROT_0);
  // set initial memory window
  hx5051ResetWindow(hdrv);
  
  return TRUE;
}

#pragma Otime
void hx5051ScreenDriverSetup(ESGUI_HDRV hdrv, int extx, int exty)
{
  if( hdrv )
  {
    esguiScreenDriverHandleInit(
      hdrv, 
      sizeof(hx5051_DRV)
    );
    
    // set native size of driven screen
    hdrv->width = extx;
    hdrv->height = exty;
    hdrv->init = hx5051Init;
    hdrv->setRotation = hx5051SetScreenRotation;
    hdrv->setPos = hx5051SetPos;
    hdrv->setWindow = hx5051SetWindow;
    hdrv->resetWindow = hx5051ResetWindow;
    hdrv->setPixel = hx5051SetPixel;
    hdrv->fillWindow = hx5051FillWindow;
    hdrv->clear = hx5051Clear;
  }
}
