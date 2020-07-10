#ifndef _es_gui_bitmap_h_
#define _es_gui_bitmap_h_

#if defined(ESGUI_USE_BITMAP) && 1 == ESGUI_USE_BITMAP

#ifdef __cplusplus
  extern "C" {
#endif

typedef enum {

  esguiBmpIteratorRead      = 0x01,
  esguiBmpIteratorWrite     = 0x02,
  esguiBmpIteratorReadWrite = esguiBmpIteratorRead|esguiBmpIteratorWrite

} esguiBmpIteratorKind;

typedef struct tagESGUI_BITMAP_HDR {
  esU32             colorFmt              : 8; //< ESGUI_COLOR_FORMAT
  esU32             haveTransparentColor  : 1;
  esU32             clrTransparent;
  
} ESGUI_BITMAP_HDR;

// bitmap struct
typedef struct tagESGUI_BITMAP {
  ESGUI_POINT       size;
  ESGUI_BITMAP_HDR  hdr;
  esU8*             data;
  
} ESGUI_BITMAP;

// packed bitmap access iterators
//
typedef struct tagESGUI_BITMAP_ITERATOR 
{
  // source bitmap
  ESGUI_BITMAP* bmp;
  // source bitmap window bounds
  ESGUI_RECT    view;
  // line pixel padding for bit-packed bitmaps
  esU32         pixelPadding;
  // iteration position & helpers
  int           lineStart;
  int           lineEnd;
  int           end;
  int           pos;

  bool          (*next)(struct tagESGUI_BITMAP_ITERATOR* it);
  void          (*setPixel)( const struct tagESGUI_BITMAP_ITERATOR* it, esU32 raw, ESGUI_BITOP bitop);
  esU32         (*getPixel)( const struct tagESGUI_BITMAP_ITERATOR* it);

} ESGUI_BITMAP_ITERATOR;

/// Check if bitmap's data and|or size is empty
bool esguiBmpIsEmpty( const ESGUI_BITMAP* bmp );

/// Return bitmap's color format
ESGUI_COLOR_FORMAT esguiBmpColorFormatGet( const ESGUI_BITMAP* bmp );

/// Return true if bitmap have transparent color assigned, false otherwise
bool esguiBmpHasTransparentColor( const ESGUI_BITMAP* bmp );

/// Return bitmap transparent color in bitmpa's color format
esU32 esguiBmpTransparentColorGet( const ESGUI_BITMAP* bmp );

/// Return bitmap width
int esguiBmpWidthGet(const ESGUI_BITMAP* bmp);

/// Return bitmap height
int esguiBmpHeightGet(const ESGUI_BITMAP* bmp);

/// Return bitmap size
ESGUI_POINT esguiBmpSizeGet(const ESGUI_BITMAP* bmp);

/// Embedded bitmap iterator interface
///

/// Iterator validity check
bool esguiBmpIteratorIsOk( const ESGUI_BITMAP_ITERATOR* iter );

/// Zero-out entire iterator
void esguiBmpIteratorZero( ESGUI_BITMAP_ITERATOR* iter );

/// Iterator initialization
bool esguiBmpIteratorInit( ESGUI_BITMAP* bmp, ESGUI_RECT* view, ESGUI_BITMAP_ITERATOR* iter, esguiBmpIteratorKind kind );

/// Return a color format used in bitmap being iterated
ESGUI_COLOR_FORMAT esguiBmpIteratorColorFmtGet(const ESGUI_BITMAP_ITERATOR* iter);

/// Forwad access iterator
void esguiBmpIteratorFirst( ESGUI_BITMAP_ITERATOR* iter );
bool esguiBmpIteratorNext( ESGUI_BITMAP_ITERATOR* iter );

/// Random access iterator
bool esguiBmpIteratorSetPos( ESGUI_BITMAP_ITERATOR* iter, ESGUI_POINT* pos );
bool esguiBmpIteratorGetPos( ESGUI_BITMAP_ITERATOR* iter, ESGUI_POINT* pos );

bool esguiBmpIteratorSetPixel( ESGUI_BITMAP_ITERATOR* iter, esU32 clr, ESGUI_BITOP bitop );
bool esguiBmpIteratorGetPixel( const ESGUI_BITMAP_ITERATOR* iter, esU32* clr);

/// Initialize bitmap view rect
void esguiBmpViewRectGet_(const ESGUI_BITMAP* bmp, int x, int y, int width, int height, ESGUI_RECT* view);
void esguiBmpViewRectGetFromPosExt(const ESGUI_BITMAP* bmp, const ESGUI_POINT* pos, const ESGUI_POINT* ext, ESGUI_RECT* view);
void esguiBmpViewRectGet(const ESGUI_BITMAP* bmp, const ESGUI_RECT* rect, ESGUI_RECT* view);

#ifdef __cplusplus
  }
#endif

#endif

#endif // _es_gui_bitmap_h_

