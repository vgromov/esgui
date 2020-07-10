#ifndef _esgui_widget_msgbox_h_
#define _esgui_widget_msgbox_h_

#if defined(ESGUI_WIDGET_USE_MSGBOX) && 1 == ESGUI_WIDGET_USE_MSGBOX

/// Standard message view widget
///

#ifdef __cplusplus
  extern "C" {
#endif

/// Show message box with possible user YES|NO confirmation and|or additional event handling callback
///
typedef enum {
  ESGUI_MBR_NONE,
  ESGUI_MBR_OK,
  ESGUI_MBR_CANCEL

} ESGUI_WgtMsgBoxResult;

typedef enum {
  /// Msgbox styles,
  ESGUI_MBT_INFO        = 0x0100,
  ESGUI_MBT_QUESTION    = 0x0200,
  ESGUI_MBT_WARNING     = 0x0300,
  ESGUI_MBT_ERROR       = 0x0400,
  ESGUI_MBT_STYLE_MASK  = 0xFF00,
  
  /// Button bits
  ESGUI_MBT_OK          = 0x0001,
  ESGUI_MBT_CANCEL      = 0x0002,
  ESGUI_MBT_YES         = 0x0004,
  ESGUI_MBT_NO          = 0x0008,
  ESGUI_MBT_BTN_MASK    = 0x00FF
 
} ESGUI_WgtMsgBoxType;

/// User-defined message box event handler callback. if this callback is set and its call return something other than
/// ESGUI_MBR_NONE, then modal message box will exit and return this value as result
///
typedef esU32 (*ESGUI_WgtMsgBoxOnEvent)(ESGUI_WIDGET*, EsguiAppEventT*);

/// Msgbox data
typedef struct tagESGUI_WgtMsgBoxData
{
  ESE_CSTR title;
  ESE_CSTR text;
  esU32 type;
  esU32 result;
  ESGUI_WgtMsgBoxOnEvent handler;
  bool (*canCreate)(void);

} ESGUI_WgtMsgBoxData;

enum {
  ESGUI_WgtMsgBoxData_SZE = sizeof(ESGUI_WgtMsgBoxData)  
};

/// Basic initializer of msg box data
void esguiWgtMsgBoxDataInit(ESGUI_WgtMsgBoxData* data, ESE_CSTR title, ESE_CSTR text, esU32 type);

/// Standard msgbox event handler. gets used automatically if customHandler is set to 0 in call to esguiWgtMsgBox
esU32 esguiWgtMsgBoxStdEventHandler(ESGUI_WIDGET* This, EsguiAppEventT* evt);

/// Standard msgbox can-create checker
bool esguiWgtMsgBoxStdCanCreate(void);

/// Message box itself. if out parameter is not 0, it may be used to return 
/// message box instance to the world (might be useful when implementing singleton msg boxes)
///
esU32 esguiWgtMsgBox(ESGUI_WIDGET_ROOT* root, ESE_CSTR title, ESE_CSTR text, const ESGUI_RECT* rect, esU32 type, int tag, ESGUI_WIDGET** out);

/// Allow creating fully customized message box using pre-created msgbox data structure derived from 
/// ESGUI_WgtMsgBoxData base structure
///
esU32 esguiWgtMsgBoxIndirect(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* rect, int tag, ESGUI_WgtMsgBoxData* data, bool ownsData, ESGUI_WIDGET** out);

/// Try to cancel message box programmatically
///
void esguiWgtMsgBoxCancel(ESGUI_WIDGET* msgbox);

/// Services to be implemented in application
///
void esguiWgtMsgBoxAppEventsProcess(void);
bool esguiWgtMsgBoxStdCanCreate(void);
bool esguiWgtEventIsMsgBoxOk(ESGUI_WIDGET* This, EsguiAppEventT* evt);
bool esguiWgtEventIsMsgBoxCancel(ESGUI_WIDGET* This, EsguiAppEventT* evt);
void esguiWgtMsgBoxBeforeShow(void);
void esguiWgtMsgBoxAfterShow(void);
void esguiWgtMsgBoxBeforeClose(void);
void esguiWgtMsgBoxAfterClose(void);

#ifdef __cplusplus
  }
#endif

#endif //< ESGUI_WIDGET_USE_MSGBOX

#endif //< _esgui_widget_msgbox_h_
