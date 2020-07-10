#ifndef _esgui_chart_series_2d_h_
#define _esgui_chart_series_2d_h_

// fwd decls
typedef struct EsGuiChartSeries2dT EsGuiChartSeries2d;

// specific flags
enum {
    esguiChartSeries2DxIsDt = 0x00000100,
    esguiChartSeries2DyIsDt = 0x00000200,
};

// series 2d renderer implementations
//
typedef struct EsGuiChartSeries2dRendererT {
    EsGuiChartSeriesRenderer cmn;
    // index cache - points between these indexes take part in plotting
    int startIdx;
    int endIdx;

} EsGuiChartSeries2dRenderer;

// bar renderer (spectrums, etc.)
void esguiChartSeries2dBarRendererInit(EsGuiChartSeries2dRenderer* renderer);
void esguiChartSeries2dBarRendererBarWidthSet(EsGuiChartSeries2dRenderer* renderer, size_t width);

#endif // _esgui_chart_series_2d_h_
