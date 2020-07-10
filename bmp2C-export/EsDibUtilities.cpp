#include "EsDibUtilities.h"
#pragma hdrstop
//---------------------------------------------------------------------------

#include <algorithm>
#include <esgui/include/esgui_bit_macros_strings.h>
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

// max text line length
static const int c_maxLineLength = 80;
//---------------------------------------------------------------------------

void EsUtilities::dibToGrayscale(TBitmap* dib, int clrFmt)
{
  for(int y = 0; y < dib->Height; ++y)
  {
    TRGBQuad* row = reinterpret_cast<TRGBQuad*>(dib->ScanLine[y]);
    for(int x = 0; x < dib->Width; ++x)
    {
      TRGBQuad& rgba = row[x];

      // 0.2126 * R + 0.7152 * G + 0.0722 * B
      uint32_t gs = (
        (static_cast<uint32_t>(rgba.rgbRed) * 2126) / 10000 +
        (static_cast<uint32_t>(rgba.rgbGreen) * 7152) / 10000 +
        (static_cast<uint32_t>(rgba.rgbBlue) * 722) / 10000
      ) & 0xFF;

      if( esguiClrfmtGrayscale2 == clrFmt )
      {
        gs = (gs / 4) * 4;
        if( 192 <= gs )
          gs = 255;
      }
      else if( esguiClrfmtGrayscale4 == clrFmt )
      {
        gs = (gs / 16) * 16;
        if( 240 <= gs )
          gs = 255;
      }

      rgba.rgbRed = gs;
      rgba.rgbGreen = gs;
      rgba.rgbBlue = gs;
    }
  }
}
//---------------------------------------------------------------------------

void EsUtilities::dibToRGB(TBitmap* dib, int clrFmt)
{
  for(int y = 0; y < dib->Height; ++y)
  {
    TRGBQuad* row = reinterpret_cast<TRGBQuad*>(dib->ScanLine[y]);
    for(int x = 0; x < dib->Width; ++x)
    {
      TRGBQuad& rgba = row[x];

      switch(clrFmt)
      {
      case esguiClrfmtRGB332:
      case esguiClrfmtBGR233:
        rgba.rgbRed = (rgba.rgbRed / 8) * 8;
        if( 224 <= rgba.rgbRed )
          rgba.rgbRed = 255;
        rgba.rgbGreen = (rgba.rgbGreen / 8) * 8;
        if( 224 <= rgba.rgbGreen )
          rgba.rgbGreen = 255;
        rgba.rgbBlue = (rgba.rgbBlue / 4) * 4;
        if( 192 <= rgba.rgbBlue )
          rgba.rgbBlue = 255;
        break;
      case esguiClrfmtRGB565:
      case esguiClrfmtBGR565:
         rgba.rgbRed = (rgba.rgbRed / 32) * 32;
        if( 248 <= rgba.rgbRed )
          rgba.rgbRed = 255;
        rgba.rgbGreen = (rgba.rgbGreen / 64) * 64;
        if( 252 <= rgba.rgbGreen )
          rgba.rgbGreen = 255;
        rgba.rgbBlue = (rgba.rgbBlue / 32) * 32;
        if( 248 <= rgba.rgbBlue )
          rgba.rgbBlue = 255;
       break;
      default:
        // Do nothing
        break;
      }
    }
  }
}
//---------------------------------------------------------------------------

static void dibDataExportPaletted(TBitmap* dib, TStrings* code, int bpp)
{
  int bytesPerLine = (dib->Width * bpp) / 8 + ((dib->Width * bpp) % 8 ? 1 : 0);

  // perform per-row export
  for(int row = 0; row < dib->Height; ++row)
  {
    const esU8* rowBuff = reinterpret_cast<const esU8*>(dib->ScanLine[row]);
    UnicodeString line;

    for( int byte = 0; byte < bytesPerLine; ++byte )
    {
      if( 1 == bpp )
        line += c_byteMacros[ rowBuff[byte] ]; // visualize monochrome bitmaps code using byte macros
      else
        line += UnicodeString::Format(
          "0x%0.2X",
          ARRAYOFCONST((
            rowBuff[byte]
          ))
         );
      line += _T(',');
    }

    code->Add(line);
  }
}
//---------------------------------------------------------------------------

static void dibDataExportGrayscale(TBitmap* dib, TStrings* code, int bpp)
{
  int pixelsPerByte = 8 / bpp;
  int lineLen = 0;

  // perform per-row export
  for(int row = 0; row < dib->Height; ++row)
  {
    const TRGBQuad* rowBuff = reinterpret_cast<const TRGBQuad*>(dib->ScanLine[row]);
    UnicodeString line;

    int pixelCnt = 0;
    uint8_t _byte = 0;
    uint8_t gs;
    for( int x = 0; x < dib->Width; ++x )
    {
      gs = rowBuff[x].rgbRed;

      _byte |= (
        (gs >> (8-bpp)) <<
        (bpp * (pixelsPerByte - pixelCnt - 1))
      );
      ++pixelCnt;

      if( pixelCnt == pixelsPerByte )
      {
        line += UnicodeString::Format(
          "0x%0.2X",
          ARRAYOFCONST((
            _byte
          ))
        );
        line += _T(',');

        ++lineLen;

        if( lineLen >= c_maxLineLength )
        {
          lineLen = 0;
          line += _T('\n');
        }

        pixelCnt = 0;
        _byte = 0;
      }
    }

    if( pixelCnt && pixelCnt < pixelsPerByte ) //< Handle non-full trailing byte
    {
      line += UnicodeString::Format(
        "0x%0.2X",
        ARRAYOFCONST((
          _byte
        ))
      );
      line += _T(',');
    }

    if( !line.IsEmpty() )
      code->Add(line);
  }
}
//---------------------------------------------------------------------------

static void dibDataExportRGB(TBitmap* dib, TStrings* code, int bpp, int fmt)
{
  // perform per-row export
  for(int row = 0; row < dib->Height; ++row)
  {
    // ESGUI_COLOR is sized and structured the same as TRGBQuad, so we may use it here
    const ESGUI_COLOR* rowBuff = reinterpret_cast<ESGUI_COLOR*>(dib->ScanLine[row]);
    UnicodeString line;

    // restrict line width to avoid C compiler fuckup
    int lineOutputCount = 0;
    for( int pixel = 0; pixel < dib->Width; ++pixel )
    {
      const ESGUI_COLOR& clr = rowBuff[pixel];

      esU32 out;
      ESGUI_RESULT result = esguiColorToColor(
        &out,
        static_cast<ESGUI_COLOR_FORMAT>(fmt),
        clr.raw,
        esguiClrfmtNative,
        NULL
      );

      if( esguiResultOk != result )
        continue;

      UnicodeString fmt;
      switch( bpp )
      {
      case 8:
        fmt = "0x%0.2X";
        break;
     case 16:
        fmt = "0x%0.4X";
        break;
      default:
        fmt = "0x%0.8X";
        break;
      }

      line += UnicodeString::Format(
        fmt,
        ARRAYOFCONST((
          out
        ))
      );

      line += _T(',');
      ++rowBuff;
      ++lineOutputCount;

      if( lineOutputCount > c_maxLineLength )
      {
        lineOutputCount = 0;
        line += _T('\n');
      }

      line += _T(' ');
    }

    code->Add(line);
  }
}
//---------------------------------------------------------------------------

void EsUtilities::dibPaletteExport(const UnicodeString& baseName, const RGBQUAD* pal, size_t palCnt, TStrings* code, int palFmt)
{
  code->Add("// Bitmap palette");
  code->Add("//");

  UnicodeString dataItemType;
  int bpp = esguiBppFromColorFormat(palFmt);

  switch(bpp)
  {
  case 8:
    dataItemType = "esU8";
    break;
  case 16:
    dataItemType = "esU16";
    break;
  default:
    dataItemType = "esU32";
    break;
  }

  code->Add(
    UnicodeString::Format(
      "static const %s sc_%s_palette_data[] = {",
      ARRAYOFCONST((
        dataItemType.c_str(),
        baseName.c_str()
      ))
    )
  );

  size_t idx = 0;
  UnicodeString palstr;
  while( idx < palCnt )
  {
    size_t top = idx + 10;
    for(; idx < std::min( palCnt, top); ++idx )
    {
      if( !palstr.IsEmpty() )
        palstr += ",";

      const RGBQUAD& pe = pal[idx];
      ESGUI_COLOR clr = esguiColorSet(
        pe.rgbRed,
        pe.rgbGreen,
        pe.rgbBlue
      );

      esU32 out;
      ESGUI_RESULT result = esguiColorToColor(
        &out,
        static_cast<ESGUI_COLOR_FORMAT>(palFmt),
        clr.raw,
        esguiClrfmtNative,
        NULL
      );

      if( esguiResultOk != result )
        continue;

      UnicodeString fmt;
      switch(bpp)
      {
      case 8:
        fmt ="0x%0.2X";
        break;
      case 16:
        fmt = "0x%0.4X";
        break;
      default:
        fmt = "0x%0.8X";
        break;
      }

      palstr += UnicodeString::Format(
        fmt,
        ARRAYOFCONST((
          out
        ))
      );

      if( idx && 0 == (idx % 10) )
        palstr += "\n";
    }
    palstr += "\n";
  }

  code->Add(palstr + _T("};\n"));

  // Palette structure itself
  code->Add(
    UnicodeString::Format(
      "const ESGUI_PALETTE c_%s_palette = {",
      ARRAYOFCONST((
        baseName.c_str()
      ))
    )
  );

  code->Add(
    UnicodeString::Format(
      "%d, // Palette entry color format\nsc_%s_palette_data, // Color data\n%d, // Colors count\n%d // Realized",
      ARRAYOFCONST((
        palFmt,
        baseName.c_str(),
        palCnt,
        0
      ))
    )
  );

  code->Add(_T("};\n"));
}
//---------------------------------------------------------------------------

void EsUtilities::dibDataExport(const UnicodeString& baseName, TBitmap* dib, TStrings* code, int fmt)
{
  int bpp = esguiBppFromColorFormat(fmt);

	code->Add(
    UnicodeString::Format(
      "// Bitmap data\nstatic const esU8 sc_%s_data[] = {",
      ARRAYOFCONST((
        baseName.c_str()
      ))
    )
  );

	if( fmt < esguiClrfmtGrayscale2 )
    dibDataExportPaletted(
      dib,
      code,
      bpp
    );
	else if( fmt < esguiClrfmtRGB332 )
    dibDataExportGrayscale(
      dib,
      code,
      bpp
    );
  else
    dibDataExportRGB(
      dib,
      code,
      bpp,
      fmt
    );

	code->Add("};\n");
}
//---------------------------------------------------------------------------

