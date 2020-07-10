#ifndef _es_dib_utilities_h_
#define _es_dib_utilities_h_

#include <vcl.h>
#include <tchar.h>
#include <esgui/include/esgui.h>
#include <Vcl.Graphics.hpp>

namespace EsUtilities
{
void dibToGrayscale(TBitmap* dib, int clrFmt);
void dibToRGB(TBitmap* dib, int clrFmt);
void dibPaletteExport(const UnicodeString& baseName, const RGBQUAD* pal, size_t palCnt, TStrings* code, int palFmt);
void dibDataExport(const UnicodeString& baseName, TBitmap* dib, TStrings* code, int fmt);
}

#endif // _es_dib_utilities_h_
