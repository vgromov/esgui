#ifndef _esgui_chart_h_
#define _esgui_chart_h_

#ifdef __cplusplus
    extern "C" {
#endif

// ESGUI chart types
//

// flags
enum {
    // chart axis flags
    //
    // axis data hint flags
    esguiChartAxisDataHintGeneric        = 0x00000001,
    esguiChartAxisDataHintRad                = 0x00000002, // angular data in radians
    esguiChartAxisDataHintDeg                = 0x00000004,    // angular data in degrees
    esguiChartAxisDataHintDateTime    = 0x00000008, // date time data
    esguiChartAxisDataHintMask            = esguiChartAxisDataHintGeneric|
                                                                        esguiChartAxisDataHintRad|
                                                                        esguiChartAxisDataHintDeg|
                                                                        esguiChartAxisDataHintDateTime,
    // axis tick position
    esguiChartAxisTickPosOutside        = 0x00000100,
    esguiChartAxisTickPosCenter            = 0x00000200,
    esguiChartAxisTickPosInside            = 0x00000400,
    esguiChartAxisTickPosMask                = esguiChartAxisTickPosOutside|
                                                                        esguiChartAxisTickPosCenter|
                                                                        esguiChartAxisTickPosInside,

    // axis behaviour
    esguiChartAxisBehaviorInverted    = 0x00002000,
    esguiChartAxisBehaviorAutoMin        = 0x00004000,
    esguiChartAxisBehaviorAutoMax        = 0x00008000,
    esguiChartAxisBehaviorAutoTicks = 0x00010000,
    esguiChartAxisBehaviorMask            = esguiChartAxisBehaviorInverted|
                                                                        esguiChartAxisBehaviorAutoMin|
                                                                        esguiChartAxisBehaviorAutoMax|
                                                                        esguiChartAxisBehaviorAutoTicks,
    // elements drawing
    esguiChartAxisDrawTicks                    = 0x00020000,
    esguiChartAxisDrawGrid                    = 0x00040000,
    esguiChartAxisDrawLabels                = 0x00080000,
    esguiChartAxisDrawMask                    = esguiChartAxisDrawTicks|
                                                                        esguiChartAxisDrawGrid|
                                                                        esguiChartAxisDrawLabels,
};

// misc chart axis defs
enum {
    esguiChartAxisDefMajorTickLen        = 4,
    esguiChartAxisDefMinorTickLen        = 2,
    esguiChartAxisDefMinorTickPerMajorTick = 1,
    esguiChartAxisMaxMinorTickPerMajorTick = 5,
    esguiChartAxisMinMajorTicksCnt    = 2,
    esguiChartAxisDefMajorTicksCnt    = 5,
    esguiChartAxisMaxMajorTicksCnt    = 10,
    esguiChartAxisLabelMargin                = 2, //minimal distance between major tick and label
    esguiChartMaxSeriesCnt                    = 4,
    esguiChartSeriesRendererDefPointSize = 5,
};

// chart world (dataspace) data type abstraction
//
typedef union {
    esDT dt;
    float f;

} EsGuiChartDataValue;

// Misc api
//
extern const EsGuiChartDataValue c_esguiChartDataValueZero;
EsGuiChartDataValue esguiChartDataValueFromFloat(float f);
EsGuiChartDataValue esguiChartDataValueFromDt(esDT dt);
bool esguiChartDataValueIsInVicinity(const EsGuiChartDataValue* baseVal, const EsGuiChartDataValue* val,
                                                                         float tolerance, bool isDt);
// generic data point compare operators 
// returned are -1 if _1 is less than _2,
// 0 if equal, and 1, if _1 is greater than _2
int esguiChartDataValueCompare(const EsGuiChartDataValue* _1, const EsGuiChartDataValue* _2, bool isDt);

typedef struct {
    EsGuiChartDataValue x;
    EsGuiChartDataValue y;

} EsGuiChartWorldPoint;

typedef struct {
    EsGuiChartWorldPoint org;
    EsGuiChartWorldPoint edge;

} EsGuiChartWorldRect;

// series abstraction
//

// series point search request responses
typedef enum {
    esguiChartSeriesFindLS,        // find index of point with val less than given value
    esguiChartSeriesFindLE,     // find index of point with val less than or equal to given value
    esguiChartSeriesFindGE,        // find index of point with val greater than or equal to given value 
    esguiChartSeriesFindGR,        // find index of point with val greater than given value
    esguiChartSeriesFindEQ,        // find index of point with val equal to given value
    esguiChartSeriesFindNONE    // find failed

} EsGuiChartSeriesFind;

// series flags
enum {
    esguiChartSeriesNeedRecalc = 0x00000001,
    esguiChartSeriesDrawLine = 0x00000002,
    esguiChartSeriesDrawArea = 0x00000004,
    esguiChartSeriesDrawPointLine = 0x00000008,
    esguiChartSeriesDrawPointArea = 0x00000010,
};

// Series renderer
struct EsGuiChartRendererT;
typedef struct EsGuiChartSeriesRendererT {
    // series drawing styles
    ESGUI_COLOR clrLine;
    ESGUI_COLOR clrArea;
    ESGUI_COLOR clrPointLine;
    ESGUI_COLOR clrPointArea;
    size_t lineWidth;
    size_t pointSize;
    struct EsGuiChartSeriesT* series;

    // api
    void (*prepare)(struct EsGuiChartSeriesRendererT*, struct EsGuiChartRendererT*);
    void (*draw)(struct EsGuiChartSeriesRendererT*, struct EsGuiChartRendererT*, ESGUI_HDC);

} EsGuiChartSeriesRenderer;

// series renderer api
//
// base initializer
void esguiChartSeriesRendererInit( EsGuiChartSeriesRenderer* renderer );

// series
typedef struct EsGuiChartSeriesT {
    // flags
    esU32 flags;
    // reference to chart
    struct EsGuiChartDataT* chart;
    // reference to axes, which this series is attached to
    struct EsGuiChartAxisT* axisX;
    struct EsGuiChartAxisT* axisY;
    // series renderer
    struct EsGuiChartSeriesRendererT* renderer;
    // abstract underlying data container pointer
    void* data;

    // series api handlers
    //
    size_t (*sizeGet)(const struct EsGuiChartSeriesT*);
    int (*getMinXIdx)(const struct EsGuiChartSeriesT*);
    int (*getMaxXIdx)(const struct EsGuiChartSeriesT*);
    int (*getMinYIdx)(const struct EsGuiChartSeriesT*);
    int (*getMaxYIdx)(const struct EsGuiChartSeriesT*);
    bool (*pointGetAt)(const struct EsGuiChartSeriesT*, size_t, EsGuiChartWorldPoint*);
    void (*cleanup)(const struct EsGuiChartSeriesT*);

} EsGuiChartSeries;

// series api
//
void esguiChartSeriesInit(EsGuiChartSeries* series, void* data);
void esguiChartSeriesFlagsSet(EsGuiChartSeries* series, esU32 flags);
bool esguiChartSeriesIsEmpty(const EsGuiChartSeries* series);
size_t esguiChartSeriesSizeGet(const EsGuiChartSeries* series);
int esguiChartSeriesMinXIdxGet(const EsGuiChartSeries* series);
int esguiChartSeriesMaxXIdxGet(const EsGuiChartSeries* series);
int esguiChartSeriesMinYIdxGet(const EsGuiChartSeries* series);
int esguiChartSeriesMaxYIdxGet(const EsGuiChartSeries* series);
bool esguiChartSeriesPointGetAt(const EsGuiChartSeries* series, size_t idx, EsGuiChartWorldPoint* wp);
void esguiChartSeriesCleanup(const EsGuiChartSeries* series);
void esguiChartSeriesRendererSet(EsGuiChartSeries* series, EsGuiChartSeriesRenderer* renderer);
// call after changes were made to series contents, renderer styles, etc.
// if needRecalc is set, series renderer get marked for preparation before drawing;
// parenting chart is marked for recalculation and invalidation as well
void esguiChartSeriesInvalidate(EsGuiChartSeries* series, bool needRecalc);

// chart axis
//
typedef struct EsGuiChartAxisT {
    // common flags
    esU32 flags;
    // reference to chart
    struct EsGuiChartDataT* chart;
    // axis minimax (internal usage only)
    EsGuiChartDataValue min;
    EsGuiChartDataValue max;
    // internal flags
    bool isX : 1;
    bool autoCalcMajorTickStep;
    // internal major && minor tick steps
    EsGuiChartDataValue majorTickStep;
    EsGuiChartDataValue minorTickStep;
    // tick values cache, internal usage only
    EsGuiChartDataValue majorTicksCache[esguiChartAxisMaxMajorTicksCnt];
    size_t ticksCacheSize;
    size_t minorTicksPreMajotTickCnt;

    // axis api
    void (*flagsSet)(struct EsGuiChartAxisT*, esU32);
    void (*majorTicksCountSet)(struct EsGuiChartAxisT*, size_t);
    void (*minorTicksPerMajorTickSet)(struct EsGuiChartAxisT*, size_t);
    void (*cleanup)(struct EsGuiChartAxisT*);

} EsGuiChartAxis;

// axis api
//
void esguiChartAxisFlagsSet(EsGuiChartAxis* axis, esU32 flags);
void esguiChartAxisFlagsClear(EsGuiChartAxis* axis, esU32 flags);
#define esguiChartAxisFlagIsSet(axis, flag)    ((flag) == ((axis)->flags & (flag)))
EsGuiChartDataValue esguiChartAxisMinGet(const EsGuiChartAxis* axis);
EsGuiChartDataValue esguiChartAxisMaxGet(const EsGuiChartAxis* axis);
void esguiChartAxisMinSet(EsGuiChartAxis* axis, EsGuiChartDataValue val);
void esguiChartAxisMaxSet(EsGuiChartAxis* axis, EsGuiChartDataValue val);
void esguiChartAxisRangeSet(EsGuiChartAxis* axis, EsGuiChartDataValue min, EsGuiChartDataValue max);
void esguiChartAxisMajorTicksCountSet(EsGuiChartAxis* axis, size_t count);
void esguiChartAxisMajorTickStepSet(EsGuiChartAxis* axis, EsGuiChartDataValue val);
void esguiChartAxisMinorTicksPerMajorTickSet(EsGuiChartAxis* axis, size_t count);
void esguiChartAxisCleanup(EsGuiChartAxis* axis);

// axis minimax and ticks recalculation
void esguiChartAxisRecalcData(EsGuiChartAxis* axis);

// chart
//
// flags
enum {
    esguiChartFlagDrawChartBorder = 0x00000001,
    esguiChartFlagDrawChartArea        = 0x00000002,
    esguiChartFlagDrawPlotBorder    = 0x00000004,
    esguiChartFlagDrawPlotArea        = 0x00000008,
    // chart parameters need recalculation
    esguiChartFlagNeedRecalc            = 0x00010000,
};

typedef struct EsGuiChartDataT {
    // reference to the owning widget
    ESGUI_WIDGET* wgt;
    esU32 flags;
    // chart components
    EsGuiChartAxis axisX;
    EsGuiChartAxis axisY;
    // chart series array
    EsGuiChartSeries* series[esguiChartMaxSeriesCnt];
    size_t seriesCnt;
    // chart renderer
    struct EsGuiChartRendererT* renderer;

} EsGuiChartData;

// chart API
//
// CAUTION! this is abstract chart creation method and should not be used directly.
// instead, one of specialized creators, like esguiCartesian2dChartCreate, must be called.
// custom instance of chart data, which created elsewhere, may be assigned.
// otherwise, if 0 is passed to chartData, the instance of EsGuiChartData is created internally
ESGUI_WIDGET* esguiChartCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, struct EsGuiChartRendererT* renderer, EsGuiChartData* chartData);

// set renderer instance to chart
void esguiChartRendererSet(EsGuiChartData* chart, struct EsGuiChartRendererT* renderer);

// flags manipulator
void esguiChartFlagsSet(EsGuiChartData* chart, esU32 flags);
void esguiChartFlagsClear(EsGuiChartData* chart, esU32 flags);

// chart invalidator. if needRecalc is TRUE, chart will recalc is internals on next redraw
// rect is optional parameter specifying which part of chart area should actually be drawn
void esguiChartInvalidate(EsGuiChartData* chart, bool needRecalc, const ESGUI_RECT* rect );

// generic series attach-detach manipulators
void esguiChartSeriesAttach(EsGuiChartData* chart, EsGuiChartSeries* series);
void esguiChartSeriesDetach(EsGuiChartData* chart, EsGuiChartSeries* series);

// chart renderer interface
//
typedef struct EsGuiChartRendererT {
    EsGuiChartData* chart;
    // styles, colors, fonts
    //
    esU32 flags;
    ESGUI_COLOR clrAxes;
    ESGUI_COLOR clrGrid;
    ESGUI_COLOR clrChartBorder;
    ESGUI_COLOR clrChartArea;
    ESGUI_COLOR clrPlotBorder;
    ESGUI_COLOR clrPlotArea;
    const ESGUI_FONT* fntAxesLabels;
    ESGUI_COLOR clrAxesLabels;
    const ESGUI_FONT* fntAxesNames;
    ESGUI_COLOR clrAxesNames;
    const ESGUI_FONT* fntChartName;
    ESGUI_COLOR clrChartName;

    // internal data, calculated in prepare method
    //
    ESGUI_RECT plotRect;

    // api
    //
    void (*prepare)(struct EsGuiChartRendererT*, ESGUI_HDC);
    void (*fmtLabel)(const struct EsGuiChartRendererT*, const EsGuiChartAxis*, ESE_STR, size_t, EsGuiChartDataValue); // application-specific handler
    void (*drawGrid)(const struct EsGuiChartRendererT*, const EsGuiChartAxis*, ESGUI_HDC);
    void (*drawAxis)(const struct EsGuiChartRendererT*, const EsGuiChartAxis*, ESGUI_HDC);
    void (*drawPlotArea)(const struct EsGuiChartRendererT*, ESGUI_HDC);
    void (*drawChartArea)(const struct EsGuiChartRendererT*, ESGUI_HDC, const ESGUI_RECT*);
    void (*paintExtra)(const struct EsGuiChartRendererT*, ESGUI_HDC);
    // return world bounds along specified axis
    void (*worldRangeGet)(const struct EsGuiChartRendererT*, const EsGuiChartAxis*, void*);
    // return canvas point given world point and vice versa
    void (*worldPointToHdc)(const struct EsGuiChartRendererT*, const void*, ESGUI_POINT*);
    void (*canvasPointToWorld)(const struct EsGuiChartRendererT*, const ESGUI_POINT*, void*);

} EsGuiChartRenderer;

// Base renderer "prepare" helpers
void esguiChartRendererMaxLabelExtentGet(EsGuiChartRenderer* renderer, ESGUI_HDC hdc, const EsGuiChartAxis* axis, ESGUI_POINT* ext);
void esguiChartRendererWorldPointToHdc(const EsGuiChartRenderer* renderer, const void* wp, ESGUI_POINT* p);
void esguiChartRendererHdcPointToWorld(const EsGuiChartRenderer* renderer, const ESGUI_POINT* p, void* wp);

// Services to be implemented in client application
//

// Allocate, statically, or dynamically, chart-specific notification event
EsguiAppEventT* esguiChartRecalculationNotificationEventCreate(void);

// custom chart notification events
enum {
    chartNotificationsBase = ESGUI_CHART_NOTIFICATION_BASE,
    NF_CHART_RECALCULATED     = chartNotificationsBase,    // chart recalculation is complete, we may notify chart's children widgets of it
};
    
#ifdef __cplusplus
    }
#endif

#endif // _esgui_chart_h_
