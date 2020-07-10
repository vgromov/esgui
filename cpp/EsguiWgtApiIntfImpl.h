#ifndef _es_gui_wgt_api_h_
#define _es_gui_wgt_api_h_

/// ESGUI Widget API interface implementation
///

class EsguiWgtApiIntfImpl : public EsguiWgtApiIntf
{
private:
  EsguiWgtApiIntfImpl() ESE_NOTHROW ESE_KEEP {}

public:
  static EsguiWgtApiIntf& instanceGet() ESE_NOTHROW ESE_KEEP;

  virtual int tagGet(ESGUI_WIDGET* wgt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_WIDGET* widgetFindByTag(ESGUI_WIDGET* parent, int tag) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_WIDGET* rootGet(ESGUI_WIDGET_ROOT* root) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual const ESGUI_RECT* invalidatedRectGet(const ESGUI_WIDGET_ROOT* root) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual const ESGUI_RECT* boundsGet(const ESGUI_WIDGET* wgt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void invalidatedRectUpdate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* rect) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void invalidatedRectReset(ESGUI_WIDGET_ROOT* root) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_HDC canvasGet(const ESGUI_WIDGET_ROOT* root) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_HDC canvasGet(const ESGUI_WIDGET* wgt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void clientToOther(const ESGUI_WIDGET* This, const ESGUI_WIDGET* other, ESGUI_RECT* rect) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void clientToScreen(const ESGUI_WIDGET* This, ESGUI_RECT* rect) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void screenToClient(const ESGUI_WIDGET* This, ESGUI_RECT* rect) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool isFocused(const ESGUI_WIDGET* widget) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool isVisible(const ESGUI_WIDGET* widget) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool isVisibleOnScreen(const ESGUI_WIDGET* widget) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool isEnabled(const ESGUI_WIDGET* widget) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool isChecked(const ESGUI_WIDGET* widget) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void lockIncrement(ESGUI_WIDGET* widget) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void lockDecrement(ESGUI_WIDGET* widget) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void clientRectGet(const ESGUI_WIDGET* widget, ESGUI_RECT* rect) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual int clientWidthGet(const ESGUI_WIDGET* widget) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual int clientHeightGet(const ESGUI_WIDGET* widget) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void virtualClientRectGet(const ESGUI_WIDGET* widget, ESGUI_RECT* rect) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual const ESGUI_POINT* viewOffsetGet(const ESGUI_WIDGET* widget) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual int virtualClientWidthGet(const ESGUI_WIDGET* widget) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual int virtualClientHeightGet(const ESGUI_WIDGET* widget) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void scrollTo(ESGUI_WIDGET* widget, const ESGUI_POINT* pnt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_WIDGET* create(ESGUI_WIDGET_ROOT* root, ESGUI_WIDGET_CLASS cls, const ESGUI_RECT* bounds, esU32 flags) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void remove(ESGUI_WIDGET* widget) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void init(ESGUI_WIDGET_ROOT* root, ESGUI_WIDGET* widget, ESGUI_WIDGET_CLASS cls, const ESGUI_RECT* bounds, esU32 flags) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool modalAdd(ESGUI_WIDGET* widget) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool parentSet(ESGUI_WIDGET* widget, ESGUI_WIDGET* parent) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool focusSet(ESGUI_WIDGET* wgt, ESGUI_WIDGET* sender) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool focusSetFirst(ESGUI_WIDGET* group, ESGUI_WIDGET* sender) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool focusSetLast(ESGUI_WIDGET* group, ESGUI_WIDGET* sender) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool focusSetNext(ESGUI_WIDGET* group, ESGUI_WIDGET* sender, bool allowWrap) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool focusSetPrev(ESGUI_WIDGET* group, ESGUI_WIDGET* sender, bool allowWrap) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void enter(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void exit(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool commit) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void show(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool show) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void showOnTop(ESGUI_WIDGET* target, ESGUI_WIDGET* subject) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool canClose(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool commit) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void close(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool commit) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void enable(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool enable) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void check(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, bool check) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void click(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void paint(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, const ESGUI_RECT* rect, esU32 flags) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void invalidate(ESGUI_WIDGET* widget, const ESGUI_RECT* rect, bool paintBg) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual int metricGet(int metric) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void eventMarkHandled(EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool eventIsHandled(EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool eventIsSecondTick(EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool eventIsInteractive(EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void paintPost(ESGUI_WIDGET* widget, ESGUI_WIDGET* sender, const ESGUI_RECT* rect, esU32 flags) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool paintEvtExtract(EsguiAppEventT* evt, ESGUI_WIDGET** pwgtSender, ESGUI_WIDGET** pwgt, ESGUI_RECT* ppaintRect, esU32* paintFlags) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;

#if defined(ESGUI_WIDGET_USE_STD) && 1 == ESGUI_WIDGET_USE_STD
  virtual void stdPaintTitle(ESGUI_HDC hdc, const ESGUI_RECT* rect, const ESGUI_BITMAP* bmp, ESE_CSTR title) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual pfnstdPaintTitleT pfnstdPaintTitle() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;

  virtual void stdPaintVscrollBar(ESGUI_HDC hdc, const ESGUI_RECT* client, int min, int max, int pos) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stdPaintHscrollBar(ESGUI_HDC hdc, const ESGUI_RECT* client, int min, int max, int pos) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stdPaintScrollbar(ESGUI_HDC hdc, const ESGUI_WIDGET* This, const ESGUI_RECT* draw, bool vertical) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_PAINT_SCROLLBAR pfnstdPaintScrollbar() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
 
  virtual ESGUI_PAINT_EVENT stdPaintBgGet() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stdPaintBg(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stdPaintStringWithSmartErase(ESGUI_HDC hdc, ESE_CSTR str, const ESGUI_POINT* pos, int* prevStrLen) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stdPaintCenteredStringWithSmartErase(ESGUI_HDC hdc, ESE_CSTR str, const ESGUI_POINT* org, int areaWidth, int* prevStrLen) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stdPaintLabeledVal(ESGUI_HDC hdc, const ESGUI_POINT* pos, int offs, ESE_CSTR lbl, ESE_CSTR val) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stdPaintLabeledValWithSmartErase(
    ESGUI_HDC hdc, ESGUI_POINT* pos, int lblRulerOffs, int unitRulerOffs,
    int* prevLenContainer, int* prevUnitLenContainer, ESGUI_COLOR fgnd, ESGUI_COLOR bgnd, ESE_CSTR lbl, const ESGUI_FONT* lblFont,
    ESE_CSTR valStr, const ESGUI_FONT* valFont, ESE_CSTR unit, const ESGUI_FONT* unitFont
  ) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void stdButtonPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  
#if defined(ESGUI_USE_FONT) && 1 == ESGUI_USE_FONT
  virtual const ESGUI_FONT* stdFontGet(const ESGUI_HDC hdc, int resourceId) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
#endif

#if defined(ESGUI_USE_BITMAP) && 1 == ESGUI_USE_BITMAP
  virtual const ESGUI_BITMAP* stdBitmapGet(const ESGUI_HDC hdc, int resourceId) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
#endif

  virtual ESGUI_COLOR stdColorGet(const ESGUI_HDC hdc, int resourceId) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESE_CSTR stdStringGet(int resourceId) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool eventIsCtlDec(EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool eventIsCtlDecBig(EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool eventIsCtlInc(EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool eventIsCtlIncBig(EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool eventIsCtlCommit(EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool eventIsCtlEnter(EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool eventIsCtlExit(EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
#endif //< ESGUI_WIDGET_USE_STD
  
#if defined(ESGUI_WIDGET_USE_CHECKBOX) && 1 == ESGUI_WIDGET_USE_CHECKBOX
  virtual void checkBoxOnPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_WIDGET* checkBoxCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, ESE_CSTR lbl, bool check) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
#endif

#if defined(ESGUI_WIDGET_USE_DTEDIT) && 1 == ESGUI_WIDGET_USE_DTEDIT
  virtual ESGUI_WIDGET* timeEditorCreate(ESGUI_WIDGET_ROOT* root, ESE_CSTR label, const esDT* dt, const ESGUI_RECT* bounds) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_WIDGET* dateEditorCreate(ESGUI_WIDGET_ROOT* root, ESE_CSTR label, const esDT* dt, const ESGUI_RECT* bounds) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual esDT currentDateTimeGet() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void currentDateTimeSet(const esDT* dt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESE_CSTR dteditTimeFmtStrGet() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESE_CSTR dteditDateFmtStrGet() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
#endif //< ESGUI_WIDGET_USE_DTEDIT

#if defined(ESGUI_WIDGET_USE_LISTVIEW) && 1 == ESGUI_WIDGET_USE_LISTVIEW
  virtual ESGUI_WgtListViewData* listViewDataCreate(size_t listViewDataSize) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void listViewDataFree(ESGUI_WgtListViewData* data) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void listViewOnEvent(ESGUI_WIDGET* This, ESGUI_WIDGET* sender, EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_WIDGET* listViewCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, ESGUI_WgtListViewData* data) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
#endif //< ESGUI_WIDGET_USE_LISTVIEW

#if defined(ESGUI_WIDGET_USE_MSGBOX) && 1 == ESGUI_WIDGET_USE_MSGBOX
  virtual void msgBoxDataInit(ESGUI_WgtMsgBoxData* data, ESE_CSTR title, ESE_CSTR text, esU32 type) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual esU32 msgBoxStdEventHandler(ESGUI_WIDGET* This, EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual esU32 msgBox(ESGUI_WIDGET_ROOT* root, ESE_CSTR title, ESE_CSTR text, const ESGUI_RECT* rect, esU32 type, int tag, ESGUI_WIDGET** out) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual esU32 msgBoxIndirect(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* rect, int tag, ESGUI_WgtMsgBoxData* data, bool ownsData, ESGUI_WIDGET** out) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void msgBoxAppEventsProcess() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool msgBoxStdCanCreate() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool eventIsMsgBoxOk(ESGUI_WIDGET* This, EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool eventIsMsgBoxCancel(ESGUI_WIDGET* This, EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void msgBoxBeforeShow() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void msgBoxAfterShow() const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void msgBoxCancel(ESGUI_WIDGET* msgbox) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
#endif //< ESGUI_WIDGET_USE_MSGBOX

#if defined(ESGUI_WIDGET_USE_PROGRESS) && 1 == ESGUI_WIDGET_USE_PROGRESS
  virtual void progressOnEvent(ESGUI_WIDGET* This, ESGUI_WIDGET* sender, EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void progressOnPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void progressSetOnTextHandler(ESGUI_WIDGET* This, ESGUI_WgtOnDisplayProgressText onDisplayProgressText) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void progressSetRange(ESGUI_WIDGET* This, int min, int max) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void progressSetPos(ESGUI_WIDGET* This, int pos) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_WIDGET* progressCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, const ESGUI_FONT* font, ESGUI_WgtOnDisplayProgressText onDisplayProgressText, int rmin, int rmax, int pos) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_WIDGET* progressCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, const ESGUI_FONT* font, ESGUI_WgtOnDisplayProgressText onDisplayProgressText) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool eventIsProgressSet(EsguiAppEventT* evt, int* pos) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual bool eventIsProgressRangeSet(EsguiAppEventT* evt, int* min, int* max) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
#endif //< ESGUI_WIDGET_USE_PROGRESS

#if defined(ESGUI_WIDGET_USE_SPINCTL) && 1 == ESGUI_WIDGET_USE_SPINCTL
  virtual void spinctlOnEvent(ESGUI_WIDGET* This, ESGUI_WIDGET* sender, EsguiAppEventT* evt) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void spinctlOnPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender, const ESGUI_RECT* rect) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void spinctlSetRangeInt(ESGUI_WIDGET* This, int min, int max) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void spinctlSetRangeFloat(ESGUI_WIDGET* This, float min, float max) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void spinctlGetRange(ESGUI_WIDGET* This, int& min, int& max) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void spinctlGetRange(ESGUI_WIDGET* This, float& min, float& max) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void spinctlSetValueInt(ESGUI_WIDGET* This, int val) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void spinctlSetValueFloat(ESGUI_WIDGET* This, float val) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual int spinctlGetValue(ESGUI_WIDGET* This) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual float spinctlGetValue(ESGUI_WIDGET* This) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void spinctlSetIncInt(ESGUI_WIDGET* This, int inc) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void spinctlSetIncFloat(ESGUI_WIDGET* This, float inc) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual int spinctlGetInc(ESGUI_WIDGET* This) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual float spinctlGetInc(ESGUI_WIDGET* This) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void spinctlSetBigIncInt(ESGUI_WIDGET* This, int bigInc) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual void spinctlSetBigIncFloat(ESGUI_WIDGET* This, float bigInc) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual int spinctlGetBigInc(ESGUI_WIDGET* This) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual float spinctlGetBigInc(ESGUI_WIDGET* This) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_WIDGET* spinctlCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, ESE_CSTR lbl, int min, int max) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  virtual ESGUI_WIDGET* spinctlCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, ESE_CSTR lbl, float min, float max, int decimals) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
#endif //< ESGUI_WIDGET_USE_SPINCTL

#if (defined(ESGUI_WIDGET_USE_STD) && 1 == ESGUI_WIDGET_USE_STD) && \
    ( \
      (defined(ESGUI_WIDGET_USE_SPINCTL) && 1 == ESGUI_WIDGET_USE_SPINCTL) || \
      (defined(ESGUI_WIDGET_USE_DTEDIT) && 1 == ESGUI_WIDGET_USE_DTEDIT) \
    )

  virtual void stdPaintEditorChar(ESGUI_HDC hdc, ESGUI_CharCallbackReason reason, ESE_CSTR pos, const ESGUI_RECT* rect) const ESE_NOTHROW ESE_OVERRIDE ESE_KEEP;
  
#endif

  ESE_NONCOPYABLE(EsguiWgtApiIntfImpl);
};

#endif //< _es_gui_wgt_api_h_
