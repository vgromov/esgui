#include <esgui/include/esgui.h>
#include <esgui/include/esgui_widgets.h>
#include <esgui/include/esgui_widget_progress.h>
#include <esfwxe/utils.h>

#if defined(ESGUI_WIDGET_USE_PROGRESS) && 1 == ESGUI_WIDGET_USE_PROGRESS

void esguiWgtProgressOnEvent(ESGUI_WIDGET* This, ESGUI_WIDGET* sender, EsguiAppEventT* evt)
{
  int val0, val1;
  if( esguiWgtEventIsProgressSet(evt, &val0) )
  {
    esguiWgtProgressSetPos(This, val0);
    esguiWgtEventMarkHandled(evt);
  }
  else if( esguiWgtEventIsProgressRangeSet(evt, &val0, &val1) )
  {
    esguiWgtProgressSetRange(This, val0, val1);
    esguiWgtEventMarkHandled(evt);
  }
}

void esguiWgtProgressSetPos(ESGUI_WIDGET* This, int pos)
{
  ESGUI_WgtProgressData* data = (ESGUI_WgtProgressData*)This->data;
  // restrict range
  pos = MAX(data->min, pos);
  pos = MIN(data->max, pos);
  if( data->pos  != pos )
  {
    int delta = pos-data->prevVisiblePos;
    if( 0 > delta )
      delta = -delta;

    data->pos  = pos;
    // request redraw only if position update is actually visible
    if( delta > data->visibleDelta || data->max == pos )
    {
      data->prevVisiblePos = pos;
      esguiWgtPaint(
        This, 
        NULL, 
        NULL, 
        esguiWgtpfPaintForce|esguiWgtpfPaintBg
      );
    }
  }
}

void esguiWgtProgressOnPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect)
{
  ESGUI_WgtProgressData* data = (ESGUI_WgtProgressData*)This->data;
  ESGUI_RECT draw = *rect;
  
  // first, draw progress bar in disabled color
  draw.edge.x = draw.org.x+((data->pos-data->min)*(esguiRectWidth(rect)))/(data->max - data->min);
  esguiCanvasBgColorSet(
    hdc, 
    esguiWgtStdColorGet(
      hdc,
      ESGUI_WgtStdColorFgDisabled
    )
  );
  esguiCanvasFillRect(
    hdc, 
    &draw
  );
  esguiCanvasBgColorSet(
    hdc, 
    esguiWgtStdColorGet(
      hdc,
      ESGUI_WgtStdColorBg
    )
  );
  draw.org.x = draw.edge.x+1;
  draw.edge.x = rect->edge.x;
  esguiCanvasFillRect(hdc, &draw);
  
  // if progress text callback is set, draw text over bar in transparent mode using hilight color
  if( data->onDisplayProgressText )
  {
    esBL oldOpaque = esguiCanvasSetOpaqueText(hdc, FALSE);
    data->onDisplayProgressText(data, esguiStrBuffGet(), ESGUI_BUFF_LEN);
    esguiCanvasFontSet(hdc, data->font);
    esguiCanvasFgColorSet(
      hdc, 
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorFg
      )
    );
    esguiCanvasDrawStringRect(
      hdc,
      rect,
      esguiStrBuffGet(),
      ESGUI_TAB_WIDTH_STD,
      ESGUI_HTA_CENTER|ESGUI_VTA_CENTER|ESGUI_TCLIP
    );
    esguiCanvasSetOpaqueText(hdc, oldOpaque);
  }  
}

static void esguiWgtProgressCalcVisibleDelta(const ESGUI_RECT* rect, ESGUI_WgtProgressData* data)
{
  int width = esguiRectWidth(rect);
  // visible change is 1 pixel. find how much progress is in 1 pixel
  int range = data->max - data->min;
  if( range > width )
    data->visibleDelta = range/width;
  else
    data->visibleDelta = 1;
}

void esguiWgtProgressSetOnTextHandler(ESGUI_WIDGET* This, ESGUI_WgtOnDisplayProgressText onDisplayProgressText)
{
  ((ESGUI_WgtProgressData*)This->data)->onDisplayProgressText = onDisplayProgressText;
}

void esguiWgtProgressSetRange(ESGUI_WIDGET* This, int min, int max)
{
  ESGUI_WgtProgressData* data = (ESGUI_WgtProgressData*)This->data;
  // restrict max > min
  if( max == min )
    max = min + 1;

  if( max < min )
  {
    data->min = max;
    data->max = min;
    esguiWgtProgressCalcVisibleDelta(&This->bounds, data);
    esguiWgtPaint(This, NULL, NULL, esguiWgtpfPaintForce|esguiWgtpfPaintBg);
  }
  else
  {
    data->max = max;
    data->min = min;
    esguiWgtProgressCalcVisibleDelta(&This->bounds, data);
    esguiWgtPaint(This, NULL, NULL, esguiWgtpfPaintForce|esguiWgtpfPaintBg);
  }
}

ESGUI_WIDGET* esguiWgtProgressCreateCustom(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, const ESGUI_FONT* font, ESGUI_WgtOnDisplayProgressText onDisplayProgressText, int rmin, int rmax, int pos)
{
  ESGUI_WIDGET* result = 0;
  ESGUI_WgtProgressData* data = (ESGUI_WgtProgressData*)esguiWgtMalloc(ESGUI_WgtProgressData_SZE);
  if( data )
  {
    data->font = font;
    data->onDisplayProgressText = onDisplayProgressText;
    result = esguiWgtCreate(
      root,
      ESGUI_STATIC,
      bounds,
      esguiWgtfDefault|esguiWgtfOwnsData
    );

    if( result )
    {
      // complete data initialization
      result->data = data;
      result->onEvent = esguiWgtProgressOnEvent;
      result->onPaint = esguiWgtProgressOnPaint;
      data->pos = pos;
      esguiWgtProgressSetRange(result, rmin, rmax);
    }
    else
      esguiWgtFree(data);
  }

  return result;
}

ESGUI_WIDGET* esguiWgtProgressCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, const ESGUI_FONT* font, ESGUI_WgtOnDisplayProgressText onDisplayProgressText)
{
  return esguiWgtProgressCreateCustom(
    root,
    bounds, 
    font, 
    onDisplayProgressText, 
    0, 
    100, 
    0
  );
}

#endif // ESGUI_WIDGET_USE_PROGRESS
