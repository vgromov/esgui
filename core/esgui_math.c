#include <esgui/include/esgui.h>
#include <math.h>
//----------------------------------------------------------------------------------------------

#if defined(__BORLANDC__)
static inline float esgui_modff(float a, float* b)
{
  double tmp;
  double ret = modf(
    (double)a,
    &tmp
  );

  *b = (float)tmp;
  return (float)ret;
}
#else
# define esgui_modff modff
#endif
//----------------------------------------------------------------------------------------------

float esguiMathDegToRad( float deg )
{
  float tmp;
  return ESGUI_PI * (deg > 360.f ? esgui_modff(deg/360.f, &tmp) : deg) / 180.f;
}
//----------------------------------------------------------------------------------------------

float esguiMathRadToDeg( float rad )
{
  float tmp;
  rad = 180.f * rad / ESGUI_PI;
  return (rad > 360.f ? esgui_modff(rad/360.f, &tmp) : rad);
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT esguiMathPointRotate(const ESGUI_POINT* p, float angle, esU32 flags)
{
  ESGUI_POINT result;
  esguiMathPointRotatePtr(
    p, 
    angle,
    flags,
    &result
  );

  return result;
}
//----------------------------------------------------------------------------------------------

void esguiMathPointRotatePtr(const ESGUI_POINT* p, float angle, esU32 flags, ESGUI_POINT* dest)
{
  if( !p || !dest )
    return;
    
  *dest = *p;

  if( !angle )
    return;
    
  float fsin, fcos;
  if(esguiMathFlagRotateAngleInDeg == (flags & esguiMathFlagRotateAngleInDeg)) 
    angle = esguiMathDegToRad(angle);

  if( esguiMathFlagRotateInverseX == (flags & esguiMathFlagRotateInverseX) )
    angle = ESGUI_PI-angle;

  if( esguiMathFlagRotateInverseY == (flags & esguiMathFlagRotateInverseY) )
    angle = -angle;

  if( esguiMathFlagRotateClockwise == (flags & esguiMathFlagRotateClockwise) )
    angle = angle;
    
  fsin = sinf(angle);
  fcos = cosf(angle);

  dest->x = (int)((float)p->x * fcos - (float)p->y * fsin);
  dest->y = (int)((float)p->x * fsin + (float)p->y * fcos);
}
//----------------------------------------------------------------------------------------------

static inline esBL internalCalc_t( int denom, int num, float* t_in, float* t_out )
{
  float t;

  if( denom > 0 ) // candidate to incoming intersection point
  {
    t = (float)num / (float)denom;

    if( t > *t_out )
      return FALSE; // t_in Max > t_out Min: whole segment is outside
    else if( t > *t_in ) // modify and return t_in
      *t_in = t;
  }
  else if( denom < 0 ) // candidate to outgoing intersection point
  {
    t = (float)num / (float)denom;

    if( t < *t_in )
      return FALSE;   // t_in Max > t_out Min: whole segment is outside
    else if( t < *t_out ) // modify and retutn t_out
       *t_out = t;
  }
  else if( num > 0 )
    return FALSE; // segment is parallel to rect's side and lies outside
  
  return TRUE;
}
//----------------------------------------------------------------------------------------------

esBL esguiMathLineIntersect(const ESGUI_POINT* beg, const ESGUI_POINT* end, const ESGUI_RECT* r, ESGUI_POINT* beg1, ESGUI_POINT* end1)
{
  ESGUI_POINT d;
  d.x = end->x - beg->x;
  d.y = end->y - beg->y;
  
  if( beg1 != NULL )
    *beg1 = *beg;

  if( end1 != NULL )
    *end1 = *end;

  if( d.x == 0 && d.y == 0 )
    return esguiPointInRect(beg, r);
  else if( 
    esguiPointInRect(beg, r) && 
    esguiPointInRect(end, r) 
  ) // segment entirely insige rect
    return TRUE;
  else // check possible intersection by Liang-Barsky algoritm
  {
    float t_in = 0.f;
    float t_out = 1.f;

    if( internalCalc_t( d.x, r->org.x - beg->x, &t_in, &t_out ) )
      if( internalCalc_t( -d.x, beg->x - r->edge.x, &t_in, &t_out ) )
        if( internalCalc_t( d.y, r->org.y - beg->y, &t_in, &t_out ) )
          if( internalCalc_t( -d.y, beg->y - r->edge.y, &t_in, &t_out ) )
          {
            if( t_in > 0.f && beg1 != NULL )
            {
              beg1->x = beg->x + (int)((float)d.x * t_in);
              beg1->y = beg->y + (int)((float)d.y * t_in);
            }

            if( t_out < 1.f && end1 != NULL )
            {
              end1->x = beg->x + (int)((float)d.x * t_out);
              end1->y = beg->y + (int)((float)d.y * t_out);
            }

            return TRUE;
          }

    return FALSE;
  }
}
//----------------------------------------------------------------------------------------------
