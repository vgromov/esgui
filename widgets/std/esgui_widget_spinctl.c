#include <esgui/include/esgui.h>
#include <esgui/include/esgui_widgets.h>
#include <esgui/include/esgui_widget_spinctl.h>
#include <esfwxe/utils.h>
#include <esfwxe/utils_str.h>
#include <string.h>
#include <stdio.h>

#if defined(ESGUI_WIDGET_USE_SPINCTL) && 1 == ESGUI_WIDGET_USE_SPINCTL

static void esguiWgtSpinctlIncDec(ESGUI_WIDGET* This, ESGUI_WgtSpinCtlData* data, esBL inc, esBL big)
{
  if( data->isFloat )
  {
    float newVal = data->fval + 
      (big ?  (inc ? data->fbigInc : -data->fbigInc) : 
              (inc ? data->finc : -data->finc));

    // trim value
    if( newVal < data->fmin )
      newVal = data->fmin;
    else if( newVal > data->fmax )
      newVal = data->fmax;
    
    if( newVal != data->fval )
    {
      data->fval = newVal;
      esguiWgtPaint(This, 0, 0, esguiWgtpfPaintForce);
    }
  }
  else
  {
    int newVal = data->val + 
      (big ?  (inc ? data->bigInc : -data->bigInc) : 
              (inc ? data->inc : -data->inc));

    // trim value
    if( newVal < data->min )
      newVal = data->min;
    else if( newVal > data->max )
      newVal = data->max;
    
    if( newVal != data->val )
    {
      data->val = newVal;
      esguiWgtPaint(This, 0, 0, esguiWgtpfPaintForce);
    }  
  }
}

// standard event handler
void esguiWgtSpinctlOnEvent(ESGUI_WIDGET* This, ESGUI_WIDGET* sender, EsguiAppEventT* evt)
{
  if( ES_BIT_IS_SET(This->flags, esguiWgtfActive) ) // active editor intercepts interactive events
  {
    ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
    
    if( esguiWgtEventIsCtlDec(evt) )
      esguiWgtSpinctlIncDec(This, data, FALSE, FALSE);
    else if( esguiWgtEventIsCtlDecBig(evt) )
      esguiWgtSpinctlIncDec(This, data, FALSE, TRUE);
    else if( esguiWgtEventIsCtlInc(evt) )
      esguiWgtSpinctlIncDec(This, data, TRUE, FALSE);
    else if( esguiWgtEventIsCtlIncBig(evt) )
      esguiWgtSpinctlIncDec(This, data, TRUE, TRUE);
    else if( esguiWgtEventIsCtlCommit(evt) )
      esguiWgtExit(This, sender, TRUE); // exit committing
    else if( esguiWgtEventIsCtlExit(evt) )
      esguiWgtExit(This, sender, FALSE);
  }
  else if( esguiWgtEventIsCtlCommit(evt) )
    esguiWgtClick(This, NULL); //< enter editor
}

// standard painter
void esguiWgtSpinctlOnPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect)
{
  ESGUI_RECT bounds = *rect;
  ESGUI_POINT ext;
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  int lblOffsMin = esguiWgtMetricGet(
    esguiWgtmStdMinLabelOffs
  );
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
    data->label,
    ESGUI_TAB_WIDTH_STD,
    &ext
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
  
  // arrange and draw strings
  bounds.org.x += esguiWgtMetricGet(esguiWgtmHspace);
  esguiCanvasDrawString(
    hdc,
    &bounds.org,
    data->label,
    ESGUI_TAB_WIDTH_STD
  );

  bounds.org.x += MAX(ext.x, lblOffsMin);
  if( ext.x > lblOffsMin )
    bounds.org.x += esguiWgtMetricGet(esguiWgtmHspace);

  if( data->isFloat )
    eseUtilsStrFmtFloat(
      esguiStrBuffGet(),
      ESGUI_BUFF_LEN,
      data->fval,
      data->decimals
    );
  else
    snprintf(
      esguiStrBuffGet(),
      ESGUI_BUFF_LEN,
      "%d",
      data->val
    );

  esguiCanvasGetStringExtent(
    hdc,
    NULL,
    esguiStrBuffGet(),
    ESGUI_TAB_WIDTH_STD,
    &ext
  );
  
  if( esguiWgtIsFocused(This) )
  {
    esguiCanvasBgColorSet(
      hdc, 
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorFg
      )
    );
    esguiCanvasFgColorSet(
      hdc, 
      esguiWgtStdColorGet(
        hdc,
        ESGUI_WgtStdColorBg
      )
    );
  }  
  esguiCanvasDrawString(
    hdc,
    &bounds.org,
    esguiStrBuffGet(),
    ESGUI_TAB_WIDTH_STD
  );
  bounds.org.x += ext.x;
  
  // erase the rest of background 
  --bounds.edge.y;
  esguiCanvasBgColorSet(
    hdc,
    esguiWgtStdColorGet(
      hdc,
      ESGUI_WgtStdColorBg
    )
  );
  esguiCanvasFillRect(hdc, &bounds);

  if( ES_BIT_IS_SET(This->flags, esguiWgtfActive) )
  {
    bounds.org.x = bounds.edge.x - esguiBmpWidthGet(bmUp);

    // draw up-down arrows
    if( data->isFloat )
    {
      if( data->fval + data->finc > data->fmax )
        esguiCanvasFgColorSet(
          hdc, 
          esguiWgtStdColorGet(
            hdc,
            ESGUI_WgtStdColorFgDisabled
          )
        );
      else
        esguiCanvasFgColorSet(
          hdc, 
          esguiWgtStdColorGet(
            hdc,
            ESGUI_WgtStdColorFg
          )
        );
    }
    else
    {
      if( data->val + data->inc > data->max )
        esguiCanvasFgColorSet(
          hdc, 
          esguiWgtStdColorGet(
            hdc,
            ESGUI_WgtStdColorFgDisabled
          )
        );
      else
        esguiCanvasFgColorSet(
          hdc, 
          esguiWgtStdColorGet(
            hdc,
            ESGUI_WgtStdColorFg
          )
        );
    }
    esguiCanvasDrawPictureROP(
      hdc,
      &bounds.org,
      bmUp,
      NULL,
      ESGUI_COPY
    );

    bounds.org.y = bounds.edge.y - esguiBmpHeightGet(bmUp);
  
    if( data->isFloat )
    {
      if( data->fval - data->finc < data->fmin )
        esguiCanvasFgColorSet(
          hdc, 
          esguiWgtStdColorGet(
            hdc,
            ESGUI_WgtStdColorFgDisabled
          )
        );
      else
        esguiCanvasFgColorSet(
          hdc, 
          esguiWgtStdColorGet(
            hdc,
            ESGUI_WgtStdColorFg
          )
        );
    }
    else
    {
      if( data->val - data->inc < data->min )
        esguiCanvasFgColorSet(
          hdc, 
          esguiWgtStdColorGet(
            hdc,
            ESGUI_WgtStdColorFgDisabled
          )
        );
      else
        esguiCanvasFgColorSet(
          hdc, 
          esguiWgtStdColorGet(
            hdc,
            ESGUI_WgtStdColorFg
          )
        );
    }

    esguiCanvasDrawPictureROP(
      hdc,
      &bounds.org,
      bmDown,
      NULL,
      ESGUI_COPY
    );
  }
}

// range and value access
void esguiWgtSpinctlSetRangeInt(ESGUI_WIDGET* This, int min, int max)
{
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  if( min < max &&
      !data->isFloat )
  {
    data->min = min;
    data->max = max;
    if( data->val < min )
      data->val = min;
    else if( data->val > max )
      data->val = max;

    esguiWgtPaint(
      This,
      NULL,
      NULL,
      esguiWgtpfPaintForce
    );
  }
}

void esguiWgtSpinctlSetRangeFloat(ESGUI_WIDGET* This, float min, float max)
{
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  if( min < max &&
      data->isFloat )
  {
    data->fmin = min;
    data->fmax = max;
    if( data->fval < min )
      data->fval = min;
    else if( data->fval > max )
      data->fval = max;

    esguiWgtPaint(
      This,
      NULL,
      NULL,
      esguiWgtpfPaintForce
    );
  }
}

void esguiWgtSpinctlGetRangeInt(ESGUI_WIDGET* This, int* min, int* max)
{
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  if( !data->isFloat &&
      min && max )
  {
    *min = data->min;
    *max = data->max;
  }
}

void esguiWgtSpinctlGetRangeFloat(ESGUI_WIDGET* This, float* min, float* max)
{
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  if( data->isFloat &&
      min && max )
  {
    *min = data->fmin;
    *max = data->fmax;
  }
}

void esguiWgtSpinctlSetValueInt(ESGUI_WIDGET* This, int val)
{
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  if( !data->isFloat &&
      val <= data->max &&
      val >= data->min &&
      data->val != val )
  {
    data->val = val;
    esguiWgtPaint(
      This,
      NULL,
      NULL,
      esguiWgtpfPaintForce
    );
  }
}

void esguiWgtSpinctlSetValueFloat(ESGUI_WIDGET* This, float val)
{
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  if( data->isFloat &&
      val <= data->fmax &&
      val >= data->fmin &&
      data->fval != val )
  {
    data->fval = val;
    esguiWgtPaint(
      This,
      NULL,
      NULL,
      esguiWgtpfPaintForce
    );
  }
}

int esguiWgtSpinctlGetValueInt(ESGUI_WIDGET* This)
{
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  if( !data->isFloat )
    return data->val;
  
  return 0;  
}

float esguiWgtSpinctlGetValueFloat(ESGUI_WIDGET* This)
{
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  if( data->isFloat )
    return data->fval;
  
  return 0.f;  
}

void esguiWgtSpinctlSetIncInt(ESGUI_WIDGET* This, int inc)
{
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  if( !data->isFloat &&
      inc > 0 )
  {
    data->inc = inc;
    if( data->bigInc < inc )
      data->bigInc = inc;
  }
}

void esguiWgtSpinctlSetIncFloat(ESGUI_WIDGET* This, float inc)
{
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  if( data->isFloat &&
      inc > 0.f )
  {
    data->finc = inc;
    if( inc > data->fbigInc )
      data->fbigInc = inc;
  }
}

int esguiWgtSpinctlGetIncInt(ESGUI_WIDGET* This)
{
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  if( !data->isFloat )
    return data->inc;
  
  return 0;
}

float esguiWgtSpinctlGetIncFloat(ESGUI_WIDGET* This)
{
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  if( data->isFloat )
    return data->finc;
  
  return 0.f;
}

void esguiWgtSpinctlSetBigIncInt(ESGUI_WIDGET* This, int bigInc)
{
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  if( !data->isFloat &&
      bigInc >= data->inc )
    data->bigInc = bigInc;
}

void esguiWgtSpinctlSetBigIncFloat(ESGUI_WIDGET* This, float bigInc)
{
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  if( data->isFloat && 
      bigInc >= data->finc )
    data->fbigInc = bigInc;
}

int esguiWgtSpinctlGetBigIncInt(ESGUI_WIDGET* This)
{
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  if( !data->isFloat )
    return data->bigInc;
  
  return 0;
}

float esguiWgtSpinctlGetBigIncFloat(ESGUI_WIDGET* This)
{
  ESGUI_WgtSpinCtlData* data = (ESGUI_WgtSpinCtlData*)This->data;
  if( data->isFloat )
    return data->fbigInc;
  
  return 0.f;
}

// spin control widget constructors.
ESGUI_WIDGET* esguiWgtSpinctlCreateInt(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, ESE_CSTR lbl,  int min, int max)
{
  ESGUI_WIDGET* result = 0;
  if( min < max )
  {
    ESGUI_WgtSpinCtlData* data = esguiWgtMalloc(ESGUI_WgtSpinCtlData_SZE);
    if( data )
    {
      memset(data, 0, ESGUI_WgtSpinCtlData_SZE);
      data->min = min;
      data->max = max;
      data->inc = esguiWgtSpinctlDefInc;
      data->bigInc = esguiWgtSpinctlDefBigInc;
      data->label = lbl;

      result = esguiWgtCreate(
        root,
        ESGUI_EDIT,
        bounds,
        esguiWgtfDefault|esguiWgtfOwnsData
      );

      if( result )
      {
        result->data = data;
        result->onPaint = esguiWgtSpinctlOnPaint;
        result->onEvent = esguiWgtSpinctlOnEvent;
      }
      else
        esguiWgtFree(data);
    }
  }
    
  return result;
}

ESGUI_WIDGET* esguiWgtSpinctlCreateFloat(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, ESE_CSTR lbl, float min, float max, int decimals)
{
  ESGUI_WIDGET* result = 0;
  if( min < max )
  {
    ESGUI_WgtSpinCtlData* data = esguiWgtMalloc(ESGUI_WgtSpinCtlData_SZE);
    if( data )
    {
      memset(data, 0, ESGUI_WgtSpinCtlData_SZE);
      data->fmin = min;
      data->fmax = max;
      data->finc = esguiWgtSpinctlDefInc;
      data->fbigInc = esguiWgtSpinctlDefBigInc;
      data->label = lbl;
      data->isFloat = TRUE;
      data->decimals = decimals;

      result = esguiWgtCreate(
        root,
        ESGUI_EDIT,
        bounds,
        esguiWgtfDefault|esguiWgtfOwnsData
      );

      if( result )
      {
        result->data = data;
        result->onPaint = esguiWgtSpinctlOnPaint;
        result->onEvent = esguiWgtSpinctlOnEvent;
      }
      else
        esguiWgtFree(data);
    }
  }
    
  return result;
}

#endif //< ESGUI_WIDGET_USE_SPINCTL
