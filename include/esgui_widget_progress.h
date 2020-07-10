#ifndef _esgui_widget_progress_h_
#define _esgui_widget_progress_h_

#if defined(ESGUI_WIDGET_USE_PROGRESS) && 1 == ESGUI_WIDGET_USE_PROGRESS

/// Standard progress display widget
///

#ifdef __cplusplus
  extern "C" {
#endif

/// Standard progress gauge widget
///
typedef struct tagESGUI_WgtProgressData
{
  const ESGUI_FONT* font;
  int min;
  int max;
  int pos;
  void (*onDisplayProgressText)(struct tagESGUI_WgtProgressData*, ESE_STR, size_t);
  // used internally, to avoid excess progress redraw
  int prevVisiblePos;
  int visibleDelta;

} ESGUI_WgtProgressData;

enum {
  ESGUI_WgtProgressData_SZE = sizeof(ESGUI_WgtProgressData)
};

typedef void (*ESGUI_WgtOnDisplayProgressText)(ESGUI_WgtProgressData*, ESE_STR, size_t);

/// Progress gauge standard event handler
void esguiWgtProgressOnEvent(ESGUI_WIDGET* This, ESGUI_WIDGET* sender, EsguiAppEventT* evt);

/// Progress gauge standard painter
void esguiWgtProgressOnPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect);

/// Progress gauge api
void esguiWgtProgressSetOnTextHandler(ESGUI_WIDGET* This, ESGUI_WgtOnDisplayProgressText onDisplayProgressText);
void esguiWgtProgressSetRange(ESGUI_WIDGET* This, int min, int max);
void esguiWgtProgressSetPos(ESGUI_WIDGET* This, int pos);

/// Progress gauge constructor
ESGUI_WIDGET* esguiWgtProgressCreateCustom(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, const ESGUI_FONT* font, ESGUI_WgtOnDisplayProgressText onDisplayProgressText, int rmin, int rmax, int pos);
ESGUI_WIDGET* esguiWgtProgressCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, const ESGUI_FONT* font, ESGUI_WgtOnDisplayProgressText onDisplayProgressText);

/// Services to be implemented in application
///
bool esguiWgtEventIsProgressSet(EsguiAppEventT* evt, int* pos);
bool esguiWgtEventIsProgressRangeSet(EsguiAppEventT* evt, int* min, int* max);

#ifdef __cplusplus
  }
#endif

#endif //< ESGUI_WIDGET_USE_PROGRESS

#endif //< _esgui_widget_progress_h_
