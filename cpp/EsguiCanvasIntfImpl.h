#ifndef _esgui_canvas_impl_h_
#define _esgui_canvas_impl_h_

/// GUI Canvas Intf implementation
///
class EsguiCanvasIntfImpl : public EsguiCanvasIntf
{
public:
  EsguiCanvasIntfImpl(ESGUI_HDRV drv = nullptr) ESE_NOTHROW ESE_KEEP;
  
  bool init(ESGUI_HDRV drv) ESE_NOTHROW ESE_KEEP;

  /// EsguiCanvasIntf implementation
  ///
  virtual bool isInitialized() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;

  /// Lockable interface support
  virtual rtosStatus lock() ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void unlock() ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;

#if defined(ESGUI_USE_STD_COLORS) && (0 != ESGUI_USE_STD_COLORS)

# if (ESGUI_USE_STD_COLORS & ESGUI_USE_STD_COLORS_MONO)
  virtual ESGUI_COLOR black() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_COLOR white() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
# endif

# if (ESGUI_USE_STD_COLORS & ESGUI_USE_STD_COLORS_BASIC_MIN)
  virtual ESGUI_COLOR darkGrey() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_COLOR lightGrey() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_COLOR darkRed() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_COLOR lightRed() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_COLOR darkGreen() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_COLOR lightGreen() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_COLOR darkBlue() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_COLOR lightBlue() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_COLOR darkYellow() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_COLOR lightYellow() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_COLOR magenta() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
# endif

#endif

  virtual ESE_STR strBufferGet() ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESE_CSTR strBufferGet() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual int strBufferLengthGet() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_HDC hdcGet() ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_SCREEN_ROTATION screenRotationSet(ESGUI_SCREEN_ROTATION rotation) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual const ESGUI_RECT* screenExtGet() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void clipRectSet(const ESGUI_RECT* r) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual const ESGUI_RECT* clipRectGet() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void clipRectReset() ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool updatePosSet(bool bUpdate) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool updatePosGet() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool opaqueTextSet(bool bOpaque) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool opaqueTextGet() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_COLOR fgColorSet(ESGUI_COLOR color) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_COLOR fgColorGet() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_Color_t fgColorGetRaw() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_COLOR bgColorSet(ESGUI_COLOR color) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_COLOR bgColorGet() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_Color_t bgColorGetRaw() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual const ESGUI_PALETTE* paletteGet() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void paletteSet(const ESGUI_PALETTE* pal) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void sysPaletteSet() ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void realizePaletteFromPalette(ESGUI_PALETTE* dest, const ESGUI_PALETTE* src) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void realizePalette(ESGUI_PALETTE* pal, const ESGUI_COLOR* cols, esU32 colCnt) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void unrealizePalette(ESGUI_COLOR* cols, esU32 colCnt, const ESGUI_PALETTE* pal) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;

#if 1 == ESGUI_USE_FONT
  virtual void fontSet(const ESGUI_FONT* fnt) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual const ESGUI_FONT* fontGet() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
#endif

  virtual void hline(const ESGUI_POINT* org, int len) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void hline(int x, int y, int len) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void vline(const ESGUI_POINT* org, int len) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void vline(int x, int y, int len) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void penMove(const ESGUI_POINT* pos) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void penMove(int x, int y) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void point(const ESGUI_POINT* pos) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void point(int x, int y) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void rectDraw(const ESGUI_RECT* r) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void rectDraw(const ESGUI_POINT* org, const ESGUI_POINT* edge) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void rectDraw(int x0, int y0, int x1, int y1) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void rectFill(const ESGUI_RECT* r) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void rectFill(const ESGUI_POINT* org, const ESGUI_POINT* edge) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void rectFill(int x0, int y0, int x1, int y1) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void clear() ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void line(const ESGUI_POINT* beg, const ESGUI_POINT* end) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void line(int x0, int y0, int x1, int y1) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void lineTo(const ESGUI_POINT* end) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void lineTo(int x, int y) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void circle(const ESGUI_POINT* center, int r) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void circle(const ESGUI_RECT* r) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;

#if defined(ESGUI_USE_FONT) && 1 == ESGUI_USE_FONT
  virtual void fontInit(ESGUI_FONT* font, const ESGUI_FONT_DATA* block) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void fontBlockAdd(ESGUI_FONT* font, ESGUI_FONT* block) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void fontBlockRemove(ESGUI_FONT* block) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual int fontHeightGet(const ESGUI_FONT* fnt = nullptr) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stringExtentN(const ESGUI_FONT* fnt, ESE_CSTR text, int strlen, int tabWidth, ESGUI_POINT* ext ) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stringExtent(const ESGUI_FONT* fnt, ESE_CSTR text, int tabWidth, ESGUI_POINT* ext) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stringExtentWrapN(const ESGUI_FONT* fnt, ESE_CSTR text, int strlen, int tabWidth, int width, ESGUI_POINT* ext ) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stringExtentWrap(const ESGUI_FONT* fnt, ESE_CSTR text, int tabWidth, int width, ESGUI_POINT* ext ) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stringDrawN(const ESGUI_POINT* pos, ESE_CSTR pStr, int strlen, int tabWidth = ESGUI_TAB_WIDTH_STD, EsguiCanvasIntf::CharDrawCallbackT cbk = nullptr, void* data = nullptr) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stringDrawN(int x, int y, ESE_CSTR pStr, int strlen, int tabWidth = ESGUI_TAB_WIDTH_STD, EsguiCanvasIntf::CharDrawCallbackT cbk = nullptr, void* data = nullptr) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stringDraw(const ESGUI_POINT* pos, ESE_CSTR pStr, int tabWidth = ESGUI_TAB_WIDTH_STD, EsguiCanvasIntf::CharDrawCallbackT cbk = nullptr, void* data = nullptr) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stringDraw(int x, int y, ESE_CSTR pStr, int tabWidth = ESGUI_TAB_WIDTH_STD, EsguiCanvasIntf::CharDrawCallbackT cbk = nullptr, void* data = nullptr) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stringDrawRect(const ESGUI_RECT* rect, ESE_CSTR pStr, int tabWidth, esU32 flags) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;

#endif // ESGUI_USE_FONT

#if defined(ESGUI_USE_BITMAP) && 1 == ESGUI_USE_BITMAP

  virtual int pictureWidthGet(const ESGUI_BITMAP* bmp) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual int pictureHeightGet(const ESGUI_BITMAP* bmp) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_POINT pictureExtGet(const ESGUI_BITMAP* bmp) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void pictureDrawROP(int x, int y, const ESGUI_BITMAP* bmp, const ESGUI_RECT* view = nullptr, ESGUI_BITOP nRop = ESGUI_COPY) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void pictureDrawROP(const ESGUI_POINT* pos, const ESGUI_BITMAP* bmp, const ESGUI_RECT* view = nullptr, ESGUI_BITOP nRop = ESGUI_COPY) ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;

#endif // ESGUI_USE_BITMAP
  
protected:
  ESGUI_DC m_dc;
  bool m_initialized;
  
  ESE_NONCOPYABLE(EsguiCanvasIntfImpl);
};
//--------------------------------------------------------------------------------------------------

#endif //< _esgui_canvas_impl_h_
