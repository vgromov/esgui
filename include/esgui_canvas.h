#ifndef _es_gui_canvas_h_
#define _es_gui_canvas_h_

#ifdef __cplusplus
  extern "C" {
#endif

/// ESGUI output control flags
///
enum {
  guiCtlUpdatePos     = 1,  ///< Should the current position be updated during drawing operations
  guiCtlOpaqueText    = 2   ///< If set, text is opaque, background is filled with colorBg, otherwise, background is preserved
};

/// Graphics device context
///
ESE_ANON_UNION
typedef struct tagESGUI_DC {
  ESGUI_HDRV driver;        ///> Screen driver handle
  ESGUI_POINT pos;          ///> Current GUI pen position
  ESGUI_COLOR colorFg;      ///> Current foreground color
  ESGUI_COLOR colorBg;      ///> Current background color
  ESGUI_PALETTE pal;        ///> Currently set palette

  ESGUI_Color_t colorFgRaw; ///> Current foreground color
  ESGUI_Color_t colorBgRaw; ///> Current background color
  union {                   ///> System palette data (for internal use only)
    esU32 sysPal32[2];
    esU16 sysPal16[2];
    esU8  sysPal8[2];
  };

#if 1 == ESGUI_USE_FONT
  const ESGUI_FONT* font;   ///> Selected GUI font
#endif

  ESGUI_RECT ext;           ///> Current screen extent
  ESGUI_RECT clip;          ///> Current clipping rect
  ESGUI_RECT internalClip;  ///> Current internal clipping rect
  esU16 ctl;                ///> GUI control flags

} ESGUI_DC;

/// Graphics device context handle
/// 
typedef ESGUI_DC* ESGUI_HDC;

/// GUI device context services
///

/// @brief HDC context initializaton.
/// @param [in] hdc Handle of device context being initialized 
/// @param [in] drv Graphics driver handle to use for device context initialization
/// @return TRUE if initialization was successfull, FALSE otherwise
///
esBL esguiCanvasInit(ESGUI_HDC hdc, ESGUI_HDRV drv);

/// @brief Attempt to lock HDC driver bus.
/// Internally tries to call bus lock on attached driver bus
/// @param [in] hdc Handle of device context being initialized 
/// @return TRUE if lock was acquired, false otherwise
///
esBL esguiCanvasLock(ESGUI_HDC hdc);

/// @brief Unlock HDC driver bus.
void esguiCanvasUnlock(ESGUI_HDC hdc);

/// @brief Wrapper around low-level driver screen rotation routine
/// @param [in] hdc      Device context handle
/// @param [in] rotation One of the screen rotation codes @see ESGUI_SCREEN_ROTATION
/// @return Code of previous screen rotation
///
ESGUI_SCREEN_ROTATION esguiCanvasSetScreenRotation(ESGUI_HDC hdc, ESGUI_SCREEN_ROTATION rotation);

/// @brief Get current screen extent rectangle
/// @param [in] hdc Device context whose extension is being requested
/// @return constant pointer to the screen rectangle. @see ESGUI_DC
///
const ESGUI_RECT* esguiCanvasGetScreenExt(const ESGUI_HDC hdc);

/// @brief Set current clipping rectangle.
/// @param [in] hdc Device context handle
/// @param [in] r   Clipping rectangle to set. r may be NULL, in which case
///                 the clipping rect is equal to screen rect 
///
void esguiCanvasSetClipRect(ESGUI_HDC hdc, const ESGUI_RECT* r);

/// @brief return the current clipping rect
/// @param [in] hdc Device context handle
/// @return Currently set clipping rect, or entire screen rect, if no clipping was set
///
const ESGUI_RECT* esguiCanvasGetClipRect(const ESGUI_HDC hdc);

/// @brief reset clipping rect to the full screen
/// @param [in] hdc Device context handle
///
void esguiCanvasResetClipRect(ESGUI_HDC hdc);

/// @brief Set position update logic
/// @param [in] hdc     Device context handle
/// @param [in] bUpdate If TRUE, position is updated upon primitive is drawn,
///                     otherwise, position is not updated
/// @return             Previous value of position update flag
///
esBL esguiCanvasSetUpdatePos(ESGUI_HDC hdc, esBL bUpdate);

/// @brief Get current position update logic
/// @param [in] hdc     Device context handle
/// @return             Value of position update flag
///
esBL esguiCanvasGetUpdatePos(const ESGUI_HDC hdc);

/// @brief Set text transparency mode. NB! transparent text rendering is slower than opaque one.
/// @param [in] hdc     Device context handle
/// @param [in] bOpaque If TRUE, text is drawn opaque, using background color,
///                     otherwise, only char pixels are drawn and background is preserved
/// @return             Previous value of text opacity flag
/// 
esBL esguiCanvasSetOpaqueText(ESGUI_HDC hdc, esBL bOpaque);

/// @brief Query text opacity mode
/// @param [in] hdc Device context handle
/// @return Value of text opacity flag
///
esBL esguiCanvasGetOpaqueText(const ESGUI_HDC hdc);

/// @brief Set foreground color.
/// @param [in] hdc   Device context handle
/// @param [in] color New foreground color
/// @return Old foreground color
/// 
ESGUI_COLOR esguiCanvasFgColorSet(ESGUI_HDC hdc, ESGUI_COLOR color);

/// @brief Get foreground color
/// @param [in] hdc Device context handle
/// @return Current foreground color
/// 
ESGUI_COLOR esguiCanvasFgColorGet(const ESGUI_HDC hdc);

/// @brief Get raw (in screen format) foreground color
/// @param [in] hdc Device context handle
/// @return Current raw foreground color
/// 
ESGUI_Color_t esguiCanvasFgColorGetRaw(const ESGUI_HDC hdc);

/// @brief Set background color.
/// @param [in] hdc   Device context handle
/// @param [in] color New background color
/// @return Old background color
/// 
ESGUI_COLOR esguiCanvasBgColorSet(ESGUI_HDC, ESGUI_COLOR color);

/// @brief Get background color
/// @param [in] hdc Device context handle
/// @return Current background color
/// 
ESGUI_COLOR esguiCanvasBgColorGet(const ESGUI_HDC hdc);

/// @brief Get raw (in screen format) background color
/// @param [in] hdc Device context handle
/// @return Current raw background color
/// 
ESGUI_Color_t esguiCanvasBgColorGetRaw(const ESGUI_HDC hdc);

/// @brief Get currently set palette
/// @param [in] hdc  Device context handle
/// @param [out] pal Currently set palette is copied
///
const ESGUI_PALETTE* esguiCanvasPaletteGet(const ESGUI_HDC hdc);
void esguiCanvasPaletteSet(ESGUI_HDC hdc, const ESGUI_PALETTE* pal);

/// @brief Reset palette to the system one [bgnd, fg], which is always realized
void esguiCanvasSysPaletteSet(ESGUI_HDC hdc);

// realize palette into native driver colours
void esguiCanvasRealizePaletteFromPalette(ESGUI_HDC hdc, ESGUI_PALETTE* dest, const ESGUI_PALETTE* src);
void esguiCanvasRealizePalette(ESGUI_HDC hdc, ESGUI_PALETTE* pal, const ESGUI_COLOR* cols, esU32 colCnt);

// unrealize palette - convert native driver colours palette to the generic ones
void esguiCanvasUnrealizePalette(ESGUI_HDC hdc, ESGUI_COLOR* cols, esU32 colCnt, const ESGUI_PALETTE* pal);

#if 1 == ESGUI_USE_FONT

// set current font
void esguiCanvasFontSet(ESGUI_HDC hdc, const ESGUI_FONT* fnt);

// get current font
const ESGUI_FONT* esguiCanvasFontGet(const ESGUI_HDC hdc);

#endif

// draw horizontal line, starting from org, with length  len
void esguiCanvasDrawHLine(ESGUI_HDC hdc, const ESGUI_POINT* org, int len);
void esguiCanvasDrawHLine_(ESGUI_HDC hdc, int x, int y, int len);

// draw vertical line, starting from org, with lenght len
void esguiCanvasDrawVLine(ESGUI_HDC hdc, const ESGUI_POINT* org, int len);
void esguiCanvasDrawVLine_(ESGUI_HDC hdc, int x, int y, int len);

// move pen
void esguiCanvasMovePen(ESGUI_HDC hdc, const ESGUI_POINT* pos);
void esguiCanvasMovePen_(ESGUI_HDC hdc, int x, int y);

// draw point at specified position
void esguiCanvasDrawPoint(ESGUI_HDC hdc, const ESGUI_POINT* pos);
void esguiCanvasDrawPoint_(ESGUI_HDC hdc, int x, int y);

// draw rectangle. do not update current position in any way
void esguiCanvasDrawRect(ESGUI_HDC hdc, const ESGUI_RECT* r);

// fill rect with background color, do not update current position in any way
void esguiCanvasFillRect(ESGUI_HDC hdc, const ESGUI_RECT* r);

// clear lcd screen with background color
void esguiCanvasClearScreen(ESGUI_HDC hdc);

// draw arbitrary line
void esguiCanvasDrawLine(ESGUI_HDC hdc, const ESGUI_POINT* beg, const ESGUI_POINT* end);
void esguiCanvasDrawLine_(ESGUI_HDC hdc, int x0, int y0, int x1, int y1);

// draw arbitrary line from the current position
void esguiCanvasLineTo(ESGUI_HDC hdc, const ESGUI_POINT* end);
void esguiCanvasLineTo_(ESGUI_HDC hdc, int x, int y);

// draw properly clipped circle centered at center, with radius r
void esguiCanvasDrawCircle(ESGUI_HDC hdc, const ESGUI_POINT* center, int r);

#if defined(ESGUI_USE_FONT) && 1 == ESGUI_USE_FONT

// return string extent, based on ESGUI_FONT data.
void esguiCanvasGetStringExtentN(ESGUI_HDC hdc, const ESGUI_FONT* fnt, ESE_CSTR text, int strlen, int tabWidth, ESGUI_POINT* ext );
void esguiCanvasGetStringExtent(ESGUI_HDC hdc, const ESGUI_FONT* fnt, ESE_CSTR text, int tabWidth, ESGUI_POINT* ext );

// minimum legal width restriction value. if width parameter specified in
// "StringExtent" functions below is not 0 and is less than MIN_WRAP_WIDTH,
// it's got reset to the latter.
#define MIN_WRAP_WIDTH 80

// return string extent when text wrapping is enabled
void esguiCanvasGetStringExtentWrapN(ESGUI_HDC hdc, const ESGUI_FONT* fnt, ESE_CSTR text, int strlen, int tabWidth, int width, ESGUI_POINT* ext );

// return string extent based on ESGUI_FONT data, when optional string wrapping
// is enabled.
void esguiCanvasGetStringExtentWrap(ESGUI_HDC hdc, const ESGUI_FONT* fnt, ESE_CSTR text, int tabWidth, int width, ESGUI_POINT* ext );

// draw string of chars using per-char callback
typedef enum {
  ESGUI_BeforeDrawChar,
  ESGUI_AfterDrawChar
  
} ESGUI_CharCallbackReason;

typedef void (*ESGUI_WgtCharCallback)(ESGUI_HDC hdc, ESGUI_CharCallbackReason reason, ESE_CSTR pos, const ESGUI_RECT* rect, void* data);
  
void esguiCanvasDrawStringWithCallbackN_(ESGUI_HDC hdc, int x, int y, ESE_CSTR pStr, int strlen, int tabWidth, ESGUI_WgtCharCallback cbk, void* Data);
void esguiCanvasDrawStringWithCallbackN(ESGUI_HDC hdc, const ESGUI_POINT* pos, ESE_CSTR pStr, int strlen, int tabWidth, ESGUI_WgtCharCallback cbk, void* Data);
  
void esguiCanvasDrawStringWithCallback(ESGUI_HDC hdc, const ESGUI_POINT* pos, ESE_CSTR pStr, int tabWidth, ESGUI_WgtCharCallback cbk, void* Data);

// draw string of chars
void esguiCanvasDrawStringN(ESGUI_HDC hdc, const ESGUI_POINT* pos, ESE_CSTR pStr, int strlen, int tabWidth);
void esguiCanvasDrawStringN_(ESGUI_HDC hdc, int x, int y, ESE_CSTR pStr, int strlen, int tabWidth);
void esguiCanvasDrawString(ESGUI_HDC hdc, const ESGUI_POINT* pos, ESE_CSTR pStr, int tabWidth);
void esguiCanvasDrawString_(ESGUI_HDC hdc, int x, int y, ESE_CSTR pStr, int tabWidth);

// draw string of chars, fitting it into the specified rectangle
// using text alignment & positioning parameters
void esguiCanvasDrawStringRect(ESGUI_HDC hdc, const ESGUI_RECT* rect, ESE_CSTR pStr, int tabWidth, esU32 flags);

#endif // ESGUI_USE_FONT

#if defined(ESGUI_USE_BITMAP) && 1 == ESGUI_USE_BITMAP

// Draw picture using ROP. optional view rect specifies which portion of bitmap should be drawn.
void esguiCanvasDrawPictureROP_(ESGUI_HDC hdc, int x, int y, const ESGUI_BITMAP* bmp, const ESGUI_RECT* view, ESGUI_BITOP nRop);
void esguiCanvasDrawPictureROP(ESGUI_HDC hdc, const ESGUI_POINT* pos, const ESGUI_BITMAP* bmp, const ESGUI_RECT* view, ESGUI_BITOP nRop);

// Draw picture using its iterator, initialized elsewhere, and ROP
void esguiCanvasDrawPictureItROP_(ESGUI_HDC hdc, int x, int y, ESGUI_BITMAP_ITERATOR* it, ESGUI_BITOP nRop);
void esguiCanvasDrawPictureItROP(ESGUI_HDC hdc, const ESGUI_POINT* pos, ESGUI_BITMAP_ITERATOR* it, ESGUI_BITOP nRop);

#endif // ESGUI_USE_BITMAP

#ifdef __cplusplus
  }
#endif

#endif // _es_gui_canvas_h_
