#include <esfwxe/target.h>
#include <esgui/include/esgui.h>

#include "EsguiWgtApiIntf.h"
#include "EsguiWgtApiIntfImpl.h"
//----------------------------------------------------------------------------------------------

#include <esgui/widgets/esgui_widgets.c>
//----------------------------------------------------------------------------------------------

EsguiWgtApiIntf& EsguiWgtApiIntfImpl::instanceGet() ESE_NOTHROW
{
  static EsguiWgtApiIntfImpl s_api;
  return s_api;
}
//----------------------------------------------------------------------------------------------

int EsguiWgtApiIntfImpl::tagGet(ESGUI_WIDGET* wgt) const ESE_NOTHROW
{
  return esguiWgtTagGet(
    wgt
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_WIDGET* EsguiWgtApiIntfImpl::widgetFindByTag(ESGUI_WIDGET* parent, int tag) const ESE_NOTHROW
{
  return esguiWgtFindWidgetByTag(
    parent,
    tag
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_WIDGET* EsguiWgtApiIntfImpl::rootGet(ESGUI_WIDGET_ROOT* root) const ESE_NOTHROW
{
  return esguiWgtRootGet(
    root
  );
}
//----------------------------------------------------------------------------------------------

const ESGUI_RECT* EsguiWgtApiIntfImpl::invalidatedRectGet(const ESGUI_WIDGET_ROOT* root) const ESE_NOTHROW
{
  return esguiWgtInvalidatedRectGet(
    root
  );
}
//----------------------------------------------------------------------------------------------

const ESGUI_RECT* EsguiWgtApiIntfImpl::boundsGet(const ESGUI_WIDGET* wgt) const ESE_NOTHROW
{
  return esguiWgtBoundsGet(wgt);
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::invalidatedRectUpdate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* rect) const ESE_NOTHROW
{
  esguiWgtInvalidatedRectUpdate(
    root,
    rect
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::invalidatedRectReset(ESGUI_WIDGET_ROOT* root) const ESE_NOTHROW
{
  esguiWgtInvalidatedRectReset(
    root
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_HDC EsguiWgtApiIntfImpl::canvasGet(const ESGUI_WIDGET_ROOT* root) const ESE_NOTHROW
{
  return esguiWgtCanvasGet(root);
}
//----------------------------------------------------------------------------------------------

ESGUI_HDC EsguiWgtApiIntfImpl::canvasGet(const ESGUI_WIDGET* wgt) const ESE_NOTHROW
{
  return esguiWgtCanvasGet_(wgt);
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::clientToOther(const ESGUI_WIDGET* This, const ESGUI_WIDGET* other, ESGUI_RECT* rect) const ESE_NOTHROW
{
  esguiWgtClientToOther(
    This,
    other,
    rect
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::clientToScreen(const ESGUI_WIDGET* This, ESGUI_RECT* rect) const ESE_NOTHROW
{
  esguiWgtClientToScreen(
    This,
    rect
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::screenToClient(const ESGUI_WIDGET* This, ESGUI_RECT* rect) const ESE_NOTHROW
{
  esguiWgtScreenToClient(
    This,
    rect
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::isFocused(const ESGUI_WIDGET* widget) const ESE_NOTHROW
{
  return esguiWgtIsFocused(widget);
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::isVisible(const ESGUI_WIDGET* widget) const ESE_NOTHROW
{
  return esguiWgtIsVisible(widget);
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::isVisibleOnScreen(const ESGUI_WIDGET* widget) const ESE_NOTHROW
{
  return esguiWgtIsVisibleOnScreen(widget);
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::isEnabled(const ESGUI_WIDGET* widget) const ESE_NOTHROW
{
  return esguiWgtIsEnabled(widget);
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::isChecked(const ESGUI_WIDGET* widget) const ESE_NOTHROW
{
  return esguiWgtIsChecked(widget);
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::lockIncrement(ESGUI_WIDGET* widget) const ESE_NOTHROW
{
  esguiWgtLockIncrement(widget);
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::lockDecrement(ESGUI_WIDGET* widget) const ESE_NOTHROW
{
  esguiWgtLockDecrement(widget);
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::clientRectGet(const ESGUI_WIDGET* widget, ESGUI_RECT* rect) const ESE_NOTHROW
{
  esguiWgtClientRectGet(
    widget,
    rect
  );
}
//----------------------------------------------------------------------------------------------

int EsguiWgtApiIntfImpl::clientWidthGet(const ESGUI_WIDGET* widget) const ESE_NOTHROW
{
  return esguiWgtClientWidthGet(widget);
}
//----------------------------------------------------------------------------------------------

int EsguiWgtApiIntfImpl::clientHeightGet(const ESGUI_WIDGET* widget) const ESE_NOTHROW
{
  return esguiWgtClientHeightGet(widget);
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::virtualClientRectGet(const ESGUI_WIDGET* widget, ESGUI_RECT* rect) const ESE_NOTHROW
{
  esguiWgtVirtualClientRectGet(
    widget,
    rect
  );
}
//----------------------------------------------------------------------------------------------

const ESGUI_POINT* EsguiWgtApiIntfImpl::viewOffsetGet(const ESGUI_WIDGET* widget) const ESE_NOTHROW
{
  return esguiWgtViewOffsetGet(widget);
}
//----------------------------------------------------------------------------------------------

int EsguiWgtApiIntfImpl::virtualClientWidthGet(const ESGUI_WIDGET* widget) const ESE_NOTHROW
{
  return esguiWgtVirtualClientWidthGet(widget);
}
//----------------------------------------------------------------------------------------------

int EsguiWgtApiIntfImpl::virtualClientHeightGet(const ESGUI_WIDGET* widget) const ESE_NOTHROW
{
  return esguiWgtVirtualClientHeightGet(widget);
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::scrollTo(ESGUI_WIDGET* widget, const ESGUI_POINT* pnt) const ESE_NOTHROW
{
  esguiWgtScrollTo(
    widget,
    pnt
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_WIDGET* EsguiWgtApiIntfImpl::create(ESGUI_WIDGET_ROOT* root, ESGUI_WIDGET_CLASS cls, const ESGUI_RECT* bounds, esU32 flags) const ESE_NOTHROW
{
  return esguiWgtCreate(
    root,
    cls,
    bounds,
    flags
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::remove(ESGUI_WIDGET* widget) const ESE_NOTHROW
{
  esguiWgtDelete(widget);
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::init(ESGUI_WIDGET_ROOT* root, ESGUI_WIDGET* widget, ESGUI_WIDGET_CLASS cls, const ESGUI_RECT* bounds, esU32 flags) const ESE_NOTHROW
{
  esguiWgtInit(
    root,
    widget,
    cls,
    bounds,
    flags
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::modalAdd(ESGUI_WIDGET* widget) const ESE_NOTHROW
{
  return esguiWgtModalAdd(widget);
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::parentSet(ESGUI_WIDGET* widget, ESGUI_WIDGET* parent) const ESE_NOTHROW
{
  return esguiWgtParentSet(
    widget,
    parent
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::focusSet(ESGUI_WIDGET* wgt, ESGUI_WIDGET* sender) const ESE_NOTHROW
{
  return esguiWgtFocusSet(
    wgt,
    sender
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::focusSetFirst(ESGUI_WIDGET* group, ESGUI_WIDGET* sender) const ESE_NOTHROW
{
  return esguiWgtFocusSetFirst(
    group,
    sender
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::focusSetLast(ESGUI_WIDGET* group, ESGUI_WIDGET* sender) const ESE_NOTHROW
{
  return esguiWgtFocusSetLast(
    group,
    sender
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::focusSetNext(ESGUI_WIDGET* group, ESGUI_WIDGET* sender, bool allowWrap) const ESE_NOTHROW
{
  return esguiWgtFocusSetNext(
    group,
    sender,
    allowWrap
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::focusSetPrev(ESGUI_WIDGET* group, ESGUI_WIDGET* sender, bool allowWrap) const ESE_NOTHROW
{
  return esguiWgtFocusSetPrev(
    group,
    sender,
    allowWrap
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::enter(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender) const ESE_NOTHROW
{
  esguiWgtEnter(
    widget,
    sender
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::exit(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool commit) const ESE_NOTHROW
{
  esguiWgtExit(
    widget,
    sender,
    commit
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::show(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool show) const ESE_NOTHROW
{
  esguiWgtShow(
    widget,
    sender,
    show
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::showOnTop(ESGUI_WIDGET* target, ESGUI_WIDGET* subject) const ESE_NOTHROW
{
  esguiWgtShowOnTop(
    target,
    subject
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::canClose(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool commit) const ESE_NOTHROW
{
  return esguiWgtCanClose(
    widget,
    sender,
    commit
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::close(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool commit) const ESE_NOTHROW
{
  esguiWgtClose(
    widget,
    sender,
    commit
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::enable(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool enable) const ESE_NOTHROW
{
  esguiWgtEnable(
    widget,
    sender,
    enable
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::check(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool check) const ESE_NOTHROW
{
  esguiWgtCheck(
    widget,
    sender,
    check
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::click(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender) const ESE_NOTHROW
{
  esguiWgtClick(
    widget,
    sender
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::paint(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, const ESGUI_RECT* rect, esU32 flags) const ESE_NOTHROW
{
  esguiWgtPaint(
    widget,
    sender,
    rect,
    flags
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::invalidate(ESGUI_WIDGET* widget, const ESGUI_RECT* rect, bool paintBg) const ESE_NOTHROW
{
  esguiWgtInvalidate(
    widget,
    rect,
    paintBg
  );
}
//----------------------------------------------------------------------------------------------

int EsguiWgtApiIntfImpl::metricGet(int metric) const ESE_NOTHROW
{
  return esguiWgtMetricGet(metric);
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::eventMarkHandled(EsguiAppEventT* evt) const ESE_NOTHROW
{
  esguiWgtEventMarkHandled(evt);
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::eventIsHandled(EsguiAppEventT* evt) const ESE_NOTHROW
{
  return esguiWgtEventIsHandled(evt);
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::eventIsSecondTick(EsguiAppEventT* evt) const ESE_NOTHROW
{
  return esguiWgtEventIsSecondTick(evt);
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::eventIsInteractive(EsguiAppEventT* evt) const ESE_NOTHROW
{
  return esguiWgtEventIsInteractive(evt);
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::paintPost(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, const ESGUI_RECT* rect, esU32 flags) const ESE_NOTHROW
{
  esguiWgtPaintPost(
    widget,
    sender,
    rect,
    flags
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::paintEvtExtract(EsguiAppEventT* evt, ESGUI_WIDGET** pwgtSender, ESGUI_WIDGET** pwgt, ESGUI_RECT* ppaintRect, esU32* paintFlags) const ESE_NOTHROW
{
  return esguiWgtPaintEvtExtract(
    evt,
    pwgtSender,
    pwgt,
    ppaintRect,
    paintFlags
  );
}
//----------------------------------------------------------------------------------------------

#if defined(ESGUI_WIDGET_USE_STD) && 1 == ESGUI_WIDGET_USE_STD
#include <esgui/widgets/std/esgui_widget_std.c>

void EsguiWgtApiIntfImpl::stdPaintTitle(ESGUI_HDC hdc, const ESGUI_RECT* rect, const ESGUI_BITMAP* bmp, ESE_CSTR title) const ESE_NOTHROW
{
  esguiWgtStdPaintTitle(
    hdc,
    rect,
    bmp,
    title
  );
}
//----------------------------------------------------------------------------------------------

EsguiWgtApiIntf::pfnstdPaintTitleT EsguiWgtApiIntfImpl::pfnstdPaintTitle() const ESE_NOTHROW
{
  return &esguiWgtStdPaintTitle;
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::stdPaintVscrollBar(ESGUI_HDC hdc, const ESGUI_RECT* client, int min, int max, int pos) const ESE_NOTHROW
{
  esguiWgtStdPaintVscrollBar(
    hdc,
    client,
    min,
    max,
    pos
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::stdPaintHscrollBar(ESGUI_HDC hdc, const ESGUI_RECT* client, int min, int max, int pos) const ESE_NOTHROW
{
  esguiWgtStdPaintHscrollBar(
    hdc,
    client,
    min,
    max,
    pos
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::stdPaintScrollbar(ESGUI_HDC hdc, const ESGUI_WIDGET* This, const ESGUI_RECT* draw, bool vertical) const ESE_NOTHROW
{
  esguiWgtStdPaintScrollbar(
    hdc,
    This,
    draw,
    vertical
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_PAINT_SCROLLBAR EsguiWgtApiIntfImpl::pfnstdPaintScrollbar() const ESE_NOTHROW
{
  return esguiWgtStdPaintScrollbar;
}
//----------------------------------------------------------------------------------------------

ESGUI_PAINT_EVENT EsguiWgtApiIntfImpl::stdPaintBgGet() const ESE_NOTHROW
{
  return &esguiWgtStdPaintBg;
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::stdPaintBg(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect) const ESE_NOTHROW
{
  esguiWgtStdPaintBg(
    hdc,
    This,
    sender,
    rect
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::stdPaintStringWithSmartErase(ESGUI_HDC hdc, ESE_CSTR str, const ESGUI_POINT* pos, int* prevStrLen) const ESE_NOTHROW
{
  esguiWgtStdPaintStringWithSmartErase(
    hdc,
    str,
    pos,
    prevStrLen
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::stdPaintCenteredStringWithSmartErase(ESGUI_HDC hdc, ESE_CSTR str, const ESGUI_POINT* org, int areaWidth, int* prevStrLen) const ESE_NOTHROW
{
  esguiWgtStdPaintCenteredStringWithSmartErase(
    hdc,
    str,
    org,
    areaWidth,
    prevStrLen
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::stdPaintLabeledVal(ESGUI_HDC hdc, const ESGUI_POINT* pos, int offs, ESE_CSTR lbl, ESE_CSTR val) const ESE_NOTHROW
{
  esguiWgtStdPaintLabeledVal(
    hdc,
    pos,
    offs,
    lbl,
    val
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::stdPaintLabeledValWithSmartErase(
  ESGUI_HDC hdc, ESGUI_POINT* pos, int lblRulerOffs, int unitRulerOffs,
  int* prevLenContainer, int* prevUnitLenContainer, ESGUI_COLOR fgnd, ESGUI_COLOR bgnd, ESE_CSTR lbl, const ESGUI_FONT* lblFont,
  ESE_CSTR valStr, const ESGUI_FONT* valFont, ESE_CSTR unit, const ESGUI_FONT* unitFont
) const ESE_NOTHROW
{
  esguiWgtStdPaintLabeledValWithSmartErase(
    hdc,
    pos,
    lblRulerOffs,
    unitRulerOffs,
    prevLenContainer,
    prevUnitLenContainer,
    fgnd,
    bgnd,
    lbl,
    lblFont,
    valStr,
    valFont,
    unit,
    unitFont
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::stdButtonPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect) const ESE_NOTHROW
{
  esguiWgtStdButtonPaint(
    hdc,
    This,
    sender,
    rect
  );
}
//----------------------------------------------------------------------------------------------

const ESGUI_FONT* EsguiWgtApiIntfImpl::stdFontGet(const ESGUI_HDC hdc, int resourceId) const ESE_NOTHROW
{
  return esguiWgtStdFontGet(
    hdc,
    resourceId
  );
}
//----------------------------------------------------------------------------------------------

const ESGUI_BITMAP* EsguiWgtApiIntfImpl::stdBitmapGet(const ESGUI_HDC hdc, int resourceId) const ESE_NOTHROW
{
  return esguiWgtStdBitmapGet(
    hdc,
    resourceId
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR EsguiWgtApiIntfImpl::stdColorGet(const ESGUI_HDC hdc, int resourceId) const ESE_NOTHROW
{
  return esguiWgtStdColorGet(
    hdc,
    resourceId
  );
}
//----------------------------------------------------------------------------------------------

ESE_CSTR EsguiWgtApiIntfImpl::stdStringGet(int resourceId) const ESE_NOTHROW
{
  return esguiWgtStdStringGet(resourceId);
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::eventIsCtlDec(EsguiAppEventT* evt) const ESE_NOTHROW
{
  return esguiWgtEventIsCtlDec(evt);
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::eventIsCtlDecBig(EsguiAppEventT* evt) const ESE_NOTHROW
{
  return esguiWgtEventIsCtlDecBig(evt);
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::eventIsCtlInc(EsguiAppEventT* evt) const ESE_NOTHROW
{
  return esguiWgtEventIsCtlInc(evt);
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::eventIsCtlIncBig(EsguiAppEventT* evt) const ESE_NOTHROW
{
  return esguiWgtEventIsCtlIncBig(evt);
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::eventIsCtlCommit(EsguiAppEventT* evt) const ESE_NOTHROW
{
  return esguiWgtEventIsCtlCommit(evt);
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::eventIsCtlEnter(EsguiAppEventT* evt) const ESE_NOTHROW
{
  return esguiWgtEventIsCtlEnter(evt);
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::eventIsCtlExit(EsguiAppEventT* evt) const ESE_NOTHROW
{
  return esguiWgtEventIsCtlExit(evt);
}
//----------------------------------------------------------------------------------------------
#endif //< ESGUI_WIDGET_USE_STD
  
#if defined(ESGUI_WIDGET_USE_CHECKBOX) && 1 == ESGUI_WIDGET_USE_CHECKBOX
#include <esgui/widgets/std/esgui_widget_checkbox.c>

void EsguiWgtApiIntfImpl::checkBoxOnPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect) const ESE_NOTHROW
{
  esguiWgtCheckBoxOnPaint(
    hdc,
    This,
    sender,
    rect
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_WIDGET* EsguiWgtApiIntfImpl::checkBoxCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, ESE_CSTR lbl, bool check) const ESE_NOTHROW
{
  return esguiWgtCheckBoxCreate(
    root,
    bounds,
    lbl,
    check
  );
}
//----------------------------------------------------------------------------------------------
#endif

#if (defined(ESGUI_WIDGET_USE_STD) && 1 == ESGUI_WIDGET_USE_STD) && \
    ( \
      (defined(ESGUI_WIDGET_USE_SPINCTL) && 1 == ESGUI_WIDGET_USE_SPINCTL) || \
      (defined(ESGUI_WIDGET_USE_DTEDIT) && 1 == ESGUI_WIDGET_USE_DTEDIT) \
    )
    
void EsguiWgtApiIntfImpl::stdPaintEditorChar(ESGUI_HDC hdc, ESGUI_CharCallbackReason reason, ESE_CSTR pos, const ESGUI_RECT* rect) const ESE_NOTHROW
{
  esguiWgtStdPaintEditorChar(
    hdc,
    reason,
    pos,
    rect
  );
}

#endif
//----------------------------------------------------------------------------------------------

#if defined(ESGUI_WIDGET_USE_DTEDIT) && 1 == ESGUI_WIDGET_USE_DTEDIT
#include <esgui/widgets/std/esgui_widget_dtedit.c>

ESGUI_WIDGET* EsguiWgtApiIntfImpl::timeEditorCreate(ESGUI_WIDGET_ROOT* root, ESE_CSTR label, const esDT* dt, const ESGUI_RECT* bounds) const ESE_NOTHROW
{
  return esguiWgtTimeEditorCreate(
    root,
    lable,
    dt,
    bounds
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_WIDGET* EsguiWgtApiIntfImpl::dateEditorCreate(ESGUI_WIDGET_ROOT* root, ESE_CSTR label, const esDT* dt, const ESGUI_RECT* bounds) const ESE_NOTHROW
{
  return esguiWgtDateEditorCreate(
    root,
    label,
    dt,
    bounds
  );
}
//----------------------------------------------------------------------------------------------

esDT EsguiWgtApiIntfImpl::currentDateTimeGet() const ESE_NOTHROW
{
  return esguiWgtCurrentDateTimeGet();
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::currentDateTimeSet(const esDT* dt) const ESE_NOTHROW
{
  esguiWgtCurrentDateTimeSet(dt);
}
//----------------------------------------------------------------------------------------------

ESE_CSTR EsguiWgtApiIntfImpl::dteditTimeFmtStrGet() const ESE_NOTHROW
{
  return esguiWgtDteditTimeFmtStrGet();
}
//----------------------------------------------------------------------------------------------

ESE_CSTR EsguiWgtApiIntfImpl::dteditDateFmtStrGet() const ESE_NOTHROW
{
  return esguiWgtDteditDateFmtstrGet();
}
//----------------------------------------------------------------------------------------------
#endif //< ESGUI_WIDGET_USE_DTEDIT

#if defined(ESGUI_WIDGET_USE_LISTVIEW) && 1 == ESGUI_WIDGET_USE_LISTVIEW
#include <esgui/widgets/std/esgui_widget_listview.c>

ESGUI_WgtListViewData* EsguiWgtApiIntfImpl::listViewDataCreate(size_t listViewDataSize) const ESE_NOTHROW
{
  return esguiWgtListViewDataCreate(listViewDataSize);
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::listViewDataFree(ESGUI_WgtListViewData* data) const ESE_NOTHROW
{
  esguiWgtListViewDataFree(
    data
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::listViewOnEvent(ESGUI_WIDGET* This, ESGUI_WIDGET* sender, EsguiAppEventT* evt) const ESE_NOTHROW
{
  esguiWgtListViewOnEvent(
    This,
    sender,
    evt
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_WIDGET* EsguiWgtApiIntfImpl::listViewCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, ESGUI_WgtListViewData* data) const ESE_NOTHROW
{
  return esguiWgtListViewCreate(
    root,
    bounds,
    data
  );
}
//----------------------------------------------------------------------------------------------
#endif //< ESGUI_WIDGET_USE_LISTVIEW

#if defined(ESGUI_WIDGET_USE_MSGBOX) && 1 == ESGUI_WIDGET_USE_MSGBOX
#include <esgui/widgets/std/esgui_widget_msgbox.c>

void EsguiWgtApiIntfImpl::msgBoxDataInit(ESGUI_WgtMsgBoxData* data, ESE_CSTR title, ESE_CSTR text, esU32 type) const ESE_NOTHROW
{
  esguiWgtMsgBoxDataInit(
    data,
    title,
    text,
    type
  );
}
//----------------------------------------------------------------------------------------------

esU32 EsguiWgtApiIntfImpl::msgBoxStdEventHandler(ESGUI_WIDGET* This, EsguiAppEventT* evt) const ESE_NOTHROW
{
  return esguiWgtMsgBoxStdEventHandler(
    This,
    evt
  );
}
//----------------------------------------------------------------------------------------------

esU32 EsguiWgtApiIntfImpl::msgBox(ESGUI_WIDGET_ROOT* root, ESE_CSTR title, ESE_CSTR text, const ESGUI_RECT* rect, esU32 type, int tag, ESGUI_WIDGET** out) const ESE_NOTHROW
{
  return esguiWgtMsgBox(
    root,
    title,
    text,
    rect,
    type,
    tag,
    out
  );
}
//----------------------------------------------------------------------------------------------

esU32 EsguiWgtApiIntfImpl::msgBoxIndirect(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* rect, int tag, ESGUI_WgtMsgBoxData* data, bool ownsData, ESGUI_WIDGET** out) const ESE_NOTHROW
{
  return esguiWgtMsgBoxIndirect(
    root,
    rect,
    tag,
    data,
    ownsData,
    out
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::msgBoxCancel(ESGUI_WIDGET* msgbox) const ESE_NOTHROW
{
  esguiWgtMsgBoxCancel(
    msgbox
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::msgBoxAppEventsProcess() const ESE_NOTHROW
{
  esguiWgtMsgBoxAppEventsProcess();
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::msgBoxStdCanCreate() const ESE_NOTHROW
{
  return esguiWgtMsgBoxStdCanCreate();
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::eventIsMsgBoxOk(ESGUI_WIDGET* This, EsguiAppEventT* evt) const ESE_NOTHROW
{
  return esguiWgtEventIsMsgBoxOk(
    This,
    evt
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::eventIsMsgBoxCancel(ESGUI_WIDGET* This, EsguiAppEventT* evt) const ESE_NOTHROW
{
  return esguiWgtEventIsMsgBoxCancel(
    This,
    evt
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::msgBoxBeforeShow() const ESE_NOTHROW
{
  esguiWgtMsgBoxBeforeShow();
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::msgBoxAfterShow() const ESE_NOTHROW
{
  esguiWgtMsgBoxAfterShow();
}
//----------------------------------------------------------------------------------------------
#endif //< ESGUI_WIDGET_USE_MSGBOX

#if defined(ESGUI_WIDGET_USE_PROGRESS) && 1 == ESGUI_WIDGET_USE_PROGRESS
#include <esgui/widgets/std/esgui_widget_progress.c>

void EsguiWgtApiIntfImpl::progressOnEvent(ESGUI_WIDGET* This, ESGUI_WIDGET* sender, EsguiAppEventT* evt) const ESE_NOTHROW
{
  esguiWgtProgressOnEvent(
    This,
    sender,
    evt
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::progressOnPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect) const ESE_NOTHROW
{
  esguiWgtProgressOnPaint(
    hdc,
    This,
    sender,
    rect
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::progressSetOnTextHandler(ESGUI_WIDGET* This, ESGUI_WgtOnDisplayProgressText onDisplayProgressText) const ESE_NOTHROW
{
  esguiWgtProgressSetOnTextHandler(
    This,
    onDisplayProgressText
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::progressSetRange(ESGUI_WIDGET* This, int min, int max) const ESE_NOTHROW
{
  esguiWgtProgressSetRange(
    This,
    min,
    max
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::progressSetPos(ESGUI_WIDGET* This, int pos) const ESE_NOTHROW
{
  esguiWgtProgressSetPos(
    This,
    pos
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_WIDGET* EsguiWgtApiIntfImpl::progressCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, const ESGUI_FONT* font, ESGUI_WgtOnDisplayProgressText onDisplayProgressText, int rmin, int rmax, int pos) const ESE_NOTHROW
{
  return esguiWgtProgressCreateCustom(
    root,
    bounds,
    font,
    onDisplayProgressText,
    rmin,
    rmax,
    pos
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_WIDGET* EsguiWgtApiIntfImpl::progressCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, const ESGUI_FONT* font, ESGUI_WgtOnDisplayProgressText onDisplayProgressText) const ESE_NOTHROW
{
  return esguiWgtProgressCreate(
    root,
    bounds,
    font,
    onDisplayProgressText
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::eventIsProgressSet(EsguiAppEventT* evt, int* pos) const ESE_NOTHROW
{
  return esguiWgtEventIsProgressSet(
    evt,
    pos
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiWgtApiIntfImpl::eventIsProgressRangeSet(EsguiAppEventT* evt, int* min, int* max) const ESE_NOTHROW
{
  return esguiWgtEventIsProgressRangeSet(
    evt,
    min,
    max
  );
}
//----------------------------------------------------------------------------------------------
#endif //< ESGUI_WIDGET_USE_PROGRESS

#if defined(ESGUI_WIDGET_USE_SPINCTL) && 1 == ESGUI_WIDGET_USE_SPINCTL
#include <esgui/widgets/std/esgui_widget_spinctl.c>

void EsguiWgtApiIntfImpl::spinctlOnEvent(ESGUI_WIDGET* This, ESGUI_WIDGET* sender, EsguiAppEventT* evt) const ESE_NOTHROW
{
  esguiWgtSpinctlOnEvent(
    This,
    sender,
    evt
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::spinctlOnPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect) const ESE_NOTHROW
{
  esguiWgtSpinctlOnPaint(
    hdc,
    This,
    sender,
    rect
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::spinctlSetRangeInt(ESGUI_WIDGET* This, int min, int max) const ESE_NOTHROW
{
  esguiWgtSpinctlSetRangeInt(
    This,
    min,
    max
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::spinctlSetRangeFloat(ESGUI_WIDGET* This, float min, float max) const ESE_NOTHROW
{
  esguiWgtSpinctlSetRangeFloat(
    This,
    min,
    max
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::spinctlGetRange(ESGUI_WIDGET* This, int& min, int& max) const ESE_NOTHROW
{
  esguiWgtSpinctlGetRangeInt(
    This,
    &min,
    &max
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::spinctlGetRange(ESGUI_WIDGET* This, float& min, float& max) const ESE_NOTHROW
{
  esguiWgtSpinctlGetRangeFloat(
    This,
    &min,
    &max
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::spinctlSetValueInt(ESGUI_WIDGET* This, int val) const ESE_NOTHROW
{
  esguiWgtSpinctlSetValueInt(
    This,
    val
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::spinctlSetValueFloat(ESGUI_WIDGET* This, float val) const ESE_NOTHROW
{
  esguiWgtSpinctlSetValueFloat(
    This,
    val
  );
}
//----------------------------------------------------------------------------------------------

int EsguiWgtApiIntfImpl::spinctlGetValue(ESGUI_WIDGET* This) const ESE_NOTHROW
{
  return esguiWgtSpinctlGetValueInt(
    This
  );
}
//----------------------------------------------------------------------------------------------

float EsguiWgtApiIntfImpl::spinctlGetValue(ESGUI_WIDGET* This) const ESE_NOTHROW
{
  return esguiWgtSpinctlGetValueFloat(
    This
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::spinctlSetIncInt(ESGUI_WIDGET* This, int inc) const ESE_NOTHROW
{
  esguiWgtSpinctlSetIncInt(
    This,
    inc
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::spinctlSetIncFloat(ESGUI_WIDGET* This, float inc) const ESE_NOTHROW
{
  esguiWgtSpinctlSetIncFloat(
    This,
    inc
  );
}
//----------------------------------------------------------------------------------------------

int EsguiWgtApiIntfImpl::spinctlGetInc(ESGUI_WIDGET* This) const ESE_NOTHROW
{
  return esguiWgtSpinctlGetIncInt(
    This
  );
}
//----------------------------------------------------------------------------------------------

float EsguiWgtApiIntfImpl::spinctlGetInc(ESGUI_WIDGET* This) const ESE_NOTHROW
{
  return esguiWgtSpinctlGetIncFloat(
    This
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::spinctlSetBigIncInt(ESGUI_WIDGET* This, int bigInc) const ESE_NOTHROW
{
  esguiWgtSpinctlSetBigIncInt(
    This,
    bigInc
  );
}
//----------------------------------------------------------------------------------------------

void EsguiWgtApiIntfImpl::spinctlSetBigIncFloat(ESGUI_WIDGET* This, float bigInc) const ESE_NOTHROW
{
  esguiWgtSpinctlSetBigIncFloat(
    This,
    bigInc
  );
}
//----------------------------------------------------------------------------------------------

int EsguiWgtApiIntfImpl::spinctlGetBigInc(ESGUI_WIDGET* This) const ESE_NOTHROW
{
  return esguiWgtSpinctlGetBigIncInt(
    This
  );
}
//----------------------------------------------------------------------------------------------

float EsguiWgtApiIntfImpl::spinctlGetBigInc(ESGUI_WIDGET* This) const ESE_NOTHROW
{
  return esguiWgtSpinctlGetBigIncFloat(
    This
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_WIDGET* EsguiWgtApiIntfImpl::spinctlCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, ESE_CSTR lbl, int min, int max) const ESE_NOTHROW
{
  return esguiWgtSpinctlCreateInt(
    root,
    bounds,
    lbl,
    min,
    max
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_WIDGET* EsguiWgtApiIntfImpl::spinctlCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, ESE_CSTR lbl, float min, float max, int decimals) const ESE_NOTHROW
{
  return esguiWgtSpinctlCreateFloat(
    root,
    bounds,
    lbl,
    min,
    max,
    decimals
  );
}
//----------------------------------------------------------------------------------------------
#endif //< ESGUI_WIDGET_USE_SPINCTL
