#include <esgui/include/esgui.h>
#include <esgui/include/esgui_widgets.h>
#include <esgui/include/esgui_widget_checkbox.h>
#include <esfwxe/utils.h>

#if defined(ESGUI_WIDGET_USE_CHECKBOX) && 1 == ESGUI_WIDGET_USE_CHECKBOX

void esguiWgtCheckBoxOnPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect)
{
  ESGUI_RECT focus, bounds = *rect;
  int hspace = esguiWgtMetricGet(esguiWgtmHspace);
  int vspace = esguiWgtMetricGet(esguiWgtmVspace);
  int lblMinOffs = esguiWgtMetricGet(esguiWgtmStdMinLabelOffs);
  
  const ESGUI_FONT* fnt = esguiWgtStdFontGet(
    hdc,
    ESGUI_WgtStdFontCtl
  );
  ESE_ASSERT(fnt);
  
  int wh = esguiFontHeightGet(fnt);

  const ESGUI_BITMAP* chk = esguiWgtStdBitmapGet(
    hdc,
    ESGUI_WgtStdBitmapCheckbox
  );
  ESE_ASSERT(chk);
  
  ESGUI_POINT pnt;
  
  esguiCanvasFontSet(
    hdc,
    fnt
  );
  esguiCanvasGetStringExtent(
    hdc,
    0,
    (ESE_CSTR)This->data,
    ESGUI_TAB_WIDTH_STD,
    &pnt
  );

  esguiCanvasBgColorSet(
    hdc,
    esguiWgtStdColorGet(
      hdc,
      ESGUI_WgtStdColorBg
    )
  );
  esguiCanvasFgColorSet(
    hdc,
    esguiWgtStdColorGet(
      hdc,
      ESGUI_WgtStdColorFg
    )
  );
  
  // calculate focus rect
  focus.org = rect->org;
  focus.edge.x = focus.org.x + pnt.x + 2;
  focus.edge.y = focus.org.y + pnt.y;
  esguiRectOffset_(1, 0, &focus);
  
  // erase focus rect
  if( !esguiWgtIsFocused(This) )
  {
    esguiCanvasFgColorSet(
      hdc,
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorBg
      )
    );
    esguiCanvasDrawRect(
      hdc,
      &focus
    );
    esguiCanvasFgColorSet(
      hdc,
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorFg
      )
    );
  }
  
  // draw label
  bounds.org.x += hspace;
  esguiCanvasDrawString(
    hdc,
    &bounds.org,
    (ESE_CSTR)This->data,
    ESGUI_TAB_WIDTH_STD
  );
  bounds.org.x += MAX(pnt.x, lblMinOffs) + 
    ((pnt.x > lblMinOffs) ? 
       hspace : 
       0
    );

  // draw check box
  pnt.x = bounds.org.x + hspace;
  pnt.y = bounds.org.y + (wh-esguiBmpHeightGet(chk))/2;
  
  esguiPointZeroSet( &bounds.org );
  bounds.edge.x = esguiBmpWidthGet(chk)/2;
  bounds.edge.y = esguiBmpHeightGet(chk);
  
  if( ES_BIT_IS_SET(This->flags, esguiWgtfChecked) )
    esguiRectOffset_(
      esguiBmpWidthGet(chk)/2,
      0, 
      &bounds
    );
    
  esguiCanvasDrawPictureROP(
    hdc, 
    &pnt, 
    chk, 
    &bounds, 
    ESGUI_COPY
  );
  
  // draw focus rectangle
  if( 
    esguiWgtIsFocused(This) &&
    esguiWgtIsEnabled(This) 
  )
    esguiCanvasDrawRect(hdc, &focus);
}

ESGUI_WIDGET* esguiWgtCheckBoxCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, ESE_CSTR lbl, bool check)
{
  ESGUI_WIDGET* result = esguiWgtCreate(
    root,
    ESGUI_CHECK_BUTTON,
    bounds,
    check ?
      esguiWgtfDefault|esguiWgtfChecked :
      esguiWgtfDefault
  );

  if( result )
  {
    result->data = (void*)lbl;
    result->onPaint = esguiWgtCheckBoxOnPaint;
  }
  
  return result;
}

#endif //< ESGUI_WIDGET_USE_CHECKBOX
