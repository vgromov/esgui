#include <esgui/include/esgui.h>
#include <esfwxe/ese_assert.h>
#include <esfwxe/trace.h>
#include <esfwxe/utils.h>
#include <esfwxe/utils_str.h>

#include <esgui/include/esgui_screen_driver.h>

#include <string.h>
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------

// internally re-calculate internal :) clipping rect
static inline void internalCalcInternalClipRect(ESGUI_HDC hdc)
{
  esguiRectIntersect( 
    &hdc->ext, 
    &hdc->clip, 
    &hdc->internalClip 
  );
}
//----------------------------------------------------------------------------------------------

// internally set screen extent (depending on rotation, it may return different values)
static inline void internalUpdateScreenExt(ESGUI_HDC hdc)
{
  switch( hdc->driver->rotation )
  {
  case ESGUI_SCREEN_ROT_0:
  case ESGUI_SCREEN_ROT_180:
    hdc->ext.edge.x = hdc->driver->width-1;
    hdc->ext.edge.y = hdc->driver->height-1;
    break;
  case ESGUI_SCREEN_ROT_90CW:
  case ESGUI_SCREEN_ROT_90CCW:
    hdc->ext.edge.x = hdc->driver->height-1;
    hdc->ext.edge.y = hdc->driver->width-1;
    break;
  default:
    break;
  }
}
//----------------------------------------------------------------------------------------------

esBL esguiCanvasInit(ESGUI_HDC hdc, ESGUI_HDRV drv)
{
  if( !hdc || !drv )
    return FALSE;

  hdc->driver = drv;
  esguiPointZeroSet(&hdc->ext.org);
  internalUpdateScreenExt(hdc);
  hdc->clip = hdc->internalClip = hdc->ext;
  hdc->font = NULL;
  // init GUI control
  hdc->ctl = guiCtlUpdatePos | guiCtlOpaqueText;
  // init curPos
  esguiPointZeroSet(&hdc->pos);
  // init system palette
  esguiCanvasSysPaletteSet(hdc);

  return TRUE;
}
//----------------------------------------------------------------------------------------------

esBL esguiCanvasLock(ESGUI_HDC hdc)
{
  if( !hdc )
    return FALSE;

  return esguiScreenLock( hdc->driver );
}
//----------------------------------------------------------------------------------------------

void esguiCanvasUnlock(ESGUI_HDC hdc)
{
  if( !hdc )
    return;
  
  esguiScreenUnlock( hdc->driver );
}
//----------------------------------------------------------------------------------------------

const ESGUI_RECT* esguiCanvasGetScreenExt(const ESGUI_HDC hdc)
{
  if( hdc )
    return &hdc->ext;
  else
    return NULL;
}
//----------------------------------------------------------------------------------------------

void esguiCanvasSetClipRect(ESGUI_HDC hdc, const ESGUI_RECT* r)
{
//#ifndef NDEBUG
//  if( r )
//    ES_DEBUG_TRACE4("esguiCanvasSetClipRect(%d,%d,%d,%d)\n", r->org.x, r->org.y, r->edge.x, r->edge.y )
//  else
//    ES_DEBUG_TRACE0("esguiCanvasSetClipRect(NULL)\n")
//#endif

  if( hdc && r )
  {
    hdc->clip = *r;
    internalCalcInternalClipRect(hdc);
  }
  else
    esguiCanvasResetClipRect(hdc);
}
//----------------------------------------------------------------------------------------------

const ESGUI_RECT* esguiCanvasGetClipRect(const ESGUI_HDC hdc)
{
  if( hdc )
    return &hdc->clip;
  else  
    return NULL;
}
//----------------------------------------------------------------------------------------------

void esguiCanvasResetClipRect(ESGUI_HDC hdc)
{
  if( hdc )
  {
    hdc->clip = hdc->ext;      
    internalCalcInternalClipRect(hdc);
  }
}
//----------------------------------------------------------------------------------------------

ESGUI_SCREEN_ROTATION esguiCanvasSetScreenRotation(ESGUI_HDC hdc, ESGUI_SCREEN_ROTATION rotation)
{
  if( hdc )
  {
    ESGUI_SCREEN_ROTATION result = hdc->driver->rotation;

    if( rotation != result )
    {
      // call driver service
      esguiScreenSetRotation(
        hdc->driver,
        rotation
      );
      internalUpdateScreenExt(hdc);
      esguiCanvasResetClipRect(hdc);
    }

    return result;
  }
  
  return ESGUI_SCREEN_ROT_INVALID;
}
//----------------------------------------------------------------------------------------------

esBL esguiCanvasSetUpdatePos(ESGUI_HDC hdc, esBL update )
{
  if( hdc )
  {
    esBL result = guiCtlUpdatePos == (hdc->ctl & guiCtlUpdatePos);
    if( update )
      hdc->ctl |= guiCtlUpdatePos;
    else
      hdc->ctl &= ~guiCtlUpdatePos;
    return result;
  }
  
  return FALSE;
}
//----------------------------------------------------------------------------------------------

esBL esguiCanvasGetUpdatePos(const ESGUI_HDC hdc)
{
  if( hdc )
    return guiCtlUpdatePos == (hdc->ctl & guiCtlUpdatePos);
  
  return FALSE;
}
//----------------------------------------------------------------------------------------------

esBL esguiCanvasSetOpaqueText( ESGUI_HDC hdc, esBL opaque )
{
  if( hdc )
  {
    esBL result = guiCtlOpaqueText == (hdc->ctl & guiCtlOpaqueText);
    if( opaque )
      hdc->ctl |= guiCtlOpaqueText;
    else
      hdc->ctl &= ~guiCtlOpaqueText;
    return result;
  }
  
  return FALSE;
}
//----------------------------------------------------------------------------------------------

// query text opacity mode
esBL esguiCanvasGetOpaqueText(const ESGUI_HDC hdc)
{
  if( hdc )
    return guiCtlOpaqueText == (hdc->ctl & guiCtlOpaqueText);
    
  return FALSE;  
}
//----------------------------------------------------------------------------------------------

static void inline internalEsguiCanvasSysPalColorSet(ESGUI_HDC hdc, int idx, ESGUI_Color_t clr)
{
  int bpp = esguiScreenBitDepthGet( hdc->driver );

  if( bpp <= 8 )
    hdc->sysPal8[idx] = (esU8)(clr & 0xFF);
  else if( bpp <= 16 )
    hdc->sysPal16[idx] = (esU16)(clr & 0xFFFF);
  else
    hdc->sysPal32[idx] = clr;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR esguiCanvasFgColorSet(ESGUI_HDC hdc, ESGUI_COLOR color)
{
  ESGUI_COLOR result;
  result.raw = 0;
  
  if( !hdc )
    return result;

  result = hdc->colorFg;
  if( result.raw != color.raw )
  {
    hdc->colorFgRaw = esguiScreenFromESGUI_COLOR( 
      hdc->driver, 
      color 
    );
    hdc->colorFg = color;
    internalEsguiCanvasSysPalColorSet(
      hdc,
      1,
      hdc->colorFgRaw
    );
  }

  return result;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR esguiCanvasFgColorGet(const ESGUI_HDC hdc)
{
  if( !hdc )
    return c_clBlack;
    
  return hdc->colorFg;
}
//----------------------------------------------------------------------------------------------

ESGUI_Color_t esguiCanvasFgColorGetRaw(const ESGUI_HDC hdc)
{
  if( !hdc )
    return 0;
  
  return hdc->colorFgRaw;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR esguiCanvasBgColorSet(ESGUI_HDC hdc, ESGUI_COLOR color)
{
  ESGUI_COLOR result;
  result.raw = 0;
  
  if( !hdc )
    return result;
  
  result = hdc->colorBg;
  if( result.raw != color.raw )
  {
    hdc->colorBgRaw = esguiScreenFromESGUI_COLOR( 
      hdc->driver, 
      color 
    );
    hdc->colorBg = color;

    internalEsguiCanvasSysPalColorSet(
      hdc,
      0,
      hdc->colorBgRaw
    );
  }
  
  return result;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR esguiCanvasBgColorGet(const ESGUI_HDC hdc)
{
  if( !hdc )
    return c_clBlack;
    
  return hdc->colorBg;
}
//----------------------------------------------------------------------------------------------

ESGUI_Color_t esguiCanvasBgColorGetRaw(const ESGUI_HDC hdc)
{
  if( !hdc )
    return 0;
    
  return hdc->colorBgRaw;
}
//----------------------------------------------------------------------------------------------

const ESGUI_PALETTE* esguiCanvasPaletteGet(const ESGUI_HDC hdc)
{
  if( !hdc )
    return NULL;

  return &hdc->pal;
}
//---------------------------------------------------------------------------

void esguiCanvasPaletteSet(ESGUI_HDC hdc, const ESGUI_PALETTE* pal)
{
  if( hdc && pal )
    hdc->pal = *pal;
}
//---------------------------------------------------------------------------

void esguiCanvasSysPaletteSet(ESGUI_HDC hdc)
{
  if( !hdc || !hdc->driver )
    return;

  hdc->pal.colorCnt = 2;
  hdc->pal.realized = true;
  hdc->pal.colorFmt = esguiScreenColorFormatGet(hdc->driver);

  int bpp = esguiBppFromColorFormat(
    hdc->pal.colorFmt
  );
  ESE_ASSERT(bpp);

  if( bpp <= 8 )
    hdc->pal.colors8 = hdc->sysPal8;
  else if( 16 == bpp )
    hdc->pal.colors16 = hdc->sysPal16;
  else 
    hdc->pal.colors32 = hdc->sysPal32;
}
//---------------------------------------------------------------------------

void esguiCanvasRealizePaletteFromPalette(ESGUI_HDC hdc, ESGUI_PALETTE* dest, const ESGUI_PALETTE* src)
{
  if(
    !hdc ||
    !dest ||
    dest->realized ||
    !src ||
    dest->colorCnt < src->colorCnt
  )
    return;

  int idx;
  dest->colorFmt = hdc->driver->colorFmt;
  dest->colorCnt = src->colorCnt;

  if( dest->colorFmt == src->colorFmt )
  {
    dest->realized = 1;

    int bpp = esguiBppFromColorFormat(src->colorFmt);
    ESE_ASSERT(bpp);

    for(idx = 0; idx < src->colorCnt; ++idx )
    {
      if(bpp <= 8)
        dest->colors8[idx] = src->colors8[idx];
      else if( 16 == bpp )
        dest->colors16[idx] = src->colors16[idx];
      else
        dest->colors32[idx] = src->colors32[idx];
    }

    return;
  }

  for(idx = 0; idx < src->colorCnt; ++idx )
    esguiPaletteColorSet(
      dest,
      idx,
      esguiPaletteColorGet(
        src,
        idx
      ),
      src->colorFmt
    );

  dest->realized = 1;
}
//---------------------------------------------------------------------------

void esguiCanvasRealizePalette(ESGUI_HDC hdc, ESGUI_PALETTE* dest, const ESGUI_COLOR* src, esU32 srcCnt)
{
  if(
    hdc &&
    dest &&
    !dest->realized &&
    src &&
    dest->colorCnt >= srcCnt
  )
  {
    int idx;
    dest->colorFmt = hdc->driver->colorFmt;
    dest->colorCnt = srcCnt;

    if( dest->colorFmt == esguiClrfmtNative )
    {
      dest->realized = 1;

      for(idx = 0; idx < srcCnt; ++idx )
        dest->colors32[idx] = src[idx].raw;

      return;
    }

    for(idx = 0; idx < srcCnt; ++idx )
      esguiPaletteColorSet(
        dest,
        idx,
        src[idx].raw,
        esguiClrfmtNative
      );

    dest->realized = 1;
  }
}
//---------------------------------------------------------------------------

void esguiCanvasUnrealizePalette(ESGUI_HDC hdc, ESGUI_COLOR* dest, esU32 cnt, const ESGUI_PALETTE* src)
{
  if(
    hdc &&
    dest &&
    cnt &&
    src &&
    src->colorCnt == cnt
  )
  {
    esU32 idx;
    for(idx = 0; idx < cnt; ++idx )
    {
      ESGUI_COLOR clr;
      if(
        esguiResultOk != esguiColorToColor(
          &clr.raw,
          esguiClrfmtNative,
          esguiPaletteColorGet(
            src,
            idx
          ),
          esguiPaletteColorFmtGet(src),
          NULL
        )
      )
        break;

      dest[idx] = clr;
    }
  }
}
//----------------------------------------------------------------------------------------------

void esguiCanvasFontSet(ESGUI_HDC hdc, const ESGUI_FONT* fnt)
{
  if( hdc )
    hdc->font = fnt;
}
//----------------------------------------------------------------------------------------------

const ESGUI_FONT* esguiCanvasFontGet(const ESGUI_HDC hdc)
{
  if( hdc )
    return hdc->font;
  else
    return 0;
}
//----------------------------------------------------------------------------------------------

static inline void internalDrawPoint(ESGUI_HDC hdc, int x, int y)
{
  esguiScreenSetPos(hdc->driver, x, y);
  esguiScreenSetPixel(
    hdc->driver, 
    hdc->colorFgRaw
  );
}
//----------------------------------------------------------------------------------------------

static void internalDrawHLine(ESGUI_HDC hdc, int x0, int y0, int x1)
{
  if(x0 > x1)             
    esguiUtilsSwapInt(&x0, &x1);
  
  esguiScreenSetPos(hdc->driver, x0, y0);
  while(x1 >= x0)
  {
    esguiScreenSetPixel(
      hdc->driver, 
      hdc->colorFgRaw
    );
    ++x0;
  }
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawHLine_(ESGUI_HDC hdc, int x, int y, int len)
{
  if( !hdc )
    return;
    
  // check display cropping, update internal coords, if needed
  ESGUI_RECT r;
  r.org.x = x;
  r.org.y = y;
  r.edge.x = r.org.x + len;
  r.edge.y = r.org.y + 1;

  esguiRectNormalize(&r);
  if( esguiRectIntersect(&r, &hdc->internalClip, &r) )
  {
    esguiScreenIoBatchBegin(hdc->driver);
    internalDrawHLine(hdc, r.org.x, r.org.y, r.edge.x);
    esguiScreenIoBatchEnd(hdc->driver);
  }

  if( hdc->ctl & guiCtlUpdatePos ) // update current position
  {
    hdc->pos.x = x + len;
    hdc->pos.y = y;
  }
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawHLine(ESGUI_HDC hdc, const ESGUI_POINT* org, int len)
{
  ESE_ASSERT(org);

  esguiCanvasDrawHLine_(
    hdc,
    org->x,
    org->y,
    len
  );
}
//----------------------------------------------------------------------------------------------

static void internalDrawVLine(ESGUI_HDC hdc, int x0, int y0, int y1)
{
  if(y0 > y1)             
    esguiUtilsSwapInt(&y0, &y1);

  while(y1 >= y0)
  {
    internalDrawPoint(hdc, x0, y0);
    y0++;
  }
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawVLine_(ESGUI_HDC hdc, int x, int y, int len)
{
  if( !hdc )
    return;

  // check display cropping, update internal coords, if needed
  ESGUI_RECT r;
  r.org.x = x;
  r.org.y = y;  
  r.edge.x = r.org.x + 1;
  r.edge.y = r.org.y + len;

  // use internalDrawVLine
  esguiRectNormalize(&r);
  if( esguiRectIntersect(&r, &hdc->internalClip, &r) )
  {
    esguiScreenIoBatchBegin(hdc->driver);
    internalDrawVLine(hdc, r.org.x, r.org.y, r.edge.y);  
    esguiScreenIoBatchEnd(hdc->driver);
  }

  if( hdc->ctl & guiCtlUpdatePos )
  {
    hdc->pos.x = x;
    hdc->pos.y = y + len;
  }
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawVLine(ESGUI_HDC hdc, const ESGUI_POINT* org, int len)
{
  ESE_ASSERT(org);

  esguiCanvasDrawVLine_(
    hdc,
    org->x,
    org->y,
    len
  );
}
//----------------------------------------------------------------------------------------------

void esguiCanvasMovePen(ESGUI_HDC hdc, const ESGUI_POINT* pos)
{
  if( hdc )
  {
    // check display cropping
    if( esguiPointInRect(pos, &hdc->ext) )
      esguiScreenSetPos(hdc->driver, pos->x, pos->y);

    // always set position due to this function's meaning, 
    // regardless hdc->ctl.m_updatePos value
    hdc->pos = *pos;
  }
}
//----------------------------------------------------------------------------------------------

void esguiCanvasMovePen_(ESGUI_HDC hdc, int x, int y)
{
  if( hdc )
  {
    ESGUI_POINT pos = {x, y};
    esguiCanvasMovePen(hdc, &pos);
  }
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawPoint(ESGUI_HDC hdc, const ESGUI_POINT* pos)
{
  if( !hdc )
    return;
    
  if( esguiPointInRect(pos, &hdc->ext) )
  {
    esguiScreenIoBatchBegin(hdc->driver);
    internalDrawPoint(hdc, pos->x, pos->y);
    esguiScreenIoBatchEnd(hdc->driver);
  }

  if( hdc->ctl & guiCtlUpdatePos )
    hdc->pos = *pos;
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawPoint_(ESGUI_HDC hdc, int x, int y)
{
  if( !hdc )
    return;

  if( esguiPointInRect_(x, y, &hdc->ext) )
  {
    esguiScreenIoBatchBegin(hdc->driver);
    internalDrawPoint(hdc, x, y);
    esguiScreenIoBatchEnd(hdc->driver);
  }

  if( hdc->ctl & guiCtlUpdatePos )
  {
    hdc->pos.x = x;
    hdc->pos.y = y;
  }
}
//----------------------------------------------------------------------------------------------

static inline void internalDrawClippedRect(ESGUI_HDC hdc, const ESGUI_RECT* r, const ESGUI_RECT* clipped)
{
  if( clipped->org.y == r->org.y )
    internalDrawHLine(
      hdc, 
      clipped->org.x, 
      clipped->org.y, 
      clipped->edge.x-1
    );
  
  if( clipped->edge.y == r->edge.y && clipped->edge.y > clipped->org.y )
    internalDrawHLine(
      hdc, 
      clipped->org.x, 
      clipped->edge.y, 
      clipped->edge.x-1
    );
  
  if( clipped->org.x == r->org.x )
    internalDrawVLine(
      hdc, 
      clipped->org.x, 
      clipped->org.y, 
      clipped->edge.y
    );
  
  if( clipped->edge.x == r->edge.x && clipped->edge.x > clipped->org.x)
    internalDrawVLine(
      hdc, 
      clipped->edge.x-1, 
      clipped->org.y, 
      clipped->edge.y
    );
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawRect(ESGUI_HDC hdc, const ESGUI_RECT* r)
{  
  ESGUI_RECT clipped;

  if( !hdc )
    return;

  // check display cropping, skip segments if needed
  if( esguiRectIntersect(r, &hdc->internalClip, &clipped) )
  {
    esguiScreenIoBatchBegin(hdc->driver);

    internalDrawClippedRect(
      hdc,
      r,
      &clipped
    );
    
    esguiScreenIoBatchEnd(hdc->driver);
  }
}
//----------------------------------------------------------------------------------------------

void esguiCanvasFillRect(ESGUI_HDC hdc, const ESGUI_RECT* r)
{  
  ESGUI_RECT clipped;

  if( !hdc )
    return;
    
  // check display cropping
  if(
    esguiRectIntersect(
      r, 
      &hdc->internalClip, 
      &clipped
    ) &&
    clipped.edge.x > clipped.org.x &&
    clipped.edge.y > clipped.org.y 
  )
  {
    esguiScreenIoBatchBegin(hdc->driver);

    esguiScreenFillWindow(
      hdc->driver, 
      clipped.org.x, 
      clipped.org.y, 
      clipped.edge.x, 
      clipped.edge.y, 
      hdc->colorBgRaw
    );
    
    esguiScreenResetWindow(hdc->driver);
 
    esguiScreenIoBatchEnd(hdc->driver);
  }
}
//----------------------------------------------------------------------------------------------

void esguiCanvasClearScreen(ESGUI_HDC hdc)
{
  if( !hdc )
    return;
    
  esguiScreenIoBatchBegin(hdc->driver);
  esguiScreenClear(
    hdc->driver, 
    hdc->colorBgRaw
  );
  esguiScreenIoBatchEnd(hdc->driver);
}
//----------------------------------------------------------------------------------------------

static void internalDrawLine(ESGUI_HDC hdc, ESGUI_POINT* beg, const ESGUI_POINT* end)
{
  ESGUI_POINT d, d2;
  signed char dx_sym = 0, dy_sym = 0;
  int di = 0;

  d.x = end->x - beg->x;
  d.y = end->y - beg->y;

  if(d.x == 0)          // vertical line
  {
    internalDrawVLine(hdc, beg->x, beg->y, end->y);
    return;
  }
  else if(d.y == 0)      // horizontal line  
  {
    internalDrawHLine(hdc, beg->x, beg->y, end->x);
    return;
  }

  if(d.x > 0)
    dx_sym = 1;
  else
    dx_sym = -1;

  if(d.y > 0)
    dy_sym = 1;
  else
    dy_sym = -1;

  d.x *= dx_sym;
  d.y *= dy_sym;
 
  d2.x = d.x * 2;
  d2.y = d.y * 2;
   
  if(d.x >= d.y)
  { 
    di = d2.y - d.x;
    while(beg->x != end->x)
    {   
      internalDrawPoint(hdc, beg->x, beg->y);
      beg->x += dx_sym;
      if( di < 0 )
        di += d2.y;
      else
      {
        di += d2.y - d2.x;
        beg->y += dy_sym;
      }
    }
    internalDrawPoint(hdc, beg->x, beg->y);
  }
  else
  {
    di = d2.x - d.y;
    while(beg->y != end->y)
    {   
      internalDrawPoint(hdc, beg->x, beg->y);
      beg->y += dy_sym;
      if(di < 0)
        di += d2.x;
      else
      {
        di += d2.x - d2.y;
        beg->x += dx_sym;
      }
    }
    internalDrawPoint(hdc, beg->x, beg->y);
  }
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawLine(ESGUI_HDC hdc, const ESGUI_POINT* beg, const ESGUI_POINT* end)
{  
  ESGUI_POINT beg1, end1;

  if( !hdc )
    return;

  // perform proper coord clipping
  if( esguiMathLineIntersect(beg, end, &hdc->internalClip, &beg1, &end1) )
  {
    esguiScreenIoBatchBegin(hdc->driver);
    
    internalDrawLine(hdc, &beg1, &end1);

    esguiScreenIoBatchEnd(hdc->driver);
  }

  if( hdc->ctl & guiCtlUpdatePos )
    hdc->pos = *end;  
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawLine_(ESGUI_HDC hdc, int x0, int y0, int x1, int y1)
{
  ESGUI_POINT beg, end;

  if( !hdc )
    return;
    
  beg.x = x0; beg.y = y0;
  end.x = x1; end.y = y1;

  esguiCanvasDrawLine(hdc, &beg, &end);
}
//----------------------------------------------------------------------------------------------

void esguiCanvasLineTo(ESGUI_HDC hdc, const ESGUI_POINT* end)
{
  esguiCanvasDrawLine(hdc, &hdc->pos, end);
}
//----------------------------------------------------------------------------------------------

void esguiCanvasLineTo_(ESGUI_HDC hdc, int x, int y)
{
  ESGUI_POINT end;
  end.x = x; end.y = y;  
  
  esguiCanvasLineTo(hdc, &end);
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawCircle(ESGUI_HDC hdc, const ESGUI_POINT* center, int r)
{
  ESGUI_POINT draw[8];
  signed short xx, yy;
  signed short  di;

  if( hdc )
    return;
     
  if(r == 0)      /* no radius */ 
    return;
  
  esguiScreenIoBatchBegin(hdc->driver); 
  draw[0].x = draw[1].x = center->x;
  draw[0].y = draw[1].y = center->y + r;
  if( esguiPointInRect(&draw[0], &hdc->internalClip) )
    internalDrawPoint(hdc, draw[0].x, draw[0].y);    /* 90 degree */
  
  draw[2].x = draw[3].x = center->x;
  draw[2].y = draw[3].y = center->y - r;
  if( esguiPointInRect(&draw[2], &hdc->internalClip) )
    internalDrawPoint(hdc, draw[2].x, draw[2].y);    /* 270 degree */
     
  draw[4].x = draw[6].x = center->x + r;
  draw[4].y = draw[6].y = center->y;
  if( esguiPointInRect(&draw[4], &hdc->internalClip) )
    internalDrawPoint(hdc, draw[4].x, draw[4].y);    /* 0 degree */
   
  draw[5].x = draw[7].x = center->x - r;
  draw[5].y = draw[7].y = center->y;
  if( esguiPointInRect(&draw[5], &hdc->internalClip) )
    internalDrawPoint(hdc, draw[5].x, draw[5].y);    /* 180 degree */
  esguiScreenIoBatchEnd(hdc->driver);

  if(r == 1)
    return;
   
  di = 3 - 2*r;
  xx = 0;
  yy = r;
  while(xx < yy)
  {  
    if(di < 0)
    {
      di += 4*xx + 6;        
    }
    else
    {  
      di += 4*(xx - yy) + 10;
      --yy;    
      --draw[0].y;
      --draw[1].y;
      ++draw[2].y;
      ++draw[3].y;
      --draw[4].x;
      ++draw[5].x;
      --draw[6].x;
      ++draw[7].x;     
    }  
    
    ++xx;   
    ++draw[0].x;
    --draw[1].x;
    ++draw[2].x;
    --draw[3].x;
    ++draw[4].y;
    ++draw[5].y;
    --draw[6].y;
    --draw[7].y;
    
    esguiScreenIoBatchBegin(hdc->driver);
    if( esguiPointInRect(&draw[0], &hdc->internalClip) )  
      internalDrawPoint(hdc, draw[0].x, draw[0].y);
      
    if( esguiPointInRect(&draw[1], &hdc->internalClip) )   
      internalDrawPoint(hdc, draw[1].x, draw[1].y);

    if( esguiPointInRect(&draw[2], &hdc->internalClip) )
      internalDrawPoint(hdc, draw[2].x, draw[2].y);

    if( esguiPointInRect(&draw[3], &hdc->internalClip) )  
      internalDrawPoint(hdc, draw[3].x, draw[3].y);

    if( esguiPointInRect(&draw[4], &hdc->internalClip) )  
      internalDrawPoint(hdc, draw[4].x, draw[4].y);

    if( esguiPointInRect(&draw[5], &hdc->internalClip) ) 
      internalDrawPoint(hdc, draw[5].x, draw[5].y);

    if( esguiPointInRect(&draw[6], &hdc->internalClip) )  
      internalDrawPoint(hdc, draw[6].x, draw[6].y);

    if( esguiPointInRect(&draw[7], &hdc->internalClip) )
      internalDrawPoint(hdc, draw[7].x, draw[7].y);

    esguiScreenIoBatchEnd(hdc->driver);
  }
}
//----------------------------------------------------------------------------------------------

void esguiCanvasGetStringExtentN(ESGUI_HDC hdc, const ESGUI_FONT* fnt, ESE_CSTR text, int strlen, int tabWidth, ESGUI_POINT* ext )
{
  if( !hdc || !text || (0 == *text) || !strlen )
    return;

  if( !fnt )
    fnt = hdc->font;

  int len = 0;
  ESE_CSTR textEnd = text+strlen;

  esguiPointZeroSet(ext);

  ext->y = esguiFontHeightGet(fnt);

  while( text < textEnd && *text )
  {
    esU32 ch;
    text = eseUtilsStrUtf32FromUtf8Get(
      text,
      textEnd,
      &ch
    );

    if( 0 == ch )
      break;

    if( ' ' == ch )
      len += esguiFontSpaceWidthGet(fnt);
    else if( '\n' == ch )
    {
      ext->y += esguiFontHeightGet(fnt);
      ext->x = esguiUtils_MAX(
        ext->x,
        len
      );
      len = 0;
    }
    else if( '\t' == ch )
      len += tabWidth * esguiFontSpaceWidthGet(fnt);
    else if( ESGUI_FONT_UNSPEC_CHAR == ch )
      len += esguiFontUnspecifiedWidthGet(fnt);
    else
      len += esguiFontWidthGet(
        fnt,
        ch
      );
  }

  ext->x = esguiUtils_MAX(
    ext->x,
    len
  );

  if( ext->x )
    ++ext->x;
}
//----------------------------------------------------------------------------------------------

void esguiCanvasGetStringExtentWrapN(ESGUI_HDC hdc, const ESGUI_FONT* fnt, ESE_CSTR text, int strlen, int tabWidth, int width, ESGUI_POINT* ext )
{
  ESGUI_POINT _ext;
  int wordCount = 0;
  ESE_CSTR lineEnd = text;

  if( !fnt )
    fnt = hdc->font;

  if( width )
    width = esguiUtils_MAX(
      MIN_WRAP_WIDTH, 
      width
    );
  else
  {
    esguiCanvasGetStringExtentN( 
      hdc, 
      fnt, 
      text, 
      strlen, 
      tabWidth,
      ext
    );
    return;
  }

  // calc text extent on per-word basis, allow breaking on any non-text char, i.e
  // punctuation and whitespace
  esguiPointZeroSet( &_ext );
  esguiPointZeroSet( ext );
  ext->y = esguiFontHeightGet(fnt);
  while(1)
  {
    esBL doLineBreak = FALSE;
    // delimiter pos has current word length, statring from pStr
    int delimPos = strcspn(
      lineEnd, 
      " :;?!,.-+=\t\n"
    );
    
    if( delimPos )
      ++wordCount;

    // first, check if we have to wrap string. do not wrap lines containing
    // the single word
    if( wordCount > 1)
    {
      esguiCanvasGetStringExtentN(
        hdc, 
        fnt, 
        text, 
        (lineEnd - text) + delimPos, 
        tabWidth,
        &_ext 
      );
      
      if( _ext.x > width )
        doLineBreak = TRUE;
    }

    // find if we have to do unconditional break: '\n' is found;
    // or we have reached the end of string
    if( !doLineBreak && (0 == lineEnd[delimPos] || '\n' == lineEnd[delimPos]) )
    {
      doLineBreak = TRUE;
      lineEnd += delimPos;
    }
    
    if( doLineBreak )
    {
      esguiCanvasGetStringExtentN( 
        hdc, 
        fnt, 
        text, 
        lineEnd - text, 
        tabWidth,
        &_ext 
      );
      
      ext->x = esguiUtils_MAX(ext->x, _ext.x);

      // exit loop - end of string reached
      if( 0 == *lineEnd )
        break;

      if( '\n' == *lineEnd )
      {
        ++lineEnd;
        if( '\r' == *lineEnd )
          ++lineEnd;
      }

      text = lineEnd;
      wordCount = 0;
      ext->y += esguiFontHeightGet(fnt);
    }
    else
      lineEnd += delimPos+1;
  }
}
//----------------------------------------------------------------------------------------------

void esguiCanvasGetStringExtent(ESGUI_HDC hdc, const ESGUI_FONT* fnt, ESE_CSTR text, int tabWidth, ESGUI_POINT* ext )
{
  esguiCanvasGetStringExtentN( 
    hdc, 
    fnt, 
    text, 
    strlen(text), // TODO: handle unicode surrogates
    tabWidth,
    ext
  );
}
//----------------------------------------------------------------------------------------------

void esguiCanvasGetStringExtentWrap( ESGUI_HDC hdc, const ESGUI_FONT* fnt, ESE_CSTR text, int tabWidth, int width, ESGUI_POINT* ext )
{
  esguiCanvasGetStringExtentWrapN(
    hdc, 
    fnt, 
    text, 
    strlen(text), // TODO: handle unicode surrogates
    tabWidth,
    width, 
    ext
  );
}
//----------------------------------------------------------------------------------------------

static void internalCanvasDrawSpaceChar(ESGUI_HDC hdc, const ESGUI_RECT* rectChar)
{
  if(hdc->ctl & guiCtlOpaqueText)
    esguiCanvasFillRect(
      hdc,
      rectChar
    );
}
//---------------------------------------------------------------------------

static void internalCanvasDrawUnspecifiedChar(ESGUI_HDC hdc, const ESGUI_RECT* rectChar)
{
  ESGUI_RECT frameRect = *rectChar;

  ESGUI_RECT clipped;
  esguiRectIntersect(
    &frameRect,
    &hdc->internalClip,
    &clipped
  );

  if(hdc->ctl & guiCtlOpaqueText)
    esguiScreenFillWindow(
      hdc->driver,
      clipped.org.x,
      clipped.org.y,
      clipped.edge.x,
      clipped.edge.y,
      hdc->colorBgRaw
    );

  if(
    esguiRectWidth(&frameRect) > 2 &&
    esguiRectHeight(&frameRect) > 2
  )
  {
    esguiRectGrow_(
      -1,
      -1,
      &frameRect
    );
    
    esguiRectIntersect(
      &frameRect,
      &hdc->internalClip,
      &clipped
    ); 

    internalDrawClippedRect(
      hdc,
      &frameRect,
      &clipped
    );
  }
}
//---------------------------------------------------------------------------

static void internalCanvasDrawChar(ESGUI_HDC hdc, const ESGUI_RECT* rectChar, ESGUI_BITMAP_ITERATOR* it)
{
  int posX = rectChar->org.x;
  int posY = rectChar->org.y;
  int edgeX = posX + esguiRectWidth(rectChar);

  ESGUI_COLOR_FORMAT drvfmt = esguiScreenColorFormatGet(hdc->driver);
  ESGUI_COLOR_FORMAT bmpfmt = esguiBmpIteratorColorFmtGet(it);

  ESGUI_BITOP nRop = esguiScreenSupportedBitopGet(
    hdc->driver,
    (esguiClrfmtBitmap1 == bmpfmt) ?
      ESGUI_COPY :
      ESGUI_BLEND
  );

  ESGUI_Color_t fg = esguiCanvasFgColorGetRaw(hdc);
  ESGUI_Color_t bg = esguiCanvasBgColorGetRaw(hdc);

  ESGUI_RECT rect = *rectChar;
  if( !esguiRectIntersect(&rect, &hdc->internalClip, &rect) )
    return;

  esguiScreenSetWindow(
    hdc->driver,
    rect.org.x,
    rect.org.y,
    rect.edge.x,
    rect.edge.y
  );

  bool opaque = esguiCanvasGetOpaqueText(hdc);
  bool updScreenPos = true;

  esguiBmpIteratorFirst(it);

  do
  {
    esU32 clr;
    if( !esguiBmpIteratorGetPixel(it, &clr) )
      break;

    if( esguiPointInRect_(posX, posY, &rect) )
    {
      if( nRop == ESGUI_COPY )
      {
        if( !opaque )
        {
          if( 0 == clr )
            updScreenPos = true;
          else
          {
            if( updScreenPos )
            {
              esguiScreenSetPos(
                hdc->driver,
                posX,
                posY
              );
              updScreenPos = false;
            }

            if( esguiClrfmtBitmap1 == bmpfmt )
              esguiScreenSetPixel(
                hdc->driver,
                fg
              );
            else
              esguiScreenSetPixel(
                hdc->driver,
                clr
              );
          }
        }
        else
        {
          if( updScreenPos )
          {
            esguiScreenSetPos(
              hdc->driver,
              posX,
              posY
            );
            updScreenPos = false;
          }

          if( esguiClrfmtBitmap1 == bmpfmt )
          {
            if( clr )
              clr = fg;
            else
              clr = bg;

            esguiScreenSetPixel(
              hdc->driver,
              clr
            );
          }
          else
          {
            ESGUI_Color_t screenClr = bg;

            if(
              esguiResultOk != esguiColorOp(
                &screenClr,
                screenClr,
                drvfmt,
                clr,
                bmpfmt,
                NULL,
                fg,
                drvfmt,
                NULL,
                nRop
              )
            )
              break;

            esguiScreenSetPixel(
              hdc->driver,
              screenClr
            );
          }
        }
      }
      else
      {
        if( updScreenPos )
        {
          esguiScreenSetPos(
            hdc->driver,
            posX,
            posY
          );
          updScreenPos = false;
        }

        esU32 screenClr;
        if( !opaque )
        {
          screenClr = esguiScreenGetPixel(
            hdc->driver
          );
          esguiScreenSetPos(
            hdc->driver,
            posX,
            posY
          );
        }
        else
          screenClr = bg;

        if(
          esguiResultOk != esguiColorOp(
            &screenClr,
            screenClr,
            drvfmt,
            clr,
            bmpfmt,
            NULL,
            fg,
            drvfmt,
            NULL,
            nRop
          )
        )
          break;

        esguiScreenSetPixel(
          hdc->driver,
          screenClr
        );
      }
    }
    else
      updScreenPos = true;

    if(++posX > edgeX)
    {
      ++posY;
      posX = rectChar->org.x;
    }

  } while( esguiBmpIteratorNext(it) );
  
  esguiScreenResetWindow(hdc->driver);
}
//---------------------------------------------------------------------------

void esguiCanvasDrawStringWithCallbackN_(ESGUI_HDC hdc, int x, int y, ESE_CSTR pStr, int strlen, int tabWidth, ESGUI_WgtCharCallback cbk, void* Data)
{
  if( !pStr || (0 == *pStr) || !strlen || !hdc || !hdc->font )
    return;

  ESE_CSTR pStrEnd = pStr + strlen;

  ESGUI_RECT charRect;
  charRect.org.x = x;
  charRect.org.y = y;
  charRect.edge.y = charRect.org.y + esguiFontHeightGet(hdc->font) - 1;

  esguiScreenIoBatchBegin(hdc->driver);
  while(pStr < pStrEnd && *pStr)
  {
    bool isCharGlyph = false;
    esU32 ch;
    ESE_CSTR pStrNext = eseUtilsStrUtf32FromUtf8Get(
      pStr,
      pStrEnd,
      &ch
    );

    if( 0 == ch )
      break;

    if( '\n' == ch )
    {
      charRect.org.x = x;
      charRect.org.y = charRect.edge.y;
      charRect.edge.y += esguiFontHeightGet(hdc->font);
    }
    else //< Draw char normally
    {
      ESGUI_BITMAP_ITERATOR it;
      if( ' ' == ch )
        charRect.edge.x = charRect.org.x +
          esguiFontSpaceWidthGet(hdc->font);
      else if( '\t' == ch )
        charRect.edge.x = charRect.org.x +
          tabWidth * esguiFontSpaceWidthGet(hdc->font);
      else
      {
        if(
          !esguiFontGlyphGet(
            hdc->font,
            ch,
            &it
          )
        )
        {
          ch = ESGUI_FONT_UNSPEC_CHAR;
          charRect.edge.x = charRect.org.x +
            esguiFontUnspecifiedWidthGet(hdc->font);
        }
        else
        {
          charRect.edge.x = charRect.org.x +
            esguiRectWidth(&it.view);
          isCharGlyph = true;
        }
      }

      if(cbk)
        cbk(
          hdc,
          ESGUI_BeforeDrawChar,
          pStr,
          &charRect,
          Data
        );

      if( ' ' == ch || '\t' == ch )
        internalCanvasDrawSpaceChar(
          hdc,
          &charRect
        );
      else if( ESGUI_FONT_UNSPEC_CHAR == ch )
        internalCanvasDrawUnspecifiedChar(
          hdc,
          &charRect
        );
      else
        internalCanvasDrawChar(
          hdc,
          &charRect,
          &it
        );

      if(cbk)
        cbk(
          hdc,
          ESGUI_AfterDrawChar,
          pStr,
          &charRect,
          Data
        );

      charRect.org.x = charRect.edge.x;
      if( isCharGlyph )
        ++charRect.org.x;
    }
    
    pStr = pStrNext;
  }

  esguiScreenIoBatchEnd(hdc->driver);
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawStringWithCallbackN(ESGUI_HDC hdc, const ESGUI_POINT* pos, ESE_CSTR pStr, int strlen, int tabWidth, ESGUI_WgtCharCallback cbk, void* Data)
{
  esguiCanvasDrawStringWithCallbackN_(
    hdc,
    pos->x,
    pos->y,
    pStr,
    strlen,
    tabWidth,
    cbk,
    Data
  );
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawStringWithCallback_(ESGUI_HDC hdc, int x, int y, ESE_CSTR pStr, int tabWidth, ESGUI_WgtCharCallback cbk, void* Data)
{
  esguiCanvasDrawStringWithCallbackN_(
    hdc, 
    x,
    y, 
    pStr, 
    strlen(pStr),
    tabWidth,
    cbk, 
    Data
  );
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawStringWithCallback(ESGUI_HDC hdc, const ESGUI_POINT* pos, ESE_CSTR pStr, int tabWidth, ESGUI_WgtCharCallback cbk, void* Data)
{
  esguiCanvasDrawStringWithCallbackN_(
    hdc, 
    pos->x,
    pos->y, 
    pStr, 
    strlen(pStr),
    tabWidth,
    cbk, 
    Data
  );
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawStringN(ESGUI_HDC hdc, const ESGUI_POINT* pos, ESE_CSTR pStr, int strlen, int tabWidth)
{ 
  esguiCanvasDrawStringWithCallbackN_(
    hdc, 
    pos->x,
    pos->y,    
    pStr, 
    strlen, 
    tabWidth,
    NULL, 
    NULL
  );
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawStringN_(ESGUI_HDC hdc, int x, int y, ESE_CSTR pStr, int strlen, int tabWidth)
{
  esguiCanvasDrawStringWithCallbackN_(
    hdc, 
    x,
    y, 
    pStr, 
    strlen, 
    tabWidth, 
    NULL, 
    NULL
  );
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawString(ESGUI_HDC hdc, const ESGUI_POINT* pos, ESE_CSTR pStr, int tabWidth)
{ 
  esguiCanvasDrawStringWithCallbackN(
    hdc, 
    pos, 
    pStr, 
    eseUtilsStrLenGet(pStr),
    tabWidth,
    NULL, 
    NULL
  );
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawString_(ESGUI_HDC hdc, int x, int y, ESE_CSTR pStr, int tabWidth)
{ 
  esguiCanvasDrawStringWithCallbackN_(
    hdc, 
    x,
    y, 
    pStr, 
    eseUtilsStrLenGet(pStr),
    tabWidth,
    NULL, 
    NULL
  );
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawStringRect(ESGUI_HDC hdc, const ESGUI_RECT* rect, ESE_CSTR pStr, int tabWidth, esU32 flags)
{
  if( pStr && hdc && hdc->font )
  {
    ESGUI_POINT pos, ext;
    ESGUI_RECT oldClipRect;
    ESE_CSTR lineEnd = pStr;
    int wordCount = 0;
    int wrapWidth = ES_BIT_IS_SET(flags, ESGUI_TWRAP) ? 
      esguiRectWidth(rect) : 
      0;

    switch( flags & ESGUI_VTA_MASK )
    {
    case ESGUI_VTA_TOP:
      pos.y = rect->org.y;
      break;
    case ESGUI_VTA_CENTER:
      esguiCanvasGetStringExtentWrap(
        hdc, 
        hdc->font, 
        pStr, 
        tabWidth,
        wrapWidth, 
        &ext 
      );
      pos.y = rect->org.y + (esguiRectHeight(rect) - ext.y) / 2;
      break;
    case ESGUI_VTA_BOTTOM:
      esguiCanvasGetStringExtentWrap(
        hdc, 
        hdc->font, 
        pStr, 
        tabWidth,
        wrapWidth, 
        &ext 
      );
      pos.y = rect->edge.y - ext.y;
      break;
    }

    if( ES_BIT_IS_SET(flags, ESGUI_TCLIP) )
    {
      oldClipRect = hdc->clip;
      esguiCanvasSetClipRect(hdc, rect);
    }

    // draw text on per-word basis, allow breaking on any non-text char, i.e
    // punctuation and whitespace
    while(1)
    {
      esBL doLineBreak = FALSE;
      // delimiter pos has current word length, statring from pStr
      int delimPos = strcspn(lineEnd, " :;?!,.-+=\t\n");
      if( delimPos )
        ++wordCount;

      // first, check if we have to wrap string. do not wrap lines containing
      // the single word
      if( ES_BIT_IS_SET(flags, ESGUI_TWRAP) && wordCount > 1)
      {
        esguiCanvasGetStringExtentN( 
          hdc, 
          hdc->font, 
          pStr, 
          (lineEnd - pStr) + delimPos, 
          tabWidth,
          &ext 
        );
        
        if( ext.x > wrapWidth )
          doLineBreak = TRUE;
      }

      // find if we have to do unconditional break: '\n' is found;
      // or we have reached the end of string
      if( !doLineBreak && (0 == lineEnd[delimPos] || '\n' == lineEnd[delimPos]) )
      {
        doLineBreak = TRUE;
        lineEnd += delimPos;
      }
      
      if( doLineBreak )
      {
        esguiCanvasGetStringExtentN( 
          hdc, 
          hdc->font, 
          pStr, 
          lineEnd - pStr, 
          tabWidth,
          &ext 
        );

        switch( flags & ESGUI_HTA_MASK )
        {
        case ESGUI_HTA_LEFT:
          pos.x = rect->org.x;
          break;
        case ESGUI_HTA_CENTER:
          pos.x = rect->org.x + (esguiRectWidth(rect) - ext.x) / 2;
          break;
        case ESGUI_HTA_RIGHT:
          pos.x = rect->edge.x - ext.x;
          break;
        }

        esguiCanvasDrawStringN(
          hdc, 
          &pos, 
          pStr, 
          lineEnd-pStr,
          tabWidth
        );

        // exit loop - end of string reached
        if( 0 == *lineEnd )
          break;

        if( '\n' == *lineEnd )
        {
          ++lineEnd;
          if( '\r' == *lineEnd )
            ++lineEnd;
        }

        pStr = lineEnd;
        wordCount = 0;
        pos.y += ext.y;  
      }
      else
        lineEnd += delimPos+1;
    }

    // restore clip rect, if needed
    if( ES_BIT_IS_SET(flags, ESGUI_TCLIP) )
      esguiCanvasSetClipRect(
        hdc,
        &oldClipRect
      );
  }
}
//----------------------------------------------------------------------------------------------

#if defined(ESGUI_USE_BITMAP) && 1 == ESGUI_USE_BITMAP

void esguiCanvasDrawPictureItROP_(ESGUI_HDC hdc, int x, int y, ESGUI_BITMAP_ITERATOR* it, ESGUI_BITOP nRop)
{
  if(
    !hdc ||
    !esguiBmpIteratorIsOk(it)
  )
    return;

  ESGUI_COLOR_FORMAT drvfmt = esguiScreenColorFormatGet(hdc->driver);
  ESGUI_COLOR_FORMAT bmpfmt = esguiBmpColorFormatGet(it->bmp);

  int posX = x;
  int edgeX = x + esguiRectWidth(&it->view);

  ESGUI_RECT rect;
  esguiRectSet_(
    posX,
    y,
    edgeX,
    y + esguiRectHeight(&it->view),
    &rect
  );

  if( !esguiRectIntersect(&rect, &hdc->internalClip, &rect) )
    return;

  esguiScreenIoBatchBegin(hdc->driver);
  esguiScreenSetWindow(
    hdc->driver,
    rect.org.x,
    rect.org.y,
    rect.edge.x,
    rect.edge.y
  );

  // Adjust to supported ROP
  nRop = esguiScreenSupportedBitopGet(hdc->driver, nRop);
  esguiBmpIteratorFirst(it);
  bool updScreenPos = false;

  do
  {
    esU32 clr;
    if(
      !esguiBmpIteratorGetPixel(
        it,
        &clr
      )
    )
      break;

    if( esguiPointInRect_(posX, y, &rect) )
    {
      if(
        esguiResultOk != esguiColorToColor(
          &clr,
          drvfmt,
          clr,
          bmpfmt,
          esguiCanvasPaletteGet(hdc)
        )
      )
        break;

      if( updScreenPos )
      {
        esguiScreenSetPos(
          hdc->driver,
          posX,
          y
        );
        updScreenPos = false;
      }

      if( nRop == ESGUI_COPY )
      {
        esguiScreenSetPixel(
          hdc->driver,
          clr
        );
      }
      else
      {
        esU32 screenClr = esguiScreenGetPixel(
          hdc->driver
        );
        esguiScreenSetPos(
          hdc->driver,
          posX,
          y
        );

        if(
          esguiResultOk != esguiColorOp(
            &screenClr,
            screenClr,
            drvfmt,
            clr,
            drvfmt,
            NULL,
            0,
            esguiClrfmtInvalid,
            NULL,
            nRop
          )
        )
          break;

        esguiScreenSetPixel(
          hdc->driver,
          screenClr
        );
      }
    }
    else
    {
//      ES_DEBUG_TRACE6(
//        "Pos [%d,%d] is out of clipping rect [%d,%d,%d,%d]",
//        posX,
//        y,
//        rect.org.x,
//        rect.org.y,
//        rect.edge.x,
//        rect.edge.y
//      )

      updScreenPos = true;
    }

    if(++posX > edgeX)
    {
      ++y;
      posX = x;
    }

  } while( esguiBmpIteratorNext(it) );

  esguiScreenResetWindow(hdc->driver);
  esguiScreenIoBatchEnd(hdc->driver);
}
//---------------------------------------------------------------------------

void esguiCanvasDrawPictureItROP(ESGUI_HDC hdc, const ESGUI_POINT* pos, ESGUI_BITMAP_ITERATOR* it, ESGUI_BITOP nRop)
{
  if( !pos )
    return;

  esguiCanvasDrawPictureItROP_(
    hdc,
    pos->x,
    pos->y,
    it,
    nRop
  );
}
//---------------------------------------------------------------------------

void esguiCanvasDrawPictureROP_(ESGUI_HDC hdc, int x, int y, const ESGUI_BITMAP* bmp, const ESGUI_RECT* view, ESGUI_BITOP nRop)
{
  ESGUI_RECT bmpView;

  esguiBmpViewRectGet_(
    bmp,
    -1,-1,-1,-1,
    &bmpView
  );

  if( !view || (view && esguiRectIntersect(&bmpView, view, &bmpView)) )
  {
    ESGUI_RECT rect;

    esguiRectSet_(
      x,
      y,
      x+esguiRectWidth(&bmpView),
      y+esguiRectHeight(&bmpView),
      &rect
    );

    // perform picture cropping
    if( esguiRectIntersect(&rect, &hdc->internalClip, &rect) )
    {
      ESGUI_BITMAP_ITERATOR iter;

      // finally get corrected bmpView after possible clipping of rect
      bmpView.edge.x = bmpView.org.x + esguiRectWidth(&rect);
      bmpView.edge.y = bmpView.org.y + esguiRectHeight(&rect);
      esguiRectOffset_(
        rect.org.x - x,
        rect.org.y - y,
        &bmpView
      );

      if(
        !esguiBmpIteratorInit(
          (ESGUI_BITMAP*)bmp,
          &bmpView,
          &iter,
          esguiBmpIteratorRead
        )
      )
        return;

      esguiCanvasDrawPictureItROP(
        hdc,
        &rect.org,
        &iter,
        nRop
      );
    }
  }
}
//----------------------------------------------------------------------------------------------

void esguiCanvasDrawPictureROP(ESGUI_HDC hdc, const ESGUI_POINT* pos, const ESGUI_BITMAP* bmp, const ESGUI_RECT* view, ESGUI_BITOP nRop)
{
  if( !pos )
    return;
    
  esguiCanvasDrawPictureROP_(
    hdc,
    pos->x,
    pos->y,
    bmp,
    view,
    nRop
  );
}
//----------------------------------------------------------------------------------------------

#endif // ESGUI_USE_BITMAP
