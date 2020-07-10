#ifndef _es_gui_color_h_
#define _es_gui_color_h_

#ifdef __cplusplus
  extern "C" {
#endif

// Generic, AKA Native ESGUI 32 bit color type
ESE_ANON_UNION
typedef union {
  struct {
    esU8 b;
    esU8 g;
    esU8 r;
    esU8 a;
  };
  esU32 raw;

} ESGUI_COLOR;

/// ESGUI generic palette type
///
ESE_ANON_UNION
typedef struct tagESGUI_PALETTE {
  ESGUI_COLOR_FORMAT colorFmt;
  union {
    esU8*  colors8;
    esU16* colors16;
    esU32* colors32;
  };
  esU8 colorCnt;
  bool realized;

} ESGUI_PALETTE;

/// Macrto to define named color constant
#define DEF_ESGUI_COLOR_CONST(clrName,r,g,b)  const ESGUI_COLOR clrName = { (b),(g),(r),0xFF }

/// Return BPP from color format
esU8 esguiBppFromColorFormat(ESGUI_COLOR_FORMAT fmt);

/// Return true if color format requires palette to code-decode
bool esguiColorFormatUsesPalette(ESGUI_COLOR_FORMAT fmt);

// Create color from components
ESGUI_COLOR esguiColorSet(esU8 r, esU8 g, esU8 b);
ESGUI_COLOR esguiColorSetA(esU8 r, esU8 g, esU8 b, esU8 a);

/// Color format recoders
///

/// RGB 8 to RGB 8 conversions
esU32 esguiBGR233toRGB332(esU32 bgr233);
esU32 esguiRGB332toBGR233(esU32 rgb332);

/// RGB 8 to RGB 16 conversions
esU32 esguiRGB332toRGB565(esU32 rgb332);
esU32 esguiRGB332toBGR565(esU32 rgb332);
esU32 esguiBGR233toRGB565(esU32 bgr233);
esU32 esguiBGR233toBGR565(esU32 bgr233);

/// RGB 8 to native conversions
esU32 esguiRGB332toNative(esU32 rgb332);
esU32 esguiBGR233toNative(esU32 bgr233);

/// RGB 16 to native conversions
esU32 esguiRGB565toNative(esU32 rgb565);
esU32 esguiBGR565toNative(esU32 bgr565);

/// RGB 16 to 16 conversions
esU32 esguiRGB565toBGR565(esU32 rgb565);
esU32 esguiBGR565toRGB565(esU32 bgr565);

/// RGB 24 to native conversions
esU32 esguiRGB888toNative(esU32 rgb888);
esU32 esguiBGR888toNative(esU32 bgr888);

/// RGBA 32-to native conversions
esU32 esguiRGBA8888toNative(esU32 rgba8888);
esU32 esguiARGB8888toNative(esU32 argb8888);
esU32 esguiBGRA8888toNative(esU32 bgra8888);
#define esguiABGR8888toNative(abgr8888) ((esU32)(abgr8888))

/// Return color entry in palette color format
esU32 esguiPaletteColorGet(const ESGUI_PALETTE* pal, int idx);

/// Assign palette color from clr, given its format
ESGUI_RESULT esguiPaletteColorSet(ESGUI_PALETTE* pal, int idx, esU32 clr, ESGUI_COLOR_FORMAT clrFmt);

/// Return color format the palette uses
ESGUI_COLOR_FORMAT esguiPaletteColorFmtGet(const ESGUI_PALETTE* pal);

/// Native to xxx conversions
///

/// ... to 1 bit
esU32 esguiNativeTo1bit(esU32 native);

/// ... to RGB 8
esU32 esguiNativeToRGB332(esU32 native);
esU32 esguiNativeToBGR233(esU32 native);

/// ... to RGB 16
esU32 esguiNativeToRGB565(esU32 native);
esU32 esguiNativeToBGR565(esU32 native);

/// ... to RGB 24
esU32 esguiNativeToRGB888(esU32 native);
esU32 esguiNativeToBGR888(esU32 native);

/// ... to RGBA 32
esU32 esguiNativeToRGBA8888(esU32 native);
esU32 esguiNativeToARGB8888(esU32 native);
esU32 esguiNativeToBGRA8888(esU32 native);
#define esguiNativeToABGR8888(native) ((esU32)(native))

/// Universal color conversion method
ESGUI_RESULT esguiColorToColor(
  esU32* clrOut, ESGUI_COLOR_FORMAT fmtOut,
  esU32 clrIn, ESGUI_COLOR_FORMAT fmtIn, const struct tagESGUI_PALETTE* palIn
);

/// Perform requested operation on two colors.
/// The result is returned in fmt1 format, even if value of in1 is not used.
/// palettes are optional, and required only if format specified is a paletted one
/// Optional in3 xxx params are used only when blending is requested,
/// in1 format supports blending, in2 is a grayscale format,
/// which is used as a transparency mask for in3 in that case.
///
ESGUI_RESULT esguiColorOp(
  esU32* out,
  esU32 in1, ESGUI_COLOR_FORMAT fmt1,
  esU32 in2, ESGUI_COLOR_FORMAT fmt2, const struct tagESGUI_PALETTE* pal2,
  esU32 in3, ESGUI_COLOR_FORMAT fmt3, const struct tagESGUI_PALETTE* pal3,
  ESGUI_BITOP op
);

#ifdef __cplusplus
  }
#endif

#endif // _es_gui_color_h_

