#include <esfwxe/target.h>
#include <esfwxe/ese_assert.h>
#include <esfwxe/utils.h>
#include <esgui/include/esgui.h>

#include <esfwxe/cpp/os/EseOsDefs.h>

#include "EsguiCanvasIntf.h"
#include "EsguiCanvasIntfImpl.h"
//----------------------------------------------------------------------------------------------

#include <esgui/core/esgui_canvas.c>
#include <esgui/core/esgui_color.c>
//----------------------------------------------------------------------------------------------

#if defined(ESGUI_USE_STD_COLORS) && (0 != ESGUI_USE_STD_COLORS)
#include <esgui/core/esgui_stdcolors.c>

# if (ESGUI_USE_STD_COLORS & ESGUI_USE_STD_COLORS_MONO)
ESGUI_COLOR EsguiCanvasIntfImpl::black() const ESE_NOTHROW
{
  return c_clBlack;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR EsguiCanvasIntfImpl::white() const ESE_NOTHROW
{
  return c_clWhite;
}
//----------------------------------------------------------------------------------------------
# endif

# if (ESGUI_USE_STD_COLORS & ESGUI_USE_STD_COLORS_BASIC_MIN)
ESGUI_COLOR EsguiCanvasIntfImpl::darkGrey() const ESE_NOTHROW
{
  return c_clDarkGrey;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR EsguiCanvasIntfImpl::lightGrey() const ESE_NOTHROW
{
  return c_clLightGrey;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR EsguiCanvasIntfImpl::darkRed() const ESE_NOTHROW
{
  return c_clDarkRed;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR EsguiCanvasIntfImpl::lightRed() const ESE_NOTHROW
{
  return c_clLightRed;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR EsguiCanvasIntfImpl::darkGreen() const ESE_NOTHROW
{
  return c_clDarkGreen;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR EsguiCanvasIntfImpl::lightGreen() const ESE_NOTHROW
{
  return c_clLightGreen;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR EsguiCanvasIntfImpl::darkBlue() const ESE_NOTHROW
{
  return c_clDarkBlue;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR EsguiCanvasIntfImpl::lightBlue() const ESE_NOTHROW
{
  return c_clLightBlue;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR EsguiCanvasIntfImpl::darkYellow() const ESE_NOTHROW
{
  return c_clDarkYellow;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR EsguiCanvasIntfImpl::lightYellow() const ESE_NOTHROW
{
  return c_clLightYellow;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR EsguiCanvasIntfImpl::magenta() const ESE_NOTHROW
{
  return c_clMagenta;
}
//----------------------------------------------------------------------------------------------
# endif

#endif
//----------------------------------------------------------------------------------------------

EsguiCanvasIntfImpl::EsguiCanvasIntfImpl(ESGUI_HDRV drv /*= nullptr*/) ESE_NOTHROW :
m_initialized(false)
{
  init(drv);
}
//----------------------------------------------------------------------------------------------

bool EsguiCanvasIntfImpl::init(ESGUI_HDRV drv) ESE_NOTHROW
{
  if(!m_initialized)
    m_initialized = (TRUE == esguiCanvasInit(
        &m_dc,
        drv
      )
    );
  
  return m_initialized;
}
//----------------------------------------------------------------------------------------------

bool EsguiCanvasIntfImpl::isInitialized() const ESE_NOTHROW
{
  return m_initialized;
}
//----------------------------------------------------------------------------------------------

rtosStatus EsguiCanvasIntfImpl::lock() ESE_NOTHROW
{
  if( 
    esguiCanvasLock(
      &m_dc
    )
  )
    return rtosOK;

  return rtosErrorResource;
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::unlock() ESE_NOTHROW
{
  esguiCanvasUnlock(
    &m_dc
  );
}
//----------------------------------------------------------------------------------------------

#if defined(ESGUI_WIDGET_USE_STD) && 1 == ESGUI_WIDGET_USE_STD

ESE_STR EsguiCanvasIntfImpl::strBufferGet() ESE_NOTHROW
{
  return esguiStrBuffGet();
}
//----------------------------------------------------------------------------------------------

ESE_CSTR EsguiCanvasIntfImpl::strBufferGet() const ESE_NOTHROW
{
  return (ESE_CSTR)esguiStrBuffGet();
}
//----------------------------------------------------------------------------------------------

int EsguiCanvasIntfImpl::strBufferLengthGet() const ESE_NOTHROW
{
  return ESGUI_BUFF_LEN;
}
//----------------------------------------------------------------------------------------------
#endif

ESGUI_HDC EsguiCanvasIntfImpl::hdcGet() ESE_NOTHROW
{
  return &m_dc;
}
//----------------------------------------------------------------------------------------------

ESGUI_SCREEN_ROTATION EsguiCanvasIntfImpl::screenRotationSet(ESGUI_SCREEN_ROTATION rotation) ESE_NOTHROW
{
  return esguiCanvasSetScreenRotation(
    &m_dc,
    rotation
  );
}
//----------------------------------------------------------------------------------------------

const ESGUI_RECT* EsguiCanvasIntfImpl::screenExtGet() const ESE_NOTHROW
{
  return esguiCanvasGetScreenExt(
    (ESGUI_HDC)&m_dc
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::clipRectSet(const ESGUI_RECT* r) ESE_NOTHROW
{
  esguiCanvasSetClipRect(
    &m_dc,
    r
  );
}
//----------------------------------------------------------------------------------------------

const ESGUI_RECT* EsguiCanvasIntfImpl::clipRectGet() const ESE_NOTHROW
{
  return esguiCanvasGetClipRect(
    (ESGUI_HDC)&m_dc
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::clipRectReset() ESE_NOTHROW
{
  esguiCanvasResetClipRect(&m_dc);
}
//----------------------------------------------------------------------------------------------

bool EsguiCanvasIntfImpl::updatePosSet(bool bUpdate) ESE_NOTHROW
{
  return TRUE == esguiCanvasSetUpdatePos(
    &m_dc,
    bUpdate ? TRUE : FALSE
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiCanvasIntfImpl::updatePosGet() const ESE_NOTHROW
{
  return TRUE == esguiCanvasGetUpdatePos(
    (ESGUI_HDC)&m_dc
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiCanvasIntfImpl::opaqueTextSet(bool bOpaque) ESE_NOTHROW
{
  return TRUE == esguiCanvasSetOpaqueText(
    &m_dc,
    bOpaque ? TRUE : FALSE
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiCanvasIntfImpl::opaqueTextGet() const ESE_NOTHROW
{
  return TRUE == esguiCanvasGetOpaqueText(
    (ESGUI_HDC)&m_dc
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR EsguiCanvasIntfImpl::fgColorSet(ESGUI_COLOR color) ESE_NOTHROW
{
  return esguiCanvasFgColorSet(
    &m_dc,
    color
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR EsguiCanvasIntfImpl::fgColorGet() const ESE_NOTHROW
{
  return esguiCanvasFgColorGet(
    (ESGUI_HDC)&m_dc
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_Color_t EsguiCanvasIntfImpl::fgColorGetRaw() const ESE_NOTHROW
{
  return esguiCanvasFgColorGetRaw(
    (ESGUI_HDC)&m_dc
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR EsguiCanvasIntfImpl::bgColorSet(ESGUI_COLOR color) ESE_NOTHROW
{
  return esguiCanvasBgColorSet(
    &m_dc,
    color
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR EsguiCanvasIntfImpl::bgColorGet() const ESE_NOTHROW
{
  return esguiCanvasBgColorGet(
    (ESGUI_HDC)&m_dc
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_Color_t EsguiCanvasIntfImpl::bgColorGetRaw() const ESE_NOTHROW
{
  return esguiCanvasBgColorGetRaw(
    (ESGUI_HDC)&m_dc
  );
}
//----------------------------------------------------------------------------------------------

const ESGUI_PALETTE* EsguiCanvasIntfImpl::paletteGet() const ESE_NOTHROW
{
  return esguiCanvasPaletteGet(
    (ESGUI_HDC)&m_dc
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::paletteSet(const ESGUI_PALETTE* pal) ESE_NOTHROW
{
  esguiCanvasPaletteSet(
    &m_dc,
    pal
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::sysPaletteSet() ESE_NOTHROW
{
  esguiCanvasSysPaletteSet(&m_dc);
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::realizePaletteFromPalette(ESGUI_PALETTE* dest, const ESGUI_PALETTE* src) ESE_NOTHROW
{
  esguiCanvasRealizePaletteFromPalette(
    &m_dc,
    dest,
    src
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::realizePalette(ESGUI_PALETTE* pal, const ESGUI_COLOR* cols, esU32 colCnt) ESE_NOTHROW
{
  esguiCanvasRealizePalette(
    &m_dc,
    pal,
    cols,
    colCnt
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::unrealizePalette(ESGUI_COLOR* cols, esU32 colCnt, const ESGUI_PALETTE* pal) ESE_NOTHROW
{
  esguiCanvasUnrealizePalette(
    &m_dc,
    cols,
    colCnt,
    pal
  );
}
//----------------------------------------------------------------------------------------------

#if defined(ESGUI_USE_FONT) && 1 == ESGUI_USE_FONT
#include <esgui/core/esgui_font.c>

void EsguiCanvasIntfImpl::fontInit(ESGUI_FONT* font, const ESGUI_FONT_DATA* block) const ESE_NOTHROW
{
  esguiFontInit(
    font,
    block
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::fontBlockAdd(ESGUI_FONT* font, ESGUI_FONT* block) const ESE_NOTHROW
{
  esguiFontBlockAdd(
    font,
    block
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::fontBlockRemove(ESGUI_FONT* block) const ESE_NOTHROW
{
  esguiFontBlockRemove(block);
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::fontSet(const ESGUI_FONT* fnt) ESE_NOTHROW
{
  esguiCanvasFontSet(
    &m_dc,
    fnt
  );
}
//----------------------------------------------------------------------------------------------

const ESGUI_FONT* EsguiCanvasIntfImpl::fontGet() const ESE_NOTHROW
{
  return esguiCanvasFontGet(
    (ESGUI_HDC)&m_dc
  );
}
//----------------------------------------------------------------------------------------------

#endif

void EsguiCanvasIntfImpl::hline(const ESGUI_POINT* org, int len) ESE_NOTHROW
{
  esguiCanvasDrawHLine(
    &m_dc,
    org,
    len
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::hline(int x, int y, int len) ESE_NOTHROW
{
  esguiCanvasDrawHLine_(
    &m_dc,
    x,
    y,
    len
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::vline(const ESGUI_POINT* org, int len) ESE_NOTHROW
{
  esguiCanvasDrawVLine(
    &m_dc,
    org,
    len
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::vline(int x, int y, int len) ESE_NOTHROW
{
  esguiCanvasDrawVLine_(
    &m_dc,
    x,
    y,
    len
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::penMove(const ESGUI_POINT* pos) ESE_NOTHROW
{
  esguiCanvasMovePen(
    &m_dc,
    pos
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::penMove(int x, int y) ESE_NOTHROW
{
  esguiCanvasMovePen_(
    &m_dc,
    x,
    y
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::point(const ESGUI_POINT* pos) ESE_NOTHROW
{
  esguiCanvasDrawPoint(
    &m_dc,
    pos
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::point(int x, int y) ESE_NOTHROW
{
  esguiCanvasDrawPoint_(
    &m_dc,
    x,
    y
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::rectDraw(const ESGUI_RECT* r) ESE_NOTHROW
{
  esguiCanvasDrawRect(
    &m_dc,
    r
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::rectDraw(const ESGUI_POINT* org, const ESGUI_POINT* edge) ESE_NOTHROW
{
  ESGUI_RECT r;
  esguiRectSet(
    org,
    edge,
    &r
  );
  esguiCanvasDrawRect(
    &m_dc,
    &r
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::rectDraw(int x0, int y0, int x1, int y1) ESE_NOTHROW
{
  ESGUI_RECT r;
  esguiRectSet_(
    x0, y0,
    x1, y1,
    &r
  );
  esguiCanvasDrawRect(
    &m_dc,
    &r
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::rectFill(const ESGUI_RECT* r) ESE_NOTHROW
{
  esguiCanvasFillRect(
    &m_dc,
    r
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::rectFill(const ESGUI_POINT* org, const ESGUI_POINT* edge) ESE_NOTHROW
{
  ESGUI_RECT r;
  esguiRectSet(
    org,
    edge,
    &r
  );
  esguiCanvasFillRect(
    &m_dc,
    &r
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::rectFill(int x0, int y0, int x1, int y1) ESE_NOTHROW
{
  ESGUI_RECT r;
  esguiRectSet_(
    x0, y0,
    x1, y1,
    &r
  );
  esguiCanvasFillRect(
    &m_dc,
    &r
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::clear() ESE_NOTHROW
{
  esguiCanvasClearScreen(&m_dc);
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::line(const ESGUI_POINT* beg, const ESGUI_POINT* end) ESE_NOTHROW
{
  esguiCanvasDrawLine(
    &m_dc,
    beg,
    end
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::line(int x0, int y0, int x1, int y1) ESE_NOTHROW
{
  esguiCanvasDrawLine_(
    &m_dc,
    x0, y0,
    x1, y1
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::lineTo(const ESGUI_POINT* end) ESE_NOTHROW
{
  esguiCanvasLineTo(
    &m_dc,
    end
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::lineTo(int x, int y) ESE_NOTHROW
{
  esguiCanvasLineTo_(
    &m_dc,
    x,
    y
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::circle(const ESGUI_POINT* center, int r) ESE_NOTHROW
{
  esguiCanvasDrawCircle(
    &m_dc,
    center,
    r
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::circle(const ESGUI_RECT* r) ESE_NOTHROW
{
  if( !r )
    return;
  
  ESGUI_POINT center = esguiRectCenterGet(r);
  esguiCanvasDrawCircle(
    &m_dc,
    &center,
    esguiUtils_MAX(
      esguiRectWidth(r),
      esguiRectHeight(r)
    )
  );
}
//----------------------------------------------------------------------------------------------

#if defined(ESGUI_USE_FONT) && 1 == ESGUI_USE_FONT

int EsguiCanvasIntfImpl::fontHeightGet(const ESGUI_FONT* fnt) const ESE_NOTHROW
{
  if( !fnt )
    fnt = esguiCanvasFontGet( const_cast<ESGUI_HDC>(&m_dc) );
  
  return esguiFontHeightGet(
    fnt
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::stringExtentN(const ESGUI_FONT* fnt, ESE_CSTR text, int strlen, int tabWidth, ESGUI_POINT* ext ) ESE_NOTHROW
{
  esguiCanvasGetStringExtentN(
    &m_dc,
    fnt,
    text,
    strlen,
    tabWidth,
    ext
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::stringExtent(const ESGUI_FONT* fnt, ESE_CSTR text, int tabWidth, ESGUI_POINT* ext ) ESE_NOTHROW
{
  esguiCanvasGetStringExtent(
    &m_dc,
    fnt,
    text,
    tabWidth,
    ext
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::stringExtentWrapN(const ESGUI_FONT* fnt, ESE_CSTR text, int strlen, int tabWidth, int width, ESGUI_POINT* ext ) ESE_NOTHROW
{
  esguiCanvasGetStringExtentWrapN(
    &m_dc,
    fnt,
    text,
    strlen,
    tabWidth,
    width,
    ext
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::stringExtentWrap(const ESGUI_FONT* fnt, ESE_CSTR text, int tabWidth, int width, ESGUI_POINT* ext ) ESE_NOTHROW
{
  esguiCanvasGetStringExtentWrap(
    &m_dc,
    fnt,
    text,
    tabWidth,
    width,
    ext
  );
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------

struct EsguiCanvasIntfCallbackClosure
{
  EsguiCanvasIntfCallbackClosure(EsguiCanvasIntfImpl& This, EsguiCanvasIntf::CharDrawCallbackT cbk, void* data) ESE_NOTHROW :
  m_this(This),
  m_cbk(cbk),
  m_data(data)
  {}
  
  inline ESGUI_WgtCharCallback proxyGet() const ESE_NOTHROW 
  {
    if( !m_cbk )
      return nullptr;
      
    return &proxy;
  }

  EsguiCanvasIntfImpl& m_this;
  EsguiCanvasIntf::CharDrawCallbackT m_cbk;
  void* m_data;
  
  static void proxy(ESGUI_HDC hdc, ESGUI_CharCallbackReason reason, ESE_CSTR pos, const ESGUI_RECT* rect, void* data) ESE_NOTHROW
  {
    EsguiCanvasIntfCallbackClosure* closure = reinterpret_cast<EsguiCanvasIntfCallbackClosure*>(data);
    ESE_ASSERT(closure);
    ESE_ASSERT(closure->m_cbk);
    
    closure->m_cbk(
      closure->m_this,
      reason,
      pos,
      rect,
      closure->m_data
    );
  }
};
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::stringDrawN(int x, int y, ESE_CSTR pStr, int strlen, int tabWidth /*= ESGUI_TAB_WIDTH_STD*/, EsguiCanvasIntf::CharDrawCallbackT cbk /*= nullptr*/, void* data /*= nullptr*/) ESE_NOTHROW
{
  EsguiCanvasIntfCallbackClosure closure(
    *this,
    cbk,
    data
  );

  esguiCanvasDrawStringWithCallbackN_(
    &m_dc,
    x,
    y,
    pStr,
    strlen,
    tabWidth,
    closure.proxyGet(),
    &closure
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::stringDrawN(const ESGUI_POINT* pos, ESE_CSTR pStr, int strlen, int tabWidth /*= ESGUI_TAB_WIDTH_STD*/, EsguiCanvasIntf::CharDrawCallbackT cbk /*= nullptr*/, void* data /*= nullptr*/) ESE_NOTHROW
{
  if( !pos )
    return;
  
  stringDrawN(
    pos->x,
    pos->y,
    pStr,
    strlen,
    tabWidth,
    cbk,
    data
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::stringDraw(int x, int y, ESE_CSTR pStr, int tabWidth /*= ESGUI_TAB_WIDTH_STD*/, EsguiCanvasIntf::CharDrawCallbackT cbk /*= nullptr*/, void* data /*= nullptr*/) ESE_NOTHROW
{
  stringDrawN(
    x,
    y,
    pStr,
    eseUtilsStrLenGet(pStr),
    tabWidth,
    cbk,
    data
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::stringDraw(const ESGUI_POINT* pos, ESE_CSTR pStr, int tabWidth /*= ESGUI_TAB_WIDTH_STD*/, EsguiCanvasIntf::CharDrawCallbackT cbk /*= nullptr*/, void* data /*= nullptr*/) ESE_NOTHROW
{
  stringDrawN(
    pos,
    pStr,
    eseUtilsStrLenGet(pStr),
    tabWidth,
    cbk,
    data
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::stringDrawRect(const ESGUI_RECT* rect, ESE_CSTR pStr, int tabWidth, esU32 flags) ESE_NOTHROW
{
  esguiCanvasDrawStringRect(
    &m_dc,
    rect,
    pStr,
    tabWidth,
    flags
  );
}
//----------------------------------------------------------------------------------------------

#endif // ESGUI_USE_FONT

#if defined(ESGUI_USE_BITMAP) && 1 == ESGUI_USE_BITMAP
#include <esgui/core/esgui_bitmap.c>

int EsguiCanvasIntfImpl::pictureWidthGet(const ESGUI_BITMAP* bmp) const ESE_NOTHROW
{
  return esguiBmpWidthGet(bmp);
}
//----------------------------------------------------------------------------------------------

int EsguiCanvasIntfImpl::pictureHeightGet(const ESGUI_BITMAP* bmp) const ESE_NOTHROW
{
  return esguiBmpHeightGet(bmp);
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT EsguiCanvasIntfImpl::pictureExtGet(const ESGUI_BITMAP* bmp) const ESE_NOTHROW
{
  return esguiBmpSizeGet(bmp);
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::pictureDrawROP(const ESGUI_POINT* pos, const ESGUI_BITMAP* bmp, const ESGUI_RECT* view /*= nullptr*/, ESGUI_BITOP nRop /*= ESGUI_COPY*/) ESE_NOTHROW
{
  esguiCanvasDrawPictureROP(
    &m_dc,
    pos,
    bmp,
    view,
    nRop
  );
}
//----------------------------------------------------------------------------------------------

void EsguiCanvasIntfImpl::pictureDrawROP(int x, int y, const ESGUI_BITMAP* bmp, const ESGUI_RECT* view /*= nullptr*/, ESGUI_BITOP nRop /*= ESGUI_COPY*/) ESE_NOTHROW
{
  esguiCanvasDrawPictureROP_(
    &m_dc,
    x,
    y,
    bmp,
    view,
    nRop
  );
}
//----------------------------------------------------------------------------------------------

#endif // ESGUI_USE_BITMAP
