#include <esgui/include/esgui.h>
#include <esfwxe/ese_assert.h>
#include <esgui/include/esgui_widgets.h>
#include <esfwxe/utils.h>
#include <esfwxe/trace.h>
#include <stdlib.h>
#include <string.h>

#if defined(ESGUI_USE_WIDGETS) && 1 == ESGUI_USE_WIDGETS

// initialize root widget  singleton
void esguiWgtRootInit(ESGUI_WIDGET_ROOT* root, ESGUI_HDC hdc, const ESGUI_RECT* bounds)
{
  if( !hdc )
    return;
    
  if( !bounds )
    bounds = &hdc->ext;

  // initialize common widget fields
  esguiWgtInit(
    root,
    &root->cmn, 
    ESGUI_GROUP, 
    bounds, 
    esguiWgtfEnabled
  );
  // assign device context
  root->hdc = hdc;
  // initialize modal list
  root->modalsCnt = 0;
  root->modalsProcessed = FALSE;
  // initialize invalidated rect  
  root->invalidated = *bounds;
}

int esguiWgtTagGet(ESGUI_WIDGET* wgt)
{
  if( !wgt )
    return -1;
    
  return wgt->tag;
}

// find widget by its tag. if not found, return 0
ESGUI_WIDGET* esguiWgtFindWidgetByTag(ESGUI_WIDGET* parent, int tag)
{
  if( parent )
  {
    if( tag == parent->tag )
      return parent;
    else
    {
      ESGUI_WIDGET* wgt = parent->firstChild;
      while( wgt )
      {
        if( tag == wgt->tag )
          return wgt;
        else
        {
          ESGUI_WIDGET* child = esguiWgtFindWidgetByTag(wgt, tag);
          if( child )
            return child;
        }
          
        wgt = wgt->next;
      }
    }
  }
  
  return NULL;
}

// access root widget
ESGUI_WIDGET* esguiWgtRootGet(ESGUI_WIDGET_ROOT* root)
{
  return &root->cmn;
}

// access invalidated rect
const ESGUI_RECT* esguiWgtInvalidatedRectGet(const ESGUI_WIDGET_ROOT* root)
{
  if( !root )
    return NULL;

  return &root->invalidated;
}

const ESGUI_RECT* esguiWgtBoundsGet(const ESGUI_WIDGET* wgt)
{
  if( !wgt )
    return NULL;
  
  return &wgt->bounds;
}

void esguiWgtInvalidatedRectUpdate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* rect)
{
  if( rect )
  {
    if( esguiRectIsZero(&root->invalidated) )
      root->invalidated = *rect;
    else
      esguiRectUnion( 
        &root->invalidated, 
        rect, 
        &root->invalidated
      );
  }
}

// reset invlaidated rect
void esguiWgtInvalidatedRectReset(ESGUI_WIDGET_ROOT* root)
{
  root->invalidated = c_zeroRect;
}

// access hdc used for widgets framework
ESGUI_HDC esguiWgtCanvasGet(const ESGUI_WIDGET_ROOT* root)
{
  return root->hdc;
}

ESGUI_HDC esguiWgtCanvasGet_(const ESGUI_WIDGET* wgt)
{
  if(!wgt || !wgt->root)
    return NULL;
  
  return wgt->root->hdc;
}

// esgui widget services implementation. optional sender parameter allows to specify
// which widget called the service. return bool value signals if service
// call succeed.
//
//---------------------------------------------------------------------------
// internal services
//

// debug trace
#if defined(USE_TRACE) && defined(USE_ESGUI_WIDGET_TRACE)
#  include <esfwxe/trace.h> 
static ESE_CSTR c_wgtClassNames[] = 
    { "ESGUI_GROUP",             
      "ESGUI_STATIC_GROUP",      
      "groupWidgets",            
      "ESGUI_STATIC",            
      "ESGUI_CUSTOM",            
      "ESGUI_EDIT",              
      "ESGUI_BUTTON",            
      "ESGUI_CHECK_BUTTON",      
      "ESGUI_RADIO_BUTTON",     
      "childWidgets" };

static void traceWgtDeleting(const ESGUI_WIDGET* wgt)
{
  if( wgt )  {
    TRACE1("deleting widget of class "TRACE_FMT_STR"...", c_wgtClassNames[wgt->cls])
  }
}
#define TRACE_WGT_DELETING(wgt)  traceWgtDeleting(wgt);

static void traceWgtRect(const ESGUI_RECT* rect)
{
  if( rect )
  {
    TRACE4("  rect=(x0=%d, y0=%d, x1=%d, y1=%d)", rect->org.x, rect->org.y, rect->edge.x, rect->edge.y)
  }
  else
  {
    TRACE0("  rect=NULL")
  }
}

static void traceWgt(const ESGUI_WIDGET* wgt, ESE_CSTR name)
{
  if(wgt)
  {
    TRACE4("  "TRACE_FMT_STR"=%p(tag=%d, class="TRACE_FMT_STR, name, wgt, wgt->tag, c_wgtClassNames[wgt->cls])
    TRACE1("  parent=%p", wgt->parent)
    traceWgtRect(&wgt->bounds);
    TRACE5("  visible="TRACE_FMT_STR", enabled="TRACE_FMT_STR", checked="TRACE_FMT_STR", active="TRACE_FMT_STR", modal="TRACE_FMT_STR,
      TRACE_BOOLSTR(ES_BIT_IS_SET(wgt->flags, esguiWgtfVisible)), TRACE_BOOLSTR(ES_BIT_IS_SET(wgt->flags, esguiWgtfEnabled)),
      TRACE_BOOLSTR(ES_BIT_IS_SET(wgt->flags, esguiWgtfChecked)), TRACE_BOOLSTR(ES_BIT_IS_SET(wgt->flags, esguiWgtfActive)),
      TRACE_BOOLSTR(ES_BIT_IS_SET(wgt->flags, esguiWgtfModal)))
    TRACE5("  invalidFg="TRACE_FMT_STR", invalidBg="TRACE_FMT_STR", dynamic="TRACE_FMT_STR", ownsData="TRACE_FMT_STR", deleted="TRACE_FMT_STR")",
      TRACE_BOOLSTR(ES_BIT_IS_SET(wgt->flags, esguiWgtfInvalidFg)), TRACE_BOOLSTR(ES_BIT_IS_SET(wgt->flags, esguiWgtfInvalidBg)),
      TRACE_BOOLSTR(ES_BIT_IS_SET(wgt->flags, esguiWgtfDynamic)), TRACE_BOOLSTR(ES_BIT_IS_SET(wgt->flags, esguiWgtfOwnsData)),
      TRACE_BOOLSTR(ES_BIT_IS_SET(wgt->flags, esguiWgtfDeleted)))
  }
  else
  {
    TRACE1("  "TRACE_FMT_STR"=NULL", name)
  }
}

static void traceWgtDeleted(void)
{
    TRACE0("widget was deleted")
    TRACE_MEM
}

static void traceWgtCreate(const ESGUI_WIDGET* wgt, ESE_CSTR name)
{  
  if( wgt )  
    traceWgt(wgt, name);
  else 
  {
    TRACE1("failed to create widget "TRACE_FMT_STR, name) 
  }
  TRACE_MEM 
}

#  define TRACE_WGT_CREATE(wgt)   traceWgtCreate(wgt, "widget");
#  define TRACE_WGT_DELETED  traceWgtDeleted();
#  define TRACE_WGT(wgt, name)  traceWgt(wgt, name);
#  define TRACE_WGT_RECT(rect) traceWgtRect(rect);
#else
#  define TRACE_WGT_CREATE(wgt)
# define TRACE_WGT_DELETING(wgt)
# define TRACE_WGT_DELETED
#  define TRACE_WGT(wgt, name)
#  define TRACE_WGT_RECT(rect)
#endif // USE_TRACE

// return true if widget may be focused, false otherwise
static inline bool esguiWgtIsFocuseable(ESGUI_WIDGET* widget)
{
  return widget &&
    (widget->cls == ESGUI_GROUP ||
    (widget->cls > ESGUI_STATIC &&
    widget->cls < childWidgets)) &&
    esguiWgtIsVisible(widget) && 
    esguiWgtIsEnabled(widget);
}

// return true if widget is of group-like class
static inline bool esguiWgtIsGroup(ESGUI_WIDGET* widget)
{
  return widget->cls < groupWidgets;
}

// return true if group may manage focused state of its items
static inline bool esguiWgtIsFocuseableGroup(ESGUI_WIDGET* widget)
{
  return widget->cls < groupWidgets && widget->cls != ESGUI_STATIC_GROUP;
}

// return widget's client rect
static void esguiWgtClientRectGetStd(const ESGUI_WIDGET* This, ESGUI_RECT* rect)
{
  if( This )
    esguiRectSet_(
      0, 
      0, 
      esguiRectWidth(&This->bounds), 
      esguiRectHeight(&This->bounds), 
      rect
    );  
  else if( rect )
    *rect = c_zeroRect;
}

void esguiWgtClientRectGet(const ESGUI_WIDGET* This, ESGUI_RECT* rect)
{
  esguiWgtClientRectGetStd(This, rect);
  if( This && This->onClientRectGet )
    This->onClientRectGet(This, rect);
  if( ES_BIT_IS_SET(This->flags, esguiWgtfScrollable) )
  {
    if( ES_BIT_IS_SET(This->flags, esguiWgtfShowVscroll) )
      rect->edge.x -= esguiWgtMetricGet(esguiWgtmVscrollWidth);
    if( ES_BIT_IS_SET(This->flags, esguiWgtfShowHscroll) )
      rect->edge.y -= esguiWgtMetricGet(esguiWgtmHscrollHeight);  
  }    
}

int esguiWgtClientWidthGet(const ESGUI_WIDGET* widget)
{
  if( !widget )
    return 0;
    
  ESGUI_RECT cr;
  esguiWgtClientRectGet(widget, &cr);
  
  return esguiRectWidth( &cr );
}

int esguiWgtClientHeightGet(const ESGUI_WIDGET* widget)
{
  if( !widget )
    return 0;

  ESGUI_RECT cr;
  esguiWgtClientRectGet(widget, &cr);
  
  return esguiRectHeight( &cr );
}

// return virtual client rect of the widget
// if widget has no children, virtual rectangle equals to the client one
// otherwise, it's union of all children's bound rects + client rectangle
void esguiWgtVirtualClientRectGet(const ESGUI_WIDGET* widget, ESGUI_RECT* rect)
{
  esguiWgtClientRectGet(
    widget, 
    rect
  );
  
  if( widget )
  {
    const ESGUI_WIDGET* child = widget->firstChild;
    while( child )
    {
      esguiRectUnion(
        rect, 
        &child->bounds, 
        rect
      );
      
      child = child->next;
    }
    
    if( widget->onVirtualClientRectGet )
      widget->onVirtualClientRectGet(
        widget,
        rect
      );
  }
}

const ESGUI_POINT* esguiWgtViewOffsetGet(const ESGUI_WIDGET* widget)
{
  if(!widget)
    return NULL;
    
  return &widget->voffs;
}

int esguiWgtVirtualClientWidthGet(const ESGUI_WIDGET* widget)
{
  if( !widget )
    return 0;
    
  ESGUI_RECT vcr;
  esguiWgtVirtualClientRectGet(
    widget,
    &vcr
  );
  
  return esguiRectWidth(&vcr);
}

int esguiWgtVirtualClientHeightGet(const ESGUI_WIDGET* widget)
{
  if( !widget )
    return 0;

  ESGUI_RECT vcr;
  esguiWgtVirtualClientRectGet(
    widget,
    &vcr
  );
  
  return esguiRectHeight(&vcr);
}

// convert rectangle from the client coords to another widget coordinates
void esguiWgtClientToOther(const ESGUI_WIDGET* This, const ESGUI_WIDGET* other, ESGUI_RECT* rect)
{
  if( This && other && This != other )
  {
    ESGUI_RECT otherBounds = other->bounds;
    esguiWgtClientToScreen(This, rect);
    esguiWgtClientToScreen(other, &otherBounds);
    rect->org.x = rect->org.x - otherBounds.org.x;
    rect->org.y = rect->org.y - otherBounds.org.y;
  }
}

// convert rectangle from the client coords to screen coordinates
void esguiWgtClientToScreen(const ESGUI_WIDGET* This, ESGUI_RECT* rect)
{
  if( This && rect )
  {
    ESGUI_WIDGET* parent = This->parent;
    esguiRectOffset(
      &This->bounds.org, 
      rect
    );

    while( parent )
    {
      esguiRectOffset(
        &parent->bounds.org, 
        rect
      );
      parent = parent->parent;
    }
  }
}

// convert rectangle from the screen to the client coordinates
void esguiWgtScreenToClient(const ESGUI_WIDGET* This, ESGUI_RECT* rect)
{
  if( This && rect )
  {
    ESGUI_RECT client;
    esguiWgtClientRectGetStd(This, &client);
    esguiWgtClientToScreen(This, &client);
    esguiRectOffset_(
      -client.org.x, 
      -client.org.y, 
      rect
    );
  }
}

// virtual space scrolling support. try to scroll virtual client space to 
// show pnt in visible client space
static void esguiWgtInternalScrollTo(ESGUI_WIDGET* widget, const ESGUI_POINT* pnt, ESGUI_RECT* cr, bool* scrollH, bool* scrollV)
{
  *scrollV = *scrollH = false;
  if( widget && 
      ES_BIT_IS_SET(widget->flags, esguiWgtfScrollable) )
  {
    ESGUI_RECT vcr;
    ESGUI_POINT dest = *pnt;
    ESGUI_POINT voffs;
    esguiWgtClientRectGet(widget, cr);
    esguiWgtVirtualClientRectGet(widget, &vcr);
    
    // normalize dest to the virtual rect
    if( dest.x < vcr.org.x )
      dest.x = vcr.org.x;
    if( dest.y < vcr.org.y )
      dest.y = vcr.org.y;
    if( dest.x > vcr.edge.x )
      dest.x = vcr.edge.x;
    if( dest.y > vcr.edge.y )
      dest.y = vcr.edge.y;
    
    // try to put top left of virtual painting space to the dest
    voffs.y = vcr.edge.y-dest.y;
    if( voffs.y < esguiRectHeight(cr) )
      voffs.y = esguiRectHeight(&vcr)-esguiRectHeight(cr);
    else
      voffs.y = dest.y-cr->org.y;
    
    voffs.x = vcr.edge.x-dest.x;
    if( voffs.x < esguiRectWidth(cr) )
      voffs.x = esguiRectWidth(&vcr)-esguiRectWidth(cr);
    else
      voffs.x = dest.x-cr->org.x;
    
    *scrollH = widget->voffs.x != voffs.x;
    *scrollV = widget->voffs.y != voffs.y;
    
    if( *scrollH || *scrollV )
      widget->voffs = voffs;
  }
}

static void esguiWgtInternalRepaintOnScroll(ESGUI_WIDGET* widget, ESGUI_RECT* cr, bool scrollH, bool scrollV)
{
  if( scrollH || scrollV )
  {
    // upon entering this call cr contains widget client rectangle - scrollbar areas
    // repaint with background erase, add scrollbar rects to the cr and call paint with background erase
    if( scrollV && 
        ES_BIT_IS_SET(widget->flags, esguiWgtfShowVscroll) )
      cr->edge.x += esguiWgtMetricGet(esguiWgtmVscrollWidth);
      
    if( scrollH && 
        ES_BIT_IS_SET(widget->flags, esguiWgtfShowHscroll) )
      cr->edge.y += esguiWgtMetricGet(esguiWgtmHscrollHeight);

    esguiWgtPaint(
      widget, 
      NULL, 
      cr, 
      esguiWgtpfPaintForce|esguiWgtpfPaintBg
    );
  }
}

void esguiWgtScrollTo(ESGUI_WIDGET* widget, const ESGUI_POINT* pnt)
{
  if( widget && 
      ES_BIT_IS_SET(widget->flags, esguiWgtfScrollable) )
  {
    bool scrollH, scrollV;
    ESGUI_RECT cr;
    esguiWgtInternalScrollTo(widget, pnt, &cr, &scrollH, &scrollV);
    esguiWgtInternalRepaintOnScroll(widget, &cr, scrollH, scrollV);
  }
}

// standard pre-drawing processing. optional clip rect is in widget's client coordinates
static bool esguiWgtBeforePaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_RECT* drawRect, ESGUI_RECT* oldClip, const ESGUI_RECT* clip)
{
  ESGUI_RECT newClip;
  const ESGUI_RECT* currentClip = esguiCanvasGetClipRect(hdc);
 
  ESE_ASSERT(oldClip);
  ESE_ASSERT(currentClip);

  *oldClip = *currentClip;
//  ES_DEBUG_TRACE4("esguiWgtBeforePaint oldClip=(%d,%d,%d,%d)\n", oldClip->org.x, oldClip->org.y, oldClip->edge.x, oldClip->edge.y )

  esguiWgtClientRectGetStd(This, drawRect);
  esguiWgtClientToScreen(This, drawRect);
  
  // take possible virtual offset into account
  if(This->parent)
    esguiRectOffset_(
      -This->parent->voffs.x,
      -This->parent->voffs.y,
      drawRect
    );

  if( clip )
  {
    newClip = *clip;
    esguiWgtClientToScreen(This, &newClip);
  }
  else
    newClip = *oldClip;

  if( !esguiRectIntersect(drawRect, &newClip, &newClip) )
    return FALSE;

  esguiCanvasSetClipRect(hdc, &newClip);

  return TRUE;
}

// standard post-drawing processing.
static inline void esguiWgtAfterPaint(ESGUI_HDC hdc, const ESGUI_RECT* oldClip)
{
  // restore saved clipping rectangle
  esguiCanvasSetClipRect(hdc, oldClip);
}

// return TRUE if widget is "closed" i.e. not visible and not active
// and does not have parent
static inline bool esguiWgtIsClosed(ESGUI_WIDGET* widget)
{
  return  !ES_BIT_IS_SET(widget->flags, esguiWgtfVisible) && 
          !ES_BIT_IS_SET(widget->flags, esguiWgtfActive) && 
          !widget->parent;
}

// internal widget paint lock api
//
static bool esguiWgtIsLocked(ESGUI_WIDGET* widget)
{
  bool result = FALSE;
  ESGUI_WIDGET* wgt = widget;

  while( wgt && !result )
  {
    result =  0 != wgt->locked;
    wgt = wgt->parent;
  }
  
  return result;
}

void esguiWgtLockIncrement(ESGUI_WIDGET* widget)
{
  if( widget )
  {
    ++widget->locked;
#if !defined(NDEBUG) && defined(USE_ESGUI_WIDGET_TRACE)
    TRACE0("esguiWgtLockIncrement(")
    TRACE_WGT(widget, "widget")
    TRACE1("  ), locked=%d", widget->locked);
#endif
  }
}

void esguiWgtLockDecrement(ESGUI_WIDGET* widget)
{
  if( widget && widget->locked )
  {
    --widget->locked;
#if !defined(NDEBUG) && defined(USE_ESGUI_WIDGET_TRACE)
    TRACE0("esguiWgtLockDecrement(")
    TRACE_WGT(widget, "widget")
    TRACE1("  ), locked=%d", widget->locked);
#endif
    if( 0 == widget->locked )
    {
      ESGUI_RECT invalidated = widget->root->invalidated;

      // repaint widget, using aggregated invalidatedRect from the root widget
      esguiWgtScreenToClient(
        widget, 
        &invalidated
      );
      
      esguiWgtPaint(
        widget, 
        NULL, 
        &invalidated, 
        NULL
      );

      // if it's top unlocked widget, immediately reset invalidated rectangle
      if( !esguiWgtIsLocked(widget) )
        esguiWgtInvalidatedRectReset(widget->root);
    }
  }
}

//---------------------------------------------------------------------------
// public services
//  

// return true if widget is enabled
bool esguiWgtIsEnabled(const ESGUI_WIDGET* widget)
{
  bool result = widget != NULL;
  const ESGUI_WIDGET* wgt = widget;

  while( result && wgt )
  {
    result = wgt && ES_BIT_IS_SET(wgt->flags, esguiWgtfEnabled);
    if( wgt != &widget->root->cmn )
      result = result && 0 != wgt->parent;
    wgt = wgt->parent;
  }
  
  return result;
}

// return true if widget is focused
bool esguiWgtIsFocused(const ESGUI_WIDGET* widget)
{
  return widget && widget->parent && widget->parent->focused == widget;
}

// return true if widget is visible
bool esguiWgtIsVisible(const ESGUI_WIDGET* widget)
{
  bool result = widget != NULL;
  const ESGUI_WIDGET* wgt = widget;

  while( wgt && result )
  {
    result = ES_BIT_IS_SET(wgt->flags, esguiWgtfVisible);
    if( wgt != &widget->root->cmn )
      result = result && 0 != wgt->parent;
    wgt = wgt->parent;
  }

  return result;
}

bool esguiWgtIsChecked(const ESGUI_WIDGET* widget)
{
  return widget && ES_BIT_IS_SET(widget->flags, esguiWgtfChecked);
}

static inline void esguiWgtWidgetScreenRectGet(const ESGUI_WIDGET* This, ESGUI_RECT* rect)
{
  *rect = This->bounds;
  if( This->parent )
  {
    esguiRectOffset_(-This->parent->voffs.x, -This->parent->voffs.y, rect);
    esguiWgtClientToScreen(This->parent, rect);
  }
}

// return true if widget is visible on screen area
bool esguiWgtIsVisibleOnScreen(const ESGUI_WIDGET* widget)
{
  // check if widget is within parent's client area
  if(  esguiWgtIsVisible(widget) )
  {
    ESGUI_RECT r, client;
    ESGUI_WIDGET* next;
    ESGUI_WIDGET* parent = widget->parent;
    
    if( widget != &widget->root->cmn )
    {
      esguiWgtClientRectGet( parent, &r );
      esguiRectOffset( &parent->voffs, &r );
      if( !esguiRectIntersect(&widget->bounds, &r, NULL) )
        return FALSE;

      // check if we're covered with siblings
      next = widget->next;
      while( next )
      {
        if( esguiWgtIsVisible(next) &&
          esguiRectIntersect(&next->bounds, &r, NULL) && 
          esguiRectInRect(&widget->bounds, &next->bounds) )
          return FALSE;

        next = next->next;
      }

      // convert portion of visible widget's rect to screen coordinates
      esguiWgtClientRectGet(parent, &client);
      r = widget->bounds;
      esguiRectOffset_( -parent->voffs.x, -parent->voffs.y, &r );
      esguiRectIntersect(&client, &r, &r);
      esguiWgtClientToScreen(parent, &r);

      while( parent )
      {
        // check all parent's visible siblings
        next = parent->next;
        // check rectangles of visible siblings
        while( next )
        {
          if( esguiWgtIsVisible(next) )
          {
            esguiWgtWidgetScreenRectGet(next, &client);            
            if( esguiRectInRect(&r, &client) )
              return FALSE;
          }

          next = next->next;
        }
        // go to upper level
        parent = parent->parent;
      }  
    }
    
    return TRUE;  
  }

  return FALSE;
}

// allocate and initialize widget on heap. widget allocated on heap
// is considered to own its data and children
ESGUI_WIDGET* esguiWgtCreate(ESGUI_WIDGET_ROOT* root, ESGUI_WIDGET_CLASS cls, const ESGUI_RECT* bounds, esU32 flags)
{
  // allocate memory for the widget
  ESGUI_WIDGET* result = (ESGUI_WIDGET*)esguiWgtMalloc( sizeof(ESGUI_WIDGET) );
  
  TRACE0("esguiWgtCreate{")
  
  if( result )
    esguiWgtInit(
      root, 
      result, 
      cls, 
      bounds, 
      flags|esguiWgtfDynamic
    );  // initialize basic widget data

  TRACE_WGT_CREATE(result)
  TRACE0("}")

  return result;
}

// find rectangle, not bigger than rect, which should be updated in
// widget's parent, when the widget is about to be re-drawn 
// that's to optimize drawing calls if widget being removed is partially
// covered by its siblings
static void esguiWgtCalcInvalidationRect(const ESGUI_WIDGET* widget, ESGUI_RECT* rect)
{
  //ESGUI_WIDGET* wgt = widget->next;
  //int left, top, right, bottom;
  *rect = widget->bounds;
  //// iterate siblings, which are higher on z-order, than widget itself
  //while( wgt )
  //{
  //  ESGUI_RECT r = wgt->bounds;
  //  // check top border
  //  if( pointInRect( &rect->org, &r) )
  //  wgt = wgt->next;
  //}
}

static void esguiWgtRemoveModalFromRoot(ESGUI_WIDGET* modal)
{
  if( 
    modal && 
    ES_BIT_IS_SET(modal->flags, esguiWgtfModal) 
  )
  {
    int idx;
    for(idx = 0; idx < modal->root->modalsCnt; ++idx)
    {
      if( modal == modal->root->modals[idx] )
      {
        // copy higher modals down to our position
        while(idx < modal->root->modalsCnt-1)
        {
          modal->root->modals[idx] = modal->root->modals[idx+1];
          ++idx;
        }
        // decrement modals count
        --modal->root->modalsCnt;
        // leave external loop
        break;
      }
    }
  }
}

// internal service to remove widget from its parent
static inline void esguiWgtRemoveFromParent(ESGUI_WIDGET* widget)
{
  // remove widget from its parent
  ESGUI_WIDGET* parent = widget->parent;
  ESGUI_RECT localInvalid;
  bool needRedraw = ES_BIT_IS_SET(widget->flags, esguiWgtfVisible);

  TRACE0("esguiWgtRemoveFromParent{")
  // find rect, not bigger than localInvalid, which should be updated in parent
  // that's to optimize drawing calls if widget being removed is partially
  // covered by its siblings
  if( needRedraw )
  {
    esguiWgtLockIncrement(parent);
    esguiWgtCalcInvalidationRect(widget, &localInvalid);
  }
  // remove from the modal list, if needed
  esguiWgtRemoveModalFromRoot(widget);

  if( widget->prev )
    widget->prev->next = widget->next;
  else
  {  // widget is the first child
    parent->firstChild = widget->next;

    if( parent->firstChild )
      parent->firstChild->prev = NULL;
  }

  if( widget->next )
    widget->next->prev = widget->prev;
  else
  {  // widget is the last child
    parent->lastChild = widget->prev;

    if( parent->lastChild )
      parent->lastChild->next = NULL;
  }
  
  // reset focused intem in parent
  if( widget == parent->focused )  
    parent->focused = NULL;

  // zero parent, prev and next fields - widget is extracted from the chain
  widget->parent = NULL;
  widget->prev = NULL;
  widget->next = NULL;

  if( needRedraw )
  {
    esguiWgtInvalidate(
      parent, 
      &localInvalid, 
      TRUE
    );
    
    esguiWgtLockDecrement(parent);
  }

  TRACE0("}")
}

// de-allocate widget. it automatically de-allocates related data
// and children
void esguiWgtDelete(ESGUI_WIDGET* widget)
{
  TRACE0("esguiWgtDelete{")
  if( widget )
  {
    TRACE_WGT_DELETING(widget)
    // mark as deleted
    ES_BIT_SET(widget->flags, esguiWgtfDeleted);
    // remove widget from parent chain, just in case
    if( widget->parent )
      esguiWgtRemoveFromParent(widget);

    // delete widget's children, if needed
    while( widget->lastChild )
      esguiWgtDelete( widget->lastChild );    

    // free data
    //
    // call custom data de-allocator, if set
    if( widget->onDeleteData )
      widget->onDeleteData(widget);        
    // next, check if widget is owning its data, and if so, deallocate data itself
    if( ES_BIT_IS_SET(widget->flags, esguiWgtfOwnsData) && widget->data )
      esguiWgtFree(widget->data);
    // reset data pointer
    widget->data = NULL;

    // free widget itself
    if( ES_BIT_IS_SET(widget->flags, esguiWgtfDynamic) )
      esguiWgtFree(widget);

    TRACE_WGT_DELETED
  }

  TRACE0("}")
}

// widget initialization. all parameters apart from cls, are optional
void esguiWgtInit(ESGUI_WIDGET_ROOT* root, ESGUI_WIDGET* widget, ESGUI_WIDGET_CLASS cls, const ESGUI_RECT* bounds, esU32 flags)
{
  if( widget )
  {
    TRACE0("esguiWgtInit{")
    
    memset(
      widget, 
      0, 
      sizeof(ESGUI_WIDGET)
    );
    
    widget->root = root;
    widget->cls = cls;

    if( bounds )
      widget->bounds = *bounds;
      
    widget->flags = flags;

#if !defined(NDEBUG) && defined(USE_ESGUI_WIDGET_TRACE)        
    TRACE0("esguiWgtInit(")
    TRACE_WGT(widget, "widget")
#endif
    TRACE0("}")
  }
}

static bool esguiWgtAddModalToRoot(ESGUI_WIDGET* modal)
{
  if( modal && 
      ES_BIT_IS_SET(modal->flags, esguiWgtfModal) && 
      modal->root->modalsCnt < ESGUI_MODALS_MAX_CNT 
  )
  {
    int idx;
    // check if it is not in modals already
    for( idx = 0; idx < modal->root->modalsCnt; ++idx )
      if( modal == modal->root->modals[idx] )
        return FALSE;

    // modal gets added to the top of the list
    modal->root->modals[modal->root->modalsCnt] = modal;
    ++modal->root->modalsCnt;
    return TRUE;
  }

  return FALSE;
}

// add modal widget to the root hierarchy
bool esguiWgtModalAdd(ESGUI_WIDGET* widget)
{
  bool result;
  TRACE0("esguiWgtModalAdd{")
  result = esguiWgtAddModalToRoot(widget) && 
    esguiWgtParentSet(widget, &widget->root->cmn);
  TRACE0("}")
  return result;
}

// find the first modal widget among the specified widget's children
// return 0 if none was found
static ESGUI_WIDGET* esguiWgtFindFirstModal(ESGUI_WIDGET* widget)
{
  ESGUI_WIDGET* result = widget->firstChild;
  while( result )
  {
    if( ES_BIT_IS_SET(result->flags, esguiWgtfModal) )
      return result;

    result = result->next;
  }

  return 0;
}

// widget hierarchy manipulation services
bool esguiWgtParentSet(ESGUI_WIDGET* widget, ESGUI_WIDGET* parent)
{
  bool result = FALSE;
  TRACE0("esguiWgtParentSet(")
#if !defined(NDEBUG) && defined(USE_ESGUI_WIDGET_TRACE)
  TRACE_WGT(widget, "widget")
  TRACE_WGT(parent, "parent")
  TRACE0("{")
#endif
  // prevent self recursion && multiple insertion
  if( widget && widget != parent && 
      parent != widget->parent)
  {
    // 1) if widget already has parent set - remove from it
    if( widget->parent )
      esguiWgtRemoveFromParent(widget);

    // 2) if new parent is not NULL - add widget to its chain
    // if parent has modal widgets added, and this widget is not modal,
    // inset this widget under the first modal widget
    if( parent )
    {
      // add widget to the end of parent's children list, if there is no modal widgets set
      // otherwise, insert widget before the first modal one
      if( parent->lastChild )
      {
        ESGUI_WIDGET* firstModal = esguiWgtFindFirstModal(parent);
        if( !ES_BIT_IS_SET(widget->flags, esguiWgtfModal) && 
            firstModal )
        {
          // handle the case when the first modal is the first child in parent
          if( parent->firstChild == firstModal )
          {
            widget->prev = NULL;
            widget->next = firstModal;
            firstModal->prev = widget;
            parent->firstChild = widget;
          }
          else
          {
            // insert inside widgets chain
            firstModal->prev->next = widget;
            widget->prev = firstModal->prev;
            firstModal->prev = widget;
            widget->next = firstModal;
          }
        }
        else
        {
          parent->lastChild->next = widget;
          widget->prev = parent->lastChild;
          parent->lastChild = widget;
          widget->next = NULL;
        }
      }
      else // this is the one and only child so far
      {
        parent->firstChild = widget;
        parent->lastChild = widget;
      }
  
      widget->parent = parent;

      // properly show reparented widget, if its visibility flag is set
      if( ES_BIT_IS_SET(widget->flags, esguiWgtfVisible) )
      {
        ES_BIT_CLR(widget->flags, esguiWgtfVisible);
        esguiWgtShow(widget, NULL, TRUE);
      }
    }

    result = TRUE;
  }

  TRACE0("}")

  return result;
}

// internal focus setter
static inline void esguiWgtSetFocus(ESGUI_WIDGET* group, ESGUI_WIDGET* focusTo,
  ESGUI_WIDGET* sender)
{
  TRACE0("esguiWgtSetFocus{")
  if( group->focused != focusTo )
  {
    bool scrollV = FALSE;
    bool scrollH = FALSE;
    ESGUI_RECT cr;    
    
    // handle possible scrolling of virtual client space
    if( ES_BIT_IS_SET(group->flags, esguiWgtfScrollable) && 
        focusTo )
    {
      esguiWgtClientRectGet(group, &cr);
      esguiRectOffset(&group->voffs, &cr);
      if( !esguiPointInRect(&focusTo->bounds.org, &cr) ||
          !esguiPointInRect_(focusTo->bounds.edge.x-1, focusTo->bounds.edge.y-1, &cr) )
        esguiWgtInternalScrollTo(group, &focusTo->bounds.org, &cr, &scrollH, &scrollV );
    }
    
    // if either scrollV or scrollH is set, we need to repaint entire client area
    // so do not do local items locking and invalidation
    if( group->focused )
    {
      ESGUI_WIDGET* focusFrom = group->focused;
      
      if( !scrollV && !scrollH )
        esguiWgtLockIncrement(focusFrom);
      
      group->focused = 0;
      if( focusFrom->onResetFocus )
        focusFrom->onResetFocus(focusFrom, sender);
      
      if( !scrollV && !scrollH )
      {
        esguiWgtInvalidate(
          focusFrom, 
          NULL, 
          TRUE
        );
        
        esguiWgtLockDecrement(focusFrom);
      }
    }

    group->focused = focusTo;
    if( focusTo )
    {
      if( !scrollV && !scrollH )
        esguiWgtLockIncrement(focusTo);

      if( focusTo->onSetFocus )
        focusTo->onSetFocus(focusTo, sender);

      if( !scrollV && !scrollH )
      {
        esguiWgtInvalidate(
          focusTo, 
          NULL, 
          TRUE
        );
        
        esguiWgtLockDecrement(focusTo);
      }
    }
    
    // if scrolling occured, invalidate client area with scrollbars repaint here
    if( scrollV || scrollH )
      esguiWgtInternalRepaintOnScroll(group, &cr, scrollH, scrollV);
  }
  TRACE0("}")
}

// set input focus to the specified widget
bool esguiWgtFocusSet(ESGUI_WIDGET* wgt, ESGUI_WIDGET* sender)
{
  if( wgt &&
      wgt->parent && 
      esguiWgtIsFocuseableGroup(wgt->parent) &&
      esguiWgtIsFocuseable(wgt) )
  {
    esguiWgtSetFocus(wgt->parent, wgt, sender);
    return TRUE;
  }

  return FALSE;
}

// set input focus to the first focuseable widget in the top group
bool esguiWgtFocusSetFirst(ESGUI_WIDGET* group, ESGUI_WIDGET* sender)
{
  if( group && esguiWgtIsFocuseableGroup(group) )
  {
    ESGUI_WIDGET* widget = group->firstChild;
    while( widget )
    {
      if( esguiWgtIsFocuseable(widget) )
      {
        esguiWgtSetFocus(group, widget, sender);
        return TRUE;
      }
      else
        widget = widget->next;
    }
  }

  return FALSE;
}

// set input focus to the last focuseable widget in the top group
bool esguiWgtFocusSetLast(ESGUI_WIDGET* group, ESGUI_WIDGET* sender)
{
  if( group && esguiWgtIsGroup(group) )
  {
    ESGUI_WIDGET* widget = group->lastChild;
    while( widget )
    {
      if( esguiWgtIsFocuseable(widget) )
      {
        esguiWgtSetFocus(group, widget, sender);
        return TRUE;
      }
      else
        widget = widget->prev;
    }  
  }

  return FALSE;
}

// move focus to the next widget in the top group. if allowWrap is true
// the will move from the last focuseable widget to the first one
bool esguiWgtFocusSetNext(ESGUI_WIDGET* group, ESGUI_WIDGET* sender, bool allowWrap)
{
  if( group && esguiWgtIsGroup(group) && group->focused )
  {
    ESGUI_WIDGET* widget = group->focused->next;
    while( widget )
    {
      if( esguiWgtIsFocuseable(widget) )
      {
        esguiWgtSetFocus(group, widget, sender);
        return TRUE;
      }
      else
        widget = widget->next;
    }

    // if allowWrap is set, then try to jump to the first focuseable
    if( allowWrap )
      return esguiWgtFocusSetFirst(group, sender);
  }

  return FALSE;
}

// move focus to the prev widget in the top group. if allowWrap is true
// the will move from the first focuseable widget to the last one
bool esguiWgtFocusSetPrev(ESGUI_WIDGET* group, ESGUI_WIDGET* sender, bool allowWrap)
{
  if( group && esguiWgtIsGroup(group) && group->focused )
  {
    ESGUI_WIDGET* widget = group->focused->prev;
    while( widget )
    {
      if( esguiWgtIsFocuseable(widget) )
      {
        esguiWgtSetFocus(group, widget, sender);
        return TRUE;
      }
      else
        widget = widget->prev;
    }

    // if allowWrap is set, then try to jump to the last focuseable
    if( allowWrap )
      return esguiWgtFocusSetLast(group, sender);
  }

  return FALSE;
}

// "enter" the specified widget, if the latter supports it. whether the widget is
// "entered" is signalled by the "active" flag
void esguiWgtEnter(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender)
{
  TRACE0("esguiWgtEnter{")
  if( 
    widget && 
    ES_BIT_IS_SET(widget->flags, esguiWgtfVisible) && 
    esguiWgtIsEnabled(widget) 
  )
  {
    if( !ES_BIT_IS_SET(widget->flags, esguiWgtfActive) )
    {
      ES_BIT_SET(widget->flags, esguiWgtfActive);
      esguiWgtLockIncrement(widget);
      if( widget->onEnter )
        widget->onEnter(widget, sender);
      
      esguiWgtInvalidate(
        widget, 
        NULL, 
        TRUE
      );
      
      esguiWgtLockDecrement(widget);
    }
  }
  TRACE0("}")
}

// "exit" the specified widget, if the latter supports it. optional "commit"
// parameter may specify whether possible changes to the data-aware widget
// should be committed upon exit from the widget
void esguiWgtExit(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool commit)
{
  TRACE0("esguiWgtExit{")
  if( widget && ES_BIT_IS_SET(widget->flags, esguiWgtfVisible) )
  {
    if( ES_BIT_IS_SET(widget->flags, esguiWgtfActive) )
    {
      ES_BIT_CLR(widget->flags, esguiWgtfActive);
      esguiWgtLockIncrement(widget);
      if( widget->onExit )
        widget->onExit(widget, sender, commit);
      esguiWgtInvalidate(
        widget, 
        NULL, 
        TRUE
      );
      
      esguiWgtLockDecrement(widget);
    }
  }
  TRACE0("}")
}

// set widget checked flag to the value of "check" parameter
void esguiWgtCheck(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool check)
{
  TRACE0("esguiWgtCheck{")
  if( widget && 
      (ES_BIT_IS_SET(widget->flags, esguiWgtfChecked) != check) )
  {
    // toggle widget check
    if( check )
      ES_BIT_SET(widget->flags, esguiWgtfChecked);
    else
      ES_BIT_CLR(widget->flags, esguiWgtfChecked);
      
    esguiWgtLockIncrement(widget);
    if( widget->onCheck )
      widget->onCheck(widget, sender);
    esguiWgtInvalidate(
      widget, 
      NULL, 
      TRUE
    );
    
    esguiWgtLockDecrement(widget);
  }
  TRACE0("}")
}

// "click" on widget
void esguiWgtClick(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender)
{
  TRACE0("esguiWgtClick{")
  if( widget && esguiWgtIsEnabled(widget) )
  {
    // lock widget to prevent multiple re-draw in case of focus, or other state
    // flag changes
    esguiWgtLockIncrement(widget);
    // first, try to focus widget being clicked
    esguiWgtSetFocus(widget->parent, widget, sender);
    // class - dependent stuff
    switch(widget->cls)
    {
    case ESGUI_EDIT:
      esguiWgtEnter(widget, sender);
      break;
    case ESGUI_CHECK_BUTTON:
      // toggle check for check-buttons
      esguiWgtCheck(widget, sender, !ES_BIT_IS_SET(widget->flags, esguiWgtfChecked));
      break;
    case ESGUI_RADIO_BUTTON:
      // if there are other checked radio buttons in the group - uncheck them
      if( widget->parent )
      {
        ESGUI_WIDGET* otherRadio = widget->parent->firstChild;
        while( otherRadio )
        {
          if( ESGUI_RADIO_BUTTON == otherRadio->cls && 
              ES_BIT_IS_SET(otherRadio->flags, esguiWgtfChecked) &&
              otherRadio != widget )
          {
            esguiWgtCheck(otherRadio, widget, FALSE);
          }

          otherRadio = otherRadio->next;
        }
      }
      // issue one-time check for radio button
      esguiWgtCheck(widget, sender, TRUE);
      break;
    }

    // decrement lock
    esguiWgtLockDecrement(widget);

    // finally, call onClick handler
    if( widget->onClick )
      widget->onClick(widget, sender);
  }
  TRACE0("}")
}

// set widget visibility flag to the value of "show" parameter
void esguiWgtShow(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool show)
{
  TRACE0("esguiWgtShow(")
#if !defined(NDEBUG) && defined(USE_ESGUI_WIDGET_TRACE)
  TRACE_WGT(widget, "widget")
  TRACE_WGT(sender, "sender")
  TRACE1(" show='"TRACE_FMT_STR"'){", TRACE_BOOLSTR(show))
#endif

  if(  widget && 
    (ES_BIT_IS_SET(widget->flags, esguiWgtfVisible) != show) )
  {
    if( !show )
    {
      ESGUI_RECT localInvalid;
      ESGUI_WIDGET* parent = widget->parent;
      // reset invalidation flags
      ES_BIT_CLR(widget->flags, esguiWgtfInvalidFg);
      ES_BIT_CLR(widget->flags, esguiWgtfInvalidBg);          
      
      // increment parent's lock
      esguiWgtLockIncrement(parent);
      // remove focus from group's widget on hide
      if( esguiWgtIsGroup(widget) )
        esguiWgtSetFocus(widget, NULL, NULL);

      // exit widget (with cancel)
      esguiWgtExit(widget, sender, FALSE);
      // unfocus widget (select next, non-wrapping)
      if( parent && widget == parent->focused )
      {
        bool focusChangeResult;

        if( parent->firstChild == widget )
          focusChangeResult = esguiWgtFocusSetNext(parent, sender, FALSE);
        else
          focusChangeResult = esguiWgtFocusSetPrev(parent, sender, FALSE);

        // if we were not able to change focus, then set focus to NULL
        if( !focusChangeResult )
          esguiWgtSetFocus(parent, NULL, NULL);
      }
      // set widget show flag
      if( show )
        ES_BIT_SET(widget->flags, esguiWgtfVisible);
      else
        ES_BIT_CLR(widget->flags, esguiWgtfVisible);
      // invalidate the parent area which was occupied by the visible widget being hidden
      localInvalid = widget->bounds;
      esguiWgtInvalidate(parent, &localInvalid, TRUE);
      // decrement parent lock
      esguiWgtLockDecrement(parent);
    }

    if(widget->onShow)
      widget->onShow(widget, sender);

    if( show )
    {
      // increment paint lock
      esguiWgtLockIncrement(widget);
      // set widget show flag
      if( show )
        ES_BIT_SET(widget->flags, esguiWgtfVisible);
      else
        ES_BIT_CLR(widget->flags, esguiWgtfVisible);
      // perform invalidation
      esguiWgtInvalidate(widget, NULL, TRUE);
      // if showing group - try to focus the first widget in group
      if( esguiWgtIsGroup(widget) )
        esguiWgtFocusSetFirst(widget, sender);
      // if we're showing inside the parent which may manage focus
      // and it does not have focused widgets, try to make us focused
      if( widget->parent && esguiWgtIsFocuseableGroup(widget->parent) &&
          !widget->parent->focused )
        esguiWgtSetFocus(widget->parent, widget, NULL);

      // decrement paint lock
      esguiWgtLockDecrement(widget);
    }
  }
  TRACE0("}")
}

// move target widget on top of subject specified, and show it
void esguiWgtShowOnTop(ESGUI_WIDGET* target, ESGUI_WIDGET* subject)
{
  TRACE0("esguiWgtShowOnTop{")
  if( 
    target && 
    target != esguiWgtRootGet(target->root) &&
    subject &&
    subject != esguiWgtRootGet(subject->root) &&
    subject->parent 
  )
  {
    // reparent widget to the subject's parent, which automatically makes it z-top
    esguiWgtParentSet(
      target, 
      NULL
    );
    
    esguiWgtParentSet(
      target, 
      subject->parent
    );
    
    esguiWgtShow(
      target, 
      NULL, 
      TRUE
    );
  }
  TRACE0("}")
}

// set widget enabled flag to the value of "enable" parameter
void esguiWgtEnable(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool enable)
{
  TRACE0("esguiWgtEnable(")
#if !defined(NDEBUG) && defined(USE_ESGUI_WIDGET_TRACE)
  TRACE_WGT(widget, "widget")
  TRACE_WGT(sender, "sender")
  TRACE1(" enable='"TRACE_FMT_STR"'){", TRACE_BOOLSTR(enable))
#endif

  if( 
    widget && 
    esguiWgtIsEnabled(widget->parent) && 
    (ES_BIT_IS_SET(widget->flags, esguiWgtfEnabled) != enable) 
  )
  {
    // increment paint lock
    esguiWgtLockIncrement(widget);
    // unfocus widget (select next, non-wrapping)
    if( !enable && widget == widget->parent->focused )
      esguiWgtFocusSetNext(widget->parent, sender, FALSE);

    // toggle widget enable state
    if( enable )
      ES_BIT_SET(widget->flags, esguiWgtfEnabled);
    else
      ES_BIT_CLR(widget->flags, esguiWgtfEnabled);
  
    if( widget->onEnable )
      widget->onEnable(widget, sender);

    // decrement paint lock
    esguiWgtLockDecrement(widget);
  }
  TRACE0("}")
}

// prepare foreground drawing rect for widget, taking into account its parent's 
// virtual scrolling offset
static void esguiWgtPrepareFgDrawRect(ESGUI_WIDGET* widget, ESGUI_RECT* clientRect, ESGUI_RECT* drawRect)
{
  esguiWgtClientRectGet(widget, clientRect);
  esguiWgtClientToScreen(widget, clientRect);
  *drawRect = *clientRect;
  // take possible virtual offset into account
  if(widget->parent)
    esguiRectOffset_(-widget->parent->voffs.x, -widget->parent->voffs.y, drawRect);
}

// paint widget.
void esguiWgtPaint(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, const ESGUI_RECT* rect, esU32 flags)
{
  if( !widget )
    return;

  if( esguiWgtIsVisibleOnScreen(widget) )
  {
    bool isLocked = esguiWgtIsLocked(widget);
    if( 
      !isLocked && 
      (  
        ES_BIT_MASK_MATCH(widget->flags, esguiWgtfInvalidFg|esguiWgtfInvalidBg) || 
        ES_BIT_MASK_MATCH(flags, esguiWgtpfPaintBg|esguiWgtpfPaintForce) 
      ) 
    )
    {
      ESGUI_HDC hdc = esguiWgtCanvasGet(widget->root);
      bool canvasLocked = esguiCanvasLock(hdc);
      ESGUI_RECT oldClip, clientRect;
      ESGUI_RECT bgDrawRect, fgDrawRect;
      bool fgDrawRectPrepared = false;

#if !defined(NDEBUG) && defined(USE_ESGUI_WIDGET_TRACE)
      TRACE0("esguiWgtPaint(")
      TRACE_WGT(widget, "widget")
      TRACE_WGT(sender, "sender")
      TRACE_WGT_RECT(rect)
      TRACE2("  paintBg='"TRACE_FMT_STR"', force='"TRACE_FMT_STR"')", TRACE_BOOLSTR( ES_BIT_IS_SET(flags, esguiWgtpfPaintBg) ), TRACE_BOOLSTR( ES_BIT_IS_SET(flags, esguiWgtpfPaintForce) ))
      TRACE1("  canvasLocked="TRACE_FMT_STR, TRACE_BOOLSTR(canvasLocked))
      
      ESE_ASSERT(canvasLocked);
#endif
      
      if( esguiWgtBeforePaint(hdc, widget, &bgDrawRect, &oldClip, rect) )
      {
        if( ES_BIT_IS_SET(widget->flags, esguiWgtfInvalidBg) || 
            ES_BIT_IS_SET(flags, esguiWgtpfPaintBg) )
        {
          if( widget->onPaintBg )
            widget->onPaintBg(hdc, widget, sender, &bgDrawRect);
          
          if( ES_BIT_IS_SET(widget->flags, esguiWgtfScrollable) && 
              (  ES_BIT_IS_SET(widget->flags, esguiWgtfShowHscroll) || 
                ES_BIT_IS_SET(widget->flags, esguiWgtfShowVscroll) ) && 
              widget->onPaintScrollbar )
          {
            if( !fgDrawRectPrepared )
            {
              esguiWgtPrepareFgDrawRect(widget, &clientRect, &fgDrawRect);            
              fgDrawRectPrepared = TRUE;
            }
              
            if( ES_BIT_IS_SET(widget->flags, esguiWgtfShowHscroll) && widget->onPaintScrollbar )
              widget->onPaintScrollbar(hdc, widget, &fgDrawRect, FALSE);
              
            if( ES_BIT_IS_SET(widget->flags, esguiWgtfShowVscroll) && widget->onPaintScrollbar )
              widget->onPaintScrollbar(hdc, widget, &fgDrawRect, TRUE);
          }
        }

        if( !fgDrawRectPrepared )
          esguiWgtPrepareFgDrawRect(widget, &clientRect, &fgDrawRect);
        
        if(widget->onPaint)
          widget->onPaint(hdc, widget, sender, &fgDrawRect);
        
        // group paining
        if( esguiWgtIsGroup( widget ) )
        {
          ESGUI_WIDGET* child = widget->firstChild;
          // update clip rectangle with clientRect
          if( 
            child &&
            esguiRectIntersect(
              &clientRect, 
              esguiCanvasGetClipRect(hdc), 
              &clientRect
            ) 
          )
          {
            esguiCanvasSetClipRect(
              hdc, 
              &clientRect
            );

            while(child)
            {
              esguiWgtPaint(
                child, 
                widget, 
                NULL, 
                flags
              );
              
              child = child->next;
            }
          }
        }

        esguiWgtAfterPaint(hdc, &oldClip);
        
        // reset invalid flags in the end
        ES_BIT_CLR(widget->flags, esguiWgtfInvalidFg|esguiWgtfInvalidBg);
      }
      
      if( canvasLocked )
        esguiCanvasUnlock(hdc);
    }
    else if(isLocked && !widget->locked)
    {
      // widget is locked because some its upper-level parent is locked
      // find the most top-level parent which is locked, and set its invalidation
      // flags, if none of them are set. it will cause all hierarchy to be re-painted properly
      // in the invalidated boundaries, when top parent is unlocked
      ESGUI_WIDGET  *topLockedParent = NULL, *parent = widget->parent;
      while( parent )
      {
        if( parent->locked )
          topLockedParent = parent;
        parent = parent->parent;
      }

      if( topLockedParent )
        ES_BIT_SET(topLockedParent->flags, esguiWgtfInvalidFg|esguiWgtfInvalidBg);
    }
  }
  else // just reset invalid flags, if invisible
  {
    ES_BIT_CLR(widget->flags, esguiWgtfInvalidFg|esguiWgtfInvalidBg);
  }
}

// ask widget confirmation for closing. commit flag signals if possible changes
// to widget data should be committed before closing
bool esguiWgtCanClose(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool commit)
{
  if( widget && !esguiWgtIsClosed(widget) && widget->onCanClose )
    return widget->onCanClose(widget, sender, commit);
  else
    return TRUE;
}

// close widget, optionally comitting changes.
void esguiWgtClose(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool commit)
{
  TRACE0("esguiWgtClose{")
  if( widget && !esguiWgtIsClosed(widget) && 
      esguiWgtCanClose(widget, sender, commit) )
  {
    ESGUI_WIDGET* parent = widget->parent;
    esguiWgtLockIncrement(parent);

    esguiWgtExit(widget, sender, commit);
    // remove widget from parent first
    esguiWgtParentSet(widget, NULL);
    // call onClose handler
    if( widget->onClose )
      widget->onClose(widget, sender);
      
    esguiWgtDelete(widget);

    esguiWgtLockDecrement(parent);
  }
  TRACE0("}")
}

// internal event handling routine
static void esguiWgtHandleAppEvent(ESGUI_WIDGET* target, ESGUI_WIDGET* sender, EsguiAppEventT* evt)
{
  if( 
    !esguiWgtEventIsHandled(evt) &&
    !ES_BIT_IS_SET(
      target->flags, 
      esguiWgtfDeleted
    ) 
  )
  {
    // do not process modals, if these were processed already
    if( 
      ES_BIT_IS_SET(
        target->flags, 
        esguiWgtfModal
      ) && 
      target->root->modalsProcessed 
    )
      return;
    
    // do not process interactive events if disabled
    if( 
      esguiWgtEventIsInteractive(evt) && 
      !esguiWgtIsEnabled(target) 
    )
      return;

    // process focused first
    if( target->focused )
      esguiWgtHandleAppEvent(
        target->focused, 
        sender, 
        evt
      );

    // process self
    if( 
      !esguiWgtEventIsHandled(evt) &&
      !ES_BIT_IS_SET(
        target->flags, 
        esguiWgtfDeleted
      ) &&
      target->onEvent 
    )
      target->onEvent(
        target, 
        sender, 
        evt
      );

    // if can proceed, then process non-focused children
    if( 
      !esguiWgtEventIsHandled(evt) &&
      !ES_BIT_IS_SET(
        target->flags, 
        esguiWgtfDeleted
      ) 
    )
    {
      ESGUI_WIDGET* sender = NULL;
      ESGUI_WIDGET* wgt = NULL;
      ESGUI_RECT paintRect = c_zeroRect;
      esU32 paintFlags = 0;
      
      // handle paint evt explicitly
      if( esguiWgtPaintEvtExtract(evt, &sender, &wgt, &paintRect, &paintFlags) )
      {
        if( 
          (NULL == wgt && target == &target->root->cmn) || 
          target == wgt 
        )
        {
          esguiWgtPaint(
            target,
            NULL,
            esguiRectIsZero(&paintRect) ?
              NULL :
              &paintRect,
            paintFlags
          );
          
          esguiWgtEventMarkHandled(evt);
        }
      }

      target = target->firstChild;
      while(
        target &&
        !esguiWgtEventIsHandled(evt) &&
        !ES_BIT_IS_SET(
          target->flags, 
          esguiWgtfDeleted
        ) 
      )
      {
        if( !esguiWgtIsFocused(target) )
          esguiWgtHandleAppEvent(
            target, 
            sender, 
            evt
          );
          
        target = target->next;
      }
    }
  }
}

// handle application events starting from the root widget
void esguiWgtHandleEvent(ESGUI_WIDGET_ROOT* root, ESGUI_WIDGET* sender, EsguiAppEventT* evt)
{
  int idx;

  // process modals the very first
  root->modalsProcessed = FALSE;
  
  for(idx = root->modalsCnt-1; idx >= 0 && !esguiWgtEventIsHandled(evt); --idx)
    esguiWgtHandleAppEvent(
      root->modals[idx], 
      sender, 
      evt
    );
    
  root->modalsProcessed = TRUE;
  
  if( !esguiWgtEventIsHandled(evt) ) //< process the rest of events as usual
    esguiWgtHandleAppEvent(
      &root->cmn, 
      sender, 
      evt
    );
}

// internal invalidation helper
static void esguiWgtInvalidateInternal(const ESGUI_WIDGET* widget, const ESGUI_RECT* rect, bool paintBg)
{
  ESGUI_WIDGET* wgt = widget->firstChild;
  while( wgt )
  {
    // if widget is falling into invalidate rectangle, 
    // mark to invalidate it as well
    ESGUI_RECT wgtRect = wgt->bounds;
    esguiWgtClientToScreen(wgt->parent, &wgtRect);
    if( esguiRectIntersect(rect, &wgtRect, NULL) )
    {
      ES_BIT_SET(wgt->flags, esguiWgtfInvalidFg);
      if(paintBg)
        ES_BIT_SET(wgt->flags, esguiWgtfInvalidBg);
      else
        ES_BIT_CLR(wgt->flags, esguiWgtfInvalidBg);
      esguiWgtInvalidateInternal(wgt, rect, paintBg);
    }
    wgt = wgt->next;    
  }
}

// invalidate widget rect, rect is in widget client coordinates, or 0 if entire widget has to be invalidated
void esguiWgtInvalidate(ESGUI_WIDGET* widget, const ESGUI_RECT* rect, bool paintBg)
{
  if( 
    widget && 
    (widget == &widget->root->cmn || widget->parent) 
  )
  {
    ESGUI_RECT localInvalid;
    
    ES_BIT_SET(widget->flags, esguiWgtfInvalidFg);
    
    if(paintBg)
      ES_BIT_SET(widget->flags, esguiWgtfInvalidBg);
    else
      ES_BIT_CLR(widget->flags, esguiWgtfInvalidBg);
      
#if !defined(NDEBUG) && defined(USE_ESGUI_WIDGET_TRACE)
    TRACE0("esguiWgtInvalidate(")
    TRACE_WGT(widget, "widget")
    TRACE_WGT_RECT(rect)
    TRACE1(" paintBg='"TRACE_FMT_STR"')", TRACE_BOOLSTR(paintBg))
#endif
    // update invalidated rect
    if( rect )
      localInvalid = *rect;
    else
    {
      if( paintBg )
        esguiWgtClientRectGetStd(widget, &localInvalid);
      else
        esguiWgtClientRectGet(widget, &localInvalid);
    }
    
    esguiWgtClientToScreen(
      widget, 
      &localInvalid
    );
    
    // take virtual client offset into account
    if( widget->parent )
      esguiRectOffset_( 
        -widget->parent->voffs.x, 
        -widget->parent->voffs.y, 
        &localInvalid 
      );
      
    esguiWgtInvalidatedRectUpdate(
      widget->root, 
      &localInvalid
    );

#if !defined(NDEBUG) && defined(USE_ESGUI_WIDGET_TRACE)
    TRACE0("invalidated rect updated:")
    TRACE_WGT_RECT(&s_root.invalidated)
#endif

    // iterate through children and invalidate them as well
    esguiWgtLockIncrement(widget);
    esguiWgtInvalidateInternal(widget, &localInvalid, paintBg);
    esguiWgtLockDecrement(widget);
  }
}

#endif //< ESGUI_USE_WIDGETS
