#ifndef _esgui_widget_checkbox_h_
#define _esgui_widget_checkbox_h_

#if defined(ESGUI_WIDGET_USE_CHECKBOX) && 1 == ESGUI_WIDGET_USE_CHECKBOX

/// Standard check box widget
///

#ifdef __cplusplus
  extern "C" {
#endif

/// Standard painter
void checkBoxOnPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect);

/// Checkbox Constructor
ESGUI_WIDGET* esguiWgtCheckBoxCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, ESE_CSTR lbl, bool check);

#ifdef __cplusplus
  }
#endif

#endif //< ESGUI_WIDGET_USE_CHECKBOX

#endif // _esgui_widget_checkbox_h_
