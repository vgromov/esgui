// ssd1306 oled driver interface implementation
#ifndef _ssd_1306_cc_
#define _ssd_1306_cc_

// ssd1306.cc file - Solomon Systech 128 x 64 Dot Matrix OLED/PLED Segment/Common Driver with Controller
//
#include <esfwxe/utils.h>

// switch display on|off

void ssd1306DisplayOn(ESGUI_HDRV hdrv, esBL on)
{
  esguiScreenBusWriteCmdB(hdrv->hbus, on ? 0xAF : 0xAE);
}

// charge pump enable|disable

void ssd1306ChargePumpEnable(ESGUI_HDRV hdrv, esBL enable)
{
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x8D);
  esguiScreenBusWriteCmdB(hdrv->hbus, enable ? 0x14 : 0x10);
}

// set pre-charge period for phase1 1..15 and phase 2 1..15

void ssd1306PrechargePeriodSet(ESGUI_HDRV hdrv, esU8 pp1, esU8 pp2)
{
  pp1 &= 0x0F; pp2 &= 0x0F;
  if(pp1 < 1) pp1 = 1; if(pp2 < 1) pp2 = 1;

  esguiScreenBusWriteCmdB(hdrv->hbus, 0xD9);
  esguiScreenBusWriteCmdB(hdrv->hbus, pp1+(pp2 << 4));
}

// set display clock ratio: 1..16 and osc frequency: 0..15

void ssd1306ClkRatioOscFreqSet(ESGUI_HDRV hdrv, esU8 clkRatio, esU8 oscFreq)
{
  if( 0 < clkRatio )
    --clkRatio;
  esguiScreenBusWriteCmdB(hdrv->hbus, 0xD5);
  esguiScreenBusWriteCmdB(hdrv->hbus, (clkRatio & 0x0F)+((oscFreq & 0x0F) << 4));
}

// set multiplex ratio: 16..64

void ssd1306MuxRatioSet(ESGUI_HDRV hdrv, esU8 ratio)
{
  if(ratio < 16)
    ratio = 16;
  if(ratio > 64)
    ratio = 64;
  --ratio;
  esguiScreenBusWriteCmdB(hdrv->hbus, 0xA8);
  esguiScreenBusWriteCmdB(hdrv->hbus, ratio);  
}

// set vcomh deselect level

void ssd1306VcomhDeselectLevelSet(ESGUI_HDRV hdrv, esU8 level)
{
  esguiScreenBusWriteCmdB(hdrv->hbus, 0xDB);
  esguiScreenBusWriteCmdB(hdrv->hbus, (level & 7) << 4);
}

// set display start line

void ssd1306StartLineSet(ESGUI_HDRV hdrv, esU8 line)
{
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x40+(line & 0x3F));
}

// set display offset

void ssd1306DisplayOffsetSet(ESGUI_HDRV hdrv, esU8 offs)
{
  esguiScreenBusWriteCmdB(hdrv->hbus, 0xD3);
  esguiScreenBusWriteCmdB(hdrv->hbus, offs & 0x3F);
}

// set-reset display inversed mode

void ssd1306InversedModeSet(ESGUI_HDRV hdrv, esBL inversed)
{
  esguiScreenBusWriteCmdB(hdrv->hbus, inversed ? 0xA7 : 0xA6);
}

// resume display output from RAM

void ssd1306DisplayRamResume(ESGUI_HDRV hdrv, esBL resume)
{
  esguiScreenBusWriteCmdB(hdrv->hbus, resume ? 0xA4 : 0xA5);
}

// set display contrast

void ssd1306ContrastSet(ESGUI_HDRV hdrv, esU8 contrast)
{
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x81);
  esguiScreenBusWriteCmdB(hdrv->hbus, contrast);
}

// set col segment remap

void ssd1306ColSegRemap(ESGUI_HDRV hdrv, esBL remap)
{
  esguiScreenBusWriteCmdB(hdrv->hbus, 0xA0 + (remap ? 1 : 0));
}

// set com pins hardware configuration

void ssd1306ComPinsConfigSet(ESGUI_HDRV hdrv, esU8 cfg)
{
  esguiScreenBusWriteCmdB(hdrv->hbus, 0xDA);
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x02+(cfg & 0x30));
}

// set com output scan direction

void ssd1306ComOutNormalScanSet(ESGUI_HDRV hdrv, esBL normal)
{
  esguiScreenBusWriteCmdB(hdrv->hbus, normal ? 0xC0 : 0xC8);  
}

// set addressing mode

void ssd1306AddrModeSet(ESGUI_HDRV hdrv, ssd1306AddrMode am)
{
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x20);
  esguiScreenBusWriteCmdB(hdrv->hbus, am & 0x03);
}

// set start column

void ssd1306StartColSet(ESGUI_HDRV hdrv, esU8 colStart)
{
  esguiScreenBusWriteCmdB(hdrv->hbus, colStart & 0x0F);
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x10+((colStart & 0x70) >> 4));
}
  
// set start page

void ssd1306StartPageSet(ESGUI_HDRV hdrv, esU8 pgStart)
{
  esguiScreenBusWriteCmdB(hdrv->hbus, 0xB0+(pgStart & 0x07));
}

// set column range

void ssd1306ColRangeSet(ESGUI_HDRV hdrv, esU8 colStart, esU8 colEnd)
{
  colStart &= 0x7F; colEnd &= 0x7F;
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x21);
  esguiScreenBusWriteCmdB(hdrv->hbus, MIN(colStart, colEnd));  
  esguiScreenBusWriteCmdB(hdrv->hbus, MAX(colStart, colEnd));  
}

// set page range

void ssd1306PageRangeSet(ESGUI_HDRV hdrv, esU8 pgStart, esU8 pgEnd)
{
  pgStart &= 0x07; pgEnd &= 0x07;
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x22);
  esguiScreenBusWriteCmdB(hdrv->hbus, MIN(pgStart, pgEnd));  
  esguiScreenBusWriteCmdB(hdrv->hbus, MAX(pgStart, pgEnd));  
}
  
// direct bit blitting

void ssd1306BitBlt(ESGUI_HDRV hdrv, const esU8* data, esU32 len, ssd1306BitOrder bitOrder)
{
  while( len-- )
  {
    esU8 b = *data++;
    if( ssd1306BitOrderInverse == bitOrder )
      b = utilsReverseBits(b);
    esguiScreenBusWriteDataB(hdrv->hbus, b);
  }
}

// display RAM filling

void ssd1306Fill(ESGUI_HDRV hdrv, esU8 val, esU32 len, ssd1306BitOrder bitOrder)
{
  esU8 b = val;
  if( ssd1306BitOrderInverse == bitOrder )
    b = utilsReverseBits(b);

  while( len-- )
    esguiScreenBusWriteDataB(hdrv->hbus, b);
}

// set\get ssd1306 screen  rotation

static void ssd1306SetScreenRotation(ESGUI_HDRV hdrv, ESGUI_SCREEN_ROTATION rotation)
{
  esguiScreenIoBatchBegin(hdrv);

  hdrv->rotation = rotation;
  switch( rotation )
  {
  case ESGUI_SCREEN_ROT_0:
    esguiScreenBusWriteCmdB(hdrv->hbus, 0xA0 | ssd1306SegmapNormal); // normal segmap
    esguiScreenBusWriteCmdB(hdrv->hbus, 0xC0 | ssd1306ComscanInc);   // normal comscan (inc)
    break;
  case ESGUI_SCREEN_ROT_180:
    esguiScreenBusWriteCmdB(hdrv->hbus, 0xA0 | ssd1306SegmapRemap);  // remapped segmap
    esguiScreenBusWriteCmdB(hdrv->hbus, 0xC0 | ssd1306ComscanDec);   // remapped comscan (dec)
    break;
  case ESGUI_SCREEN_ROT_90CW:
    break;
  case ESGUI_SCREEN_ROT_90CCW:
    break;
  default:
    break;
  }
  esguiScreenIoBatchEnd(hdrv);
}

// prepare coordinates depending on screen rotation

static inline void ssd1306PreparePoint(ESGUI_HDRV hdrv, int* x, int* y)
{
  switch( hdrv->rotation )
  {
  case ESGUI_SCREEN_ROT_90CW:  
    eseUtilsSwapInt(x, y);
    break;
  case ESGUI_SCREEN_ROT_90CCW:
    eseUtilsSwapInt(x, y);
    *y = hdrv->height - *y -1;
    break;
  case ESGUI_SCREEN_ROT_180:
    *y = hdrv->height - *y -1;
    break;
  default:
    break;
  }  
}

// internal position setter, x is in columns, y is in pages

static __inline void internal_ssd1306SetPos(ESGUI_HDRV hdrv, int x, int y)
{
  // lower col address
  esguiScreenBusWriteCmdB(hdrv->hbus, (x & 0x0F));
  // higher col address
  esguiScreenBusWriteCmdB(hdrv->hbus, 0x10 + ((x >> 4) & 0x0F));
  // page start
  esguiScreenBusWriteCmdB(hdrv->hbus, 0xB0 + (y & 0x07));
}

// set ssd1306 screen position. consecutive ssd1306 memory writes
// will start at this position

static void ssd1306SetPos(ESGUI_HDRV hdrv, int x, int y)
{
  ssd1306PreparePoint(hdrv, &x, &y);
  internal_ssd1306SetPos(hdrv, x, y);
}

// internal window setter

static __inline void internal_ssd1306SetWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1)
{
  ssd1306ColRangeSet(hdrv, x0, x1);
  ssd1306PageRangeSet(hdrv, y0, y1);
}

// set virtual window in which all ssd1306 memory writes will go

void ssd1306SetWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1)
{
  ssd1306PreparePoint(hdrv, &x0, &y0);
  ssd1306PreparePoint(hdrv, &x1, &y1);
  // normalize x & y
  if( x0 > x1 )
    eseUtilsSwapInt(&x0, &x1);
  if( y0 > y1 )
    eseUtilsSwapInt(&y0, &y1);

  internal_ssd1306SetWindow( hdrv, x0, y0, x1, y1);
}

// reset virtual window

void ssd1306ResetWindow(ESGUI_HDRV hdrv)
{
  internal_ssd1306SetWindow( hdrv, 0, 0, hdrv->width-1, hdrv->height-1);
}

// driver initializer

static esBL ssd1306Init(ESGUI_HDRV hdrv, ESGUI_COLOR_FORMAT clrFmt)
{
  if(hdrv)
  {
    hdrv->colorFmt = clrFmt;
    
    // reset display hardware
    esguiScreenHwReset(hdrv);
    // begin display io batch
    esguiScreenIoBatchBegin(hdrv);
   
    // execute initialization commands
    //
    ssd1306DisplayOn(hdrv, FALSE);
    ssd1306StartColSet(hdrv, 0);
    ssd1306StartPageSet(hdrv, 0);
    ssd1306MuxRatioSet(hdrv, 64);
    ssd1306ClkRatioOscFreqSet(hdrv, 1, 8);
    ssd1306ChargePumpEnable(hdrv, TRUE);
    ssd1306DisplayOffsetSet(hdrv, 0);
    ssd1306StartLineSet(hdrv, 0);
    ssd1306ColSegRemap(hdrv, FALSE);
    ssd1306ComOutNormalScanSet(hdrv, TRUE);
    ssd1306ComPinsConfigSet(hdrv, ssd1306ComPinsCfgAlternative|ssd1306ComPinsCfgNoLR_Remap);
    ssd1306DisplayRamResume(hdrv, TRUE);
    ssd1306InversedModeSet(hdrv, FALSE);
    ssd1306AddrModeSet(hdrv, ssd1306AddrModeVertical);
    ssd1306ColRangeSet(hdrv, 0, 127);
    ssd1306PageRangeSet(hdrv, 0, 7);
    ssd1306Fill(hdrv, 0, 1024, ssd1306BitOrderNormal);
    ssd1306DisplayOn(hdrv, TRUE);
    // end display io batch
    esguiScreenIoBatchEnd(hdrv);

    return TRUE;
  }

  return FALSE;
}

// set pixel to color at current position

static __inline void ssd1306SetPixel(ESGUI_HDRV hdrv, ESGUI_Color_t clr)
{
  // by design display's always ready to receive data
  esguiScreenBusWriteDataB(hdrv->hbus, (clr & 0xFF));
}

// internal window filler

static void internal_ssd1306FillWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1, ESGUI_Color_t clr)
{
  int iTmp = x0;
  // perform fill operation
  while(y0 <= y1)
  {
    while(x0 <= x1)
    {
      ssd1306SetPixel(hdrv, clr);
      ++x0;
    }

    x0 = iTmp;
    ++y0;
  }
}

// fill current ssd1306 window area with specified color

static void ssd1306FillWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1, ESGUI_Color_t clr)
{
  esguiScreenSetWindow( hdrv, x0, y0, x1, y1 );
  internal_ssd1306FillWindow( hdrv, x0, y0, x1, y1, clr );
}

// window clear 

static void ssd1306Clear(ESGUI_HDRV hdrv, ESGUI_Color_t clr)
{
  ssd1306ResetWindow( hdrv );
  internal_ssd1306FillWindow( hdrv, 0, 0, hdrv->width-1, hdrv->height-1, clr );  
}

// driver setup

void ssd1306ScreenDriverSetup(ESGUI_HDRV hdrv, int extx, int exty)
{
  if( hdrv )
  {
    esguiScreenDriverHandleInit(hdrv, sizeof(ESGUI_DRV));
    // set native size of driven screen
    hdrv->width = extx;
    hdrv->height = exty;
    hdrv->init = ssd1306Init;
    hdrv->setRotation = ssd1306SetScreenRotation;
    hdrv->setPos = ssd1306SetPos;
    hdrv->setWindow = ssd1306SetWindow;
    hdrv->resetWindow = ssd1306ResetWindow;
    hdrv->setPixel = ssd1306SetPixel;
    hdrv->fillWindow = ssd1306FillWindow;
    hdrv->clear = ssd1306Clear;
  }
}

#endif // _ssd_1306_cc_
