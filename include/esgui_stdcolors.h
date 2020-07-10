#ifndef _esgui_std_colors_h_
#define _esgui_std_colors_h_

#ifdef __cplusplus
  extern "C" {
#endif

# if (ESGUI_USE_STD_COLORS & ESGUI_USE_STD_COLORS_MONO)
extern const ESGUI_COLOR c_clBlack;
extern const ESGUI_COLOR c_clWhite;
#endif

# if (ESGUI_USE_STD_COLORS & ESGUI_USE_STD_COLORS_BASIC_MIN)
extern const ESGUI_COLOR c_clDarkGrey;
extern const ESGUI_COLOR c_clLightGrey;
extern const ESGUI_COLOR c_clDarkRed;
extern const ESGUI_COLOR c_clLightRed;
extern const ESGUI_COLOR c_clDarkGreen;
extern const ESGUI_COLOR c_clLightGreen;
extern const ESGUI_COLOR c_clDarkBlue;
extern const ESGUI_COLOR c_clLightBlue;
extern const ESGUI_COLOR c_clDarkYellow;
extern const ESGUI_COLOR c_clLightYellow;
extern const ESGUI_COLOR c_clMagenta;
#endif

#ifdef __cplusplus
  };
#endif

#endif // _esgui_std_colors_h_
