#include <esgui/include/esgui.h>
#include <esfwxe/core/rtos_intf.h>
#pragma hdrstop

#include "esgui_chart_series2d.h"

// standard series point search facilities
//
// return index of point satisfying search criterion in EsGuiChartSeriesFind or -1 if search failed
// additional info on search result is returned back in EsGuiChartSeriesFind
static int esguiChartSeries2dFindXidx( const EsGuiChartSeries* series, const EsGuiChartDataValue* x, 
                                                         int startIdx, int endIdx, EsGuiChartSeriesFind* searchCode )
{
    int result = -1;
    int sze = esguiChartSeriesSizeGet(series);
    // don't even try to search if index bounds are invalid or series is empty
    if( startIdx < 0 || startIdx >= sze || 
            endIdx >= sze || startIdx > endIdx ||
            esguiChartSeriesIsEmpty(series) )
        *searchCode = esguiChartSeriesFindNONE;
    else
    {
        EsGuiChartWorldPoint wp;
        if( esguiChartSeriesPointGetAt(series, startIdx, &wp) )
        {
            EsGuiChartDataValue minX = wp.x;
            if( esguiChartSeriesPointGetAt(series, endIdx, &wp) )
            {
                EsGuiChartDataValue maxX = wp.x;
                esBL valIsInWindow;
                int compareMin, compareMax;

// re-entrance point    
_findXidx_Loop:
                compareMin = esguiChartDataValueCompare(x, &minX, (series->flags & esguiChartSeries2DxIsDt));
                compareMax = esguiChartDataValueCompare(x, &maxX, (series->flags & esguiChartSeries2DxIsDt));
                valIsInWindow = compareMin > -1 && compareMax < 1;

                if( valIsInWindow )
                {
                    //if we hit the edges
                    if( 0 == compareMin )
                    {
                        switch( *searchCode )
                        {
                        case esguiChartSeriesFindLS:
                            *searchCode = esguiChartSeriesFindNONE;
                            break;
                        case esguiChartSeriesFindLE:
                        case esguiChartSeriesFindGE:
                        case esguiChartSeriesFindEQ:
                            *searchCode = esguiChartSeriesFindEQ;
                            result = startIdx;
                            break;
                        case esguiChartSeriesFindGR:
                            if( startIdx != endIdx )
                            {
                                *searchCode = esguiChartSeriesFindGR;
                                result = startIdx+1;
                            }
                            else
                                *searchCode = esguiChartSeriesFindNONE;
                            break;
            default:
              // Pacify compiler
              break;
                        }
                    }
                    else if( 0 == compareMax )
                    {
                        switch( *searchCode )
                        {
                        case esguiChartSeriesFindLS:
                            if( startIdx != endIdx )
                            {
                                *searchCode = esguiChartSeriesFindLS;
                                result = endIdx-1;
                            }
                            break;
                        case esguiChartSeriesFindLE:
                        case esguiChartSeriesFindGE:
                        case esguiChartSeriesFindEQ:
                            *searchCode = esguiChartSeriesFindEQ;
                            result = endIdx;
                            break;
                        case esguiChartSeriesFindGR:
                            *searchCode = esguiChartSeriesFindNONE;
                            break;
            default:
              // Pacify compiler
              break;
                        }
                    }
                    else //search iteratively
                    {
                        int delta = (endIdx-startIdx)/2;

                        if( delta )
                        {
                            int midIdx = startIdx + delta;
                            if( esguiChartSeriesPointGetAt(series, midIdx, &wp) )
                            {
                                if( 1 == esguiChartDataValueCompare(x, &wp.x, (series->flags & esguiChartSeries2DxIsDt)) )
                                {
                                    startIdx = midIdx;
                                    minX = wp.x;
                                }
                                else
                                {
                                    endIdx = midIdx;
                                    maxX = wp.x;
                                }

                                //go to re-entrance point
                                goto _findXidx_Loop;
                            }
                        }
                        else
                        {
                            //check hit between two nodes
                            switch( *searchCode )
                            {
                            case esguiChartSeriesFindLS:
                            case esguiChartSeriesFindLE:
                                *searchCode = esguiChartSeriesFindLS;
                                result = startIdx;
                                break;
                            case esguiChartSeriesFindGE:
                            case esguiChartSeriesFindGR:
                                *searchCode = esguiChartSeriesFindGR;
                                result = endIdx;
                                break;
                            case esguiChartSeriesFindEQ:
                                *searchCode = esguiChartSeriesFindNONE;
                                break;
              default:
                // Pacify compiler
                break;
                            }
                        }
                    }
                }
                else
                {
                    //check out-of window conditions
                    switch( *searchCode )
                    {
                    case esguiChartSeriesFindLS:
                    case esguiChartSeriesFindLE:
                        if( compareMin < 0 )
                            *searchCode = esguiChartSeriesFindNONE;
                        else
                        {
                            *searchCode = esguiChartSeriesFindLS;
                            result = endIdx;
                        }
                        break;
                    case esguiChartSeriesFindGE:
                    case esguiChartSeriesFindGR:
                        if( compareMin < 0 )
                        {
                            *searchCode = esguiChartSeriesFindGR;
                            result = startIdx;
                        }
                        else
                            *searchCode = esguiChartSeriesFindNONE;
                        break;
                    case esguiChartSeriesFindEQ:
                        *searchCode = esguiChartSeriesFindNONE;
                        break;
          default:
            // Pacify compiler
            break;
                    }
                }
            }
        }
    }

    return result;
}

// return point next to src, so as it results in canvas point different from canvas point 
// calculated from src itself (large data series decimation). index of decimated point is returned
// if no decimation is done, idx remains intact and returned point is src. If no decimated point is found during
// search, the last point before endIdx is returned even if it results in the same canvas point as src
static void esguiChartSeries2dNextDecimatedPointGet(    const EsGuiChartRenderer* renderer, 
    const EsGuiChartSeries* series, const EsGuiChartWorldPoint* src, EsGuiChartWorldPoint* wp, int* idx, int endIdx)
{
    const EsGuiChartCartesian2dRenderer* r = (const EsGuiChartCartesian2dRenderer*)renderer;
    esBL ok = esguiChartSeriesPointGetAt(series, *idx, wp);
    while( ok && 
        esguiChartDataValueIsInVicinity(&src->x, &wp->x, r->_1_factorX, (series->flags & esguiChartSeries2DxIsDt)) &&
        esguiChartDataValueIsInVicinity(&src->y, &wp->y, r->_1_factorY, (series->flags & esguiChartSeries2DyIsDt)) && 
        *idx < endIdx-1 )
    {
        *idx += 1;
        ok = esguiChartSeriesPointGetAt(series, *idx, wp);
    }    
}

// try to find reasonable range of indexes 
static void esguiChartSeries2dFindPointsToDraw( EsGuiChartSeries* series, 
                                                                                                const EsGuiChartRenderer* renderer, int* start, int* end )
{
    //find points that fall into the world window
    EsGuiChartSeriesFind result = esguiChartSeriesFindGE;
    EsGuiChartWorldPoint wp;
    int idx;
    int lastIdx = esguiChartSeriesSizeGet(series)-1;
    *start = -1;
    *end = -1;

    renderer->worldRangeGet(renderer, series->axisX, &wp);
    idx = esguiChartSeries2dFindXidx( series, &wp.x, 0, lastIdx, &result );
    if( idx != -1 )
    {
        int endIdx;
        //start drawing from the previous point
        if( (result == esguiChartSeriesFindGR) && idx > 0 )
            --idx;

        result = esguiChartSeriesFindLE;
        endIdx = esguiChartSeries2dFindXidx( series, &wp.y, idx, lastIdx, &result );
        if( endIdx != -1 )
        {
            //end drawing at the next point
            if( result == esguiChartSeriesFindLS && 
                    endIdx < lastIdx )
                ++endIdx;

            *start = idx;
            *end = endIdx;
        }
    }
}

// bar renderer
//
static void esguiChartSeries2dBarRendererPrepare(EsGuiChartSeriesRenderer* sr, EsGuiChartRenderer* r)
{
    // find points to draw
    EsGuiChartSeries* s = sr->series;
    EsGuiChartSeries2dRenderer* renderer = (EsGuiChartSeries2dRenderer*)sr;
    esguiChartSeries2dFindPointsToDraw( s, r, &renderer->startIdx, &renderer->endIdx );
}

static void esguiChartSeries2dBarRendererDrawBar(EsGuiChartSeriesRenderer* sr, ESGUI_HDC hdc, 
                                                                                                 const ESGUI_POINT* pos, int len)
{
    ESGUI_POINT org = *pos;
    size_t w = sr->lineWidth;
    org.x -= w/2;
    while(w-- != 0)
    {
        esguiCanvasDrawVLine(hdc, &org, len);
        ++org.x;
    }
}

static void esguiChartSeries2dBarRendererDraw(EsGuiChartSeriesRenderer* sr, EsGuiChartRenderer* cr, ESGUI_HDC hdc)
{
    EsGuiChartSeries* s = sr->series;
    EsGuiChartSeries2dRenderer* r2d = (EsGuiChartSeries2dRenderer*)sr; 
    // check if there were found points to draw
    if( r2d->startIdx > -1 && 
            r2d->endIdx > -1 && 
            r2d->endIdx >= r2d->startIdx )
    {
        EsGuiChartWorldPoint wpCur;
        EsGuiChartWorldPoint wp;
        ESGUI_POINT org;

        int idx = r2d->startIdx;
        esguiCanvasFgColorSet(hdc, r2d->cmn.clrLine);
        if( esguiChartSeriesPointGetAt(s, idx, &wpCur) )
        {
            esguiChartRendererWorldPointToHdc(cr, &wpCur, &org);
            esguiChartSeries2dBarRendererDrawBar(sr, hdc, &org, cr->plotRect.edge.y - org.y -1);
            while( idx <= r2d->endIdx )
            {
                esguiChartSeries2dNextDecimatedPointGet(cr, s, &wpCur, &wp, &idx, r2d->endIdx);
                wpCur = wp; 
                esguiChartRendererWorldPointToHdc(cr, &wpCur, &org);
                esguiChartSeries2dBarRendererDrawBar(sr, hdc, &org, cr->plotRect.edge.y - org.y -1);
                ++idx;
            } //while
        }
    } //if( check found indeces )
}

// bar renderer (spectrums, etc.)
void esguiChartSeries2dBarRendererInit(EsGuiChartSeries2dRenderer* renderer)
{
    if( renderer )
    {
        // csll base initializer first
        esguiChartSeriesRendererInit((EsGuiChartSeriesRenderer*)renderer);
        // do custom initialization
        renderer->startIdx = -1;
        renderer->endIdx = -1;
        renderer->cmn.prepare = esguiChartSeries2dBarRendererPrepare;
        renderer->cmn.draw = esguiChartSeries2dBarRendererDraw;
    }
}

void esguiChartSeries2dBarRendererBarWidthSet(EsGuiChartSeries2dRenderer* renderer, size_t width)
{
    if( renderer && width )
        renderer->cmn.lineWidth = width;
}
