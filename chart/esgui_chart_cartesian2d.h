#ifndef _esgui_chart_cartesian2d_h_
#define _esgui_chart_cartesian2d_h_

#ifdef __cplusplus
    extern "C" {
#endif

// Cartesian 2D chart renderer specialization
//
typedef struct {
    EsGuiChartRenderer cmn;
    // specific cartesian 2d renderer data
    //
    // cached stuff
    ESGUI_POINT axisXmaxLabelExt;
    ESGUI_POINT axisYmaxLabelExt;
    // factors are used in conversion from worldrect to DC rect
    float factorX;
    float factorY;
    float _1_factorX;
    float _1_factorY;
    EsGuiChartWorldPoint worldOrg;
    EsGuiChartWorldPoint worldEdge;    
    // internal flags
    esBL invertX : 1;
    esBL invertY : 1;
    esBL xIsDt     : 1;
    esBL yIsDt     : 1;

} EsGuiChartCartesian2dRenderer;

// necessary implementation specialization for Cartesian 2D chart type
//
// creation internally sets-up necessary axis and plot handlers for 2D cartesian dataspace.
// if 0 is passed to chartData, the instance of EsGuiChartData is created internally
ESGUI_WIDGET* esguiChartCartesian2dCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds,    EsGuiChartData* chartData);

#ifdef __cplusplus
    }
#endif

    // 2d chart automatically includes basic 2d series support
#include <esgui/chart/esgui_chart_series2d.h>

#endif // _esgui_chart_cartesian2d_h_
