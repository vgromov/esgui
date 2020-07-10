#include <esgui/include/esgui.h>
#include <esgui/include/esgui_widgets.h>
#include <esgui/include/esgui_widget_listview.h>
#include <esfwxe/utils.h>
#include <string.h>

#if defined(ESGUI_WIDGET_USE_LISTVIEW) && 1 == ESGUI_WIDGET_USE_LISTVIEW

ESGUI_WgtListViewData* esguiWgtListViewDataCreate(size_t listViewDataSize)
{
  listViewDataSize = (listViewDataSize < ESGUI_WgtListViewData_SZE) ? 
    ESGUI_WgtListViewData_SZE : 
    listViewDataSize;
    
  ESGUI_WgtListViewData* result = (ESGUI_WgtListViewData*)esguiWgtMalloc(listViewDataSize);
  
  if( result )
  {
    memset(result, 0, listViewDataSize);
    result->itemsCount = 0;
    result->bigInc = defListViewBigInc;
    result->itemIndex = -1;
    result->listData = NULL;
    result->flags = esguiWgtListviewFlagListWrap;
    result->onChangedItemIndex = NULL;
    result->onChangeItemIndex = NULL;
    result->onFmtDisplayString = NULL;
  }

  return result;
}

// list view data deallocator
void esguiWgtListViewDataFree(ESGUI_WgtListViewData* data)
{
  if( data )
  {
    data->onChangedItemIndex = NULL;
    data->onChangeItemIndex = NULL;
    data->onFmtDisplayString = NULL;
    if( 
      ES_BIT_IS_SET(data->flags, esguiWgtListviewFlagOwnsData) && 
      data->listData 
    )
      esguiWgtFree(data->listData);
    data->listData = NULL;
  }
}

// list view data deallocator handler
static void esguiWgtListViewOnDelete(ESGUI_WIDGET* This)
{
  ESGUI_WgtListViewData* data = (ESGUI_WgtListViewData*)This->data;
  esguiWgtListViewDataFree( data );
}

// internal index setter methods
static void esguiWgtListViewItemIndexSet(ESGUI_WIDGET* This, int idx)
{
  ESGUI_WgtListViewData* data = (ESGUI_WgtListViewData*)This->data;
  if( data->itemsCount )
  {
    if( idx < 0 )    
    {
      if( ES_BIT_IS_SET(data->flags, esguiWgtListviewFlagListWrap) )
        idx = data->itemsCount-1;
      else
        idx = 0;
    }
    else if( idx >= (int)data->itemsCount )
    {
      if( ES_BIT_IS_SET(data->flags, esguiWgtListviewFlagListWrap) )
        idx = 0;
      else
        idx = data->itemsCount-1;
    }
  }

  if( idx != data->itemIndex )
  {
    if( !data->onChangeItemIndex )
      data->itemIndex = idx;
    else if( !data->onChangeItemIndex(This, idx) )
      return;

    if( data->onChangedItemIndex )
      data->onChangedItemIndex(This);
    
    // request repaint
    esguiWgtPaint(
      This, 
      NULL, 
      NULL, 
      esguiWgtpfPaintForce
    );
  }
}

// standard list view event handler
void esguiWgtListViewOnEvent(ESGUI_WIDGET* This, ESGUI_WIDGET* sender, EsguiAppEventT* evt)
{
  if( ES_BIT_IS_SET(This->flags, esguiWgtfActive) ) // active editor intercepts keyboard events
  {
    ESGUI_WgtListViewData* data = (ESGUI_WgtListViewData*)This->data;
    
    if( esguiWgtEventIsCtlDec(evt) )
    {
      esguiWgtListViewItemIndexSet(This, data->itemIndex-1);
      esguiWgtEventMarkHandled(evt);
    }
    else if( esguiWgtEventIsCtlDecBig(evt) )
    {
      esguiWgtListViewItemIndexSet(This, data->itemIndex-data->bigInc);
      esguiWgtEventMarkHandled(evt);
    }
    else if( esguiWgtEventIsCtlInc(evt) )
    {
      esguiWgtListViewItemIndexSet(This, data->itemIndex+1);
      esguiWgtEventMarkHandled(evt);
    }
    else if( esguiWgtEventIsCtlIncBig(evt) )
    {
      esguiWgtListViewItemIndexSet(This, data->itemIndex+data->bigInc);
      esguiWgtEventMarkHandled(evt);
    }
    else if( esguiWgtEventIsCtlCommit(evt) )
    {
      // exit committing
      esguiWgtExit(This, sender, TRUE);
      // request repaint
      esguiWgtPaint(
        This,
        sender,
        NULL,
        esguiWgtpfPaintForce
      );
      esguiWgtEventMarkHandled(evt);
    }
  }
  else if( esguiWgtEventIsCtlEnter(evt) )
  {
    // enter widget
    esguiWgtClick(This, sender);
    esguiWgtEventMarkHandled(evt);
  }
}

// list view constructor. data instance should be initialized beforehand
ESGUI_WIDGET* esguiWgtListViewCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, ESGUI_WgtListViewData* data)
{
  ESGUI_WIDGET* result = NULL;

  result = esguiWgtCreate(
    root,
    ESGUI_EDIT,
    bounds,
    esguiWgtfDefault|esguiWgtfOwnsData
  );

  if( result )
  {
    result->data = data;
    result->onEvent = esguiWgtListViewOnEvent;
    result->onDeleteData = esguiWgtListViewOnDelete;
  }

  return result;
}

#endif //< ESGUI_WIDGET_USE_LISTVIEW
