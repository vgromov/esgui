#include <esgui/include/esgui.h>
#include <esfwxe/ese_assert.h>
#include <esgui/include/esgui_widgets.h>
#include <esgui/include/esgui_widget_std.h>
#include <esfwxe/utils.h>
#include <esfwxe/trace.h>

#if defined(ESGUI_WIDGET_USE_STD) && 1 == ESGUI_WIDGET_USE_STD

void esguiWgtStdPaintTitle(ESGUI_HDC hdc, const ESGUI_RECT* rect, const ESGUI_BITMAP* bmp, ESE_CSTR title)
{
  ESGUI_RECT bounds;
  if( !bmp && !title )
    return;

  bounds = *rect;
  bounds.edge.y = bounds.org.y + esguiWgtMetricGet(esguiWgtmTitleHeight);
  esguiCanvasBgColorSet(
    hdc, 
    esguiWgtStdColorGet(
      hdc,
      ESGUI_WgtStdColorBg
    )
  );
  esguiCanvasFillRect(hdc, &bounds);
  
  esguiCanvasFgColorSet(
    hdc, 
    esguiWgtStdColorGet(
      hdc,
      ESGUI_WgtStdColorFgDisabled
    )
  );
  
  if( bmp )
  {
    esguiCanvasDrawPictureROP(
      hdc,
      &bounds.org,
      bmp,
      NULL,
      ESGUI_COPY
    );
    bounds.org.x += bmp->size.x;
  }
  if( title )
  {
    esguiCanvasFontSet(
      hdc,
      esguiWgtStdFontGet(
        hdc,
        ESGUI_WgtStdFontTitle
      )
    );
    esguiCanvasDrawStringRect(
      hdc,
      &bounds,
      title,
      ESGUI_TAB_WIDTH_STD,
      ESGUI_HTA_CENTER|ESGUI_VTA_CENTER|ESGUI_TCLIP
    );
  }
  bounds.org.y = bounds.edge.y;
  esguiCanvasDrawHLine(
    hdc,
    &bounds.org,
    esguiRectWidth(&bounds)
  );
}

// paint vertical scroll bar
void esguiWgtStdPaintVscrollBar(ESGUI_HDC hdc, const ESGUI_RECT* client, int min, int max, int pos)
{
  ESGUI_RECT rect = *client;
  ESGUI_POINT pnt;
  bool activeUp = min != max && pos != min;
  bool activeDn = min != max && pos != max;
  
  ES_DEBUG_TRACE7(
    "esguiWgtStdPaintVscrollBar min=%d, max=%d, pos=%d, client={%d,%d, %d,%d}\n",
    min,
    max, 
    pos,
    client->org.x,
    client->org.y,
    client->edge.x,
    client->edge.y
  )
  
  const ESGUI_BITMAP* bmUp = esguiWgtStdBitmapGet(
    hdc,
    ESGUI_WgtStdBitmapArrowUp
  );
  ESE_ASSERT(bmUp);
  
  const ESGUI_BITMAP* bmDown = esguiWgtStdBitmapGet(
    hdc,
    ESGUI_WgtStdBitmapArrowDown
  );
  ESE_ASSERT(bmDown);
  
  rect.org.x = rect.edge.x-esguiWgtMetricGet(esguiWgtmVscrollWidth);
  esguiCanvasBgColorSet(
    hdc, 
    esguiWgtStdColorGet(
      hdc,
      ESGUI_WgtStdColorBg
    )    
  );
  
  if( activeUp )
    esguiCanvasFgColorSet(
      hdc, 
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorFg
      )
    );
  else
    esguiCanvasFgColorSet(
      hdc, 
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorFgDisabled
      )
    );
  
  esguiCanvasDrawPictureROP(
    hdc, 
    &rect.org, 
    bmUp,
    NULL, 
    ESGUI_COPY
  );
  rect.org.y += esguiBmpHeightGet(bmUp);
  
  if( activeDn )
    esguiCanvasFgColorSet(
      hdc, 
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorFg
      )
    );
  else
    esguiCanvasFgColorSet(
      hdc, 
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorFgDisabled
      )
   );
    
  rect.edge.y -= esguiBmpHeightGet(bmDown);
  pnt.x = rect.org.x;
  pnt.y = rect.edge.y;
  
  esguiCanvasDrawPictureROP(
    hdc, 
    &pnt, 
    bmDown, 
    NULL, 
    ESGUI_COPY
  );
  
  --rect.edge.y;
  esguiCanvasFillRect(hdc, &rect);
  esguiCanvasFgColorSet(
    hdc, 
    esguiWgtStdColorGet(
      hdc,
      ESGUI_WgtStdColorFgDisabled
    )
  );

  esguiCanvasDrawRect(hdc, &rect);

  // calc & draw scroller thumb
  if( min != max && pos >= min && pos <= max )
  {
    size_t height, range, vOffs;
    esguiRectGrow_(-1, -2, &rect);
    height = esguiRectHeight(&rect)-esguiWgtMetricGet(esguiWgtmVscrollThumbHeight);
    range = max-min;
    vOffs = ((pos-min)*height)/range;
    rect.edge.y = rect.org.y+esguiWgtMetricGet(esguiWgtmVscrollThumbHeight);
    esguiRectOffset_(0, vOffs, &rect);
    
    esguiCanvasBgColorSet(
      hdc, 
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorFg
      )
    );
    esguiCanvasFillRect(hdc, &rect);

    esguiCanvasBgColorSet(
      hdc, 
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorBg
      )
    );
  }
}

// paint horizontal scroll bar
void esguiWgtStdPaintHscrollBar(ESGUI_HDC hdc, const ESGUI_RECT* client, int min, int max, int pos)
{
  ESGUI_RECT rect = *client;
  ESGUI_POINT pnt;
  esBL activeL = min != max && pos != min;
  esBL activeR = min != max && pos != max;
  const ESGUI_BITMAP* bmLeft = esguiWgtStdBitmapGet(
    hdc,
    ESGUI_WgtStdBitmapArrowLeft
  );
  ESE_ASSERT(bmLeft);
  
  const ESGUI_BITMAP* bmRight = esguiWgtStdBitmapGet(
    hdc,
    ESGUI_WgtStdBitmapArrowRight
  );
  ESE_ASSERT(bmRight);  
  
  rect.org.y = rect.edge.y-esguiWgtMetricGet(esguiWgtmHscrollHeight);
  
  esguiCanvasBgColorSet(
    hdc, 
    esguiWgtStdColorGet(
      hdc,
      ESGUI_WgtStdColorBg
    )
  );
  
  if( activeL )
    esguiCanvasFgColorSet(
      hdc, 
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorFg
      )
    );
  else
    esguiCanvasFgColorSet(
      hdc, 
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorFgDisabled
      )
    );
    
  esguiCanvasDrawPictureROP(
    hdc, 
    &rect.org, 
    bmLeft, 
    NULL, 
    ESGUI_COPY
  );
  
  rect.org.x += esguiBmpWidthGet(
    bmLeft
  );
  
  if( activeR )
    esguiCanvasFgColorSet(
      hdc, 
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorFg
      )
    );
  else
    esguiCanvasFgColorSet(
      hdc, 
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorFgDisabled
      )
    );
    
  rect.edge.x -= esguiBmpWidthGet(
    bmRight
  );
  pnt.x = rect.edge.x;
  pnt.y = rect.org.y;
  
  esguiCanvasDrawPictureROP(
    hdc, 
    &pnt, 
    bmRight, 
    NULL, 
    ESGUI_COPY
  );
  --rect.edge.x;
  
  esguiCanvasFillRect(hdc, &rect);
  esguiCanvasFgColorSet(
    hdc, 
    esguiWgtStdColorGet(
      hdc,
      ESGUI_WgtStdColorFgDisabled
    )
  );
  esguiCanvasDrawRect(hdc, &rect);
  
  // calc & draw scroller thumb
  if( min != max && pos >= min && pos <= max )
  {
    size_t width, range, hOffs;
    esguiRectGrow_(-2, -1, &rect);
    width = esguiRectWidth(&rect)-esguiWgtMetricGet(esguiWgtmHscrollThumbWidth);
    range = max-min;
    hOffs = ((pos-min)*width)/range;
    rect.edge.x = rect.org.x+esguiWgtMetricGet(esguiWgtmHscrollThumbWidth);
    esguiRectOffset_(hOffs, 0, &rect);
    
    esguiCanvasBgColorSet(
      hdc, 
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorFg
      )
    );
    esguiCanvasFillRect(hdc, &rect);
    
    esguiCanvasBgColorSet(
      hdc, 
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorBg
      )
    );
  }
}

void esguiWgtStdPaintScrollbar(ESGUI_HDC hdc, const ESGUI_WIDGET* This, const ESGUI_RECT* draw, bool vertical)
{
  ESGUI_RECT r = *draw;
  ESGUI_RECT vcr;
  esguiWgtVirtualClientRectGet(This, &vcr);

  // paint scrollbars at the right and|or at the bottom part of the drawing rect
  if( vertical )
  {
    r.edge.x += esguiWgtMetricGet(esguiWgtmVscrollWidth);
    esguiWgtStdPaintVscrollBar(
      hdc,
      &r,
      vcr.org.y,
      vcr.edge.y-esguiRectHeight(draw),
      vcr.org.y+This->voffs.y
    );
  }  
  else
  {
    r.edge.y += esguiWgtMetricGet(esguiWgtmHscrollHeight);
    esguiWgtStdPaintHscrollBar(
      hdc, 
      &r, 
      vcr.org.x, 
      vcr.edge.x-esguiRectWidth(draw), 
      vcr.org.x+This->voffs.x
    );
  }
}

// screen background painter
void esguiWgtStdPaintBg(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect)
{
  esguiCanvasBgColorSet(
    hdc, 
    esguiWgtStdColorGet(
      hdc,
      ESGUI_WgtStdColorBg
    )
  );
  esguiCanvasFillRect(hdc, rect);
}

// draw gui string with smart erase
void esguiWgtStdPaintStringWithSmartErase(ESGUI_HDC hdc, ESE_CSTR str, const ESGUI_POINT* pos, int* prevStrLen)
{
  ESGUI_POINT ext;
  esguiCanvasGetStringExtent(hdc, 0, str, ESGUI_TAB_WIDTH_STD, &ext);
  // clean portions of the old string data if needed
  if( *prevStrLen > ext.x )
  {
    ESGUI_RECT erase;
    esguiRectSet_(0, 0, (*prevStrLen - ext.x) / 2, ext.y, &erase);
    // if rect is empty because of screen granularity, so as org.x equal to edge.x
    // expand it by 1 pixel in x direction
    if( 0 == esguiRectWidth(&erase) )
      ++erase.edge.x;
    esguiRectOffset_(pos->x+ext.x, pos->y, &erase);
    esguiCanvasFillRect(hdc, &erase);
  }
  // update length saver
  *prevStrLen = ext.x;
  esguiCanvasDrawString(hdc, pos, str, ESGUI_TAB_WIDTH_STD);
}

// draw centered gui string with smart erase
void esguiWgtStdPaintCenteredStringWithSmartErase(ESGUI_HDC hdc, ESE_CSTR str, const ESGUI_POINT* org, int areaWidth, int* prevStrLen)
{
  ESGUI_POINT dummy;
  int newBeg;
  esguiCanvasGetStringExtent(hdc, 0, str, ESGUI_TAB_WIDTH_STD, &dummy);
  newBeg = org->x + (areaWidth - dummy.x) / 2;
  // clean portions of the old string data if needed
  if( *prevStrLen > dummy.x )
  {
    ESGUI_RECT erase;
    esguiRectSet_(0, 0, (*prevStrLen - dummy.x) / 2, dummy.y, &erase);
    // if rect is empty because of screen granularity, so as org.x equal to edge.x
    // expand it by 1 pixel in x direction
    if( 0 == esguiRectWidth(&erase) )
      ++erase.edge.x;
    esguiRectOffset_(org->x+(areaWidth - *prevStrLen) / 2, org->y, &erase);
    esguiCanvasFillRect(hdc, &erase);

    esguiRectOffset_(dummy.x+esguiRectWidth(&erase), 0, &erase);
    esguiCanvasFillRect(hdc, &erase);
  }
  // update length saver
  *prevStrLen = dummy.x;
  // output the string itself
  dummy.y = org->y;
  dummy.x = newBeg;
  esguiCanvasDrawString(hdc, &dummy, str, ESGUI_TAB_WIDTH_STD);
}

// draw labeled value starting at pos. value string is started at offs from pos
void esguiWgtStdPaintLabeledVal(ESGUI_HDC hdc, const ESGUI_POINT* pos, int offs, ESE_CSTR lbl, ESE_CSTR val)
{
  ESGUI_POINT start;
  esguiCanvasDrawString(hdc, pos, lbl, ESGUI_TAB_WIDTH_STD);
  esguiCanvasGetStringExtent(hdc, NULL, lbl, ESGUI_TAB_WIDTH_STD, &start);
  if( offs < start.x )
    offs = start.x;
  start = *pos;
  start.x += offs;
  esguiCanvasDrawString(hdc, &start, val, ESGUI_TAB_WIDTH_STD);
}

// value string gets drawn right-aligned relative to the unitRulerOffs
void esguiWgtStdPaintLabeledValWithSmartErase(
  ESGUI_HDC hdc, ESGUI_POINT* pos, int lblRulerOffs, int unitRulerOffs, int* prevLenContainer, int* prevUnitLenContainer,
  ESGUI_COLOR fgnd, ESGUI_COLOR bgnd, ESE_CSTR lbl, const ESGUI_FONT* lblFont, ESE_CSTR valStr, const ESGUI_FONT* valFont,
  ESE_CSTR unit, const ESGUI_FONT* unitFont
)
{
  ESGUI_POINT ext;
  ESGUI_POINT org;

  int lblFontHeight = lbl ?
    (lblFont ?
      esguiFontHeightGet(lblFont) :
      esguiFontHeightGet(
        esguiCanvasFontGet(hdc)
      )
    ) :
    0;

  int valFontHeight = valStr ?
    (valFont ?
      esguiFontHeightGet(valFont) :
      esguiFontHeightGet(
        esguiCanvasFontGet(hdc)
      )
    ) :
    0;

  int unitsFontHeight = unit ?
    (unitFont ?
      esguiFontHeightGet(unitFont) :
      esguiFontHeightGet(
        esguiCanvasFontGet(hdc)
      )
    ) :
    0;

  int totalHeight = esguiUtils_MAX(
    esguiUtils_MAX(
      lblFontHeight,
      valFontHeight
    ),
    unitsFontHeight
  );

  esguiCanvasFgColorSet(hdc, fgnd);
  esguiCanvasGetStringExtent(
    hdc, 
    valFont, 
    valStr, 
    ESGUI_TAB_WIDTH_STD, 
    &ext
  );

  // define value string org
  org.x = pos->x + unitRulerOffs - ext.x;
  org.y = pos->y + totalHeight-ext.y;

  // pre-erase previous string
  if(
    prevLenContainer &&
    *prevLenContainer > ext.x
  )
  {
    ESGUI_RECT erase;
    // define erase rect & fill it with background
    esguiRectSet_(
      org.x-*prevLenContainer+ext.x,
      org.y,
      org.x,
      pos->y + ext.y,
      &erase
    );

    esguiCanvasBgColorSet(hdc, bgnd);
    esguiCanvasFillRect(hdc, &erase);
  }

  // draw value string
  if( valFont )
    esguiCanvasFontSet(hdc, valFont);

  esguiCanvasDrawString(hdc, &org, valStr, ESGUI_TAB_WIDTH_STD);

  // save value string extent
  if( prevLenContainer )
    *prevLenContainer = ext.x;

  // draw label string
  org.x = pos->x;
  org.y = pos->y + totalHeight-lblFontHeight;

  if( lblFont )
    esguiCanvasFontSet(hdc, lblFont);

  esguiCanvasDrawString(hdc, &org, lbl, ESGUI_TAB_WIDTH_STD);

  if( unit )
  {
    esguiCanvasGetStringExtent(hdc, unitFont, unit, ESGUI_TAB_WIDTH_STD, &ext);

    if( prevUnitLenContainer &&
        *prevUnitLenContainer > ext.x )
    {
      ESGUI_RECT erase;
      // define erase rect & fill it with background
      esguiRectSet_( org.x-*prevUnitLenContainer+ext.x, org.y, org.x, pos->y + ext.y, &erase);
      esguiCanvasBgColorSet(hdc, bgnd);
       esguiCanvasFillRect(hdc, &erase);
    }

    // draw units string
    org.x = pos->x + unitRulerOffs + esguiWgtMetricGet(esguiWgtmHspace);
    org.y = pos->y + totalHeight-unitsFontHeight;

    // draw units string
    if( unitFont )
      esguiCanvasFontSet(hdc, unitFont);
    esguiCanvasDrawString(hdc, &org, unit, ESGUI_TAB_WIDTH_STD);

    // save value string extent
    if( prevUnitLenContainer )
      *prevUnitLenContainer = ext.x;
  }

  // update pos
  pos->y += totalHeight+esguiWgtMetricGet(esguiWgtmVspace);
}

// generic button painter
void esguiWgtStdButtonPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect)
{
  ESGUI_POINT pos;
  if( This->data )
  {
    ESGUI_RECT focus;
    esguiCanvasFontSet(
      hdc,
      esguiWgtStdFontGet(
        hdc,
        ESGUI_WgtStdFontCtl
      )
    );
    esguiCanvasGetStringExtent(
      hdc,
      NULL,
      (ESE_CSTR)This->data,
      ESGUI_TAB_WIDTH_STD,
      &pos
    );
    esguiRectSet_(0, 0, pos.x, pos.y, &focus);
    esguiRectGrow_(1, -1, &focus);

    // center string
    pos.y = rect->org.y;
    pos.x = rect->org.x + (esguiRectWidth(rect) - pos.x) / 2;
    esguiRectOffset(&pos, &focus);

    esguiCanvasBgColorSet(
      hdc, 
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorBg
      )
    );
    if( !esguiWgtIsFocused(This) )
    {
      esguiCanvasFgColorSet(
        hdc, 
        esguiWgtStdColorGet(
          hdc,
          ESGUI_WgtStdColorBg
        )
      );
      
      // erase focus rect
      esguiCanvasDrawRect(hdc, &focus);
    }

    if( ES_BIT_IS_SET(This->flags, esguiWgtfEnabled) )
      esguiCanvasFgColorSet(
        hdc, 
        esguiWgtStdColorGet(
          hdc,
          ESGUI_WgtStdColorFg
        )
      );
    else
      esguiCanvasFgColorSet(
        hdc, 
        esguiWgtStdColorGet(
          hdc,
          ESGUI_WgtStdColorFgDisabled
        )
      );

    esguiCanvasDrawString(
      hdc,
      &pos,
      (ESE_CSTR)This->data,
      ESGUI_TAB_WIDTH_STD
    );

    if( 
      esguiWgtIsFocused(This) &&
      ES_BIT_IS_SET(This->flags, esguiWgtfEnabled) 
    )
    {
      esguiCanvasFgColorSet(
        hdc, 
        esguiWgtStdColorGet(
          hdc,
          ESGUI_WgtStdColorFg
        )
      );
      
      esguiCanvasDrawRect(
        hdc, 
        &focus
      );
    }
  }
}

#endif //< ESGUI_WIDGET_USE_STD
