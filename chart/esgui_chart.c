#include <esgui/include/esgui.h>

#include <math.h>
#include <float.h>
#include <esfwxe/utils.h>
#include "esgui_chart.h"
#include <string.h>

#ifdef __arm__
#  ifndef esIsInf
#    define esIsInf      isinf
#  endif

#  ifndef esFinite
#    define esFinite      isfinite
#  endif

#  ifndef esIsNan
#    define esIsNan      isnan
#  endif

#elif defined(_MSC_VER)

#  ifndef esIsInf
#    define esIsInf(x)      (0==_finite(x))
#  endif

#  ifndef esFinite
#    define esFinite      _finite
#  endif

#  ifndef esIsNan
#    define esIsNan      _isnan
#  endif

#elif defined(_MSC_VER)

#  ifndef esIsInf
#    define esIsInf(x)      (0==_finite(x))
#  endif

#  ifndef esFinite
#    define esFinite      _finite
#  endif

#  ifndef esIsNan
#    define esIsNan      _isnan
#  endif

#elif defined(__BORLANDC__)

#  ifndef esFinite
#    define esFinite      _finite
#  endif

#  ifndef esIsInf
#    define esIsInf(x)      (0==_finite(x))
#  endif

#  ifndef esIsNan
#    define esIsNan      _isnan
#  endif

# define fabsf  fabs
# define log10f log10

#ifdef __cplusplus
  extern "C" {
#endif

int es_finite(double d);
int es_isnan(double d);

#endif

const EsGuiChartDataValue c_esguiChartDataValueZero = {0};

// init data values either from float of esDT
EsGuiChartDataValue esguiChartDataValueFromFloat(float f)
{
  EsGuiChartDataValue result;
  result.f = f;
  return result;
}

EsGuiChartDataValue esguiChartDataValueFromDt(esDT dt)
{
  EsGuiChartDataValue result;
  result.dt = dt;
  return result;
}

// calculate humanized value
//
//calculate "humanized" value from existing one
static float esguiChartCalcHumanizedValueFloat( float val )
{
  if( val == 0.f || esIsNan(val) || esIsInf(val) )
    return 0.f;
  else if( val >= 10.f )
    return 10.f * esguiChartCalcHumanizedValueFloat( val * 0.1f );
  else if( val < 1.f )
    return 0.1f * esguiChartCalcHumanizedValueFloat( val * 10.f );
  else if( val < 2.f )
    return 2.f;
  else if( val < 5.f )
    return 5.f; 
  else 
    return 10.f;
}

//predefined datetime step values
static const esDT c_esguiChartDateTimeSteps[] =
{
  //seconds
  IMMEDIATE_INT64( 1000 ),
  IMMEDIATE_INT64( 5*1000 ),
  IMMEDIATE_INT64( 10*1000 ),
  IMMEDIATE_INT64( 15*1000 ),
  IMMEDIATE_INT64( 30*1000 ),
  //minutes
  IMMEDIATE_INT64( 60*1000 ),
  IMMEDIATE_INT64( 10*60*1000 ),
  IMMEDIATE_INT64( 15*60*1000 ),
  IMMEDIATE_INT64( 30*60*1000 ),
  //hours
  IMMEDIATE_INT64( 60*60*1000 ),
  IMMEDIATE_INT64( 2*60*60*1000 ),
  IMMEDIATE_INT64( 6*60*60*1000 ),
  IMMEDIATE_INT64( 12*60*60*1000 ),
  //days
  IMMEDIATE_INT64( 24*60*60*1000 ),
  IMMEDIATE_INT64( 2*24*60*60*1000 ),
  IMMEDIATE_INT64( 3*24*60*60*1000 ),
  IMMEDIATE_INT64( 7*24*60*60*1000 ),
  IMMEDIATE_INT64( 15*24*60*60*1000 ),
  //months
  IMMEDIATE_INT64( 30*24*60*60*1000 ),
  IMMEDIATE_INT64( 2*30*24*60*60*1000 ),
  IMMEDIATE_INT64( 3*30*24*60*60*1000 ),
  IMMEDIATE_INT64( 4*30*24*60*60*1000 ),
  IMMEDIATE_INT64( 182*24*60*60*1000 ),
  //years
  IMMEDIATE_INT64( 365*24*60*60*1000 )
};

static esDT esguiChartCalcHumanizedValueDateTime( esDT val )
{
  size_t idx = CONST_ARRAY_COUNT(c_esguiChartDateTimeSteps)-1;
  for( ; idx != 0; --idx )
  {
    if( val >= c_esguiChartDateTimeSteps[idx] )
      return c_esguiChartDateTimeSteps[idx];
  }

  return c_esguiChartDateTimeSteps[0];
}

// calculate the value of src, rounded to the nearest value of the sample order
float esguiChartRoundToNearestFloat( float src, float sample, bool up )
{
  float powOf10;
  float intPart;
  float fracPart;

  // inverse rounding direction if sample&source have different signs
  if( (src < 0.f && sample > 0.f) ||
      (src > 0.f && sample < 0.f) )
    up = !up;

  //further work is with absolute sample value
  sample = fabsf(sample);
  // if sample is 0, just round src to int
  if( sample < FLT_EPSILON )
    return (float)(int)src;

  powOf10 = log10f(sample);
  // round powOf10 to nearest int
  powOf10 = utilsRoundf( powOf10 );
  
  if( powOf10 >= 0.f )
    powOf10 -= 1.f;
  else
    powOf10 += 1.f;

  powOf10 = powf(10.f, powOf10);
  src /= powOf10;
  intPart = (float)(int)src;
  fracPart = src-intPart;
  fracPart = fabsf( fracPart * powOf10 );

  if( fracPart < sample )
  {
    if( up && (fracPart != 0.f))
      fracPart = sample;
    else
      fracPart = 0.f;
  }
  else if( fracPart > sample )
  {
    float samplesInFrac = (float)(int)(fracPart / sample);
    if( up )
      fracPart = (samplesInFrac + 1.f) * sample;
    else
      fracPart = samplesInFrac * sample;
  }
  
  //return result, copying the integral part sign to the fractional one
  if( (intPart < 0.f && fracPart > 0.f) ||
      (intPart > 0.f && fracPart < 0.f) )
    fracPart = -fracPart;
  return (intPart + fracPart / powOf10) * powOf10;
}

// generic data point compare operators 
// returned are -1 if _1 is less than _2,
// 0 if equal, and 1, if _1 is greater than _2
int esguiChartDataValueCompare(const EsGuiChartDataValue* _1, const EsGuiChartDataValue* _2, bool isDt)
{
  if( (isDt ? _1->dt < _2->dt : _1->f < _2->f) )
    return -1;
  else if( (isDt ? _1->dt == _2->dt : _1->f == _2->f) )
    return 0;
  
  return 1;
}

bool esguiChartDataValueIsInVicinity(const EsGuiChartDataValue* baseVal, const EsGuiChartDataValue* val,
                                     float tolerance, bool isDt)
{
  return (isDt ? (fabsf((float)val->dt-(float)baseVal->dt) <= fabsf(tolerance)) : 
    (fabsf(val->f-baseVal->f) <= fabsf(tolerance)) );
}

// series api
//
static bool esguiChartSeriesCanDraw(const EsGuiChartSeries* series)
{
  return series && 
    series->renderer &&
    !esguiChartSeriesIsEmpty(series) &&
    ES_BIT_MASK_MATCH(
      series->flags,
      esguiChartSeriesDrawLine|
      esguiChartSeriesDrawArea|
      esguiChartSeriesDrawPointLine|
      esguiChartSeriesDrawPointArea
    );
}

bool esguiChartSeriesIsEmpty(const EsGuiChartSeries* series)
{
  if(series && series->sizeGet)
    return 0 == series->sizeGet(series);
    
  return TRUE;
}

size_t esguiChartSeriesSizeGet(const EsGuiChartSeries* series)
{
  if(series && series->sizeGet)
    return series->sizeGet(series);
    
  return 0;
}

int esguiChartSeriesMinXIdxGet(const EsGuiChartSeries* series)
{
  if( series && series->getMinXIdx )
    return series->getMinXIdx(series);
    
  return -1;
}

int esguiChartSeriesMaxXIdxGet(const EsGuiChartSeries* series)
{
  if( series && series->getMaxXIdx )
    return series->getMaxXIdx(series);
    
  return -1;
}

int esguiChartSeriesMinYIdxGet(const EsGuiChartSeries* series)
{
  if( series && series->getMinYIdx )
    return series->getMinYIdx(series);
    
  return -1;
}

int esguiChartSeriesMaxYIdxGet(const EsGuiChartSeries* series)
{
  if( series && series->getMaxYIdx )
    return series->getMaxYIdx(series);
    
  return -1;
}

bool esguiChartSeriesPointGetAt(const EsGuiChartSeries* series, size_t idx, EsGuiChartWorldPoint* wp)
{
  if( series && wp && 
      idx < esguiChartSeriesSizeGet(series) && 
      series->pointGetAt )
    return series->pointGetAt(series, idx, wp);

  return FALSE;
}

void esguiChartSeriesCleanup(const EsGuiChartSeries* series)
{
  if( series && series->cleanup )
    series->cleanup(series);
}

void esguiChartSeriesInit(EsGuiChartSeries* series, void* data)
{
  if(series && data)
  {
    memset(series, 0, sizeof(EsGuiChartSeries));
    series->data = data;
    series->flags = 0;
  }
}

void esguiChartSeriesFlagsSet(EsGuiChartSeries* series, esU32 flags)
{
  if( series && series->flags != flags )
  {
    series->flags = flags;
    if( series->chart )
    {
      if( ES_BIT_MASK_MATCH(flags, esguiChartSeriesDrawPointLine|esguiChartSeriesDrawPointArea) )
        ES_BIT_SET(series->chart->flags, esguiChartFlagNeedRecalc);

      ES_BIT_SET(series->chart->wgt->flags, esguiWgtfInvalidFg);
      ES_BIT_SET(series->chart->wgt->flags, esguiWgtfInvalidBg);
    }
  }
}

// basic renderer initializer
void esguiChartSeriesRendererInit(EsGuiChartSeriesRenderer* renderer)
{
  if( renderer )
  {
    memset(renderer, 0, sizeof(EsGuiChartSeriesRenderer));
    renderer->lineWidth = 1;
    renderer->pointSize = esguiChartSeriesRendererDefPointSize;
  }
}

void esguiChartSeriesRendererSet(EsGuiChartSeries* series, EsGuiChartSeriesRenderer* renderer)
{
  if( series && renderer )
  {
    if( series->renderer )
    {
      series->renderer->series = 0;
      series->renderer = 0;
    }
    series->renderer = renderer;
    renderer->series = series;
    esguiChartSeriesFlagsSet(series,  esguiChartSeriesNeedRecalc|
                                      esguiChartSeriesDrawLine);
  }
}

void esguiChartSeriesInvalidate(EsGuiChartSeries* series, bool needRecalc)
{
  if( series )
  {
    if( needRecalc )
      series->flags |= esguiChartSeriesNeedRecalc;

    // if we need to recalc series, we should also recalc parenting chart
    esguiChartInvalidate( series->chart, needRecalc, 0 );
  }
}

// chart axis
//
// internal standard api
//
static void esguiChartAxisFlagsSet_internal(EsGuiChartAxis* axis, esU32 flags)
{
  axis->flags = (flags & (esguiChartAxisDataHintMask|
                          esguiChartAxisTickPosMask|
                          esguiChartAxisBehaviorMask|
                          esguiChartAxisDrawMask));
  ES_BIT_SET(axis->chart->flags, esguiChartFlagNeedRecalc);
}

// internal service - get minimum over all chart series, attached to this axis
static EsGuiChartDataValue esguiChartAxisDataMinGet(const EsGuiChartAxis* axis)
{
  EsGuiChartDataValue result = esguiChartDataValueFromDt(0);
  EsGuiChartWorldPoint tmp;
  const EsGuiChartSeries* series = 0;
  bool isDt = esguiChartAxisFlagIsSet(axis, esguiChartAxisDataHintDateTime);
  size_t idx = 0;  
  // find the first drawable series attached to this axis
  while( idx < axis->chart->seriesCnt )
  {
    series = axis->chart->series[idx++];
    if( esguiChartSeriesCanDraw(series) &&
        (axis == series->axisX || axis == series->axisY) )
    {
      if( axis->isX )
      {
        if( esguiChartSeriesPointGetAt( series, esguiChartSeriesMinXIdxGet(series), &tmp ) )
          result = tmp.x;
      }
      else
      {
        if( esguiChartSeriesPointGetAt( series, esguiChartSeriesMinYIdxGet(series), &tmp ) )
          result = tmp.y;
      }
      break;
    }
  }
  // calc min over all series also attached to this axis
  for( ; idx < axis->chart->seriesCnt; ++idx )
  {
    series = axis->chart->series[idx];
    if( esguiChartSeriesCanDraw(series) && 
        (axis == series->axisX || axis == series->axisY) )
    {
      if( axis->isX )
      {
        if( esguiChartSeriesPointGetAt( series, esguiChartSeriesMinXIdxGet(series), &tmp ) )
        {
          if( isDt )
            result.dt = MIN( tmp.x.dt, result.dt );
          else
            result.f = MIN( tmp.x.f, result.f );
        }
      }
      else
      {
        if( esguiChartSeriesPointGetAt( series, esguiChartSeriesMinYIdxGet(series), &tmp ) )
        {
          if( isDt )
            result.dt = MIN( tmp.y.dt, result.dt );
          else
            result.f = MIN( tmp.y.f, result.f );
        }
      }
    }
  }
  
  return result;
}

// internal service - get maximum over all chart series, attached to this axis
static EsGuiChartDataValue esguiChartAxisDataMaxGet(const EsGuiChartAxis* axis)
{
  EsGuiChartDataValue result;
  EsGuiChartWorldPoint tmp;
  const EsGuiChartSeries* series = 0;
  bool isDt = esguiChartAxisFlagIsSet(axis, esguiChartAxisDataHintDateTime);
  size_t idx = 0;  
  result.dt = 0;
  // find the first series attached to this axis
  while( idx < axis->chart->seriesCnt )
  {
    series = axis->chart->series[idx++];
    if( axis == series->axisX || axis == series->axisY )
    {
      if( axis->isX )
      {
        if( esguiChartSeriesPointGetAt( series, esguiChartSeriesMaxXIdxGet(series), &tmp ) )
          result = tmp.x;
      }
      else
      {
        if( esguiChartSeriesPointGetAt( series, esguiChartSeriesMaxYIdxGet(series), &tmp ) )
          result = tmp.y;
      }    
      break;
    }
  }
  // calc min over all series also attached to this axis
  for( ; idx < axis->chart->seriesCnt; ++idx )
  {
    series = axis->chart->series[idx];
    if( axis == series->axisX || axis == series->axisY )
    {
      if( axis->isX )
      {
        if( esguiChartSeriesPointGetAt( series, esguiChartSeriesMaxXIdxGet(series), &tmp ) )
        {
          if( isDt )
            result.dt = MAX( tmp.x.dt, result.dt );
          else
            result.f = MAX( tmp.x.f, result.f );
        }
      }
      else
      {
        if( esguiChartSeriesPointGetAt( series, esguiChartSeriesMaxYIdxGet(series), &tmp ) )
        {
          if( isDt )
            result.dt = MAX( tmp.y.dt, result.dt );
          else
            result.f = MAX( tmp.y.f, result.f );
        }
      }
    }
  }
  
  return result;
}

static void esguiChartAxisMajorTicksCountSet_internal(EsGuiChartAxis* axis, size_t cnt)
{
  if( cnt <= esguiChartAxisMaxMajorTicksCnt &&
      cnt >= esguiChartAxisMinMajorTicksCnt )
  {
    axis->flags &= ~esguiChartAxisBehaviorAutoTicks;
    axis->ticksCacheSize = cnt;
    ES_BIT_SET(axis->chart->flags, esguiChartFlagNeedRecalc);    
  }
}

static void esguiChartAxisMinorTicksPerMajorTickSet_internal(EsGuiChartAxis* axis, size_t cnt)
{
  if( cnt != axis->minorTicksPreMajotTickCnt &&
      cnt <= esguiChartAxisMaxMinorTickPerMajorTick )
  {
    axis->minorTicksPreMajotTickCnt = cnt;
    ES_BIT_SET(axis->chart->flags, esguiChartFlagNeedRecalc);    
  }
}

void esguiChartAxisMajorTickStepSet(EsGuiChartAxis* axis, EsGuiChartDataValue val)
{
  if( axis && axis->chart && 
      esguiChartDataValueCompare(&val, &c_esguiChartDataValueZero, 
        esguiChartAxisFlagIsSet(axis, esguiChartAxisDataHintDateTime)) > 0)
  {
    axis->majorTickStep = val;
    ES_BIT_CLR(axis->flags, esguiChartAxisBehaviorAutoTicks);
    axis->autoCalcMajorTickStep = FALSE;
    ES_BIT_SET(axis->chart->flags, esguiChartFlagNeedRecalc);
  }
}

// axis minimax and ticks recalculation
void esguiChartAxisRecalcData(EsGuiChartAxis* axis)
{  
  EsGuiChartDataValue curTickVal;
  bool isDt = esguiChartAxisFlagIsSet(axis, esguiChartAxisDataHintDateTime);
  size_t majorTickIdx = 0;
  // recalc range if auto min && max calculation flag is set
  if( axis->flags & esguiChartAxisBehaviorAutoMin )
    axis->min = esguiChartAxisDataMinGet(axis);

  if( axis->flags & esguiChartAxisBehaviorAutoMax )
    axis->max = esguiChartAxisDataMaxGet(axis);

  // prevent minimax zeroing
  if( 0 == esguiChartDataValueCompare( &axis->min, &axis->max, isDt) )
  {
    if( isDt )
      axis->max.dt = axis->min.dt + IMMEDIATE_INT64(1000);
    else
      axis->max.f = axis->min.f + 1.f;
  }

  // estimate major ticks count when using current major tick step
  if( !esguiChartAxisFlagIsSet(axis, esguiChartAxisBehaviorAutoTicks) &&
      !axis->autoCalcMajorTickStep ) 
  {
    size_t majorTicksCnt;
    if( isDt )
      majorTicksCnt = (size_t)((axis->max.dt - axis->min.dt) / axis->majorTickStep.dt);
    else
      majorTicksCnt = (size_t)utilsRoundf((axis->max.f - axis->min.f) / axis->majorTickStep.f);

    // if estimated major ticks count is less than esguiChartAxisMaxMajorTicksCnt,
    // return to the automated ticks calculation mode
    if( majorTicksCnt > esguiChartAxisMaxMajorTicksCnt )
      axis->flags |= esguiChartAxisBehaviorAutoTicks;
    else
      axis->ticksCacheSize = majorTicksCnt;
  }

  //recalc major & minor tick steps
  if( esguiChartAxisFlagIsSet(axis, esguiChartAxisBehaviorAutoTicks) )
  {
    axis->autoCalcMajorTickStep = TRUE;
    axis->ticksCacheSize = esguiChartAxisDefMajorTicksCnt;
  }

  if( axis->autoCalcMajorTickStep )
  {
    if( isDt )
      axis->majorTickStep.dt = esguiChartCalcHumanizedValueDateTime(
        (axis->max.dt - axis->min.dt) / (esDT)(axis->ticksCacheSize - 1));
    else
      axis->majorTickStep.f = esguiChartCalcHumanizedValueFloat(
        (axis->max.f - axis->min.f) / (float)(axis->ticksCacheSize - 1));  
  }

  if( isDt )
    axis->minorTickStep.dt = 
      axis->majorTickStep.dt / (esDT)(axis->minorTicksPreMajotTickCnt + 1);
  else
    axis->minorTickStep.f = 
      axis->majorTickStep.f / (float)(axis->minorTicksPreMajotTickCnt + 1);

  // calculate starting major tick value
  if( isDt )
    curTickVal.dt = (esDT)esguiChartRoundToNearestFloat((float)axis->min.dt, (float)axis->majorTickStep.dt, TRUE);
  else
    curTickVal.f = esguiChartRoundToNearestFloat(axis->min.f, axis->majorTickStep.f, TRUE);

  //int iMinorTickCnt = 0;
  ////fill in minor ticks that go before any major ones
  //double dNewTickVal = dCurTickVal - m_MinorTicksPerMajorTick * dMinorTickStep;
  //while( (iMinorTickCnt < m_MinorTicksPerMajorTick) && (dNewTickVal < m_Max) )
  //{
  //  if( dNewTickVal > m_Min )
  //    m_MinorTicks.push_back(dNewTickVal);

  //  dNewTickVal += dMinorTickStep;
  //  ++iMinorTickCnt;
  //}  

  while( esguiChartDataValueCompare(&curTickVal, &axis->max, isDt) <= 0 && majorTickIdx < axis->ticksCacheSize )
  {
    EsGuiChartDataValue newTickVal;
    
    axis->majorTicksCache[majorTickIdx++] = curTickVal;
    if( isDt )
      newTickVal.dt = curTickVal.dt + axis->majorTickStep.dt;
    else
      newTickVal.f = curTickVal.f + axis->majorTickStep.f;
    curTickVal = newTickVal;
    
    //insert minor ticks
    //dCurTickVal += dMinorTickStep;
    //iMinorTickCnt = 0;
    //while( (iMinorTickCnt < m_MinorTicksPerMajorTick) && (dCurTickVal < m_Max) )
    //{
    //  m_MinorTicks.push_back(dCurTickVal);
    //  dCurTickVal += dMinorTickStep;
    //  ++iMinorTickCnt;
    //}
  }

  // finally adjust ticks cache size
  axis->ticksCacheSize = majorTickIdx;
}

// initialize chart axis
static void esguiChartAxisInit(EsGuiChartData* chart, EsGuiChartAxis* axis, bool isX)
{
  axis->flags = esguiChartAxisDataHintGeneric|
                esguiChartAxisTickPosOutside|
                esguiChartAxisBehaviorAutoMin|
                esguiChartAxisBehaviorAutoMax|
                esguiChartAxisBehaviorAutoTicks|
                esguiChartAxisDrawTicks|
                esguiChartAxisDrawGrid|
                esguiChartAxisDrawLabels;
  // reference to chart
  axis->chart = chart;
  axis->ticksCacheSize = esguiChartAxisDefMajorTicksCnt;
  axis->minorTicksPreMajotTickCnt = esguiChartAxisDefMinorTickPerMajorTick;
  axis->isX = isX;
  axis->autoCalcMajorTickStep = TRUE;
  axis->min.f = 0.f;
  axis->max.f = 1.f;
  // set-up api
  axis->flagsSet = esguiChartAxisFlagsSet_internal;
  axis->majorTicksCountSet = esguiChartAxisMajorTicksCountSet_internal;
  axis->minorTicksPerMajorTickSet = esguiChartAxisMinorTicksPerMajorTickSet_internal;
}

// axis api
//
void esguiChartAxisFlagsSet(EsGuiChartAxis* axis, esU32 flags)
{
  if( axis && axis->flagsSet )
    axis->flagsSet(axis, flags);
}

void esguiChartAxisFlagsClear(EsGuiChartAxis* axis, esU32 flags)
{
  if( axis && axis->flagsSet )
  {
    esU32 tmp = axis->flags;
    ES_BIT_CLR(tmp, flags);
    axis->flagsSet(axis, tmp);
  }
}

EsGuiChartDataValue esguiChartAxisMinGet(const EsGuiChartAxis* axis)
{
  EsGuiChartDataValue result = c_esguiChartDataValueZero;
  
  if( axis )
  {
    if( esguiChartAxisFlagIsSet(axis, esguiChartAxisBehaviorAutoMin) )
      result = esguiChartAxisDataMinGet(axis);  
    else
      result = axis->min;
  }

  return result;
}

EsGuiChartDataValue esguiChartAxisMaxGet(const EsGuiChartAxis* axis)
{
  EsGuiChartDataValue result = c_esguiChartDataValueZero;

  if(axis)
  {
    if( axis->flags & esguiChartAxisBehaviorAutoMax )
      result = esguiChartAxisDataMaxGet(axis);
    else
      result = axis->max;  
  }
    
  return result;
}

void esguiChartAxisMinSet(EsGuiChartAxis* axis, EsGuiChartDataValue val)
{
  if(axis)
  {
    ES_BIT_CLR(axis->flags, esguiChartAxisBehaviorAutoMin);
    axis->min = val;
    ES_BIT_SET(axis->chart->flags, esguiChartFlagNeedRecalc);
  }
}

void esguiChartAxisMaxSet(EsGuiChartAxis* axis, EsGuiChartDataValue val)
{
  if(axis)
  {
    ES_BIT_CLR(axis->flags, esguiChartAxisBehaviorAutoMax);
    axis->max = val;
    ES_BIT_SET(axis->chart->flags, esguiChartFlagNeedRecalc);
  }
}

void esguiChartAxisRangeSet(EsGuiChartAxis* axis, EsGuiChartDataValue min, EsGuiChartDataValue max)
{
  if( axis && esguiChartDataValueCompare(&min, &max, 
      esguiChartAxisFlagIsSet(axis, esguiChartAxisDataHintDateTime)) < 0 )
  {
    ES_BIT_CLR(axis->flags, esguiChartAxisBehaviorAutoMax|
                            esguiChartAxisBehaviorAutoMin);
    axis->min = min;
    axis->max = max;
    ES_BIT_SET(axis->chart->flags, esguiChartFlagNeedRecalc);
  }
}

void esguiChartAxisMajorTicksCountSet(EsGuiChartAxis* axis, size_t count)
{
  if(axis && axis->majorTicksCountSet)
    axis->majorTicksCountSet(axis, count);    
}

void esguiChartAxisMinorTicksPerMajorTickSet(EsGuiChartAxis* axis, size_t count)
{
  if(axis && axis->minorTicksPerMajorTickSet)
    axis->minorTicksPerMajorTickSet(axis, count);
}

void esguiChartAxisCleanup(EsGuiChartAxis* axis)
{
  if(axis && axis->cleanup)
    axis->cleanup(axis);
}

// chart API
//
// chart invalidator. if needRecalc is TRUE, chart will recalc is internals on next redraw
void esguiChartInvalidate(EsGuiChartData* chart, bool needRecalc, const ESGUI_RECT* rect )
{
  if( chart )
  {
    if(needRecalc) 
      ES_BIT_SET(chart->flags, esguiChartFlagNeedRecalc);
    else
      ES_BIT_CLR(chart->flags, esguiChartFlagNeedRecalc);
    esguiWgtInvalidate(chart->wgt, rect, TRUE);
  }
}

void esguiChartRendererSet(EsGuiChartData* chart, EsGuiChartRenderer* renderer)
{
  if( chart )
  {
    // cleanup the old renderer if needed
    if( chart->renderer )
      esguiWgtFree( chart->renderer );
      
    chart->renderer = renderer;
    if( renderer )
      renderer->chart = chart;
  }
}

// flags setter
void esguiChartFlagsSet(EsGuiChartData* chart, esU32 flags)
{
  if(chart)
  {  
    // preserve need recalc flag
    if( ES_BIT_IS_SET(chart->flags, esguiChartFlagNeedRecalc) )
      ES_BIT_SET(flags, esguiChartFlagNeedRecalc);

    chart->flags = flags;
    if(chart->wgt)
      ES_BIT_SET(chart->wgt->flags, esguiWgtfInvalidFg|esguiWgtfInvalidBg);
  }
}

void esguiChartFlagsClear(EsGuiChartData* chart, esU32 flags)
{
  if(chart)
  {  
    // preserve need recalc flag
    if( ES_BIT_IS_SET(chart->flags, esguiChartFlagNeedRecalc) )
      ES_BIT_CLR(flags, esguiChartFlagNeedRecalc);

    ES_BIT_CLR(chart->flags, flags);
    if(chart->wgt)
      ES_BIT_SET(chart->wgt->flags, esguiWgtfInvalidFg|esguiWgtfInvalidBg);
  }
}

void esguiChartSeriesAttach(EsGuiChartData* chart, EsGuiChartSeries* series)
{
  if(chart && series && chart->seriesCnt < esguiChartMaxSeriesCnt)
  {
    chart->series[chart->seriesCnt++] = series;
    series->chart = chart;
    series->axisX = &chart->axisX;
    series->axisY = &chart->axisY;
  }
}

void esguiChartSeriesDetach(EsGuiChartData* chart, EsGuiChartSeries* series)
{
  if( chart && series )
  {
    size_t idx = 0;
    // find corresponding series slot
    while(idx < chart->seriesCnt)
    {
      if(series == chart->series[idx++])
      {
        chart->series[idx-1] = 0;
        series->chart = 0;
        series->axisX = 0;
        series->axisY = 0;
        --chart->seriesCnt;
        break;
      }
    }
    // compact remaining series in series array
    while(idx <= chart->seriesCnt)
    {
      chart->series[idx-1] = chart->series[idx];
      ++idx;
    }
  }
}

// chart object cleanup
void esguiChartCleanup(EsGuiChartData* chart)
{
  if( chart )
  {
    // cleanup renderer
    if( chart->renderer )
    {
      esguiWgtFree(chart->renderer);
      chart->renderer = 0;
    }
  }
}

static void esguiChartOnPaintBg(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender,
  const ESGUI_RECT* rect)
{
  if( This && hdc )
  {
    EsGuiChartData* chart = (EsGuiChartData*)This->data;
    if(chart && chart->renderer && chart->renderer->drawChartArea)
      chart->renderer->drawChartArea(chart->renderer, hdc, rect);
  }
}

// series drawing call
static void esguiChartSeriesDraw(EsGuiChartSeries* series, ESGUI_HDC hdc)
{
  if( esguiChartSeriesCanDraw(series) )
  {
    if( ES_BIT_IS_SET(series->flags, esguiChartSeriesNeedRecalc) ||
        ES_BIT_IS_SET(series->chart->flags, esguiChartFlagNeedRecalc) )
    {
      if( series->renderer->prepare )
        series->renderer->prepare(series->renderer, series->chart->renderer);
      series->flags &= ~esguiChartSeriesNeedRecalc;
    }
    if( series->renderer->draw )
      series->renderer->draw(series->renderer, series->chart->renderer, hdc);
  }
}

// now we have all bounds and coord space prepared
// so allow for chart's children widgets to accommodate their bounds
static void esguiChartRecalculatedNotifyChildren(ESGUI_WIDGET* This)
{
  ESGUI_WIDGET* child = This->firstChild;
  if( !child )
    return;

  EsguiAppEventT* evt = esguiChartRecalculationNotificationEventCreate();

  while( child && !esguiWgtEventIsHandled(evt) )
  {
    if( child->onEvent )
      child->onEvent(child, This, evt);

    child = child->next;
  }

  esguiWgtEventMarkHandled(evt);
  esguiWgtFree(evt);
}

static void esguiChartOnPaint(ESGUI_HDC hdc, ESGUI_WIDGET* This, ESGUI_WIDGET* sender,
  const ESGUI_RECT* rect)
{
  if( This && hdc )
  {
    EsGuiChartData* chart = (EsGuiChartData*)This->data;
    if( chart && chart->renderer )
    {
      EsGuiChartRenderer* renderer = chart->renderer;
      ESGUI_RECT oldClip;
      size_t idx = 0;
      // prepare renderer
      if( ES_BIT_IS_SET(chart->flags, esguiChartFlagNeedRecalc) )
      {
        // calc axes data
        esguiChartAxisRecalcData(&chart->axisX);
        esguiChartAxisRecalcData(&chart->axisY);
        // prepare renderer
        if( renderer->prepare )
          renderer->prepare(renderer, hdc);
          
        // now we have all bounds and coord space prepared
        // so allow for chart's children widgets to accommodate their bounds
        esguiChartRecalculatedNotifyChildren(This);
      }
      // draw plot area
      if( renderer->drawPlotArea )
        renderer->drawPlotArea(renderer, hdc);
      // draw grids
      if( renderer->drawGrid )
      {
        if( chart->axisX.flags & esguiChartAxisDrawGrid )
          renderer->drawGrid(renderer, &chart->axisX, hdc);
        if( chart->axisY.flags & esguiChartAxisDrawGrid )
          renderer->drawGrid(renderer, &chart->axisY, hdc);
      }
      // draw data series. clip series to the plot rect
      oldClip = *esguiCanvasGetClipRect(hdc);
      esguiCanvasSetClipRect(hdc, &renderer->plotRect);
      while(idx < chart->seriesCnt)
        esguiChartSeriesDraw(chart->series[idx++], hdc);
      esguiCanvasSetClipRect(hdc, &oldClip);
      // draw axes
      if( renderer->drawAxis )
      {
        renderer->drawAxis(renderer, &chart->axisX, hdc);
        renderer->drawAxis(renderer, &chart->axisY, hdc);
      }
      // draw extra stuff
      if( renderer->paintExtra  )
        renderer->paintExtra(renderer, hdc);
      // clear need recalc flag
      ES_BIT_CLR(chart->flags, esguiChartFlagNeedRecalc);
    }
  }
}

static void esguiChartOnDeleteData(ESGUI_WIDGET* This)
{
  // chart data are the owner of the renderer object
  if( This && This->data )
  {
    EsGuiChartData* chart = (EsGuiChartData*)This->data;
    esguiChartRendererSet(chart, 0);
  }
}

ESGUI_WIDGET* esguiChartCreate(ESGUI_WIDGET_ROOT* root, const ESGUI_RECT* bounds, EsGuiChartRenderer* renderer, EsGuiChartData* chartData)
{
  ESGUI_WIDGET* result = NULL;
  EsGuiChartData* chart = chartData;
  if( !chart )
  {
    chart = (EsGuiChartData*)esguiWgtMalloc(sizeof(EsGuiChartData));
    // zero-out all data
    if( chart )
      memset(chart, 0, sizeof(EsGuiChartData));
  }

  if( chart && renderer )
  {
    // initialize data members
    esguiChartFlagsSet(chart, esguiChartFlagDrawChartArea|
                              esguiChartFlagDrawChartBorder|
                              esguiChartFlagDrawPlotArea|
                              esguiChartFlagDrawPlotBorder);
    esguiChartRendererSet( chart, renderer );
    esguiChartAxisInit( chart, &chart->axisX, TRUE );
    esguiChartAxisInit( chart, &chart->axisY, FALSE );  
    // create widget
    result = esguiWgtCreate(
      root,
      ESGUI_STATIC_GROUP,
      bounds,
      esguiWgtfDefault
    );

    if(result)
    {
      result->data = chart;
      chart->wgt = result;
      ES_BIT_SET(chart->flags, esguiChartFlagNeedRecalc);
      result->onPaint = esguiChartOnPaint;
      result->onPaintBg = esguiChartOnPaintBg;
      result->onDeleteData = esguiChartOnDeleteData;
    }
    else
      esguiWgtFree(chart);
  }

  return result;
}

void esguiChartRendererMaxLabelExtentGet(EsGuiChartRenderer* renderer, ESGUI_HDC hdc,
  const EsGuiChartAxis* axis, ESGUI_POINT* ext)
{
  if(ext)
    esguiPointZeroSet(ext);

  if(
    renderer &&
    hdc &&
    axis &&
    ext &&
    renderer->fmtLabel &&
    esguiChartAxisFlagIsSet(
      axis,
      esguiChartAxisDrawLabels
    )
  )
  {
    size_t idx = 0;
    while(idx < axis->ticksCacheSize)
    {
      ESGUI_POINT tmp;
      renderer->fmtLabel(renderer, axis, esguiStrBuffGet(), ESGUI_BUFF_LEN, axis->majorTicksCache[idx++]);
      esguiCanvasGetStringExtent(
        hdc,
        renderer->fntAxesLabels,
        esguiStrBuffGet(),
        ESGUI_TAB_WIDTH_STD,
        &tmp
      );

      if( ext->x < tmp.x )
        ext->x = tmp.x;
      if( ext->y < tmp.y )
        ext->y = tmp.y;
    }
  }
}

void esguiChartRendererWorldPointToHdc(const EsGuiChartRenderer* renderer, const void* wp, ESGUI_POINT* p)
{
  if( renderer && wp && p && renderer->worldPointToHdc )
    renderer->worldPointToHdc(renderer, wp, p);
}

void esguiChartRendererHdcPointToWorld(const EsGuiChartRenderer* renderer, const ESGUI_POINT* p, void* wp)
{
  if( renderer && wp && p && renderer->canvasPointToWorld )
    renderer->canvasPointToWorld(renderer, p, wp);
}
