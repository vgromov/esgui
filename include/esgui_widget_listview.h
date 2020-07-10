#ifndef _esgui_widget_listview_h_
#define _esgui_widget_listview_h_

#if defined(ESGUI_WIDGET_USE_LISTVIEW) && 1 == ESGUI_WIDGET_USE_LISTVIEW

/// Standard list view widget
///

#ifdef __cplusplus
  extern "C" {
#endif

/// List view widget
///

/// List view data
typedef struct tagESGUI_WgtListViewData
{
  esU32 itemsCount;    // count of items in list view
  int bigInc;          // value of increment for fast scrolling
  int itemIndex;      // index of the current list view item, -1 if no item selected|no data available
  int pageIndex;      // listview page index. reserved, implementation-dependent. defaults to 0 
  int pagesCount;      // listview pages count. reserved, implementation-dependent. defaults to 0 
  void* listData;      // optional pointer to list data, user defined
  esU32 flags;
  // optional callbacks (used mainly when list view is in virtual mode)
  esBL (*onChangeItemIndex)(ESGUI_WIDGET* This, size_t idx);  // called when itemIndex is about to change
  void (*onChangedItemIndex)(ESGUI_WIDGET* This);  // called when itemIndex is changed
  void (*onFmtDisplayString)(ESGUI_WIDGET* This, int idx, ESE_STR buff, size_t buffLen); // called when item string is required for painting

} ESGUI_WgtListViewData;

enum {
  // List view flags
  esguiWgtListviewFlagOwnsData    = 1,  ///< list data instance owns data block, should be freed when list data is freed
  esguiWgtListviewFlagListWrap    = 2,  ///< when min|max item index is reached - move to the other list end
  
  // Misc consts
  defListViewBigInc               = 10,
  ESGUI_WgtListViewData_SZE       = sizeof(ESGUI_WgtListViewData)
};

/// List view data standard allocator
ESGUI_WgtListViewData* esguiWgtListViewDataCreate(size_t listViewDataSize);

/// List view data deallocator
void esguiWgtListViewDataFree(ESGUI_WgtListViewData* data);

/// List view standard event handler
void esguiWgtListViewOnEvent(ESGUI_WIDGET* This, ESGUI_WIDGET* sender, EsguiAppEventT* evt);

/// List view constructor. data instance should be initialized beforehand
ESGUI_WIDGET* esguiWgtListViewCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, ESGUI_WgtListViewData* data);

#ifdef __cplusplus
  }
#endif

#endif //< ESGUI_WIDGET_USE_LISTVIEW

#endif //< _esgui_widget_listview_h_
