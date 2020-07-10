#include <esgui/include/esgui.h>
#include <string.h>
//----------------------------------------------------------------------------------------------

#if defined(ESGUI_USE_BITMAP) && 1 == ESGUI_USE_BITMAP

bool esguiBmpIsEmpty( const ESGUI_BITMAP* bmp )
{
  if( !bmp || !bmp->data || 0 == bmp->size.x || 0 == bmp->size.y )
    return true;

  return false;
}
//---------------------------------------------------------------------------

ESGUI_COLOR_FORMAT esguiBmpColorFormatGet( const ESGUI_BITMAP* bmp )
{
  if( !bmp )
    return esguiClrfmtInvalid;

  return (ESGUI_COLOR_FORMAT)bmp->hdr.colorFmt;
}
//---------------------------------------------------------------------------

bool esguiBmpHasTransparentColor( const ESGUI_BITMAP* bmp )
{
  if( !bmp )
    return false;

  return 0 != bmp->hdr.haveTransparentColor;
}
//---------------------------------------------------------------------------

esU32 esguiBmpTransparentColorGet( const ESGUI_BITMAP* bmp )
{
  if( !bmp )
    return 0;

  return bmp->hdr.clrTransparent;
}
//---------------------------------------------------------------------------

int esguiBmpWidthGet(const ESGUI_BITMAP* bmp)
{
  if(!bmp)
    return 0;

  return bmp->size.x;
}
//---------------------------------------------------------------------------

int esguiBmpHeightGet(const ESGUI_BITMAP* bmp)
{
  if(!bmp)
    return 0;

  return bmp->size.y;
}
//---------------------------------------------------------------------------

ESGUI_POINT esguiBmpSizeGet(const ESGUI_BITMAP* bmp)
{
  if( !bmp )
    return c_zeroPoint;

  return bmp->size;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

// packed pixel access masks and multipliers
typedef struct
{
  esU8 mask;
  esU8 shift;

} ESGUI_MASK_MUL;
//----------------------------------------------------------------------------------------------

// masks and multiplier constants for bit-packed bitmap access
const ESGUI_MASK_MUL c_1bppMaskShift[] =
  { {0x80, 7},
    {0x40, 6},
    {0x20, 5},
    {0x10, 4},
    {0x08, 3},
    {0x04, 2},
    {0x02, 1},
    {0x01, 0} };
//----------------------------------------------------------------------------------------------

const ESGUI_MASK_MUL c_2bppMaskShift[] =
  { {0xC0, 6},
    {0x30, 4},
    {0x0C, 2},
    {0x03, 0} };
//----------------------------------------------------------------------------------------------

const ESGUI_MASK_MUL c_4bppMaskShift[] =
  { {0xF0, 4},
    {0x0F, 0} };
//----------------------------------------------------------------------------------------------

static inline esU32 bpp1IteratorGetPixel( const ESGUI_BITMAP_ITERATOR* iter )
{  
  esU8 mask = c_1bppMaskShift[iter->pos % 8].mask;
  
  return (mask == (iter->bmp->data[iter->pos / 8] & mask)) ? 1 : 0;
}
//----------------------------------------------------------------------------------------------

static inline esU32 bpp2IteratorGetPixel( const ESGUI_BITMAP_ITERATOR* iter )
{  
  esU8 pixelIdx = iter->pos % 4;
  return ((iter->bmp->data[iter->pos / 4] & c_2bppMaskShift[pixelIdx].mask) >> c_2bppMaskShift[pixelIdx].shift);
}
//----------------------------------------------------------------------------------------------

static inline esU32 bpp4IteratorGetPixel( const ESGUI_BITMAP_ITERATOR* iter )
{
  esU8 pixelIdx = iter->pos % 2;
  return ((iter->bmp->data[iter->pos / 2] & c_4bppMaskShift[pixelIdx].mask) >> c_4bppMaskShift[pixelIdx].shift);
}
//----------------------------------------------------------------------------------------------

static inline esU32 bpp8IteratorGetPixel( const ESGUI_BITMAP_ITERATOR* iter )
{
  return *(iter->bmp->data + iter->pos);
}
//----------------------------------------------------------------------------------------------

static inline esU32 bpp16IteratorGetPixel( const ESGUI_BITMAP_ITERATOR* iter )
{
  return *(((esU16*)iter->bmp->data) + iter->pos);
}
//----------------------------------------------------------------------------------------------

static inline esU32 bpp24IteratorGetPixel( const ESGUI_BITMAP_ITERATOR* iter )
{
  // due to alignment problems, 24bpp must be processed differently
  union {  
    esU32 raw;
    esU8 bb[3];
  } out;
  
  const esU8* pos = iter->bmp->data + (iter->pos * 3);
  out.bb[0] = *pos++;
  out.bb[1] = *pos++;
  out.bb[2] = *pos;
  
  return out.raw;
}
//----------------------------------------------------------------------------------------------

static esU32 bpp32IteratorGetPixel( const ESGUI_BITMAP_ITERATOR* iter )
{
  return *(((esU32*)iter->bmp->data) + iter->pos);
}
//----------------------------------------------------------------------------------------------

static inline void bpp1IteratorSetPixel( const ESGUI_BITMAP_ITERATOR* iter, esU32 val, ESGUI_BITOP bitop )
{  
  esU8 mask = c_1bppMaskShift[iter->pos % 8].mask;
  
  if( val )
    iter->bmp->data[iter->pos / 8] |= mask;
  else
    iter->bmp->data[iter->pos / 8] &= ~mask;
}
//----------------------------------------------------------------------------------------------

static inline void bpp2IteratorSetPixel( const ESGUI_BITMAP_ITERATOR* iter, esU32 val, ESGUI_BITOP bitop )
{  
  esU8 pixelIdx = iter->pos % 4;
  esU8 mask = c_2bppMaskShift[pixelIdx].mask;
  esU8 dest = iter->bmp->data[iter->pos / 4];
  switch( bitop )
  {
  case ESGUI_COPY:
    dest &= ~mask;
    dest |= ((val << c_2bppMaskShift[pixelIdx].shift) & mask);
    break;
  case ESGUI_AND:
    dest &= ((val << c_2bppMaskShift[pixelIdx].shift) & mask);
    break;
  case ESGUI_OR:
    dest |= ((val << c_2bppMaskShift[pixelIdx].shift) & mask);
    break;
  case ESGUI_XOR:
    dest ^= ((val << c_2bppMaskShift[pixelIdx].shift) & mask);
    break;
  default:
    break;
  }
  
  iter->bmp->data[iter->pos / 4] = dest;
}
//----------------------------------------------------------------------------------------------

static inline void bpp4IteratorSetPixel( const ESGUI_BITMAP_ITERATOR* iter, esU32 val, ESGUI_BITOP bitop )
{
  esU8 pixelIdx = iter->pos % 2;
  esU8 mask = c_4bppMaskShift[pixelIdx].mask;
  esU8 dest = iter->bmp->data[iter->pos / 2];
  switch( bitop )
  {
  case ESGUI_COPY:
    dest &= ~mask;
    dest |= ((val << c_4bppMaskShift[pixelIdx].shift) & mask);
    break;
  case ESGUI_AND:
    dest &= ((val << c_4bppMaskShift[pixelIdx].shift) & mask);
    break;
  case ESGUI_OR:
    dest |= ((val << c_4bppMaskShift[pixelIdx].shift) & mask);
    break;
  case ESGUI_XOR:
    dest ^= ((val << c_4bppMaskShift[pixelIdx].shift) & mask);
    break;
  default:
    break;
  }
  
  iter->bmp->data[iter->pos / 2] = dest;
}
//----------------------------------------------------------------------------------------------

static inline void bpp8IteratorSetPixel( const ESGUI_BITMAP_ITERATOR* iter, esU32 val, ESGUI_BITOP bitop )
{
  switch( bitop )
  {
  case ESGUI_COPY:
    *(iter->bmp->data + iter->pos) = (val & 0xFF);
    break;
  case ESGUI_AND:
    *(iter->bmp->data + iter->pos) &= (val & 0xFF);
    break;
  case ESGUI_OR:
    *(iter->bmp->data + iter->pos) |= (val & 0xFF);
    break;
  case ESGUI_XOR:
    *(iter->bmp->data + iter->pos) ^= (val & 0xFF);
    break;
  default:
    break;
  }
}
//----------------------------------------------------------------------------------------------

static inline void bpp16IteratorSetPixel( const ESGUI_BITMAP_ITERATOR* iter, esU32 val, ESGUI_BITOP bitop )
{
  switch( bitop )
  {
  case ESGUI_COPY:
    *((esU16*)(iter->bmp->data + iter->pos)) = (val & 0xFFFF);
    break;
  case ESGUI_AND:
    *((esU16*)(iter->bmp->data + iter->pos)) &= (val & 0xFFFF);
    break;
  case ESGUI_OR:
    *((esU16*)(iter->bmp->data + iter->pos)) |= (val & 0xFFFF);
    break;
  case ESGUI_XOR:
    *((esU16*)(iter->bmp->data + iter->pos)) ^= (val & 0xFFFF);
    break;
  default:
    break;
  }
}
//----------------------------------------------------------------------------------------------

static inline void bpp24IteratorSetPixel( const ESGUI_BITMAP_ITERATOR* iter, esU32 val, ESGUI_BITOP bitop )
{
  // due to alignment problems, 24bpp must be processed differently
  union {  
    esU32 raw;
    esU8 bb[3];
  } in;
  
  esU8* beg = iter->bmp->data + (iter->pos * 3);
  esU8* pos = beg;
  
  in.bb[0] = *pos++;
  in.bb[1] = *pos++;
  in.bb[2] = *pos;

  switch( bitop )
  {
  case ESGUI_COPY:
    in.raw = (val & 0xFFFFFF);
    break;
  case ESGUI_AND:
    in.raw &= (val & 0xFFFFFF);
    break;
  case ESGUI_OR:
    in.raw |= (val & 0xFFFFFF);
    break;
  case ESGUI_XOR:
    in.raw ^= (val & 0xFFFFFF);
    break;
  default:
    break;
  }

  pos = beg;
  *pos++ = in.bb[0];
  *pos++ = in.bb[1];
  *pos   = in.bb[2];
}
//----------------------------------------------------------------------------------------------

static void bpp32IteratorSetPixel( const ESGUI_BITMAP_ITERATOR* iter, esU32 val, ESGUI_BITOP bitop )
{
  esU32* dest = ((esU32*)iter->bmp->data) + iter->pos;

  ESGUI_COLOR_FORMAT fmt = esguiBmpIteratorColorFmtGet(iter);
  esguiColorOp(
    dest,
    *dest,
    fmt,
    val,
    fmt,
    NULL,
    0,
    esguiClrfmtInvalid,
    NULL,
    bitop
  );
}
//----------------------------------------------------------------------------------------------

bool esguiBmpIteratorIsOk( const ESGUI_BITMAP_ITERATOR* iter )
{
  if( !iter )
    return false;

  if( !iter->bmp )
    return false;

  if( NULL == iter->setPixel && NULL == iter->getPixel )
    return false;

  return true;
}
//---------------------------------------------------------------------------

void esguiBmpIteratorZero( ESGUI_BITMAP_ITERATOR* iter )
{
  if( !iter )
    return;

  memset(
    iter,
    0,
    sizeof(ESGUI_BITMAP_ITERATOR)
  );
}
//---------------------------------------------------------------------------

bool esguiBmpIteratorInit( ESGUI_BITMAP* bmp, ESGUI_RECT* view, ESGUI_BITMAP_ITERATOR* iter, esguiBmpIteratorKind kind )
{
  ESGUI_RECT bmpRect;
  esU32 pixelsPerByte;

  // zero-out entire iterator
  esguiBmpIteratorZero( iter );

  if( !bmp )
    return FALSE;

  int bpp = esguiBppFromColorFormat(
    esguiBmpColorFormatGet(bmp)
  );

  // how many pixels per esU8 do we have
  pixelsPerByte = 8 / bpp;
  
  esguiBmpViewRectGet_(
    bmp,
    -1, -1, -1, -1,
    &bmpRect
  );

  // initialize members
  //
  // source bitmap
  iter->bmp = bmp;

  if( !view )
    iter->view = bmpRect;
  else if( !esguiRectIntersect( &bmpRect, view, &iter->view) )
    return FALSE;

  // take possible line padding for bit-packed bitmaps into account
  if( pixelsPerByte > 1 )
  {
    esU32 lastPixels = esguiBmpWidthGet(bmp) % pixelsPerByte;
    if( lastPixels )
      iter->pixelPadding = pixelsPerByte - lastPixels;
  }

  // data end pos
  iter->end = (esguiBmpWidthGet(bmp) + iter->pixelPadding)*iter->view.edge.y +
    iter->view.edge.x + 1;

  // pixel getters
  switch( bpp )
  {
  case 1:
    if( esguiBmpIteratorRead & kind )
      iter->getPixel = bpp1IteratorGetPixel;

    if( esguiBmpIteratorWrite & kind )
      iter->setPixel = bpp1IteratorSetPixel;

    break;
  case 2:
    if( esguiBmpIteratorRead & kind )
      iter->getPixel = bpp2IteratorGetPixel;

    if( esguiBmpIteratorWrite & kind )
      iter->setPixel = bpp2IteratorSetPixel;

    break;
  case 4:
    if( esguiBmpIteratorRead & kind )
      iter->getPixel = bpp4IteratorGetPixel;

    if( esguiBmpIteratorWrite & kind )
      iter->setPixel = bpp4IteratorSetPixel;

    break;
  case 8:
    if( esguiBmpIteratorRead & kind )
      iter->getPixel = bpp8IteratorGetPixel;

    if( esguiBmpIteratorWrite & kind )
      iter->setPixel = bpp8IteratorSetPixel;

    break;
  case 16:
    if( esguiBmpIteratorRead & kind )
      iter->getPixel = bpp16IteratorGetPixel;

    if( esguiBmpIteratorWrite & kind )
      iter->setPixel = bpp16IteratorSetPixel;

    break;
  case 24:
    if( esguiBmpIteratorRead & kind )
      iter->getPixel = bpp24IteratorGetPixel;

    if( esguiBmpIteratorWrite & kind )
      iter->setPixel = bpp24IteratorSetPixel;

    break;
  default: // 32
    if( esguiBmpIteratorRead & kind )
      iter->getPixel = bpp32IteratorGetPixel;

    if( esguiBmpIteratorWrite & kind )
      iter->setPixel = bpp32IteratorSetPixel;

    break;
  }

  return TRUE;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR_FORMAT esguiBmpIteratorColorFmtGet(const ESGUI_BITMAP_ITERATOR* iter)
{
  if( !esguiBmpIteratorIsOk(iter) )
    return esguiClrfmtInvalid;

  return esguiBmpColorFormatGet(iter->bmp);
}
//---------------------------------------------------------------------------

void esguiBmpIteratorFirst( ESGUI_BITMAP_ITERATOR* iter )
{
  if( !esguiBmpIteratorIsOk(iter) )
    return;

  // iteration position & helpers
  iter->lineStart = iter->view.org.x + 
    iter->view.org.y * (esguiBmpWidthGet(iter->bmp) + iter->pixelPadding);
  iter->lineEnd = iter->lineStart + esguiRectWidth(&iter->view)+1;
  iter->pos = iter->lineStart;
}
//----------------------------------------------------------------------------------------------

static inline bool esguiBmpIteratorIsEnd(const ESGUI_BITMAP_ITERATOR* it)
{
  if( it->pos >= it->end )
    return true;

  return false;
}
//---------------------------------------------------------------------------

static bool esguiBmpIteratorNextDefault(ESGUI_BITMAP_ITERATOR* it)
{
  ++it->pos;

  if( it->pos < it->lineEnd )
    return true;

  int width = esguiRectWidth(&it->view)+1;

  // go to the next line
  it->lineStart = it->lineEnd + it->pixelPadding +
    esguiBmpWidthGet(it->bmp) - width;
  it->lineEnd = it->lineStart + width;
  it->pos = it->lineStart;

  if( esguiBmpIteratorIsEnd(it) )
    return false;

  return true;
}
//---------------------------------------------------------------------------

bool esguiBmpIteratorNext( ESGUI_BITMAP_ITERATOR* iter )
{
  if( !esguiBmpIteratorIsOk(iter) )
    return false;

  if( esguiBmpIteratorIsEnd(iter) )
    return false;

  if( iter->next )
    return iter->next(iter);

  return esguiBmpIteratorNextDefault(iter);
}
//----------------------------------------------------------------------------------------------

bool esguiBmpIteratorSetPos( ESGUI_BITMAP_ITERATOR* iter, ESGUI_POINT* pos )
{
  if( !esguiBmpIteratorIsOk(iter) )
    return false;

  if( esguiPointInRect(pos, &iter->view) )
  {
    int width = esguiBmpWidthGet(iter->bmp) + iter->pixelPadding;
    iter->pos = pos->x + pos->y * width;
    iter->lineStart = (iter->pos / width) * width;
    iter->lineEnd = iter->lineStart + esguiRectWidth(&iter->view);

    return !esguiBmpIteratorIsEnd(iter);
  }

  return false;
}
//----------------------------------------------------------------------------------------------

bool esguiBmpIteratorGetPos( ESGUI_BITMAP_ITERATOR* iter, ESGUI_POINT* pos )
{
  if( !esguiBmpIteratorIsOk(iter) || esguiBmpIteratorIsEnd(iter) )
    return false;

  int width = iter->bmp->size.x + iter->pixelPadding;
  pos->x = iter->pos % width;
  pos->y = iter->pos / width;

  return true;
}
//----------------------------------------------------------------------------------------------

bool esguiBmpIteratorGetPixel( const ESGUI_BITMAP_ITERATOR* iter, esU32* clr)
{
  if( !esguiBmpIteratorIsOk(iter) || esguiBmpIteratorIsEnd(iter) || !iter->getPixel )
    return false;

  *clr = iter->getPixel(iter);
  return true;
}
//----------------------------------------------------------------------------------------------

bool esguiBmpIteratorSetPixel( ESGUI_BITMAP_ITERATOR* iter, esU32 clr, ESGUI_BITOP bitop )
{
  if( !esguiBmpIteratorIsOk(iter) || esguiBmpIteratorIsEnd(iter) || !iter->setPixel )
    return false;

  iter->setPixel(
    iter,
    clr,
    bitop
  );

  return true;
}
//----------------------------------------------------------------------------------------------

void esguiBmpViewRectGet_(const ESGUI_BITMAP* bmp, int x, int y, int width, int height, ESGUI_RECT* view)
{
  if( !bmp || !view )
    return;
    
  esguiPointZeroSet( &view->org );
  esguiRectExtentSet(
    &bmp->size,
    view
  );
  --view->edge.x;
  --view->edge.y;

  if( 0 > x )
    x = 0;

  if( 0 > y )
    y = 0;
  
  --width;
  if( 0 > width )
    width = esguiRectWidth(view);
  
  --height;
  if( 0 > height )
    height = esguiRectHeight(view);

  ESGUI_RECT rect;
  esguiRectSet_(
    x,
    y,
    x+width,
    y+height,
    &rect
  );
  
  if(
    !esguiRectIntersect(
      &rect,
      view,
      view
    )
  )
    esguiRectZeroSet(view);
}
//----------------------------------------------------------------------------------------------

void esguiBmpViewRectGetFromPosExt(const ESGUI_BITMAP* bmp, const ESGUI_POINT* pos, const ESGUI_POINT* ext, ESGUI_RECT* view)
{
  esguiBmpViewRectGet_(
    bmp, 
    pos ? 
      pos->x :
      -1,
    pos ?
      pos->y :
      -1,
    ext ?
      ext->x :
      -1,
    ext ?
      ext->y :
      -1,
    view
  );
}
//----------------------------------------------------------------------------------------------

void esguiBmpViewRectGet(const ESGUI_BITMAP* bmp, const ESGUI_RECT* rect, ESGUI_RECT* view)
{
  esguiBmpViewRectGet_(
    bmp, 
    rect ?
      rect->org.x :
      -1,
    rect ?
      rect->org.y :
      -1,
    rect ?
      esguiRectWidth(rect) :
      -1,
    rect ?
      esguiRectHeight(rect) :
      -1,
    view
  );
}
//----------------------------------------------------------------------------------------------

#endif // ESGUI_USE_BITMAP

