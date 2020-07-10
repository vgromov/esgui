#ifndef _esgui_widget_spinctl_h_
#define _esgui_widget_spinctl_h_

#if defined(ESGUI_WIDGET_USE_SPINCTL) && 1 == ESGUI_WIDGET_USE_SPINCTL

/// Standard spin edit widget
///

#ifdef __cplusplus
  extern "C" {
#endif

ESE_ANON_UNION
typedef struct tagESGUI_WgtSpinCtlData {
  // editor data
  union {
    // integer spin editor
    struct {
      int min;
      int max;
      int val;
      int inc;
      int bigInc;
    };
    // float spin editor
    struct {
      float fmin;
      float fmax;
      float fval;
      float finc;
      float fbigInc;
      int decimals;
    };
  };
  // widget label
  ESE_CSTR label;
  // spin editor type
  esBL isFloat : 1;
    
} ESGUI_WgtSpinCtlData;

enum {
  esguiWgtSpinctlDefMin     = 0,
  esguiWgtSpinctlDefMax     = 100,
  esguiWgtSpinctlDefInc     = 1,
  esguiWgtSpinctlDefBigInc  = 10,
  ESGUI_WgtSpinCtlData_SZE  = sizeof(ESGUI_WgtSpinCtlData)
};

/// Spin editor widget api
///

/// Standard event handler
void esguiWgtSpinctlOnEvent(ESGUI_WIDGET* This, ESGUI_WIDGET* sender, EsguiAppEventT* evt);

/// Standard painter
void esguiWgtSpinctlOnPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect);

/// Range, inc, and value access
void esguiWgtSpinctlSetRangeInt(ESGUI_WIDGET* This, int min, int max);
void esguiWgtSpinctlSetRangeFloat(ESGUI_WIDGET* This, float min, float max);
void esguiWgtSpinctlGetRangeInt(ESGUI_WIDGET* This, int* min, int* max);
void esguiWgtSpinctlGetRangeFloat(ESGUI_WIDGET* This, float* min, float* max);
void esguiWgtSpinctlSetValueInt(ESGUI_WIDGET* This, int val);
void esguiWgtSpinctlSetValueFloat(ESGUI_WIDGET* This, float val);
int esguiWgtSpinctlGetValueInt(ESGUI_WIDGET* This);
float esguiWgtSpinctlGetValueFloat(ESGUI_WIDGET* This);
void esguiWgtSpinctlSetIncInt(ESGUI_WIDGET* This, int inc);
void esguiWgtSpinctlSetIncFloat(ESGUI_WIDGET* This, float inc);
int esguiWgtSpinctlGetIncInt(ESGUI_WIDGET* This);
float esguiWgtSpinctlGetIncFloat(ESGUI_WIDGET* This);
void esguiWgtSpinctlSetBigIncInt(ESGUI_WIDGET* This, int bigInc);
void esguiWgtSpinctlSetBigIncFloat(ESGUI_WIDGET* This, float bigInc);
int esguiWgtSpinctlGetBigIncInt(ESGUI_WIDGET* This);
float esguiWgtSpinctlGetBigIncFloat(ESGUI_WIDGET* This);

/// Spin control widget constructors.
ESGUI_WIDGET* esguiWgtSpinctlCreateInt(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, ESE_CSTR lbl,  int min, int max);
ESGUI_WIDGET* esguiWgtSpinctlCreateFloat(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, ESE_CSTR lbl, float min, float max, int decimals);

#ifdef __cplusplus
  }
#endif

#endif //< ESGUI_WIDGET_USE_SPINCTL

#endif //< _esgui_widget_spinctl_h_
