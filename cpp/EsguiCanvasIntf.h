#ifndef _esgui_canvas_intf_h_
#define _esgui_canvas_intf_h_

class ESE_ABSTRACT EsguiCanvasIntf
{
public:
  /// @brief Return true if context was initialized
  /// @return true, if context instance was initialized, false otherwise
  ///
  virtual bool isInitialized() const ESE_NOTHROW = 0;
  
  /// Lockable interface support
  virtual rtosStatus lock() ESE_NOTHROW = 0;
  virtual void unlock() ESE_NOTHROW = 0;

#if defined(ESGUI_USE_STD_COLORS) && (0 != ESGUI_USE_STD_COLORS)

# if (ESGUI_USE_STD_COLORS & ESGUI_USE_STD_COLORS_MONO)
  virtual ESGUI_COLOR black() const ESE_NOTHROW = 0;
  virtual ESGUI_COLOR white() const ESE_NOTHROW = 0;
# endif

# if (ESGUI_USE_STD_COLORS & ESGUI_USE_STD_COLORS_BASIC_MIN)
  virtual ESGUI_COLOR darkGrey() const ESE_NOTHROW = 0;
  virtual ESGUI_COLOR lightGrey() const ESE_NOTHROW = 0;
  virtual ESGUI_COLOR darkRed() const ESE_NOTHROW = 0;
  virtual ESGUI_COLOR lightRed() const ESE_NOTHROW = 0;
  virtual ESGUI_COLOR darkGreen() const ESE_NOTHROW = 0;
  virtual ESGUI_COLOR lightGreen() const ESE_NOTHROW = 0;
  virtual ESGUI_COLOR darkBlue() const ESE_NOTHROW = 0;
  virtual ESGUI_COLOR lightBlue() const ESE_NOTHROW = 0;
  virtual ESGUI_COLOR darkYellow() const ESE_NOTHROW = 0;
  virtual ESGUI_COLOR lightYellow() const ESE_NOTHROW = 0;
  virtual ESGUI_COLOR magenta() const ESE_NOTHROW = 0;
# endif

#endif

  /// @brief Access common canvas string buffer contents
  virtual ESE_STR strBufferGet() ESE_NOTHROW = 0;
  virtual ESE_CSTR strBufferGet() const ESE_NOTHROW = 0;
  virtual int strBufferLengthGet() const ESE_NOTHROW = 0;
  
  /// @brief Direct access to an internal HDC instance
  /// @return HDC instance, held internally
  ///
  virtual ESGUI_HDC hdcGet() ESE_NOTHROW = 0;

  /// @brief Wrapper around low-level driver screen rotation routine
  /// @param [in] rotation One of the screen rotation codes @see ESGUI_SCREEN_ROTATION
  /// @return Code of previous screen rotation
  ///
  virtual ESGUI_SCREEN_ROTATION screenRotationSet(ESGUI_SCREEN_ROTATION rotation) ESE_NOTHROW = 0;

  /// @brief Get current screen extent rectangle
  /// @return constant pointer to the screen rectangle. @see ESGUI_DC
  ///
  virtual const ESGUI_RECT* screenExtGet() const ESE_NOTHROW = 0;

  /// @brief Set current clipping rectangle.
  /// @param [in] r   Clipping rectangle to set. r may be NULL, in which case
  ///                 the clipping rect is equal to screen rect 
  ///
  virtual void clipRectSet(const ESGUI_RECT* r) ESE_NOTHROW = 0;

  /// @brief return the current clipping rect
  /// @return Currently set clipping rect, or entire screen rect, if no clipping was set
  ///
  virtual const ESGUI_RECT* clipRectGet() const ESE_NOTHROW = 0;

  /// @brief reset clipping rect to the full screen
  ///
  virtual void clipRectReset() ESE_NOTHROW = 0;

  /// @brief Set position update logic
  /// @param [in] bUpdate If true, position is updated upon primitive is drawn,
  ///                     otherwise, position is not updated
  /// @return             Previous value of position update flag
  ///
  virtual bool updatePosSet(bool bUpdate) ESE_NOTHROW = 0;

  /// @brief Get current position update logic
  /// @return             Value of position update flag
  ///
  virtual bool updatePosGet() const ESE_NOTHROW = 0;

  /// @brief Set text transparency mode. NB! transparent text rendering is slower than opaque one.
  /// @param [in] bOpaque If true, text is drawn opaque, using background color,
  ///                     otherwise, only char pixels are drawn and background is preserved
  /// @return             Previous value of text opacity flag
  /// 
  virtual bool opaqueTextSet(bool bOpaque) ESE_NOTHROW = 0;

  /// @brief Query text opacity mode
  /// @return Value of text opacity flag
  ///
  virtual bool opaqueTextGet() const ESE_NOTHROW = 0;

  /// @brief Set foreground color.
  /// @param [in] color New foreground color
  /// @return Old foreground color
  /// 
  virtual ESGUI_COLOR fgColorSet(ESGUI_COLOR color) ESE_NOTHROW = 0;

  /// @brief Get foreground color
  /// @return Current foreground color
  /// 
  virtual ESGUI_COLOR fgColorGet() const ESE_NOTHROW = 0;

  /// @brief Get raw (in screen format) foreground color
  /// @return Current raw foreground color
  /// 
  virtual ESGUI_Color_t fgColorGetRaw() const ESE_NOTHROW = 0;

  /// @brief Set background color.
  /// @param [in] color New background color
  /// @return Old background color
  /// 
  virtual ESGUI_COLOR bgColorSet(ESGUI_COLOR color) ESE_NOTHROW = 0;

  /// @brief Get background color
  /// @return Current background color
  /// 
  virtual ESGUI_COLOR bgColorGet() const ESE_NOTHROW = 0;

  /// @brief Get raw (in screen format) background color
  /// @return Current raw background color
  /// 
  virtual ESGUI_Color_t bgColorGetRaw() const ESE_NOTHROW = 0;

  /// @brief Get currently set palette
  /// @param [out] pal Currently set palette is copied
  ///
  virtual const ESGUI_PALETTE* paletteGet() const ESE_NOTHROW = 0;
  virtual void paletteSet(const ESGUI_PALETTE* pal) ESE_NOTHROW = 0;

  /// @brief Reset palette to the system one [bgnd, fg], which is always realized
  virtual void sysPaletteSet() ESE_NOTHROW = 0;

  /// Realize palette into native driver colours
  virtual void realizePaletteFromPalette(ESGUI_PALETTE* dest, const ESGUI_PALETTE* src) ESE_NOTHROW = 0;
  virtual void realizePalette(ESGUI_PALETTE* pal, const ESGUI_COLOR* cols, esU32 colCnt) ESE_NOTHROW = 0;

  /// Unrealize palette - convert native driver colours palette to the generic ones
  virtual void unrealizePalette(ESGUI_COLOR* cols, esU32 colCnt, const ESGUI_PALETTE* pal) ESE_NOTHROW = 0;

#if defined(ESGUI_USE_FONT) && 1 == ESGUI_USE_FONT

  /// Set current font
  virtual void fontSet(const ESGUI_FONT* fnt) ESE_NOTHROW = 0;

  /// Get current font
  virtual const ESGUI_FONT* fontGet() const ESE_NOTHROW = 0;

#endif

  /// Draw horizontal line, starting from org, with length  len
  virtual void hline(const ESGUI_POINT* org, int len) ESE_NOTHROW = 0;
  virtual void hline(int x, int y, int len) ESE_NOTHROW = 0;
  
  /// Draw vertical line, starting from org, with lenght len
  virtual void vline(const ESGUI_POINT* org, int len) ESE_NOTHROW = 0;
  virtual void vline(int x, int y, int len) ESE_NOTHROW = 0;

  /// Move pen position
  virtual void penMove(const ESGUI_POINT* pos) ESE_NOTHROW = 0;
  virtual void penMove(int x, int y) ESE_NOTHROW = 0;

  /// Draw pixel point at specified position
  virtual void point(const ESGUI_POINT* pos) ESE_NOTHROW = 0;
  virtual void point(int x, int y) ESE_NOTHROW = 0;

  /// Draw rectangle. do not update current position in any way
  virtual void rectDraw(const ESGUI_RECT* r) ESE_NOTHROW = 0;
  virtual void rectDraw(const ESGUI_POINT* org, const ESGUI_POINT* edge) ESE_NOTHROW = 0;
  virtual void rectDraw(int x0, int y0, int x1, int y1) ESE_NOTHROW = 0;

  /// Fill rect with background color, do not update current position in any way
  virtual void rectFill(const ESGUI_RECT* r) ESE_NOTHROW = 0;
  virtual void rectFill(const ESGUI_POINT* org, const ESGUI_POINT* edge) ESE_NOTHROW = 0;
  virtual void rectFill(int x0, int y0, int x1, int y1) ESE_NOTHROW = 0;

  /// Clear canvas with background color
  virtual void clear() ESE_NOTHROW = 0;

  /// Draw an arbitrary line
  virtual void line(const ESGUI_POINT* beg, const ESGUI_POINT* end) ESE_NOTHROW = 0;
  virtual void line(int x0, int y0, int x1, int y1) ESE_NOTHROW = 0;

  /// Draw an arbitrary line from the current position
  virtual void lineTo(const ESGUI_POINT* end) ESE_NOTHROW = 0;
  virtual void lineTo(int x, int y) ESE_NOTHROW = 0;

  /// Draw properly clipped circle centered at center, with radius r
  virtual void circle(const ESGUI_POINT* center, int r) ESE_NOTHROW = 0;  
  
  /// Draw circle centered in rect, with radius equal to minimal rect extent, 
  /// height or width
  ///
  virtual void circle(const ESGUI_RECT* r) ESE_NOTHROW = 0;

#if defined(ESGUI_USE_FONT) && 1 == ESGUI_USE_FONT

  /// Font block management
  ///
  
  /// Initialize font structure block
  virtual void fontInit(ESGUI_FONT* font, const ESGUI_FONT_DATA* block) const ESE_NOTHROW = 0;

  /// Append font sub-block to the existing font block chain
  virtual void fontBlockAdd(ESGUI_FONT* font, ESGUI_FONT* block) const ESE_NOTHROW = 0;

  /// Remove font block from the chain
  virtual void fontBlockRemove(ESGUI_FONT* block) const ESE_NOTHROW = 0;

  /// String drawing
  ///

  /// Return font height. If NULL, return height of the currently set canvas font
  virtual int fontHeightGet(const ESGUI_FONT* fnt = NULL) const ESE_NOTHROW = 0;

  /// Return string extent, based on ESGUI_FONT data.
  virtual void stringExtentN(const ESGUI_FONT* fnt, ESE_CSTR text, int strlen, int tabWidth, ESGUI_POINT* ext ) ESE_NOTHROW = 0;
  virtual void stringExtent(const ESGUI_FONT* fnt, ESE_CSTR text, int tabWidth, ESGUI_POINT* ext ) ESE_NOTHROW = 0;

  /// Return string extent when text wrapping is enabled
  virtual void stringExtentWrapN(const ESGUI_FONT* fnt, ESE_CSTR text, int strlen, int tabWidth, int width, ESGUI_POINT* ext ) ESE_NOTHROW = 0;

  /// Return string extent based on ESGUI_FONT data, when optional string wrapping
  /// is enabled.
  ///
  virtual void stringExtentWrap(const ESGUI_FONT* fnt, ESE_CSTR text, int tabWidth, int width, ESGUI_POINT* ext ) ESE_NOTHROW = 0;

  typedef void (*CharDrawCallbackT)(EsguiCanvasIntf& canvas, ESGUI_CharCallbackReason reason, ESE_CSTR pos, const ESGUI_RECT* rect, void* data);

  /// Draw string of chars
  virtual void stringDrawN(const ESGUI_POINT* pos, ESE_CSTR pStr, int strlen, int tabWidth = ESGUI_TAB_WIDTH_STD, EsguiCanvasIntf::CharDrawCallbackT cbk = NULL, void* data = NULL) ESE_NOTHROW = 0;
  virtual void stringDrawN(int x, int y, ESE_CSTR pStr, int strlen, int tabWidth = ESGUI_TAB_WIDTH_STD, EsguiCanvasIntf::CharDrawCallbackT cbk = NULL, void* data = NULL) ESE_NOTHROW = 0;
  virtual void stringDraw(const ESGUI_POINT* pos, ESE_CSTR pStr, int tabWidth = ESGUI_TAB_WIDTH_STD, EsguiCanvasIntf::CharDrawCallbackT cbk = NULL, void* data = NULL) ESE_NOTHROW = 0;
  virtual void stringDraw(int x, int y, ESE_CSTR pStr, int tabWidth = ESGUI_TAB_WIDTH_STD, EsguiCanvasIntf::CharDrawCallbackT cbk = NULL, void* data = NULL) ESE_NOTHROW = 0;

  /// Draw string of chars, fitting it into the specified rectangle
  /// using text alignment & positioning parameters
  ///
  virtual void stringDrawRect(const ESGUI_RECT* rect, ESE_CSTR pStr, int tabWidth, esU32 flags) ESE_NOTHROW = 0;

#endif // ESGUI_USE_FONT

#if defined(ESGUI_USE_BITMAP) && 1 == ESGUI_USE_BITMAP

  /// Return bitmap height, width, and extent
  virtual int pictureWidthGet(const ESGUI_BITMAP* bmp) const ESE_NOTHROW = 0;
  virtual int pictureHeightGet(const ESGUI_BITMAP* bmp) const ESE_NOTHROW = 0;
  virtual ESGUI_POINT pictureExtGet(const ESGUI_BITMAP* bmp) const ESE_NOTHROW = 0;

  /// Draw picture using ROP. optional view rect specifies which portion of bitmap should be drawn.
  virtual void pictureDrawROP(int x, int y, const ESGUI_BITMAP* bmp, const ESGUI_RECT* view = NULL, ESGUI_BITOP nRop = ESGUI_COPY) ESE_NOTHROW = 0;
  virtual void pictureDrawROP(const ESGUI_POINT* pos, const ESGUI_BITMAP* bmp, const ESGUI_RECT* view = NULL, ESGUI_BITOP nRop = ESGUI_COPY) ESE_NOTHROW = 0;
#endif // ESGUI_USE_BITMAP

};

#endif //< _esgui_canvas_intf_h_
