#ifndef _es_gui_font_h_
#define _es_gui_font_h_

#if defined(ESGUI_USE_FONT) && 1 == ESGUI_USE_FONT

#ifdef __cplusplus
  extern "C" {
#endif

/// Unspecified char UTF-32 code
#define ESGUI_FONT_UNSPEC_CHAR  ((esU32)0xFFFFFFFF)

/// Font block structure.
/// Font Block defines a continuous bitmap data of UTF32 symbols
/// in chFirst .. chLast range.
/// Symbols glyphs offsets in block are defined by ESGUI_FONT_GLYPH_DESCRIPTOR map data.
/// The width of each symbol in block range may be found as:
/// glyphsMap[ch-chFirst+1]-glyphsMap[ch-chFirst]
///
typedef struct tagESGUI_FONT_DATA
{
  const struct tagESGUI_BITMAP* glyphs; //< Selected continuous font block glyphs
  esU32 chFirst;          ///< The first UTF32 character in block. NB! block should be continuous
  esU32 chLast;           ///< The last UTF32 character in block
  esU16 spaceWidth;       ///< Space characted width in pixels
  esU16 unspecifiedWidth; ///< Unspecified (unsupported) character width in pixels
  const esU16* glyphsMap; ///< The glyphs map table should contain +1 element with offset to the end of the glyps data
    
} ESGUI_FONT_DATA;

typedef struct tagESGUI_FONT
{
  const ESGUI_FONT_DATA* data;

  struct tagESGUI_FONT* prev; //< Prev font block (optional, for multi-block font definitions)
  struct tagESGUI_FONT* next; //< Next font block (optional, for multi-block font definitions)

} ESGUI_FONT;

/// Initialize font structure block
void esguiFontInit(ESGUI_FONT* font, const ESGUI_FONT_DATA* block);

/// Append font sub-block to the existing font block chain
void esguiFontBlockAdd(ESGUI_FONT* font, ESGUI_FONT* block);

/// Remove font block from the chain
void esguiFontBlockRemove(ESGUI_FONT* block);

/// Return true if font has character with code ch
bool esguiFontHasChar(const ESGUI_FONT* font, esU32 ch);

/// Return font glyph data bitmap
bool esguiFontGlyphGet(const ESGUI_FONT* font, esU32 ch, struct tagESGUI_BITMAP_ITERATOR* it);

/// Return font character width
esU16 esguiFontWidthGet(const ESGUI_FONT* font, esU32 ch);

/// Get total font height
esU16 esguiFontHeightGet(const ESGUI_FONT* font);

/// Get space width for font block
esU16 esguiFontSpaceWidthGet(const ESGUI_FONT* font);

/// Get unspecified (unsupported) char width for font block
esU16 esguiFontUnspecifiedWidthGet(const ESGUI_FONT* font);

#ifdef __cplusplus
  }
#endif 

#endif // ESGUI_USE_FONT

#endif // _es_gui_font_h_
