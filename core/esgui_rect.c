#include <esgui/include/esgui.h>
#include <esgui/include/esgui_point.h>
#include <esgui/include/esgui_rect.h>
//----------------------------------------------------------------------------------------------

const ESGUI_RECT c_zeroRect = {{0,0}, {0,0}};
//----------------------------------------------------------------------------------------------

esBL esguiPointInRect_(int x, int y, const ESGUI_RECT* r)
{
  return x >= r->org.x && x <= r->edge.x &&
    y >= r->org.y && y <= r->edge.y;
}
//----------------------------------------------------------------------------------------------

esBL esguiPointInRect(const ESGUI_POINT* p, const ESGUI_RECT* r)
{
  return esguiPointInRect_(p->x, p->y, r);
}
//----------------------------------------------------------------------------------------------

esBL esguiRectInRect(const ESGUI_RECT* subj, const ESGUI_RECT* r)
{
  return esguiPointInRect(&subj->org, r) && 
    subj->edge.x <= r->edge.x &&
    subj->edge.y <= r->edge.y; 
}
//----------------------------------------------------------------------------------------------

void esguiRectNormalize(ESGUI_RECT* r)
{
  if( r->org.x > r->edge.x )
    esguiUtilsSwapInt(&r->org.x, &r->edge.x);

  if( r->org.y > r->edge.y )
    esguiUtilsSwapInt(&r->org.y, &r->edge.y);
}
//----------------------------------------------------------------------------------------------

void esguiRectSet(const ESGUI_POINT* org, const ESGUI_POINT* edge, ESGUI_RECT* dest)
{
  dest->org = *org;
  dest->edge = *edge;  
  esguiRectNormalize(dest);
}
//----------------------------------------------------------------------------------------------

void esguiRectSet_(int x0, int y0, int x1, int y1, ESGUI_RECT* dest)
{
  dest->org.x = x0;
  dest->org.y  = y0;
  dest->edge.x = x1;  
  dest->edge.y = y1;  
  esguiRectNormalize(dest);
}
//----------------------------------------------------------------------------------------------

void esguiRectZeroSet( ESGUI_RECT* r )
{
  if( r )
    *r = c_zeroRect;
}
//----------------------------------------------------------------------------------------------

esBL esguiRectsEqual(const ESGUI_RECT* r1, const ESGUI_RECT* r2)
{
  return 0 == esguiPointsCompare(
    &r1->org, 
    &r2->org
  ) && 
  0 == esguiPointsCompare(
    &r1->edge, 
    &r2->edge
  );
}
//----------------------------------------------------------------------------------------------

esBL esguiRectsEqual_(int r1x0, int r1y0, int r1x1, int r1y1, 
  int r2x0, int r2y0, int r2x1, int r2y1)
{
  return 0 == esguiPointsCompare_(
    r1x0, 
    r1y0, 
    r2x0, 
    r2y0
  ) && 
  0 == esguiPointsCompare_(
    r1x1, 
    r1y1, 
    r2x1, 
    r2y1
  );
}
//----------------------------------------------------------------------------------------------

esBL esguiRectIsZero(const ESGUI_RECT* r)
{
  return 0 == esguiPointsCompare(&r->org, &r->edge);
}
//----------------------------------------------------------------------------------------------

void esguiRectCenter(const ESGUI_RECT* r, ESGUI_POINT* dest)
{
  dest->x = (r->edge.x + r->org.x) / 2;
  dest->y = (r->edge.y + r->org.y) / 2;
}
//----------------------------------------------------------------------------------------------

void esguiRectGrow_(int x, int y, ESGUI_RECT* dest)
{
  dest->org.x -= x;
  dest->edge.x += x;
  dest->org.y -= y;
  dest->edge.y += y;
  esguiRectNormalize(dest);
}
//----------------------------------------------------------------------------------------------

void esguiRectGrow(const ESGUI_POINT* p, ESGUI_RECT* dest)
{
  esguiRectGrow_(p->x, p->y, dest);
}
//----------------------------------------------------------------------------------------------

void esguiRectOffset_(int x, int y, ESGUI_RECT* dest)
{
  dest->org.x += x;
  dest->edge.x += x;
  dest->org.y += y;
  dest->edge.y += y;
}
//----------------------------------------------------------------------------------------------

void esguiRectOffset(const ESGUI_POINT* p, ESGUI_RECT* dest)
{
  esguiRectOffset_(p->x, p->y, dest);
}
//----------------------------------------------------------------------------------------------

int esguiRectWidth(const ESGUI_RECT* r)
{
  return r->edge.x - r->org.x;
}
//----------------------------------------------------------------------------------------------

int esguiRectHeight(const ESGUI_RECT* r)
{
  return r->edge.y - r->org.y;
}
//----------------------------------------------------------------------------------------------

esBL esguiRectIsTall(const ESGUI_RECT* r)
{
  return esguiRectWidth(r) < esguiRectHeight(r);
}
//----------------------------------------------------------------------------------------------

esBL esguiRectIsWide(const ESGUI_RECT* r)
{
  return esguiRectWidth(r) > esguiRectHeight(r);
}
//----------------------------------------------------------------------------------------------

esBL esguiRectIsSquare(const ESGUI_RECT* r)
{
  return esguiRectWidth(r) == esguiRectHeight(r);
}
//----------------------------------------------------------------------------------------------

esBL esguiRectIntersect(const ESGUI_RECT* r1, const ESGUI_RECT* r2, ESGUI_RECT* dest)
{
  ESGUI_RECT r;
  esBL bResult;

  r.org.y = esguiUtils_MAX( r1->org.y, r2->org.y );
  r.edge.y = esguiUtils_MIN( r1->edge.y, r2->edge.y );
  r.org.x = esguiUtils_MAX( r1->org.x, r2->org.x );
  r.edge.x = esguiUtils_MIN( r1->edge.x, r2->edge.x );

  bResult = (r.org.x <= r.edge.x) && (r.org.y <= r.edge.y);

  if( dest )
  {
    esguiRectZeroSet( dest );

    if( bResult )
      *dest = r;
  }

  return bResult;
}
//----------------------------------------------------------------------------------------------

void esguiRectUnion(const ESGUI_RECT* r1, const ESGUI_RECT* r2, ESGUI_RECT* dest)
{
  dest->org.y = esguiUtils_MIN( r1->org.y, r2->org.y );
  dest->edge.y = esguiUtils_MAX( r1->edge.y, r2->edge.y );
  dest->org.x = esguiUtils_MIN( r1->org.x, r2->org.x );
  dest->edge.x = esguiUtils_MAX( r1->edge.x, r2->edge.x );
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT esguiRectExtentGet(const ESGUI_RECT* r) 
{ 
  ESGUI_POINT result;
  result.x = esguiRectWidth(r);
  result.y = esguiRectHeight(r);
  return result; 
}
//----------------------------------------------------------------------------------------------

void esguiRectExtentPtrGet(const ESGUI_RECT* r, ESGUI_POINT* dest) 
{ 
  if( !dest )
    return;
    
  dest->x = esguiRectWidth(r);
  dest->y = esguiRectHeight(r);
}
//----------------------------------------------------------------------------------------------

void esguiRectWidthSet(int cx, ESGUI_RECT* r)
{
  r->edge.x = r->org.x + cx; 
}
//----------------------------------------------------------------------------------------------

void esguiRectHeightSet(int cy, ESGUI_RECT* r) 
{
  r->edge.y = r->org.y + cy; 
}
//----------------------------------------------------------------------------------------------

void esguiRectExtentSet_(int cx, int cy, ESGUI_RECT* r) 
{ 
  esguiRectWidthSet(cx, r);
  esguiRectHeightSet(cy, r); 
}
//----------------------------------------------------------------------------------------------

void esguiRectExtentSet(const ESGUI_POINT* ext, ESGUI_RECT* r) 
{ 
  esguiRectExtentSet_(ext->x, ext->y, r); 
}
//----------------------------------------------------------------------------------------------

int esguiRectXCenterGet(const ESGUI_RECT* r) 
{ 
  return r->org.x + esguiRectWidth(r)/2; 
}
//----------------------------------------------------------------------------------------------

int esguiRectYCenterGet(const ESGUI_RECT* r)
{
  return r->org.y + esguiRectHeight(r)/2;
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT esguiRectCenterGet(const ESGUI_RECT* r)
{  
  ESGUI_POINT result;
  result.x = esguiRectXCenterGet(r);
  result.y = esguiRectYCenterGet(r);
  return result; 
}
//----------------------------------------------------------------------------------------------

void esguiRectCenterPtrGet(const ESGUI_RECT* r, ESGUI_POINT* dest)
{  
  if(!dest)
    return;
    
  dest->x = esguiRectXCenterGet(r);
  dest->y = esguiRectYCenterGet(r);
}
//----------------------------------------------------------------------------------------------

void esguiRectCenterSet_(int x, int y, ESGUI_RECT* r) 
{ 
  ESGUI_POINT oldCtr = esguiRectCenterGet(r);
  esguiRectOffset_(x-oldCtr.x, y-oldCtr.y, r); 
}
//----------------------------------------------------------------------------------------------

void esguiRectCenterSet(const ESGUI_POINT* c, ESGUI_RECT* r) 
{ 
  esguiRectCenterSet_(c->x, c->y, r);
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT esguiRectTopRightGet(const ESGUI_RECT* r) 
{ 
  ESGUI_POINT result;
  result.x = r->edge.x;
  result.y = r->org.y;
  return result; 
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT esguiRectBottomLeftGet(const ESGUI_RECT* r) 
{ 
  ESGUI_POINT result;
  result.x = r->org.x;
  result.y = r->edge.y;
  return result;
}
//----------------------------------------------------------------------------------------------

void esguiRectTopRightSet_(int x, int y, ESGUI_RECT* r)
{
  r->edge.x = x;
  r->org.y = y;
}
//----------------------------------------------------------------------------------------------

void esguiRectBottomLeftSet_(int x, int y, ESGUI_RECT* r)
{
  r->org.x = x;
  r->edge.y = y;
}
//----------------------------------------------------------------------------------------------

void esguiRectTopRightSet(const ESGUI_POINT* p, ESGUI_RECT* r) 
{ 
  esguiRectTopRightSet_(p->x, p->y, r);
}
//----------------------------------------------------------------------------------------------

void esguiRectBottomLeftSet(const ESGUI_POINT* p, ESGUI_RECT* r) 
{ 
  esguiRectBottomLeftSet_(p->x, p->y, r);
}
//----------------------------------------------------------------------------------------------
