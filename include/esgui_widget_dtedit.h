#ifndef _esgui_widget_dtedit_h_
#define _esgui_widget_dtedit_h_

#if defined(ESGUI_WIDGET_USE_DTEDIT) && 1 == ESGUI_WIDGET_USE_DTEDIT

/// Ready-to-use date and time editors
///

#ifdef __cplusplus
  extern "C" {
#endif

/// Time editor constructor
ESGUI_WIDGET* esguiWgtTimeEditorCreate(ESGUI_WIDGET_ROOT* root, ESE_CSTR label, const esDT* dt, const ESGUI_RECT* bounds);

/// Date editor constructor
ESGUI_WIDGET* esguiWgtDateEditorCreate(ESGUI_WIDGET_ROOT* root, ESE_CSTR label, const esDT* dt, const ESGUI_RECT* bounds);

/// Services to be implemented in application
///

/// Application-specific services to get current esDT value
//esDT esguiWgtCurrentDateTimeGet(void);
//void esguiWgtCurrentDateTimeSet(const esDT* dt);

ESE_CSTR esguiWgtDteditTimeFmtStrGet(void);
ESE_CSTR esguiWgtDteditDateFmtStrGet(void);

#ifdef __cplusplus
  }
#endif

#endif //< ESGUI_WIDGET_USE_DTEDIT

#endif //< _esgui_widget_dtedit_h_
