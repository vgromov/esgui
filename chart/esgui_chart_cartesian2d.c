#include <esgui/include/esgui.h>
#include <esfwxe/core/rtos_intf.h>
#pragma hdrstop

#include <math.h>
#include <float.h>
#include <esfwxe/utils.h>
#include "esgui_chart_cartesian2d.h"
#include <string.h>

#ifdef __BORLANDC__
# define fabsf fabs
#endif

// renderer - specific api
//
// 
// return canvas point given world point and vice versa. NB! these methods are valid only after prepare is called 
static void esguiChartRendererCartesian2dWorldPointToHdc(const EsGuiChartRenderer* renderer, 
                                                         const void* world, ESGUI_POINT* p)
{
  const EsGuiChartCartesian2dRenderer* r = (const EsGuiChartCartesian2dRenderer*)renderer;
  const EsGuiChartWorldPoint* wp = (const EsGuiChartWorldPoint*)world;

  if( r->invertX )
    p->x = (int)( (r->xIsDt ? (float)(r->worldEdge.x.dt - wp->x.dt) : (r->worldEdge.x.f - wp->x.f)) * 
      r->factorX + (float)renderer->plotRect.org.x);
  else
    p->x = (int)( (r->xIsDt ? (float)(wp->x.dt - r->worldOrg.x.dt) : (wp->x.f - r->worldOrg.x.f)) * 
      r->factorX + (float)renderer->plotRect.org.x);

  if( r->invertY )
    p->y = (int)( (float)renderer->plotRect.edge.y - 
      (r->yIsDt ? (float)(r->worldEdge.y.dt - wp->y.dt) : (r->worldEdge.y.f - wp->y.f)) * 
        r->factorY);
  else
    p->y = (int)( (float)renderer->plotRect.edge.y - 
      (r->yIsDt ? (float)(wp->y.dt - r->worldOrg.y.dt) : (wp->y.f - r->worldOrg.y.f)) * 
        r->factorY);
}

static void esguiChartRendererCartesian2dHdcPointToWorld(const EsGuiChartRenderer* renderer, const ESGUI_POINT* p, void* world)
{
  const EsGuiChartCartesian2dRenderer* r = (const EsGuiChartCartesian2dRenderer*)renderer;
  EsGuiChartWorldPoint* wp = (EsGuiChartWorldPoint*)world;
  float adjustment = (float)(p->x - renderer->plotRect.org.x) * r->_1_factorX;
  if( r->invertX )
  {
    if( r->xIsDt ) 
      wp->x.dt = r->worldEdge.x.dt - (esDT)adjustment;
    else
      wp->x.f = r->worldEdge.x.f - adjustment;
  }
  else
  {
    if( r->xIsDt ) 
      wp->x.dt = (esDT)adjustment + r->worldOrg.x.dt;
    else
      wp->x.f = adjustment + r->worldOrg.x.f;
  }
  
  adjustment = (float)(renderer->plotRect.edge.y - p->y) * r->_1_factorY;  
  if( r->invertY )
  {
    if( r->yIsDt ) 
      wp->y.dt = r->worldEdge.y.dt - (esDT)adjustment;
    else
      wp->y.f = r->worldEdge.y.f - adjustment;
  }
  else
  {
    if( r->yIsDt ) 
      wp->y.dt = (esDT)adjustment + r->worldOrg.y.dt;
    else
      wp->y.f = adjustment + r->worldOrg.y.f;
  }
}

// renderer preparation
static void esguiChartRendererCartesian2dPrepare(EsGuiChartRenderer* renderer, ESGUI_HDC hdc)
{
  EsGuiChartCartesian2dRenderer* r = (EsGuiChartCartesian2dRenderer*)renderer;
  ESGUI_RECT* rct = &r->cmn.plotRect;
  EsGuiChartAxis* axis;
  // set initial plot esguiRectangle, in chart widget's client coordinates
  *rct = r->cmn.chart->wgt->bounds;
  esguiRectOffset_(-rct->org.x, -rct->org.y, rct);
  // axes stuff
  //
  // todo: adjust for axes && chart names
  axis = &r->cmn.chart->axisX;
  r->worldOrg.x = esguiChartAxisMinGet(axis);
  r->worldEdge.x = esguiChartAxisMaxGet(axis);
  r->invertX = esguiChartAxisFlagIsSet(axis, esguiChartAxisBehaviorInverted);
  r->xIsDt = esguiChartAxisFlagIsSet(axis, esguiChartAxisDataHintDateTime);
  // adjust for labels margin and ticks
  rct->edge.y -= (esguiChartAxisFlagIsSet(axis, esguiChartAxisDrawLabels) ? esguiChartAxisLabelMargin : 0) +
    (esguiChartAxisFlagIsSet(axis, esguiChartAxisDrawTicks) ? esguiChartAxisDefMajorTickLen : 0);
  esguiChartRendererMaxLabelExtentGet(renderer, hdc, axis, &r->axisXmaxLabelExt);
  axis = &r->cmn.chart->axisY;
  r->worldOrg.y = esguiChartAxisMinGet(axis);
  r->worldEdge.y = esguiChartAxisMaxGet(axis);    
  r->invertY = esguiChartAxisFlagIsSet(axis, esguiChartAxisBehaviorInverted);
  r->yIsDt = esguiChartAxisFlagIsSet(axis, esguiChartAxisDataHintDateTime);
  // adjust for labels margin and ticks
  rct->org.x += (esguiChartAxisFlagIsSet(axis, esguiChartAxisDrawLabels) ? esguiChartAxisLabelMargin : 0) +
    (esguiChartAxisFlagIsSet(axis, esguiChartAxisDrawTicks) ? esguiChartAxisDefMajorTickLen : 0);
  esguiChartRendererMaxLabelExtentGet(renderer, hdc, axis, &r->axisYmaxLabelExt);
  // adjust plot rect for axes labels
  rct->org.x += MAX(r->axisYmaxLabelExt.x, r->axisXmaxLabelExt.x/2);
  rct->org.y += r->axisYmaxLabelExt.y / 2;
  rct->edge.y -= r->axisXmaxLabelExt.y;
  rct->edge.x -= r->axisXmaxLabelExt.x / 2;
  // calculate cordinate conversion factors
  //
  if( r->xIsDt )
    r->factorX = (float)esguiRectWidth(rct) / (float)(r->worldEdge.x.dt - r->worldOrg.x.dt);
  else
    r->factorX = (float)esguiRectWidth(rct) / (r->worldEdge.x.f - r->worldOrg.x.f);

  if( r->yIsDt )
    r->factorY = (float)esguiRectHeight(rct) / (float)(r->worldEdge.y.dt - r->worldOrg.y.dt);
  else
    r->factorY = (float)esguiRectHeight(rct) / (r->worldEdge.y.f - r->worldOrg.y.f);

  if( fabsf(r->factorX) > FLT_EPSILON )
    r->_1_factorX = 1 / r->factorX;
  else
    r->_1_factorX = 1;

  if( fabsf(r->factorY) > FLT_EPSILON )  
    r->_1_factorY = 1 / r->factorY;
  else
    r->_1_factorY = 1;  
  // convert plot rect to real screen coordinates
  esguiWgtClientToScreen(renderer->chart->wgt, rct);
}

static void esguiChartRendererCartesian2dDrawGrid(const EsGuiChartRenderer* renderer, const EsGuiChartAxis* axis, ESGUI_HDC hdc)
{ 
  EsGuiChartWorldPoint wp;
  ESGUI_POINT pos;
  size_t idx = 0;
  int len = axis->isX ? -esguiRectHeight(&renderer->plotRect) : esguiRectWidth(&renderer->plotRect);
  esguiCanvasFgColorSet(hdc, renderer->clrGrid);
  if( axis->isX )
  {
    wp.x = esguiChartAxisMinGet(axis);
    wp.y = esguiChartAxisMinGet(&renderer->chart->axisY);
  }
  else
  {
    wp.x = esguiChartAxisMinGet(&renderer->chart->axisX);
    wp.y = esguiChartAxisMinGet(axis);
  }

  while(idx < axis->ticksCacheSize)
  {
    if( axis->isX )
    {
      // draw x grid (vertical)
      wp.x = axis->majorTicksCache[idx++];
      esguiChartRendererWorldPointToHdc(renderer, &wp, &pos);
      // todo: check for inversion
      esguiCanvasDrawVLine(hdc, &pos, len);
    }
    else
    {
      // draw y grid (horizontal)
      wp.y = axis->majorTicksCache[idx++];
      esguiChartRendererWorldPointToHdc(renderer, &wp, &pos);
      // todo: check for inversion
      esguiCanvasDrawHLine(hdc, &pos, len);
    }
  }
}

static void esguiChartRendererCartesian2dDrawAxis(const EsGuiChartRenderer* renderer, const EsGuiChartAxis* axis, ESGUI_HDC hdc)
{
  const EsGuiChartCartesian2dRenderer* r = (const EsGuiChartCartesian2dRenderer*)renderer;
  ESGUI_POINT pos;
  // first, draw ticks and labels, if needed
  if( esguiChartAxisFlagIsSet(axis, esguiChartAxisDrawTicks) || 
      esguiChartAxisFlagIsSet(axis, esguiChartAxisDrawLabels) )
  {
    EsGuiChartWorldPoint wp;
    ESGUI_RECT lblRect;
    size_t idx = 0;
    int tickOffs = 0;
    int tickOutsideLen = 0;
    esBL drawLabel = renderer->fmtLabel && esguiChartAxisFlagIsSet(axis, esguiChartAxisDrawLabels);

    if( axis->isX )
    {
      wp.x = esguiChartAxisMinGet(axis);
      wp.y = esguiChartAxisMinGet(&renderer->chart->axisY);
    }
    else
    {
      wp.x = esguiChartAxisMinGet(&renderer->chart->axisX);
      wp.y = esguiChartAxisMinGet(axis);
    }

    esguiRectSet_(0, 0, axis->isX ? r->axisXmaxLabelExt.x : r->axisYmaxLabelExt.x,
      axis->isX ? r->axisXmaxLabelExt.y : r->axisYmaxLabelExt.y, &lblRect);

    if( esguiChartAxisFlagIsSet(axis, esguiChartAxisDrawTicks) )
    {
      if( esguiChartAxisFlagIsSet(axis, esguiChartAxisTickPosCenter) )
        tickOffs = -esguiChartAxisDefMajorTickLen /2;
      else if( esguiChartAxisFlagIsSet(axis, esguiChartAxisTickPosInside) )
        tickOffs = -esguiChartAxisDefMajorTickLen;
        
      tickOutsideLen = esguiChartAxisDefMajorTickLen+tickOffs;  
    }
    
    esguiCanvasFontSet(hdc, renderer->fntAxesLabels);
    while( idx < axis->ticksCacheSize )
    {
      esguiCanvasFgColorSet(hdc, renderer->clrAxes);
      // todo: draw minor ticks
      if( axis->isX )
      {
        // draw x major tick (vertical)
        wp.x = axis->majorTicksCache[idx++];
        esguiChartRendererWorldPointToHdc(renderer, &wp, &pos);
        if( esguiChartAxisFlagIsSet(axis, esguiChartAxisDrawTicks) )
        {
          pos.y += tickOffs;
          esguiCanvasDrawVLine(hdc, &pos, esguiChartAxisDefMajorTickLen);
        }        
        // position label rect
        if( drawLabel )
          esguiRectCenterSet_( pos.x,
            pos.y + r->axisXmaxLabelExt.y/2 + esguiChartAxisLabelMargin + tickOutsideLen, &lblRect );
      }
      else
      {
        // draw y major tick (horizontal)
        wp.y = axis->majorTicksCache[idx++];
        esguiChartRendererWorldPointToHdc(renderer, &wp, &pos);
        if( esguiChartAxisFlagIsSet(axis, esguiChartAxisDrawTicks) )
        {        
          pos.x += tickOffs;  
          esguiCanvasDrawHLine(hdc, &pos, -esguiChartAxisDefMajorTickLen);
        }
        // position label rect
        if( drawLabel )
          esguiRectCenterSet_( pos.x - r->axisYmaxLabelExt.x/2 - esguiChartAxisLabelMargin - tickOutsideLen,
            pos.y, &lblRect );
      }  
      
      // draw label
      if( drawLabel )
      {
        renderer->fmtLabel(renderer, axis, esguiStrBuffGet(), ESGUI_BUFF_LEN,
          axis->isX ? wp.x : wp.y );
        esguiCanvasFgColorSet(hdc, renderer->clrAxesLabels);
        if( axis->isX )
          esguiCanvasDrawStringRect(
            hdc,
            &lblRect,
            esguiStrBuffGet(),
            ESGUI_TAB_WIDTH_STD,
            ESGUI_HTA_CENTER|ESGUI_VTA_CENTER|ESGUI_TWRAP
          );
        else
          esguiCanvasDrawStringRect(
            hdc,
            &lblRect,
            esguiStrBuffGet(),
            ESGUI_TAB_WIDTH_STD,
            ESGUI_HTA_RIGHT|ESGUI_VTA_CENTER|ESGUI_TWRAP
          );
      }                
    }
  }
  
  // draw axis itself
  esguiCanvasFgColorSet(hdc, renderer->clrAxes);
  if( axis->isX )
  {
    pos = esguiRectBottomLeftGet(&renderer->plotRect);
    esguiCanvasDrawHLine(hdc, &pos, esguiRectWidth(&renderer->plotRect));
  }
  else
    esguiCanvasDrawVLine(hdc, &renderer->plotRect.org, esguiRectHeight(&renderer->plotRect));
}

static void esguiChartRendererCartesian2dDrawPlotArea(const EsGuiChartRenderer* renderer, ESGUI_HDC hdc)
{
  if( ES_BIT_IS_SET(renderer->chart->flags, esguiChartFlagDrawPlotArea) )
  {
    esguiCanvasBgColorSet(hdc, renderer->clrPlotArea);
    esguiCanvasFillRect(hdc, &renderer->plotRect);
  }

  if( ES_BIT_IS_SET(renderer->chart->flags, esguiChartFlagDrawPlotBorder) )
  {
    esguiCanvasFgColorSet(hdc, renderer->clrPlotBorder);
    esguiCanvasDrawRect(hdc, &renderer->plotRect);
  }
}

static void esguiChartRendererCartesian2dDrawChartArea(const EsGuiChartRenderer* renderer, ESGUI_HDC hdc,
                                                       const ESGUI_RECT* rect)
{
  if( ES_BIT_IS_SET(renderer->chart->flags, esguiChartFlagDrawChartArea) )
  {
    esguiCanvasBgColorSet(hdc, renderer->clrChartArea);
    esguiCanvasFillRect(hdc, rect);
  }

  if( ES_BIT_IS_SET(renderer->chart->flags, esguiChartFlagDrawChartBorder) )
  {
    esguiCanvasFgColorSet(hdc, renderer->clrChartBorder);
    esguiCanvasDrawRect(hdc, rect);
  }
}

// return world bounds along specified axis
static void esguiChartCartesian2dWorldRangeGet(const EsGuiChartRenderer* renderer, const EsGuiChartAxis* axis, void* wp)
{
  const EsGuiChartCartesian2dRenderer* r = (const EsGuiChartCartesian2dRenderer*)renderer;
  EsGuiChartWorldPoint* world = (EsGuiChartWorldPoint*)wp;
  if( renderer && axis && wp )
  {
    if( axis->isX )
    {
      world->x = r->worldOrg.x;
      world->y = r->worldEdge.x;
    }
    else
    {
      world->x = r->worldOrg.y;
      world->y = r->worldEdge.y;
    }
  }
}

// creation internally sets-up necessary axis and plot handlers for 2D cartesian dataspace
ESGUI_WIDGET* esguiChartCartesian2dCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, EsGuiChartData* chartData)
{
  ESGUI_WIDGET* result = 0;
  EsGuiChartCartesian2dRenderer* renderer = (EsGuiChartCartesian2dRenderer*)esguiWgtMalloc( sizeof(EsGuiChartCartesian2dRenderer) );
  if( renderer )
  {
    memset(renderer, 0, sizeof(EsGuiChartCartesian2dRenderer));
    // set-up cartesian 2d renderer api
    renderer->cmn.prepare = esguiChartRendererCartesian2dPrepare;
    renderer->cmn.worldRangeGet = esguiChartCartesian2dWorldRangeGet;
    renderer->cmn.worldPointToHdc = esguiChartRendererCartesian2dWorldPointToHdc;
    renderer->cmn.canvasPointToWorld = esguiChartRendererCartesian2dHdcPointToWorld;
    renderer->cmn.drawGrid = esguiChartRendererCartesian2dDrawGrid;
    renderer->cmn.drawAxis = esguiChartRendererCartesian2dDrawAxis;
    renderer->cmn.drawChartArea = esguiChartRendererCartesian2dDrawChartArea;
    renderer->cmn.drawPlotArea = esguiChartRendererCartesian2dDrawPlotArea;
    // create chart object
    result = esguiChartCreate(
      root,
      bounds,
      (EsGuiChartRenderer*)renderer,
      chartData
    );

    if( !result )
      esguiWgtFree(renderer);
  }

  return result;
}
