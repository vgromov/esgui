#include <esgui/include/esgui.h>
#include <esfwxe/ese_assert.h>

#include <esgui/include/esgui_font.h>
//----------------------------------------------------------------------------------------------

#if 1 == ESGUI_USE_FONT

// Find font block in chain by char, starting from 'block'. If no block is found - return NUULL
static ESGUI_FONT* esguiFontBlockGetByChar(ESGUI_FONT* block, esU32 ch, ESGUI_FONT* prevLookup)
{
  if( !block || ESGUI_FONT_UNSPEC_CHAR == ch )
    return NULL;
    
  if( block->data->chFirst > ch ) //< Got to move to the next block in chain. All blocks are sorted by by char range, by design
  {
    if( prevLookup == block->prev ) //< We already looked up the previous one
      return NULL; //< Not found
    else
      return esguiFontBlockGetByChar(
        block->prev,
        ch,
        block
      );
  }
  else if( block->data->chLast < ch ) //< Got to move to the next block in chain. All blocks are sorted by char range, by design
  {
    if( prevLookup == block->next )
      return NULL;
    else
      return esguiFontBlockGetByChar(
        block->next,
        ch,
        block
      );
  }
  
  return block;
}
//----------------------------------------------------------------------------------------------

void esguiFontInit(ESGUI_FONT* font, const ESGUI_FONT_DATA* block)
{
  ESE_ASSERT(font);
  ESE_ASSERT(block);
  
  font->data = block;
  font->prev = NULL;
  font->next = NULL;
}
//----------------------------------------------------------------------------------------------

void esguiFontBlockAdd(ESGUI_FONT* font, ESGUI_FONT* block)
{
  if( !font || !block )
    return;

  ESE_ASSERT( esguiFontHeightGet(font) == esguiFontHeightGet(block) ); //< Ensure font blocks have equal heights
  ESE_ASSERT( block->data->chLast >= block->data->chFirst ); //< Ensure char range

  // Just in case - unchain block being inserted
  esguiFontBlockRemove(block);

  // Get the first block in chain to which font belongs to
  while( font->prev )
    font = font->prev;

  // TODO: Ensure there will be no overlapping char regions

  // Insert new block in chain, maintaining character range order
  while( font )
  {
    if( font->next && font->next->data->chLast < block->data->chFirst )
      font = font->next;
    else
      break;
  }
  
  ESE_ASSERT( font );
  ESE_ASSERT( font->data->chLast < block->data->chFirst );
  
  ESGUI_FONT* oldNext = font->next;
  font->next = block;
  block->prev = font;
  
  if( oldNext )
    oldNext->prev = block;
    
  block->next = oldNext;
}
//----------------------------------------------------------------------------------------------

void esguiFontBlockRemove(ESGUI_FONT* block)
{
  if( !block )
    return;
    
  ESGUI_FONT* blockPrev = block->prev;
  ESGUI_FONT* blockNext = block->next;
  
  if( blockPrev )
    blockPrev->next = blockNext;
  if( blockNext )
    blockNext->prev = blockPrev;
  
  block->prev = NULL;
  block->next = NULL;
}
//----------------------------------------------------------------------------------------------

bool esguiFontHasChar(const ESGUI_FONT* font, esU32 ch)
{
  if( !font || !font->data )
    return false;

  const ESGUI_FONT* found = esguiFontBlockGetByChar(
    (ESGUI_FONT*)font,
    ch,
    (ESGUI_FONT*)font
  );

  return found != NULL;
}
//----------------------------------------------------------------------------------------------

bool esguiFontGlyphGet(const ESGUI_FONT* font, esU32 ch, ESGUI_BITMAP_ITERATOR* it)
{
  if( !font || !it || !font->data || !font->data->glyphs || !font->data->glyphsMap )
    return false;

  const ESGUI_FONT* found = esguiFontBlockGetByChar(
    (ESGUI_FONT*)font,
    ch,
    (ESGUI_FONT*)font
  );

  if( !found )
    return false;

  ESGUI_RECT view;
  esguiRectSet_(
    found->data->glyphsMap[ch - found->data->chFirst],
    0,
    found->data->glyphsMap[ch - found->data->chFirst + 1]-1,
    esguiBmpHeightGet(found->data->glyphs)-1,
    &view
  );

  return esguiBmpIteratorInit(
    (ESGUI_BITMAP*)found->data->glyphs,
    &view,
    it,
    esguiBmpIteratorRead
  );
}
//----------------------------------------------------------------------------------------------

esU16 esguiFontHeightGet(const ESGUI_FONT* font)
{
  if( !font || !font->data || !font->data->glyphs )
    return 0;

  return esguiBmpHeightGet(font->data->glyphs);
}
//----------------------------------------------------------------------------------------------

esU16 esguiFontWidthGet(const ESGUI_FONT* font, esU32 ch)
{
  if( !font || !font->data )
    return 0;

  if( ' ' == ch )
    return esguiFontSpaceWidthGet(font);

  if( ESGUI_FONT_UNSPEC_CHAR == ch )
    return esguiFontUnspecifiedWidthGet(font);

  const ESGUI_FONT* found = esguiFontBlockGetByChar(
    (ESGUI_FONT*)font,
    ch,
    (ESGUI_FONT*)font
  );

  if( !found )
    return esguiFontUnspecifiedWidthGet(font);

  ch -= found->data->chFirst;
  return found->data->glyphsMap[ch + 1] -
    found->data->glyphsMap[ch];
}
//----------------------------------------------------------------------------------------------

esU16 esguiFontSpaceWidthGet(const ESGUI_FONT* font)
{
  if( !font || !font->data )
    return 0;

  return font->data->spaceWidth;
}
//----------------------------------------------------------------------------------------------

esU16 esguiFontUnspecifiedWidthGet(const ESGUI_FONT* font)
{
  if( !font || !font->data )
    return 0;

  return font->data->unspecifiedWidth;
}
//----------------------------------------------------------------------------------------------

#endif // ESGUI_USE_FONT

