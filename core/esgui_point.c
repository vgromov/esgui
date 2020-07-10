#include <esgui/include/esgui.h>
#include <esgui/include/esgui_point.h>
#include <math.h>
//----------------------------------------------------------------------------------------------

const ESGUI_POINT c_zeroPoint = {0,0};
//----------------------------------------------------------------------------------------------

void esguiPointZeroSet(ESGUI_POINT* p)
{
  if( p )
    *p = c_zeroPoint;
}
//----------------------------------------------------------------------------------------------

esBL esguiPointIsZero(ESGUI_POINT* p)
{
  return 0 == esguiPointsCompare(
    &c_zeroPoint, 
    p
  );
}
//----------------------------------------------------------------------------------------------

int esguiPointsCompare_(int p1x, int p1y, int p2x, int p2y)
{
  if( p1x == p2x && p1y == p2y )
    return 0;
  else if( (p1x > p2x) || ((p1x == p2x) && (p1y > p2y)) )
    return 1;

  return -1;  
}
//----------------------------------------------------------------------------------------------

int esguiPointsCompare(const ESGUI_POINT* p1, const ESGUI_POINT* p2)
{
  if( p1 == p2 )
    return 0;
  
  if( !p1 )
    return -1;
    
  if( !p2 )
    return 1;
  
  return esguiPointsCompare_(
    p1->x, 
    p1->y, 
    p2->x, 
    p2->y
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT esguiPointsAdd(const ESGUI_POINT* p1, const ESGUI_POINT* p2)
{
  ESGUI_POINT result;
  result.x = p1->x + p2->x;
  result.y = p1->y + p2->y;
  return result;
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT esguiPointsSub(const ESGUI_POINT* p1, const ESGUI_POINT* p2)
{
  ESGUI_POINT result;
  result.x = p1->x - p2->x;
  result.y = p1->y - p2->y;
  return result;
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT esguiPointNeg(const ESGUI_POINT* p)
{
  ESGUI_POINT result;
  result.x = -p->x;
  result.y = -p->y;
  return result;
}
//----------------------------------------------------------------------------------------------

int esguiPointsDistance_(int p1x, int p1y, int p2x, int p2y)
{
  int dx = p1x - p2x;
  int dy = p1y - p2y;
  return (int)(sqrtf( (float)(dx * dx + dy * dy) ));
}
//----------------------------------------------------------------------------------------------

int esguiPointsDistance(const ESGUI_POINT* p1, const ESGUI_POINT* p2)
{
  return esguiPointsDistance_(p1->x, p1->y, p2->x, p2->y);
}
//----------------------------------------------------------------------------------------------
