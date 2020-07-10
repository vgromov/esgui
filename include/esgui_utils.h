#ifndef _es_gui_utils_h_
#define _es_gui_utils_h_

#ifdef __cplusplus
  extern "C" {
#endif

// miscellaneous utilities
//
// min & max macros
#define esguiUtils_MAX(a,b)  (((a) < (b)) ? (b) : (a))
#define esguiUtils_MIN(a,b)  (((a) < (b)) ? (a) : (b))

#if defined(ESGUI_USE_COLOR) && 1 == ESGUI_USE_COLOR

// color creation from RGB components
ESGUI_COLOR ESGUI_RGB(esU8 r, esU8 g, esU8 b);

#endif

// swap two integer values
void esguiUtilsSwapInt(int* a, int* b);
// swap odd and even bytes in buffer. !no check is made against 2^n buffer length!
void esguiUtilsSwapB(esU8* pb, int count);

#ifdef __cplusplus
  }
#endif

#endif //  _es_gui_utils_h_
