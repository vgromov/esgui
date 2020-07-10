#include <esgui/include/esgui.h>
#include <esgui/include/esgui_widgets.h>
#include <esgui/include/esgui_widget_msgbox.h>
#include <string.h>
#include <esfwxe/ese_assert.h>
#include <esfwxe/utils.h>


#if defined(ESGUI_WIDGET_USE_MSGBOX) && 1 == ESGUI_WIDGET_USE_MSGBOX

#ifdef USE_EMULATOR
# include <emulator_stubs.h>
#endif

// standard msgbox event handler
esU32 esguiWgtMsgBoxStdEventHandler(ESGUI_WIDGET* This, EsguiAppEventT* evt)
{
  esU32 result = ESGUI_MBR_NONE;

  if( esguiWgtEventIsMsgBoxOk(This, evt) )
  {
    esguiWgtEventMarkHandled(evt);
    result = ESGUI_MBR_OK;
  }
  else if( esguiWgtEventIsMsgBoxCancel(This, evt) )
  {
    esguiWgtEventMarkHandled(evt);
    result = ESGUI_MBR_CANCEL;
  }
  else if( esguiWgtEventIsInteractive(evt) ) //< Intercept all other interactive events
    esguiWgtEventMarkHandled(evt);

  return result;
}

static void esguiWgtMsgBoxOnEvent(ESGUI_WIDGET* This, ESGUI_WIDGET* sender,  EsguiAppEventT* evt)
{
  ESGUI_WgtMsgBoxData* data = (ESGUI_WgtMsgBoxData*)This->data;

  if( data->handler )
    data->result = data->handler(
      This, 
      evt
    );
  else
    data->result = ESGUI_MBR_NONE;
}

static void esguiWgtMsgBoxPaintBg(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect)
{
  ESGUI_WgtMsgBoxData* data = (ESGUI_WgtMsgBoxData*)This->data;

  // draw title
  const ESGUI_BITMAP* bm = esguiWgtStdBitmapGet(
    hdc,
    ESGUI_WgtStdBitmapInfo
  );
  switch( (data->type & ESGUI_MBT_STYLE_MASK) ) 
  {
  case ESGUI_MBT_QUESTION:
    bm = esguiWgtStdBitmapGet(
      hdc,
      ESGUI_WgtStdBitmapQuestion
    );
    break;
  case ESGUI_MBT_WARNING:
    bm = esguiWgtStdBitmapGet(
      hdc,
      ESGUI_WgtStdBitmapWarning
    );
    break;
  case ESGUI_MBT_ERROR:
    bm = esguiWgtStdBitmapGet(
      hdc,
      ESGUI_WgtStdBitmapError
    );
    break;
  }
  
  esguiWgtStdPaintTitle(
    hdc, 
    &This->bounds, 
    bm, 
    data->title
  );
}

static void esguiWgtMsgBoxPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect)
{
  ESGUI_WgtMsgBoxData* data = (ESGUI_WgtMsgBoxData*)This->data;
  ESGUI_RECT bounds = *rect;
  
  const ESGUI_FONT* fnt = esguiWgtStdFontGet(
    hdc,
    ESGUI_WgtStdFontTitle
  );
  ESE_ASSERT(fnt);

  esguiCanvasBgColorSet(
    hdc, 
    esguiWgtStdColorGet(
      hdc,
      ESGUI_WgtStdColorBg
    )
  );
  
  // clear only text area
  bounds.org.y += MAX(
    esguiWgtMetricGet(esguiWgtmTitleHeight), 
    esguiFontHeightGet(fnt)
  )+1;
  
  esguiCanvasFillRect(hdc, &bounds);
  
  // draw text  
  esguiCanvasFgColorSet(
    hdc, 
    esguiWgtStdColorGet(
      hdc,
      ESGUI_WgtStdColorFg
    )
  );
  
  fnt = esguiWgtStdFontGet(
    hdc,
    ESGUI_WgtStdFontText
  );
  ESE_ASSERT(fnt);
  
  esguiCanvasFontSet(
    hdc, 
    fnt
  );
  esguiCanvasDrawStringRect(
    hdc,
    &bounds,
    data->text,
    ESGUI_TAB_WIDTH_STD,
    ESGUI_HTA_CENTER|ESGUI_VTA_CENTER|ESGUI_TWRAP|ESGUI_TCLIP
  );
}

// basic initializer of msg box data
void esguiWgtMsgBoxDataInit(ESGUI_WgtMsgBoxData* data, ESE_CSTR title, ESE_CSTR text, esU32 type)
{
  if( data )
  {
    memset(data, 0, ESGUI_WgtMsgBoxData_SZE);
    data->canCreate = esguiWgtMsgBoxStdCanCreate;
    data->handler = esguiWgtMsgBoxStdEventHandler;
    data->type = type;
    if( title )
      data->title = title;
    else
    {
      // handle standard titles
      data->title = esguiWgtStdStringGet(
        ESGUI_WgtStdStringTitleInfo
      );

      switch( (data->type & ESGUI_MBT_STYLE_MASK) ) 
      {
      case ESGUI_MBT_QUESTION:
        data->title = esguiWgtStdStringGet(
          ESGUI_WgtStdStringTitleQuestion
        );
        break;
      case ESGUI_MBT_WARNING:
        data->title = esguiWgtStdStringGet(
          ESGUI_WgtStdStringTitleWarning
        );
        break;
      case ESGUI_MBT_ERROR:
        data->title = esguiWgtStdStringGet(
          ESGUI_WgtStdStringTitleError
        );
        break;
      }
    }
    data->text = text;
    data->result = ESGUI_MBR_NONE;
  }
}

esU32 esguiWgtMsgBoxIndirect(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* rect, int tag, ESGUI_WgtMsgBoxData* data, bool ownsData, ESGUI_WIDGET** out)
{
  esU32 result = ESGUI_MBR_NONE;

  if( data && data->canCreate && data->canCreate() )
  {  
    ESGUI_WIDGET* box = esguiWgtCreate(
      root,
      ESGUI_CUSTOM,
      rect,
      ownsData ?
        (esguiWgtfDefault|esguiWgtfModal|esguiWgtfOwnsData) :
        (esguiWgtfDefault|esguiWgtfModal)
    );

    if( box )
    {
      box->tag = tag;
    
      // return instance, if requested
      if(out)
        *out = box;

      box->onEvent = esguiWgtMsgBoxOnEvent;
      box->onPaint = esguiWgtMsgBoxPaint;
      box->onPaintBg = esguiWgtMsgBoxPaintBg;

      box->data = data;

      esguiWgtMsgBoxBeforeShow();
      esguiWgtModalAdd(box);
      esguiWgtShow(box, NULL, TRUE);
      esguiWgtMsgBoxAfterShow();
  
      // modal operation-run our own event processing instance
      while(
#ifdef USE_EMULATOR
        !emuIsTerminating() &&
#endif
        ESGUI_MBR_NONE == data->result 
      )
        esguiWgtMsgBoxAppEventsProcess();
  
      esguiWgtMsgBoxBeforeClose();
      // retrieve result 
      result = data->result;
      esguiWgtClose(box, NULL, FALSE);
      // report instance is deleted
      if( out )
        *out = NULL;
      esguiWgtMsgBoxAfterClose();
    }
  }

  return result;
}

esU32 esguiWgtMsgBox(ESGUI_WIDGET_ROOT* root, ESE_CSTR title, ESE_CSTR text, const ESGUI_RECT* rect, esU32 type, int tag, ESGUI_WIDGET** out )
{
  if( NULL != out && NULL != *out ) //< Already executing msgBox
    return ESGUI_MBR_NONE;
    
  esU32 result = ESGUI_MBR_NONE;  
  ESGUI_WgtMsgBoxData* data = (ESGUI_WgtMsgBoxData*)esguiWgtMalloc( ESGUI_WgtMsgBoxData_SZE );
  if(data)
  {
    esguiWgtMsgBoxDataInit(data, title, text, type);
    result = esguiWgtMsgBoxIndirect(root, rect, tag, data, TRUE, out);
  }
  return result;
}

void esguiWgtMsgBoxCancel(ESGUI_WIDGET* msgbox)
{
  if( NULL == msgbox )
    return;
    
  ESGUI_WgtMsgBoxData* data = (ESGUI_WgtMsgBoxData*)msgbox->data;
  if( NULL == data )
    return;
    
  data->result = ESGUI_MBR_CANCEL;
}
#endif //< ESGUI_WIDGET_USE_MSGBOX
