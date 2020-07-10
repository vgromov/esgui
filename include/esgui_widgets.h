#ifndef _es_gui_widgets_h_
#define _es_gui_widgets_h_

#if defined(ESGUI_USE_WIDGETS) && 1 == ESGUI_USE_WIDGETS

#ifdef __cplusplus
  extern "C" {
#endif

/// Special widget tag value, meaning 'None'
///
#define NO_TAG ((esU32)(-1))

/// Widget "metrics" enumeration. This stuff is implementation - dependent, and 
/// actual body for wgtMetricGet must be implemented for specific platform elsewhere
///
typedef enum {
  esguiWgtmVscrollWidth,         ///< Width of vertical scroll bar
  esguiWgtmHscrollHeight,        ///< Height of horizontal scroll bar
  esguiWgtmVscrollThumbHeight,   ///< Height of vertical scroll bar thumb
  esguiWgtmHscrollThumbWidth,    ///< Width of the horizontal scroll bar thumb
  esguiWgtmVspace,               ///< Vertical elements spacing
  esguiWgtmHspace,               ///< Horizontal elements spacing
  esguiWgtmTitleHeight,          ///< Default group widget title height
  
  // Special ID, must be the last one
  esguiWgtMetricCount

} ESGUI_WIDGET_METRIC;

/// Widget "classes"
/// 
typedef enum {
  /// Group widget classes
  /// 
  ESGUI_GROUP,          ///< Generic group. Widget group is a management container.
                        ///< It may manage its children focus, selected state, etc.
  ESGUI_STATIC_GROUP,   ///< Static group does neither support children focusing, nor
                        ///< it may be focused itself
  groupWidgets,         ///< Special marker value, to augment classification
  
  /// Non-group widget classes
  /// 
  ESGUI_STATIC,         ///< Static widget. Static widget cannot be focused
  ESGUI_CUSTOM,         ///< Any non-specific focuseable widget
  ESGUI_EDIT,           ///< Widget used to augment the interactive user input
  ESGUI_BUTTON,         ///< "Clickable" button
  ESGUI_CHECK_BUTTON,   ///< Widget which behaves like button, with its checked state flag toggled on each press
  ESGUI_RADIO_BUTTON,   ///< Widget, whose checked stated may depend on other radio buttons, when 
                        ///< contained in group 
  childWidgets,         ///< Special marker value, to augment classification

} ESGUI_WIDGET_CLASS;

/// Widget state and intent flags
///
enum {
  /// State flags
  /// 
  esguiWgtfVisible       = 0x00000001,  ///< Widget is visible
  esguiWgtfEnabled       = 0x00000002,  ///< Widget is enabled
  esguiWgtfChecked       = 0x00000004,  ///< Widget is checked
  esguiWgtfActive        = 0x00000008,  ///< Widget is active (used a priori by edit classes)
  esguiWgtfModal         = 0x00000010,  ///< Widget is modal
  esguiWgtfDefault       = esguiWgtfVisible| ///< Default flags set
                           esguiWgtfEnabled,
  esguiWgtfDeleted       = 0x00001000,  ///< Widget marked for deletion
  
  /// Invalidation flags
  /// 
  esguiWgtfInvalidFg     = 0x00000020,  ///< foreground needs repainting
  esguiWgtfInvalidBg     = 0x00000040,  ///< background needs repainting
  
  /// Creation-deletion related
  /// 
  esguiWgtfDynamic       = 0x00000080,  ///< Widget created on heap. May be deleted to free-up memory
  esguiWgtfOwnsData      = 0x00000100,  ///< Widget owns its data, delete them along with widget
  
  /// Group scrolling - related
  /// 
  esguiWgtfScrollable    = 0x00000200,  ///< Scrolling is supported
  esguiWgtfShowVscroll   = 0x00000400,  ///< Show V scroll bar for group windows
  esguiWgtfShowHscroll   = 0x00000800,  ///< Show H scroll bar for group windows
  
  /// Paint flags
  /// 
  esguiWgtpfPaintBg      = 0x00000001,  ///< Repaint widget background even if it is not marked invalid
  esguiWgtpfPaintForce   = 0x00000002,  ///< Repaint widget even if it is not marked invalid
};

// Opaque application event type
typedef void EsguiAppEventT;

// Forward declaration
struct tagESGUI_WIDGET;

// Widget event handler types
//

// Generic notification event hendler
typedef void (*ESGUI_NOTIFY_EVENT)(struct tagESGUI_WIDGET* This, struct tagESGUI_WIDGET* sender);

// Notification event with one extra bool parameter
typedef void (*ESGUI_FLAG_EVENT)(struct tagESGUI_WIDGET* This, struct tagESGUI_WIDGET* sender, bool param);

// Paint notification event
typedef void (*ESGUI_PAINT_EVENT)(ESGUI_HDC hdc, struct tagESGUI_WIDGET* This, struct tagESGUI_WIDGET* sender, const ESGUI_RECT* rect);

// Action confirmation request event
typedef bool (*ESGUI_ALLOW_EVENT)(struct tagESGUI_WIDGET* This, struct tagESGUI_WIDGET* sender,  bool param);

// Application event handler
typedef void (*ESGUI_APP_EVENT)(struct tagESGUI_WIDGET* This, struct tagESGUI_WIDGET* sender, EsguiAppEventT* evt);

// Scrollbar painter
typedef void (*ESGUI_PAINT_SCROLLBAR)(ESGUI_HDC hdc, const struct tagESGUI_WIDGET* This, const ESGUI_RECT* rect, bool vertical);

// ESGUI widgets types defs and services
//
typedef struct tagESGUI_WIDGET 
{
  struct tagESGUI_WIDGET_ROOT* root;
  // widget visual hierarchy
  //
  // parent container
  struct tagESGUI_WIDGET* parent;
  // siblings
  struct tagESGUI_WIDGET* prev;
  struct tagESGUI_WIDGET* next;
  // children
  struct tagESGUI_WIDGET* firstChild;
  struct tagESGUI_WIDGET* lastChild;
  // focused item
  struct tagESGUI_WIDGET* focused;

  // attacheable event handlers
  //
  ESGUI_NOTIFY_EVENT      onSetFocus;
  ESGUI_NOTIFY_EVENT      onResetFocus;
  ESGUI_NOTIFY_EVENT      onEnter;
  ESGUI_FLAG_EVENT        onExit;
  ESGUI_NOTIFY_EVENT      onEnable;
  ESGUI_NOTIFY_EVENT      onShow;
  ESGUI_NOTIFY_EVENT      onCheck;
  ESGUI_NOTIFY_EVENT      onClick;
  ESGUI_PAINT_EVENT       onPaint;
  ESGUI_PAINT_EVENT       onPaintBg;
  ESGUI_ALLOW_EVENT       onCanClose;
  ESGUI_APP_EVENT         onEvent;
  void                    (*onDeleteData)(struct tagESGUI_WIDGET*);
  ESGUI_NOTIFY_EVENT      onClose;
  void                    (*onClientRectGet)(const struct tagESGUI_WIDGET*, ESGUI_RECT*);
  void                    (*onVirtualClientRectGet)(const struct tagESGUI_WIDGET*, ESGUI_RECT*);
  // widget scrollbar painting handler. vertical scrollbar should be placed at the right side next to draw rect,
  // horizontal should go at the bottom of the rect specified
  ESGUI_PAINT_SCROLLBAR   onPaintScrollbar;

  // bounds, in parent coordinate space
  ESGUI_RECT              bounds;
  // virtual client origin offset, defaults to (0,0)
  ESGUI_POINT             voffs;
  // widget flags
  esU32                   flags;
  // nested paint lock
  esU32                   locked;
  // optional data & tag
  void*                   data;
  int                     tag;
  // widget class
  int                     cls;

} ESGUI_WIDGET;

// Widget root object (AKA screen)
typedef struct tagESGUI_WIDGET_ROOT {
  // common fields
  ESGUI_WIDGET cmn;
  // widgets frameword device context
  ESGUI_HDC hdc;
  // invalidated rect, in screen coordinates
  ESGUI_RECT invalidated;
  // modal widgets list. modal widgets, though normally parented inside root, have to be additionally listed in this structure
  // to allow proper separate events processing
  ESGUI_WIDGET* modals[ESGUI_MODALS_MAX_CNT];

  int modalsCnt;
  bool modalsProcessed;

} ESGUI_WIDGET_ROOT;

// ESGUI widget services. optional sender parameter allows to specify
// which widget called the service. return bool value signals if service
// call succeed.
//

/// Return widget tag value
int esguiWgtTagGet(ESGUI_WIDGET* wgt);

// Find widget by its tag. if not found, return NULL
ESGUI_WIDGET* esguiWgtFindWidgetByTag(ESGUI_WIDGET* parent, int tag);

// Access root widget
ESGUI_WIDGET* esguiWgtRootGet(ESGUI_WIDGET_ROOT* root);

// Access invalidated rect
const ESGUI_RECT* esguiWgtInvalidatedRectGet(const ESGUI_WIDGET_ROOT* root);

// Access bounding rect
const ESGUI_RECT* esguiWgtBoundsGet(const ESGUI_WIDGET* wgt);

// Update invalidated rect
void esguiWgtInvalidatedRectUpdate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* rect);

// Aeset invlaidated rect
void esguiWgtInvalidatedRectReset(ESGUI_WIDGET_ROOT* root);

// Access hdc used for widgets framework
ESGUI_HDC esguiWgtCanvasGet(const ESGUI_WIDGET_ROOT* root);
ESGUI_HDC esguiWgtCanvasGet_(const ESGUI_WIDGET* wgt);

// Convert rectangle from the client coords to another widget coordinates
void esguiWgtClientToOther(const ESGUI_WIDGET* This, const ESGUI_WIDGET* other, ESGUI_RECT* rect);

// Convert rectangle from the client to the screen coordinates
void esguiWgtClientToScreen(const ESGUI_WIDGET* This, ESGUI_RECT* rect);

// Convert rectangle from the screen to the client coordinates
void esguiWgtScreenToClient(const ESGUI_WIDGET* This, ESGUI_RECT* rect);

// Return true if widget is focused
bool esguiWgtIsFocused(const ESGUI_WIDGET* widget);

// Return true if widget is visible
bool esguiWgtIsVisible(const ESGUI_WIDGET* widget);

// Return true if widget is visible on screen area
bool esguiWgtIsVisibleOnScreen(const ESGUI_WIDGET* widget);

// Return true if widget is enabled
bool esguiWgtIsEnabled(const ESGUI_WIDGET* widget);

// Return true if widget is checked
bool esguiWgtIsChecked(const ESGUI_WIDGET* widget);

// Widget locking|unlocking
void esguiWgtLockIncrement(ESGUI_WIDGET* widget);
void esguiWgtLockDecrement(ESGUI_WIDGET* widget);

// Return widget client rectangle
void esguiWgtClientRectGet(const ESGUI_WIDGET* widget, ESGUI_RECT* rect);

// Return widget client width
int esguiWgtClientWidthGet(const ESGUI_WIDGET* widget);

// Return widget client height
int esguiWgtClientHeightGet(const ESGUI_WIDGET* widget);

// Return virtual client rect of the widget
// if widget has no children, virtual rectangle equals to the client one
// otherwise, it's union of all children's bound rects + client rectangle
//
void esguiWgtVirtualClientRectGet(const ESGUI_WIDGET* widget, ESGUI_RECT* rect);

/// Return client view area offset (used in scrollable widgets)
const ESGUI_POINT* esguiWgtViewOffsetGet(const ESGUI_WIDGET* widget);

// Return widget virtual client width
int esguiWgtVirtualClientWidthGet(const ESGUI_WIDGET* widget);

// Return widget virtual client height
int esguiWgtVirtualClientHeightGet(const ESGUI_WIDGET* widget);


// Virtual space scrolling support. try to scroll virtual client space to
// show pnt in visible client space
//
void esguiWgtScrollTo(ESGUI_WIDGET* widget, const ESGUI_POINT* pnt);

// Allocate and initialize widget on heap. widget allocated on heap
// is considered to own its data and children
//
ESGUI_WIDGET* esguiWgtCreate(ESGUI_WIDGET_ROOT* root, ESGUI_WIDGET_CLASS cls, const ESGUI_RECT* bounds, esU32 flags);

// De-allocate widget. it automatically de-allocates related data
// and children. must not be called directly, especially from handler services
// instead, it's called in application event handling loop, as a response to the
// MSG_WGT_DELETE message
//
void esguiWgtDelete(ESGUI_WIDGET* widget);

// Widget initialization. all parameters apart from cls, are optional
void esguiWgtInit(ESGUI_WIDGET_ROOT* root, ESGUI_WIDGET* widget, ESGUI_WIDGET_CLASS cls, const ESGUI_RECT* bounds, esU32 flags);

// Initialize root widget  singleton
void esguiWgtRootInit(ESGUI_WIDGET_ROOT* root, ESGUI_HDC hdc, const ESGUI_RECT* bounds);

// Modal widgets management
//

// Add modal widget to the root hierarchy
bool esguiWgtModalAdd(ESGUI_WIDGET* widget);

// Widget hierarchy manipulation service
bool esguiWgtParentSet(ESGUI_WIDGET* widget, ESGUI_WIDGET* parent);

// Set input focus to the specified widget
bool esguiWgtFocusSet(ESGUI_WIDGET* wgt, ESGUI_WIDGET* sender);

// Set input focus to the first focuseable widget in the top group
bool esguiWgtFocusSetFirst(ESGUI_WIDGET* group, ESGUI_WIDGET* sender);

// Set input focus to the last focuseable widget in the top group
bool esguiWgtFocusSetLast(ESGUI_WIDGET* group, ESGUI_WIDGET* sender);

// Move focus to the next widget in the top group. if allowWrap is true
// the will move from the last focuseable widget to the first one
//
bool esguiWgtFocusSetNext(ESGUI_WIDGET* group, ESGUI_WIDGET* sender, bool allowWrap);

// Move focus to the prev widget in the top group. if allowWrap is true
// the will move from the first focuseable widget to the last one
//
bool esguiWgtFocusSetPrev(ESGUI_WIDGET* group, ESGUI_WIDGET* sender, bool allowWrap);

// "enter" the specified widget, if the latter supports it. whether the widget is
// "entered" is signalled by the "active" flag
//
void esguiWgtEnter(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender);

// "exit" the specified widget, if the latter supports it. optional "commit"
// parameter may specify whether possible changes to the data-aware widget
// should be committed upon exit from the widget
//
void esguiWgtExit(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool commit);

// Set widget visibility flag to the value of "show" parameter
void esguiWgtShow(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool show);

// Move target widget on top of subject specified, and show it
void esguiWgtShowOnTop(ESGUI_WIDGET* target, ESGUI_WIDGET* subject);

// Ask widget confirmation for closing. commit flag signals if possible changes
// to widget data should be comitted before closing
bool esguiWgtCanClose(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool commit);

// Close widget, optionally comitting changes.
void esguiWgtClose(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool commit);

// Set widget enabled flag to the value of "enable" parameter
void esguiWgtEnable(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool enable);

// Set widget checked flag to the value of "check" parameter
void esguiWgtCheck(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool check);

// "click" on widget
void esguiWgtClick(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender);

// Paint widget. rect is optional parameter specifying which portion of client
// area to draw. otherwise, entire widget is redrawn. force signals that
// widget has to be redrawn even it's not invalidated.
//
void esguiWgtPaint(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, const ESGUI_RECT* rect, esU32 flags);

// Invalidate widget rect, rect is in widget client coordinates, or 0 if entire widget has to be invalidated
void esguiWgtInvalidate(ESGUI_WIDGET* widget, const ESGUI_RECT* rect, bool paintBg);

// Handle application event starting from root
void esguiWgtHandleEvent(ESGUI_WIDGET_ROOT* root, ESGUI_WIDGET* sender, EsguiAppEventT* evt);

/// Services to be implemented in application:
///

/// void* esguiWgtMalloc(size_t);
/// void esguiWgtFree(void*);

/// @brief Widget metrics value getter
/// @param [in] metric Requested metic code
/// @return Requested metric value
/// 
int esguiWgtMetricGet(int metric);

/// Application-specific service to mark an event as 'handled'
void esguiWgtEventMarkHandled(EsguiAppEventT* evt);

/// Application-specific service to check if an event is marked as 'handled'
bool esguiWgtEventIsHandled(EsguiAppEventT* evt);

/// Application-specific service to check if an event a per-second tick notification
bool esguiWgtEventIsSecondTick(EsguiAppEventT* evt);

/// Application-specific service to check if an event is an interactive one -
/// i.e. from Human-Device Interface, like keyboard, mouse, touchpad, etc.
///
bool esguiWgtEventIsInteractive(EsguiAppEventT* evt);

/// Application-specific service to post a widget paint event
void esguiWgtPaintPost(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, const ESGUI_RECT* rect, esU32 flags);

/// Application-specific service to extract a widget paint event and its data
bool esguiWgtPaintEvtExtract(EsguiAppEventT* evt, ESGUI_WIDGET** pwgtSender, ESGUI_WIDGET** pwgt, ESGUI_RECT* ppaintRect, esU32* paintFlags);

#ifdef __cplusplus
  }
#endif

#endif //< ESGUI_USE_WIDGETS

#endif // _es_gui_widgets_h_

