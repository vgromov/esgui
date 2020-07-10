#ifndef _esgui_widget_std_h_
#define _esgui_widget_std_h_

#if defined(ESGUI_WIDGET_USE_STD) && 1 == ESGUI_WIDGET_USE_STD

/// Common standard widgets functionality
///

#ifdef __cplusplus
  extern "C" {
#endif

/// Title painter
void esguiWgtStdPaintTitle(ESGUI_HDC hdc, const ESGUI_RECT* rect, const ESGUI_BITMAP* bmp, ESE_CSTR title);

/// Paint vertical scroll bar
void esguiWgtStdPaintVscrollBar(ESGUI_HDC hdc, const ESGUI_RECT* client, int min, int max, int pos);

/// Paint horizontal scroll bar
void esguiWgtStdPaintHscrollBar(ESGUI_HDC hdc, const ESGUI_RECT* client, int min, int max, int pos);

// widget common scrollbar painting handler. vertical scrollbar is placed at the right side next to draw rect
// horizontal should go at the bottom of the rect specified
void esguiWgtStdPaintScrollbar(ESGUI_HDC hdc, const ESGUI_WIDGET* This, const ESGUI_RECT* draw, bool vertical);

/// Widget common scrollbar painting handler. vertical scrollbar is placed at the right side next to draw rect
/// horizontal should go at the bottom of the rect specified
///
void esguiWgtStdPaintScrollbar(ESGUI_HDC hdc, const ESGUI_WIDGET* This, const ESGUI_RECT* draw, bool vertical);

/// Standard bg painter. just erases widget rect, filling it with bgnd
void esguiWgtStdPaintBg(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect);

/// Draw gui string with smart erase
void esguiWgtStdPaintStringWithSmartErase(ESGUI_HDC hdc, ESE_CSTR str, const ESGUI_POINT* pos, int* prevStrLen);

/// Draw gui string with smart erase
void esguiWgtStdPaintCenteredStringWithSmartErase(ESGUI_HDC hdc, ESE_CSTR str, const ESGUI_POINT* org, int areaWidth, int* prevStrLen);

/// Draw labeled value starting at pos. value string is started at offs from pos
void esguiWgtStdPaintLabeledVal(ESGUI_HDC hdc, const ESGUI_POINT* pos, int offs, ESE_CSTR lbl, ESE_CSTR val);

/// Draw labeled value string with units, using custom fonts for label, value, and units. smart erase is used for value string
void esguiWgtStdPaintLabeledValWithSmartErase(
  ESGUI_HDC hdc, ESGUI_POINT* pos, int lblRulerOffs, int unitRulerOffs,
  int* prevLenContainer, int* prevUnitLenContainer, ESGUI_COLOR fgnd, ESGUI_COLOR bgnd, ESE_CSTR lbl, const ESGUI_FONT* lblFont,
  ESE_CSTR valStr, const ESGUI_FONT* valFont, ESE_CSTR unit, const ESGUI_FONT* unitFont
);

/// Generic editor draw char callback
void esguiWgtStdPaintEditorChar(ESGUI_HDC hdc, ESGUI_CharCallbackReason reason, ESE_CSTR pos, const ESGUI_RECT* rect);

/// Generic button painter
void esguiWgtStdButtonPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect);

/// Standard widget metrics
///
typedef enum {
  esguiWgtmStdMinLabelOffs = esguiWgtMetricCount,       ///< Minimum space for label before editor data string
  
  esguiWgtStdMetricCount                                ///< Special value. Must be the last one
  
} ESGUI_WIDGET_STD_METRIC;

/// Standard resource IDs
///
enum {
  /// Standard fonts
  ESGUI_WgtStdFontTitle,
  ESGUI_WgtStdFontText,
  ESGUI_WgtStdFontCtl,
  
  /// Standard images  
  ESGUI_WgtStdBitmapCheckbox,
  ESGUI_WgtStdBitmapInfo,
  ESGUI_WgtStdBitmapQuestion,
  ESGUI_WgtStdBitmapWarning,
  ESGUI_WgtStdBitmapError,
  ESGUI_WgtStdBitmapArrowUp,
  ESGUI_WgtStdBitmapArrowDown,
  ESGUI_WgtStdBitmapArrowLeft,
  ESGUI_WgtStdBitmapArrowRight,
  
  /// Standard colors
  ESGUI_WgtStdColorFg,
  ESGUI_WgtStdColorFgDisabled,
  ESGUI_WgtStdColorBg,
  
  /// Standard strings
  ESGUI_WgtStdStringTitleInfo,
  ESGUI_WgtStdStringTitleQuestion,
  ESGUI_WgtStdStringTitleWarning,
  ESGUI_WgtStdStringTitleError
};

/// Services to be implemented in an application:
///

/// Standard font accessor
const ESGUI_FONT* esguiWgtStdFontGet(const ESGUI_HDC hdc, int resourceId);

/// Standard resource accessor
const ESGUI_BITMAP* esguiWgtStdBitmapGet(const ESGUI_HDC hdc, int resourceId);

/// Standard color accessor
ESGUI_COLOR esguiWgtStdColorGet(const ESGUI_HDC hdc, int resourceId);

/// Standard strings accessor
ESE_CSTR esguiWgtStdStringGet(int resourceId);


/// Services to be implemented in application
///
bool esguiWgtEventIsCtlDec(EsguiAppEventT* evt);
bool esguiWgtEventIsCtlDecBig(EsguiAppEventT* evt);
bool esguiWgtEventIsCtlInc(EsguiAppEventT* evt);
bool esguiWgtEventIsCtlIncBig(EsguiAppEventT* evt);
bool esguiWgtEventIsCtlCommit(EsguiAppEventT* evt);
bool esguiWgtEventIsCtlEnter(EsguiAppEventT* evt);
bool esguiWgtEventIsCtlExit(EsguiAppEventT* evt);

#ifdef __cplusplus
  }
#endif

#endif //< ESGUI_WIDGET_USE_STD

#endif //< _esgui_widget_std_h_
