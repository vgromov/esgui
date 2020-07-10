#include <esgui/include/esgui.h>
#include <esgui/include/esgui_widgets.h>
#include <esgui/include/esgui_widget_dtedit.h>
#include <esfwxe/utils.h>
#include <esfwxe/datetime.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(ESGUI_WIDGET_USE_DTEDIT) && 1 == ESGUI_WIDGET_USE_DTEDIT

typedef struct {
  ESE_CSTR label;
  int  caret;
  char buff[11];

} EsguiWgtDateTimeEditData;

static void esguiWgtStdEditorCharCallback( ESGUI_HDC hdc, ESGUI_CharCallbackReason reason, ESE_CSTR pos, const ESGUI_RECT* rect, void* data)
{
  ESGUI_WIDGET* widget = (ESGUI_WIDGET*)data;
  EsguiWgtDateTimeEditData* Data = (EsguiWgtDateTimeEditData*)widget->data;

  switch(reason)
  {
  case ESGUI_BeforeDrawChar:
    if( esguiWgtIsFocused(widget) )
    {
      if( ES_BIT_IS_SET(widget->flags, esguiWgtfActive) )
      {
        if( Data->caret == (pos - Data->buff) )
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
        else
        {
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
        }
      }
      else
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
    }
    else
    {
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
    }
    break;
  default:
    // Pacify compilers
    break;
  }
}

// time editor on-enter handler
static void esguiWgtTimeEditorOnEnter(ESGUI_WIDGET* This, ESGUI_WIDGET* sender)
{
  ((EsguiWgtDateTimeEditData*)This->data)->caret = 0;
}

// date editor on-enter handler
static void esguiWgtDateEditorOnEnter(ESGUI_WIDGET* This, ESGUI_WIDGET* sender)
{
  ((EsguiWgtDateTimeEditData*)This->data)->caret = 2;  // skip thousand and hundred digits
}

// time editor event handler
static void esguiWgtTimeEditorOnEvent(ESGUI_WIDGET* This, ESGUI_WIDGET* sender, EsguiAppEventT* evt)
{
  if( !esguiWgtEventIsInteractive(evt) )
    return;
  
  EsguiWgtDateTimeEditData* data = (EsguiWgtDateTimeEditData*)This->data;

  if( ES_BIT_IS_SET(This->flags, esguiWgtfActive) ) // active editor intercepts interactive events
  {
    esBL isHourEditing = 2 > data->caret;
    esBL isTensEditing = 0 == data->caret || 3 == data->caret;
    char sym = data->buff[data->caret];

    // up-down - inc|dec char, ok - move right|exits with save
    bool doIncDec = false;
    if( esguiWgtEventIsCtlDec(evt) || esguiWgtEventIsCtlDecBig(evt) )
    {
      --sym;
      doIncDec = true;
    }
    else if( esguiWgtEventIsCtlInc(evt) || esguiWgtEventIsCtlIncBig(evt) )
    {
      ++sym;
      doIncDec = true;
    }

    if( doIncDec )
    {
      if( isHourEditing )
      {
        if( isTensEditing )
        {
          // wrap sym on decrement
          if( sym < '0' )
            sym = '2';

          if( sym > '1' )
          {
            // correct hours unit digit
            if( data->buff[1] > '3' )
              data->buff[1] = '0';

            // wrap tens of hours
            if( sym > '2' )
              sym = '0';
          }
        }
        else
        {
          // wrap sym on decrement
          if( sym < '0' )
            sym = '9';

          // limit hours unit digit
          if( data->buff[0] > '1' )
          {
            if( sym > '3' )
              sym = '0';
          }
          else
          {
            // wrap hours unit digit
            if( sym > '9' )
              sym = '0';
          }
        }
      }
      else
      {
        if( isTensEditing )
        {
          if( sym < '0' )
            sym = '5';
          else if( sym > '5' )
            sym = '0';
        }
        else
        {
          if( sym < '0' )
            sym = '9';
          else if( sym > '9' )
            sym = '0';
        }
      }
      data->buff[data->caret] = sym;
      
      esguiWgtEventMarkHandled(evt);
      esguiWgtPaint(
        This,
        sender,
        NULL,
        esguiWgtpfPaintForce
      );
    }
    else if( esguiWgtEventIsCtlCommit(evt) )
    {
      if( 4 < ++data->caret )
        esguiWgtExit(
          This,
          sender,
          TRUE
        ); // exit committing
      else
      {
       if( 2 == data->caret )
          ++data->caret; // skip delimiter char

        esguiWgtPaint(
          This,
          sender,
          NULL,
          esguiWgtpfPaintForce
        );
      }
      
      esguiWgtEventMarkHandled(evt);
    }
    else if( esguiWgtEventIsCtlExit(evt) )
    {
      esguiWgtExit(
        This,
        sender,
        FALSE
      ); // exit with cancel

      esguiWgtEventMarkHandled(evt);
    }
  }
  else //< Control is not active
  {
    if( esguiWgtEventIsCtlEnter(evt) )
    {
      // enter editor
      esguiWgtClick(This, NULL);
    }
    else if( esguiWgtEventIsSecondTick(evt) )
    {
      esDT dt = esguiWgtCurrentDateTimeGet();
      int hr, mn;
      dtExtractTime(
        &dt,
        &hr,
        &mn,
        NULL,
        NULL
      );

      // respond to seconds ping notification by updating editor string
      // to keep inactive time editor in sync with RTC
      snprintf(
        data->buff,
        sizeof(data->buff),
        esguiWgtDteditTimeFmtStrGet(),
        hr,
        mn
      );

      esguiWgtPaint(
        This,
        NULL,
        NULL,
        esguiWgtpfPaintForce
      );
    }
  }
}

// date validation helpers
//
static void esguiWgtDteditCorrectDays(EsguiWgtDateTimeEditData* data)
{
  int year = atoi(data->buff);
  int month = atoi(data->buff+5);
  int days = atoi(data->buff+8);
  int maxDays = dtGetMonthDayCount(year, month);
  if( days > maxDays )
  {
    data->buff[8] = maxDays / 10 + 0x30;
    data->buff[9] = maxDays % 10 + 0x30;
  }  
}

// date editor event handler
static void esguiWgtDateEditorOnEvent(ESGUI_WIDGET* This, ESGUI_WIDGET* sender, EsguiAppEventT* evt)
{
  if( !esguiWgtEventIsInteractive(evt) )
    return;
    
  EsguiWgtDateTimeEditData* data = (EsguiWgtDateTimeEditData*)This->data;
  
  if( ES_BIT_IS_SET(This->flags, esguiWgtfActive) ) // active editor intercepts interactive events
  {
    bool isYearEditing = 4 > data->caret;
    bool isMonthEditing = 5 <= data->caret && 6 >= data->caret;
    bool isTensEditing = 2 == data->caret || 5 == data->caret || 8 == data->caret;
    char sym = data->buff[data->caret];
    
    bool doIncDec = false;
    if( esguiWgtEventIsCtlDec(evt) || esguiWgtEventIsCtlDecBig(evt) )
    {
      --sym;
      doIncDec = true;
    }
    else if( esguiWgtEventIsCtlInc(evt) || esguiWgtEventIsCtlIncBig(evt) )
    {
      ++sym;
      doIncDec = true;
    }
      
    if( doIncDec )
    {
      // up-down - inc|dec char, ok - move right|exits with save
      if( isYearEditing )
      {
        // wrap sym on decrement | increment
        if( sym < '0' )
          sym = '9';
        else if( sym > '9' )
          sym = '0';
      }
      else if( isMonthEditing )
      {
        if( isTensEditing )
        {
          if( sym < '0' )
            sym = '1';
          else if( sym > '1' )
            sym = '0';
          // correct month units
          if( sym == '1' && data->buff[data->caret+1] > '2' )
            data->buff[data->caret+1] = '0';
        }
        else
        {
          if( data->buff[data->caret-1] == '0' )
          {
            // we can change units from 1 to 9
            if( sym < '1' )
              sym = '9';
            else if( sym > '9' )
              sym = '1';
          }
          else
          {
            // we can only change from 0 to 2
            if( sym < '0' )
              sym = '2';
            else if( sym > '2' )
              sym = '0';
          }
        }
      }
      else
      {
        // days editing. generally allow tens from 0 to 3
        // and units from 0 to 9. correction will be done
        // automatically in subsequent call to correctDays
        if( isTensEditing )
        {
          if( sym < '0' )
            sym = '3';
          else if( sym > '3' )
            sym = '0';
        }
        else
        {
          if( sym < '0' )
            sym = '9';
          else if( sym > '9' )
            sym = '0';
        }
      }
      data->buff[data->caret] = sym;
      
      // correct days
      esguiWgtDteditCorrectDays(data);
      esguiWgtEventMarkHandled(evt);
      esguiWgtPaint(
        This, 
        sender, 
        NULL,  
        esguiWgtpfPaintForce
      );
    }
    else if( esguiWgtEventIsCtlCommit(evt) )
    {
      if( 9 < ++data->caret )
        esguiWgtExit(This, sender, TRUE); // exit committing
      else
      {
        if( 4 == data->caret || 7 == data->caret )
          ++data->caret; // skip delimiter char
        esguiWgtPaint(
          This, 
          sender, 
          NULL,
          esguiWgtpfPaintForce
        );
      }
      esguiWgtEventMarkHandled(evt);
    }
    else if( esguiWgtEventIsCtlExit(evt) )
    {
      esguiWgtExit(This, sender, FALSE); // exit with cancel
      esguiWgtEventMarkHandled(evt);
    }
  }
  else if( esguiWgtEventIsCtlEnter(evt) )
  {
    // enter editor
    esguiWgtClick(This, NULL);
  }
}

// date time editor data constructor
static EsguiWgtDateTimeEditData* esguiWgtDtDataCreate(ESE_CSTR label)
{
  EsguiWgtDateTimeEditData* result = (EsguiWgtDateTimeEditData*)esguiWgtMalloc(sizeof(EsguiWgtDateTimeEditData));  
  memset(
    result, 
    0, 
    sizeof(EsguiWgtDateTimeEditData)
  );  
  result->label = label;

  return result;
}

// time editor on-exit handler
static void esguiWgtTimeEditorOnExit(ESGUI_WIDGET* This, ESGUI_WIDGET* sender, bool commit)
{
  if( commit )
  {
    // convert time string to ds28dg02 date time and set time portion to RTC
    esDT dt = esguiWgtCurrentDateTimeGet();
    int y, mn, d, h, m;
    dtDecomposeDateTime(
      &dt,
      &y,
      NULL,
      &mn,
      &d,
      &h,
      &m,
      NULL,
      NULL
    );

    if(
      2 == sscanf(
        ((EsguiWgtDateTimeEditData*)This->data)->buff,
        esguiWgtDteditTimeFmtStrGet(),
        &h,
        &m
      ) &&
      dtComposeDateTime(
        &dt,
        y,
        mn,
        d,
        h,
        m,
        0,
        0
      )
    )
    {
      esguiWgtCurrentDateTimeSet(&dt);
    }
  }
}

// date editor on-exit handler
static void esguiWgtDateEditorOnExit(ESGUI_WIDGET* This, ESGUI_WIDGET* sender, bool commit)
{
  if( commit )
  {
    // convert time string to ds28dg02 date time and set time portion to RTC
    esDT dt = esguiWgtCurrentDateTimeGet();
    int y, mn, d, h, m, s;
    dtDecomposeDateTime(
      &dt,
      &y,
      NULL,
      &mn,
      &d,
      &h,
      &m,
      &s,
      NULL
    );

    if(
      3 == sscanf(
        ((EsguiWgtDateTimeEditData*)This->data)->buff,
        esguiWgtDteditDateFmtStrGet(),
        &y,
        &mn,
        &d
      ) &&
      dtComposeDateTime(
        &dt,
        y,
        mn,
        d,
        h,
        m,
        s,
        0
      )
    )
    {
      esguiWgtCurrentDateTimeSet(&dt);
    }
  }
}

// date time editor painter
static void esguiWgtDateTimeEditorPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender,
  const ESGUI_RECT* rect)
{
  ESGUI_RECT bounds = *rect;
  ESGUI_POINT ext;
  int lblOffsMin = esguiWgtMetricGet(
    esguiWgtmStdMinLabelOffs
  );

  EsguiWgtDateTimeEditData* data = (EsguiWgtDateTimeEditData*)This->data;

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

  esguiCanvasGetStringExtent(
    hdc,
    NULL,
    data->buff,
    ESGUI_TAB_WIDTH_STD,
    &ext
  );
  esguiCanvasDrawStringWithCallback(
    hdc,
    &bounds.org,
    data->buff,
    ESGUI_TAB_WIDTH_STD,
    esguiWgtStdEditorCharCallback,
    This
  );

  // erase the rest of background 
  bounds.org.x += ext.x;
  --bounds.edge.y;
  esguiCanvasBgColorSet(
    hdc, 
    esguiWgtStdColorGet(
      hdc,
      ESGUI_WgtStdColorBg
    )
  );
  esguiCanvasFillRect(hdc, &bounds);
}

// time editor constructor
ESGUI_WIDGET* esguiWgtTimeEditorCreate(ESGUI_WIDGET_ROOT* root, ESE_CSTR label, const esDT* dt, const ESGUI_RECT* bounds)
{
  ESGUI_WIDGET* editor = esguiWgtCreate(
    root,
    ESGUI_EDIT,
    bounds,
    esguiWgtfDefault|esguiWgtfOwnsData
  );

  if( editor )
  {
    EsguiWgtDateTimeEditData* data = esguiWgtDtDataCreate(label);
    int h, m;

    dtExtractTime(
      dt,
      &h,
      &m,
      NULL,
      NULL
    );
    snprintf(
      data->buff,
      sizeof(data->buff),
      esguiWgtDteditTimeFmtStrGet(),
      h,
      m
    );

    editor->data = data;
    editor->onEvent = esguiWgtTimeEditorOnEvent;
    editor->onEnter = esguiWgtTimeEditorOnEnter;
    editor->onExit = esguiWgtTimeEditorOnExit;
    editor->onPaint = esguiWgtDateTimeEditorPaint;
  }

  return editor;
}

// date editor constructor
ESGUI_WIDGET* esguiWgtDateEditorCreate(ESGUI_WIDGET_ROOT* root, ESE_CSTR label, const esDT* dt, const ESGUI_RECT* bounds)
{
  ESGUI_WIDGET* editor = esguiWgtCreate(
    root,
    ESGUI_EDIT,
    bounds,
    esguiWgtfDefault|esguiWgtfOwnsData
  );

  if( editor )
  {
    EsguiWgtDateTimeEditData* data = esguiWgtDtDataCreate(label);
    int y, mn, d, h, m;

    dtDecomposeDateTime(
      dt,
      &y,
      NULL,
      &mn,
      &d,
      &h,
      &m,
      NULL,
      NULL
    );

    snprintf(
      data->buff,
      sizeof(data->buff),
      esguiWgtDteditDateFmtStrGet(),
      y,
      mn,
      d
    );

    editor->data = data;
    editor->onEvent = esguiWgtDateEditorOnEvent;
    editor->onPaint = esguiWgtDateTimeEditorPaint;      
    editor->onEnter = esguiWgtDateEditorOnEnter;
    editor->onExit = esguiWgtDateEditorOnExit;
  }

  return editor;
}

#endif //< ESGUI_WIDGET_USE_DTEDIT
