#ifndef _es_gui_types_h_
#define _es_gui_types_h_

#ifdef __cplusplus
  extern "C" {
#endif

/// Common gui data types and defines
///

/// ESGUI operation error codes
///
typedef enum {
  esguiResultOk,
  esguiResultError,
  esguiResultOperationNotSupported,
  esguiResultInvalidParameter

} ESGUI_RESULT;

// Common screen driver interface definition.
//
typedef enum {
  ESGUI_SCREEN_ROT_0,        // native orientation
  ESGUI_SCREEN_ROT_90CW,    // 90 degrees clockwise
  ESGUI_SCREEN_ROT_90CCW,    // 90 degrees counter-clockwise
  ESGUI_SCREEN_ROT_180,      // 180 degrees
  // special value, indicates invalid rotation. never set it programmatically
  ESGUI_SCREEN_ROT_INVALID = -1

} ESGUI_SCREEN_ROTATION;

/// Supported Color packing formats
///
typedef enum {
  esguiClrfmtInvalid = 0,
  esguiClrfmtBitmap1,
  esguiClrfmtPalette4,
  esguiClrfmtPalette8,
  esguiClrfmtGrayscale2,
  esguiClrfmtGrayscale4,
  esguiClrfmtGrayscale8,
  esguiClrfmtRGB332,
  esguiClrfmtBGR233,
  esguiClrfmtRGB565,
  esguiClrfmtBGR565,
  esguiClrfmtRGB888,
  esguiClrfmtBGR888,
  esguiClrfmtRGBA8888,
  esguiClrfmtARGB8888,
  esguiClrfmtBGRA8888,
  esguiClrfmtABGR8888,

  // Special value - must go last in autoenum list
  //
  esguiClrfmtCnt,
  esguiClrfmtNative = esguiClrfmtARGB8888

} ESGUI_COLOR_FORMAT;

// Device-neutral color type
typedef esU32 ESGUI_Color_t;

// ROP types
typedef enum {
  ESGUI_OR,
  ESGUI_AND,
  ESGUI_XOR,
  ESGUI_COPY,
  ESGUI_BLEND
    
} ESGUI_BITOP;

/// Text positioning parameters
///
enum {
  /// Horizontal alignment
  ESGUI_HTA_LEFT,
  ESGUI_HTA_CENTER,
  ESGUI_HTA_RIGHT,
  ESGUI_HTA_MASK      = 0x00000003,
  
  /// Vertical alignment
  ESGUI_VTA_TOP       = 0,
  ESGUI_VTA_CENTER    = 0x00000004,
  ESGUI_VTA_BOTTOM    = 0x00000008,
  ESGUI_VTA_MASK      = 0x0000000C,
  
  /// Wrapping & clipping
  ESGUI_TWRAP         = 0x00000010,
  ESGUI_TCLIP         = 0x00000020
};

#ifdef __cplusplus
  }
#endif

#endif // _es_gui_types_h_

