#include <esgui/include/esgui.h>
#include <esfwxe/ese_assert.h>

#include <esgui/include/esgui_color.h>
//---------------------------------------------------------------------------

esU8 esguiBppFromColorFormat(ESGUI_COLOR_FORMAT fmt)
{
  static const esU8 sc_bpps[esguiClrfmtCnt] = {
    0,    // esguiClrfmtInvalid
    1,    // esguiClrfmtBitmap1,
    4,    // esguiClrfmtPalette4,
    8,    // esguiClrfmtPalette8,
    2,    // esguiClrfmtGrayscale2,
    4,    // esguiClrfmtGrayscale4,
    8,    // esguiClrfmtGrayscale8,
    8,    // esguiClrfmtRGB332,
    8,    // esguiClrfmtBGR233,
    16,   // esguiClrfmtRGB565,
    16,   // esguiClrfmtBGR565,
    24,   // esguiClrfmtRGB888,
    24,   // esguiClrfmtBGR888,
    32,   // esguiClrfmtRGBA8888, esguiClrfmtNative,
    32,   // esguiClrfmtARGB8888,
    32,   // esguiClrfmtBGRA8888,
    32    // esguiClrfmtABGR8888,
  };

  if( fmt >= esguiClrfmtCnt )
    return 0;

  return sc_bpps[fmt];
}
//---------------------------------------------------------------------------

bool esguiColorFormatUsesPalette(ESGUI_COLOR_FORMAT fmt)
{
  return (esguiClrfmtBitmap1 <= fmt && esguiClrfmtPalette8 >= fmt);
}
//---------------------------------------------------------------------------

ESGUI_COLOR esguiColorSet(esU8 r, esU8 g, esU8 b)
{
  ESGUI_COLOR clr;
  clr.r = r;
  clr.g = g;
  clr.b = b;
  clr.a = 0xFF;

  return clr;
}
//---------------------------------------------------------------------------

ESGUI_COLOR esguiColorSetA(esU8 r, esU8 g, esU8 b, esU8 a)
{
  ESGUI_COLOR clr;
  clr.r = r;
  clr.g = g;
  clr.b = b;
  clr.a = a;

  return clr;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

esU32 esguiBGR233toRGB332(esU32 bgr233)
{
  return ((bgr233 >> 6) & 0x03) |
    ((bgr233 >> 1) & 0x1C) |
    ((bgr233 & 0x07) << 5);
}
//---------------------------------------------------------------------------

esU32 esguiRGB332toBGR233(esU32 rgb332)
{
  return ((rgb332 & 0x03) << 6) |
    ((rgb332 << 1) & 0x38) |
    ((rgb332 >> 5) & 0x07);
}
//---------------------------------------------------------------------------

static const esU8 sc_b3to6[8] = { 0, 9, 18, 27, 36, 45, 54, 63 };
static const esU8 sc_b3to5[8] = { 0, 4, 9, 13, 18, 22, 27, 31 };
static const esU8 sc_b2to5[4] = { 0, 10, 21, 31 };
//---------------------------------------------------------------------------

esU32 esguiRGB332toRGB565(esU32 rgb332)
{
  return ((esU32)sc_b3to5[(rgb332 >> 5) & 0x07] << 11) |
    ((esU32)sc_b3to6[(rgb332 >> 2) & 0x07] << 5) |
    ((esU32)sc_b2to5[rgb332 & 0x03]);
}
//---------------------------------------------------------------------------

esU32 esguiRGB332toBGR565(esU32 rgb332)
{
  return ((esU32)sc_b3to5[(rgb332 >> 5) & 0x07]) |
    ((esU32)sc_b3to6[(rgb332 >> 2) & 0x07] << 5) |
    (((esU32)sc_b2to5[rgb332 & 0x03]) << 11);
}
//---------------------------------------------------------------------------

esU32 esguiBGR233toRGB565(esU32 bgr233)
{
  return (esU32)sc_b2to5[(bgr233 >> 6) & 0x03] |
    ((esU32)sc_b3to6[(bgr233 >> 3) & 0x07] << 5) |
    ((esU32)sc_b3to5[bgr233 & 0x07] << 11);
}
//---------------------------------------------------------------------------

esU32 esguiBGR233toBGR565(esU32 bgr233)
{
  return ((esU32)sc_b2to5[(bgr233 >> 6) & 0x03] << 11) |
    ((esU32)sc_b3to6[(bgr233 >> 3) & 0x07] << 5) |
    (esU32)sc_b3to5[bgr233 & 0x07];
}
//---------------------------------------------------------------------------

esU32 esguiRGB332toNative(esU32 rgb332)
{
  ESGUI_COLOR out;

  out.r = (((rgb332 >> 5) & 0x07) * 255) / 7;
  out.g = (((rgb332 >> 2) & 0x07) * 255) / 7;
  out.b = ((rgb332 & 0x03) * 255) / 3;
  out.a = 0xFF;

  return out.raw;
}
//---------------------------------------------------------------------------

esU32 esguiBGR233toNative(esU32 bgr233)
{
  ESGUI_COLOR out;

  out.r = ((bgr233 & 0x07) * 255) / 7;
  out.g = (((bgr233 >> 3) & 0x07) * 255) / 7;
  out.b = (((bgr233 >> 6) & 0x03) * 255) / 3;
  out.a = 0xFF;

  return out.raw;
}
//---------------------------------------------------------------------------

esU32 esguiRGB565toBGR565(esU32 rgb565)
{
  return ((rgb565 >> 11) & 0x1F) |
    ((rgb565 & 0x1F) << 11) |
    (rgb565 & 0x07E0);
}
//---------------------------------------------------------------------------

esU32 esguiBGR565toRGB565(esU32 bgr565)
{
  return ((bgr565 >> 11) & 0x1F) |
    ((bgr565 & 0x1F) << 11) |
    (bgr565 & 0x07E0);
}
//---------------------------------------------------------------------------

esU32 esguiRGB565toNative(esU32 rgb565)
{
  ESGUI_COLOR out;

  out.r = (((rgb565 >> 11) & 0x1F) * 255) / 0x1F;
  out.g = (((rgb565 >> 5) & 0x3F) * 255) / 0x3F;
  out.b = ((rgb565 & 0x1F) * 255) / 0x1F;
  out.a = 0xFF;

  return out.raw;
}
//---------------------------------------------------------------------------

esU32 esguiBGR565toNative(esU32 bgr565)
{
  ESGUI_COLOR out;

  out.r = ((bgr565 & 0x1F) * 255) / 0x1F;
  out.g = (((bgr565 >> 5) & 0x3F) * 255) / 0x3F;
  out.b = (((bgr565 >> 11) & 0x1F) * 255) / 0x1F;
  out.a = 0xFF;

  return out.raw;
}
//---------------------------------------------------------------------------

esU32 esguiRGB888toNative(esU32 rgb888)
{
  ESGUI_COLOR out;

  out.r = (rgb888 >> 16) & 0xFF;
  out.g = (rgb888 >> 8) & 0xFF;
  out.b = rgb888 & 0xFF;
  out.a = 0xFF;

  return out.raw;
}
//---------------------------------------------------------------------------

esU32 esguiBGR888toNative(esU32 bgr888)
{
  ESGUI_COLOR out;

  out.r = bgr888 & 0xFF;
  out.g = (bgr888 >> 8) & 0xFF;
  out.b = (bgr888 >> 16) & 0xFF;
  out.a = 0xFF;

  return out.raw;
}
//---------------------------------------------------------------------------

esU32 esguiRGBA8888toNative(esU32 rgba8888)
{
  return ((rgba8888 & 0xFF) << 24) | (rgba8888 >> 8);
}
//---------------------------------------------------------------------------

esU32 esguiARGB8888toNative(esU32 argb8888)
{
  ESGUI_COLOR out;

  out.r = (argb8888 >> 16) & 0xFF;
  out.g = (argb8888 >> 8) & 0xFF;
  out.b = argb8888 & 0xFF;
  out.a = (argb8888 >> 24) & 0xFF;

  return out.raw;
}
//---------------------------------------------------------------------------

esU32 esguiBGRA8888toNative(esU32 bgra8888)
{
  ESGUI_COLOR out;

  out.r = (bgra8888 >> 8) & 0xFF;
  out.g = (bgra8888 >> 16) & 0xFF;
  out.b = (bgra8888 >> 24) & 0xFF;
  out.a = bgra8888 & 0xFF;

  return out.raw;
}
//---------------------------------------------------------------------------

esU32 esguiNativeTo1bit(esU32 native)
{
  ESGUI_COLOR clr;
  clr.raw = native;
  
  if( 0 == clr.a )
    return 0;
    
  clr.a = 0;
  if( clr.raw )
    return 1;
  
  return 0;
}
//---------------------------------------------------------------------------

esU32 esguiNativeToRGB332(esU32 native)
{
  ESGUI_COLOR clr;
  clr.raw = native;

  return (((esU32)clr.r) & 0xE0) |
    (((esU32)(clr.g >> 5)) << 2) |
    ((esU32)(clr.b >> 6));
}
//---------------------------------------------------------------------------

esU32 esguiNativeToBGR233(esU32 native)
{
  ESGUI_COLOR clr;
  clr.raw = native;

  return ((esU32)(clr.r >> 5)) |
    (((esU32)(clr.g >> 5)) << 3) |
    (((esU32)clr.b) & 0xC0);
}
//---------------------------------------------------------------------------

esU32 esguiNativeToRGB565(esU32 native)
{
  ESGUI_COLOR clr;
  clr.raw = native;

  return ((((((esU32)clr.r)*0x1F)/255) & 0x1F) << 11) |
    ((((((esU32)clr.g)*0x3F)/255) & 0x3F) << 5) |
    (((((esU32)clr.b)*0x1F)/255) & 0x1F);
}
//---------------------------------------------------------------------------

esU32 esguiNativeToBGR565(esU32 native)
{
  ESGUI_COLOR clr;
  clr.raw = native;

  return (((((esU32)clr.r)*0x1F)/255) & 0x1F) |
    ((((((esU32)clr.g)*0x3F)/255) & 0x3F) << 5) |
    ((((((esU32)clr.b)*0x1F)/255) & 0x1F) << 11);
}
//---------------------------------------------------------------------------

esU32 esguiNativeToRGB888(esU32 native)
{
  ESGUI_COLOR clr;
  clr.raw = native;

  return (((esU32)clr.r) << 16) |
    (((esU32)clr.g) << 8) |
    ((esU32)clr.b);
}
//---------------------------------------------------------------------------

esU32 esguiNativeToBGR888(esU32 native)
{
  ESGUI_COLOR clr;
  clr.raw = native;

  return ((esU32)clr.r) |
    (((esU32)clr.g) << 8) |
    (((esU32)clr.b) << 16);
}
//---------------------------------------------------------------------------

esU32 esguiNativeToRGBA8888(esU32 native)
{
  ESGUI_COLOR clr;
  clr.raw = native;

  return (((esU32)clr.r) << 24) |
    (((esU32)clr.g) << 16) |
    (((esU32)clr.b) << 8) |
    ((esU32)clr.a);
}
//---------------------------------------------------------------------------

esU32 esguiNativeToARGB8888(esU32 native)
{
  ESGUI_COLOR clr;
  clr.raw = native;

  return (((esU32)clr.a) << 24) |
    (((esU32)clr.r) << 16) |
    (((esU32)clr.g) << 8) |
    ((esU32)clr.b);
}
//---------------------------------------------------------------------------

esU32 esguiNativeToBGRA8888(esU32 native)
{
  ESGUI_COLOR clr;
  clr.raw = native;

  return (((esU32)clr.b) << 24) |
    (((esU32)clr.g) << 16) |
    (((esU32)clr.r) << 8) |
    ((esU32)clr.a);
}
//---------------------------------------------------------------------------

ESGUI_COLOR_FORMAT esguiPaletteColorFmtGet(const ESGUI_PALETTE* pal)
{
  if( !pal )
    return esguiClrfmtInvalid;

  return (ESGUI_COLOR_FORMAT)pal->colorFmt;
}
//---------------------------------------------------------------------------

esU32 esguiPaletteColorGet(const ESGUI_PALETTE* pal, int idx)
{
  ESE_ASSERT(pal);
  ESE_ASSERT(-1 < idx);
  ESE_ASSERT(idx < (int)pal->colorCnt);

  ESGUI_COLOR_FORMAT palFmt = esguiPaletteColorFmtGet(pal);
  ESE_ASSERT(esguiClrfmtBitmap1 == palFmt || esguiClrfmtRGB332 <= palFmt);
  ESE_ASSERT(esguiClrfmtABGR8888 >= palFmt);

  int bpp = esguiBppFromColorFormat(palFmt);
  ESE_ASSERT(bpp);

  if( bpp <= 8 )
    return pal->colors8[idx];
  else if( 16 == bpp )
    return pal->colors16[idx];
  else
    return pal->colors32[idx];
}
//---------------------------------------------------------------------------

ESGUI_RESULT esguiPaletteColorSet(ESGUI_PALETTE* pal, int idx, esU32 clr, ESGUI_COLOR_FORMAT clrFmt)
{
  ESE_ASSERT(pal);
  ESE_ASSERT(-1 < idx);
  ESE_ASSERT(idx < (int)pal->colorCnt);

  ESE_ASSERT(esguiClrfmtBitmap1 == clrFmt || esguiClrfmtRGB332 <= clrFmt);
  ESE_ASSERT(esguiClrfmtABGR8888 >= clrFmt);

  ESGUI_COLOR_FORMAT palFmt = esguiPaletteColorFmtGet(pal);
  int bpp = esguiBppFromColorFormat( palFmt );
  ESE_ASSERT(bpp);

  ESGUI_RESULT result = esguiColorToColor(
    &clr,
    palFmt,
    clr,
    clrFmt,
    NULL
  );

  if( esguiResultOk == result )
  {
    if(bpp <= 8)
      pal->colors8[idx] = (esU8)clr;
    else if( 16 == bpp )
      pal->colors16[idx] = (esU16)clr;
    else
      pal->colors32[idx] = clr;
  }

  return result;
}
//---------------------------------------------------------------------------

ESGUI_RESULT esguiColorToColor(
  esU32* clrOut,
  ESGUI_COLOR_FORMAT fmtOut,
  esU32 clrIn,
  ESGUI_COLOR_FORMAT fmtIn,
  const ESGUI_PALETTE* pal
)
{
  if( !clrOut )
    return esguiResultInvalidParameter;

  if(
    esguiClrfmtInvalid == fmtIn ||
    esguiClrfmtInvalid == fmtOut ||
    esguiClrfmtCnt <= fmtIn ||
    esguiClrfmtCnt <= fmtOut
  )
    return esguiResultInvalidParameter;

  if( fmtOut > esguiClrfmtBitmap1 && esguiColorFormatUsesPalette(fmtOut) )
    return esguiResultOperationNotSupported;

  if( esguiColorFormatUsesPalette(fmtIn) )
  {
    ESE_ASSERT(pal);

    clrIn = esguiPaletteColorGet(
      pal,
      clrIn
    );

    fmtIn = pal->colorFmt;
  }

  if( fmtIn == fmtOut )
  {
    *clrOut = clrIn;
    return esguiResultOk;
  }

  switch( fmtOut )
  {
  case esguiClrfmtBitmap1:
    switch( fmtIn )
    {
    case esguiClrfmtRGBA8888:
      clrIn = esguiRGBA8888toNative(clrIn);
      break;
    case esguiClrfmtARGB8888:
      clrIn = esguiARGB8888toNative(clrIn);
      break;
    case esguiClrfmtBGRA8888:
      clrIn = esguiBGRA8888toNative(clrIn);
      break;
    case esguiClrfmtABGR8888:
      clrIn = esguiABGR8888toNative(clrIn);
      break;
    default: // For all other non-transparent formats we may just do quick != 0 comparison
      *clrOut = (0 != clrIn) ? 1 : 0;
      return esguiResultOk;
    }
    *clrOut = esguiNativeTo1bit(clrIn);
    return esguiResultOk;
  case esguiClrfmtRGB332:
    switch( fmtIn )
    {
    case esguiClrfmtBGR233:
      *clrOut = esguiBGR233toRGB332(clrIn);
      return esguiResultOk;
    case esguiClrfmtRGB565:
      clrIn = esguiRGB565toNative(clrIn);
      break;
    case esguiClrfmtBGR565:
      clrIn = esguiBGR565toNative(clrIn);
      break;
    case esguiClrfmtRGB888:
      clrIn = esguiRGB888toNative(clrIn);
      break;
    case esguiClrfmtBGR888:
      clrIn = esguiBGR888toNative(clrIn);
      break;
    case esguiClrfmtRGBA8888:
      clrIn = esguiRGBA8888toNative(clrIn);
      break;
    case esguiClrfmtARGB8888:
      clrIn = esguiARGB8888toNative(clrIn);
      break;
    case esguiClrfmtBGRA8888:
      clrIn = esguiBGRA8888toNative(clrIn);
      break;
    case esguiClrfmtABGR8888:
      clrIn = esguiABGR8888toNative(clrIn);
      break;
    default:
      return esguiResultOperationNotSupported;
    }
    *clrOut = esguiNativeToRGB332(clrIn);
    return esguiResultOk;
  case esguiClrfmtBGR233:
    switch(fmtIn)
    {
    case esguiClrfmtRGB332:
      *clrOut = esguiRGB332toBGR233(clrIn);
      return esguiResultOk;
    case esguiClrfmtRGB565:
      clrIn = esguiRGB565toNative(clrIn);
      break;
    case esguiClrfmtBGR565:
      clrIn = esguiBGR565toNative(clrIn);
      break;
    case esguiClrfmtRGB888:
      clrIn = esguiRGB888toNative(clrIn);
      break;
    case esguiClrfmtBGR888:
      clrIn = esguiBGR888toNative(clrIn);
      break;
    case esguiClrfmtRGBA8888:
      clrIn = esguiRGBA8888toNative(clrIn);
      break;
    case esguiClrfmtARGB8888:
      clrIn = esguiARGB8888toNative(clrIn);
      break;
    case esguiClrfmtBGRA8888:
      clrIn = esguiBGRA8888toNative(clrIn);
      break;
    case esguiClrfmtABGR8888:
      clrIn = esguiABGR8888toNative(clrIn);
      break;
    default:
      return esguiResultOperationNotSupported;
    }
    *clrOut = esguiNativeToBGR233(clrIn);
    return esguiResultOk;
  case esguiClrfmtRGB565:
    switch(fmtIn)
    {
    case esguiClrfmtRGB332:
      *clrOut = esguiRGB332toRGB565(clrIn);
      return esguiResultOk;
    case esguiClrfmtBGR233:
      *clrOut = esguiBGR233toRGB565(clrIn);
      return esguiResultOk;
    case esguiClrfmtBGR565:
      *clrOut = esguiBGR565toRGB565(clrIn);
      return esguiResultOk;
    case esguiClrfmtRGB888:
      clrIn = esguiRGB888toNative(clrIn);
      break;
    case esguiClrfmtBGR888:
      clrIn = esguiBGR888toNative(clrIn);
      break;
    case esguiClrfmtRGBA8888:
      clrIn = esguiRGBA8888toNative(clrIn);
      break;
    case esguiClrfmtARGB8888:
      clrIn = esguiARGB8888toNative(clrIn);
      break;
    case esguiClrfmtBGRA8888:
      clrIn = esguiBGRA8888toNative(clrIn);
      break;
    case esguiClrfmtABGR8888:
      clrIn = esguiABGR8888toNative(clrIn);
      break;
    default:
      return esguiResultOperationNotSupported;
    }
    *clrOut = esguiNativeToRGB565(clrIn);
    return esguiResultOk;
  case esguiClrfmtBGR565:
    switch(fmtIn)
    {
    case esguiClrfmtRGB332:
      *clrOut = esguiRGB332toBGR565(clrIn);
      return esguiResultOk;
    case esguiClrfmtBGR233:
      *clrOut = esguiBGR233toBGR565(clrIn);
      return esguiResultOk;
    case esguiClrfmtRGB565:
      *clrOut = esguiRGB565toBGR565(clrIn);
      return esguiResultOk;
     case esguiClrfmtRGB888:
      clrIn = esguiRGB888toNative(clrIn);
      break;
    case esguiClrfmtBGR888:
      clrIn = esguiBGR888toNative(clrIn);
      break;
    case esguiClrfmtRGBA8888:
      clrIn = esguiRGBA8888toNative(clrIn);
      break;
    case esguiClrfmtARGB8888:
      clrIn = esguiARGB8888toNative(clrIn);
      break;
    case esguiClrfmtBGRA8888:
      clrIn = esguiBGRA8888toNative(clrIn);
      break;
    case esguiClrfmtABGR8888:
      clrIn = esguiABGR8888toNative(clrIn);
      break;
    default:
      return esguiResultOperationNotSupported;
    }
    *clrOut = esguiNativeToBGR565(clrIn);
    return esguiResultOk;
  case esguiClrfmtRGB888:
    switch(fmtIn)
    {
    case esguiClrfmtRGB332:
      clrIn = esguiRGB332toNative(clrIn);
      break;
    case esguiClrfmtBGR233:
      clrIn = esguiBGR233toNative(clrIn);
      break;
    case esguiClrfmtRGB565:
      clrIn = esguiRGB565toNative(clrIn);
      break;
    case esguiClrfmtBGR565:
      clrIn = esguiBGR565toNative(clrIn);
      break;
    case esguiClrfmtBGR888:
      clrIn = esguiBGR888toNative(clrIn);
      break;
    case esguiClrfmtRGBA8888:
      clrIn = esguiRGBA8888toNative(clrIn);
      break;
    case esguiClrfmtARGB8888:
      clrIn = esguiARGB8888toNative(clrIn);
      break;
    case esguiClrfmtBGRA8888:
      clrIn = esguiBGRA8888toNative(clrIn);
      break;
    case esguiClrfmtABGR8888:
      clrIn = esguiABGR8888toNative(clrIn);
      break;
    default:
      return esguiResultOperationNotSupported;
    }
    *clrOut = esguiNativeToRGB888(clrIn);
    return esguiResultOk;
  case esguiClrfmtBGR888:
    switch(fmtIn)
    {
    case esguiClrfmtRGB332:
      clrIn = esguiRGB332toNative(clrIn);
      break;
    case esguiClrfmtBGR233:
      clrIn = esguiBGR233toNative(clrIn);
      break;
    case esguiClrfmtRGB565:
      clrIn = esguiRGB565toNative(clrIn);
      break;
    case esguiClrfmtBGR565:
      clrIn = esguiBGR565toNative(clrIn);
      break;
    case esguiClrfmtRGB888:
      clrIn = esguiRGB888toNative(clrIn);
      break;
    case esguiClrfmtRGBA8888:
      clrIn = esguiRGBA8888toNative(clrIn);
      break;
    case esguiClrfmtARGB8888:
      clrIn = esguiARGB8888toNative(clrIn);
      break;
    case esguiClrfmtBGRA8888:
      clrIn = esguiBGRA8888toNative(clrIn);
      break;
    case esguiClrfmtABGR8888:
      clrIn = esguiABGR8888toNative(clrIn);
      break;
    default:
      return esguiResultOperationNotSupported;
    }
    *clrOut = esguiNativeToBGR888(clrIn);
    return esguiResultOk;
  case esguiClrfmtRGBA8888:
    switch(fmtIn)
    {
    case esguiClrfmtRGB332:
      clrIn = esguiRGB332toNative(clrIn);
      break;
    case esguiClrfmtBGR233:
      clrIn = esguiBGR233toNative(clrIn);
      break;
    case esguiClrfmtRGB565:
      clrIn = esguiRGB565toNative(clrIn);
      break;
    case esguiClrfmtBGR565:
      clrIn = esguiBGR565toNative(clrIn);
      break;
    case esguiClrfmtRGB888:
      clrIn = esguiRGB888toNative(clrIn);
      break;
    case esguiClrfmtBGR888:
      clrIn = esguiBGR888toNative(clrIn);
      break;
    case esguiClrfmtARGB8888:
      clrIn = esguiARGB8888toNative(clrIn);
      break;
    case esguiClrfmtBGRA8888:
      clrIn = esguiBGRA8888toNative(clrIn);
      break;
    case esguiClrfmtABGR8888:
      clrIn = esguiABGR8888toNative(clrIn);
      break;
    default:
      return esguiResultOperationNotSupported;
    }
    *clrOut = esguiNativeToRGBA8888(clrIn);
    return esguiResultOk;
  case esguiClrfmtARGB8888:
    switch(fmtIn)
    {
    case esguiClrfmtRGB332:
      clrIn = esguiRGB332toNative(clrIn);
      break;
    case esguiClrfmtBGR233:
      clrIn = esguiBGR233toNative(clrIn);
      break;
    case esguiClrfmtRGB565:
      clrIn = esguiRGB565toNative(clrIn);
      break;
    case esguiClrfmtBGR565:
      clrIn = esguiBGR565toNative(clrIn);
      break;
    case esguiClrfmtRGB888:
      clrIn = esguiRGB888toNative(clrIn);
      break;
    case esguiClrfmtBGR888:
      clrIn = esguiBGR888toNative(clrIn);
      break;
    case esguiClrfmtRGBA8888:
      clrIn = esguiRGBA8888toNative(clrIn);
      break;
    case esguiClrfmtBGRA8888:
      clrIn = esguiBGRA8888toNative(clrIn);
      break;
    case esguiClrfmtABGR8888:
      clrIn = esguiABGR8888toNative(clrIn);
      break;
    default:
      return esguiResultOperationNotSupported;
    }
    *clrOut = esguiNativeToARGB8888(clrIn);
    return esguiResultOk;
  case esguiClrfmtBGRA8888:
    switch(fmtIn)
    {
    case esguiClrfmtRGB332:
      clrIn = esguiRGB332toNative(clrIn);
      break;
    case esguiClrfmtBGR233:
      clrIn = esguiBGR233toNative(clrIn);
      break;
    case esguiClrfmtRGB565:
      clrIn = esguiRGB565toNative(clrIn);
      break;
    case esguiClrfmtBGR565:
      clrIn = esguiBGR565toNative(clrIn);
      break;
    case esguiClrfmtRGB888:
      clrIn = esguiRGB888toNative(clrIn);
      break;
    case esguiClrfmtBGR888:
      clrIn = esguiBGR888toNative(clrIn);
      break;
    case esguiClrfmtARGB8888:
      clrIn = esguiARGB8888toNative(clrIn);
      break;
    case esguiClrfmtRGBA8888:
      clrIn = esguiRGBA8888toNative(clrIn);
      break;
    case esguiClrfmtABGR8888:
      clrIn = esguiABGR8888toNative(clrIn);
      break;
    default:
      return esguiResultOperationNotSupported;
    }
    *clrOut = esguiNativeToBGRA8888(clrIn);
    return esguiResultOk;
  case esguiClrfmtABGR8888:
    switch(fmtIn)
    {
    case esguiClrfmtRGB332:
      clrIn = esguiRGB332toNative(clrIn);
      break;
    case esguiClrfmtBGR233:
      clrIn = esguiBGR233toNative(clrIn);
      break;
    case esguiClrfmtRGB565:
      clrIn = esguiRGB565toNative(clrIn);
      break;
    case esguiClrfmtBGR565:
      clrIn = esguiBGR565toNative(clrIn);
      break;
    case esguiClrfmtRGB888:
      clrIn = esguiRGB888toNative(clrIn);
      break;
    case esguiClrfmtBGR888:
      clrIn = esguiBGR888toNative(clrIn);
      break;
    case esguiClrfmtARGB8888:
      clrIn = esguiARGB8888toNative(clrIn);
      break;
    case esguiClrfmtRGBA8888:
      clrIn = esguiRGBA8888toNative(clrIn);
      break;
    case esguiClrfmtBGRA8888:
      clrIn = esguiBGRA8888toNative(clrIn);
      break;
    default:
      return esguiResultOperationNotSupported;
    }
    *clrOut = esguiNativeToABGR8888(clrIn);
    return esguiResultOk;
  default:
    return esguiResultOperationNotSupported;
  }
}
//---------------------------------------------------------------------------

ESGUI_RESULT esguiColorOp(
  esU32* out,
  esU32 in1, ESGUI_COLOR_FORMAT fmt1,
  esU32 in2, ESGUI_COLOR_FORMAT fmt2, const struct tagESGUI_PALETTE* pal2,
  esU32 in3, ESGUI_COLOR_FORMAT fmt3, const struct tagESGUI_PALETTE* pal3,
  ESGUI_BITOP op
)
{
  ESGUI_RESULT result = esguiResultError;
  switch(op)
  {
  case ESGUI_COPY: //< Ignore in1 pixel, just use its color format
    result = esguiColorToColor(
      out,
      fmt1,
      in2,
      fmt2,
      pal2
    );
    break;
  case ESGUI_OR:
  case ESGUI_AND:
  case ESGUI_XOR:
    {
      // Convert in2 to color format of in1, put in temporarily in out
      result = esguiColorToColor(
        out,
        fmt1,
        in2,
        fmt2,
        pal2
      );

      if( esguiResultOk == result )
      {
        if(ESGUI_OR == op)
          *out = (in1 | *out);
        else if(ESGUI_AND == op)
          *out = (in1 & *out);
        else
          *out = (in1 ^ *out);
      }
    }
    break;
  case ESGUI_BLEND:
    {
      ESGUI_COLOR bgnd;
      // Convert in1 to native format, use it as bgnd color
      result = esguiColorToColor(
        &bgnd.raw,
        esguiClrfmtNative,
        in1,
        fmt1,
        NULL
      );

      if( esguiResultOk == result )
      {
        ESGUI_COLOR fgnd;

        // If in2 is a grayscale, use it as apha channel, and in3 as tint
        if( fmt2 >= esguiClrfmtGrayscale2 && fmt2 <= esguiClrfmtGrayscale8 )
        {
          result = esguiColorToColor(
            &fgnd.raw,
            esguiClrfmtNative,
            in3,
            fmt3,
            pal3
          );

          if( esguiResultOk == result )
          {
            // Calculate alpha channel from a grayscale value
            switch(fmt2)
            {
            case esguiClrfmtGrayscale2:
              fgnd.a = (in2 * 255) / 3;
              break;
            case esguiClrfmtGrayscale4:
              fgnd.a = (in2 * 255) / 15;
              break;
            default:
              fgnd.a = in2;
              break;
            }
          }
        }
        else // Use in2 as a foreground color
          result = esguiColorToColor(
            &fgnd.raw,
            esguiClrfmtNative,
            in2,
            fmt2,
            pal2
          );

        if( esguiResultOk == result )  // Alpha - blend fgnd into bgnd
        {
          if( 255 == fgnd.a ) // Opaque - no need in blending, just do copy
            result = esguiColorToColor(
              out,
              fmt1,
              fgnd.raw,
              esguiClrfmtNative,
              NULL
            );
          else
          {
            bgnd.r = (((esU32)fgnd.r) * ((esU32)fgnd.a)) / 255 +
              ((esU32)bgnd.r * (255-fgnd.a))/255;
            bgnd.g = (((esU32)fgnd.g) * ((esU32)fgnd.a)) / 255 +
              ((esU32)bgnd.g * (255-fgnd.a))/255;
            bgnd.b = (((esU32)fgnd.b) * ((esU32)fgnd.a)) / 255 +
              ((esU32)bgnd.b * (255-fgnd.a))/255;

            // Convert the result of blending back into fmt1, and return to out
            result = esguiColorToColor(
              out,
              fmt1,
              bgnd.raw,
              esguiClrfmtNative,
              NULL
            );
          }
        }
      }
    }
    break;
  }

  return result;
}
//---------------------------------------------------------------------------

