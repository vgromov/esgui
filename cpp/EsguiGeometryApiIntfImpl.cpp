#include <esgui/include/esgui.h>

#include "EsguiGeometryApiIntf.h"
#include "EsguiGeometryApiIntfImpl.h"
//----------------------------------------------------------------------------------------------

#include <esgui/core/esgui_point.c>
#include <esgui/core/esgui_rect.c>
#include <esgui/core/esgui_math.c>
//----------------------------------------------------------------------------------------------

EsguiGeometryApiIntf& EsguiGeometryApiIntfImpl::instanceGet() ESE_NOTHROW
{
  static EsguiGeometryApiIntfImpl s_api;
  return s_api;
}
//----------------------------------------------------------------------------------------------

const ESGUI_POINT* EsguiGeometryApiIntfImpl::pointZeroGet() const ESE_NOTHROW
{
  return &c_zeroPoint;
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::pointZeroSet(ESGUI_POINT* p) const ESE_NOTHROW
{
  esguiPointZeroSet(p);
}
//----------------------------------------------------------------------------------------------

bool EsguiGeometryApiIntfImpl::pointIsZero(ESGUI_POINT* p) const ESE_NOTHROW
{
  return TRUE == esguiPointIsZero(p);
}
//----------------------------------------------------------------------------------------------

int EsguiGeometryApiIntfImpl::pointsCompare(int p1x, int p1y, int p2x, int p2y) const ESE_NOTHROW
{
  return esguiPointsCompare_(
    p1x,
    p1y,
    p2x,
    p2y
  );
}
//----------------------------------------------------------------------------------------------

int EsguiGeometryApiIntfImpl::pointsCompare(const ESGUI_POINT* p1, const ESGUI_POINT* p2) const ESE_NOTHROW
{
  return esguiPointsCompare(
    p1,
    p2
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT EsguiGeometryApiIntfImpl::pointsAdd(int x0, int y0, int x1, int y1) const ESE_NOTHROW
{
  ESGUI_POINT p1 = {x0, y0};
  ESGUI_POINT p2 = {x1, y1};
  
  return pointsAdd(
    &p1,
    &p2
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT EsguiGeometryApiIntfImpl::pointsAdd(const ESGUI_POINT* p1, const ESGUI_POINT* p2) const ESE_NOTHROW
{
  return esguiPointsAdd(
    p1,
    p2
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT EsguiGeometryApiIntfImpl::pointsSub(int x0, int y0, int x1, int y1) const ESE_NOTHROW
{
  ESGUI_POINT p1 = {x0, y0};
  ESGUI_POINT p2 = {x1, y1};
  
  return pointsSub(
    &p1,
    &p2
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT EsguiGeometryApiIntfImpl::pointsSub(const ESGUI_POINT* p1, const ESGUI_POINT* p2) const ESE_NOTHROW
{
  return esguiPointsSub(
    p1,
    p2
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT EsguiGeometryApiIntfImpl::pointNeg(const ESGUI_POINT* p) const ESE_NOTHROW
{
  return esguiPointNeg(p);
}
//----------------------------------------------------------------------------------------------

int EsguiGeometryApiIntfImpl::pointsDistance(int p1x, int p1y, int p2x, int p2y) const ESE_NOTHROW
{
  return esguiPointsDistance_(
    p1x,
    p1y,
    p2x,
    p2y
  );
}
//----------------------------------------------------------------------------------------------

int EsguiGeometryApiIntfImpl::pointsDistance(const ESGUI_POINT* p1, const ESGUI_POINT* p2) const ESE_NOTHROW
{
  return esguiPointsDistance(
    p1,
    p2
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiGeometryApiIntfImpl::pointInRect(int x, int y, const ESGUI_RECT* r) const ESE_NOTHROW
{
  return TRUE == esguiPointInRect_(
    x,
    y,
    r
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiGeometryApiIntfImpl::pointInRect(const ESGUI_POINT* p, const ESGUI_RECT* r) const ESE_NOTHROW
{
  return TRUE == esguiPointInRect(
    p,
    r
  );
}
//----------------------------------------------------------------------------------------------

const ESGUI_RECT* EsguiGeometryApiIntfImpl::rectZeroGet() const ESE_NOTHROW
{
  return &c_zeroRect;
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectZeroSet(ESGUI_RECT* r) const ESE_NOTHROW
{
  esguiRectZeroSet(r);
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectNormalize(ESGUI_RECT* r) const ESE_NOTHROW
{
  esguiRectNormalize(r);
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectSet(ESGUI_RECT* dest, const ESGUI_POINT* org, const ESGUI_POINT* edge) const ESE_NOTHROW
{
  esguiRectSet(
    org,
    edge,
    dest
  );
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectSet(ESGUI_RECT* dest, int x0, int y0, int x1, int y1) const ESE_NOTHROW
{
  esguiRectSet_(
    x0, y0,
    x1, y1,
    dest
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiGeometryApiIntfImpl::rectsEqual(const ESGUI_RECT* r1, const ESGUI_RECT* r2) const ESE_NOTHROW
{
  return TRUE == esguiRectsEqual(
    r1,
    r2
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiGeometryApiIntfImpl::rectsEqual(int r1x0, int r1y0, int r1x1, int r1y1, int r2x0, int r2y0, int r2x1, int r2y1) const ESE_NOTHROW
{
  return TRUE == esguiRectsEqual_(
    r1x0,
    r1y0,
    r1x1,
    r1y1,
    r2x0,
    r2y0,
    r2x1,
    r2y1
  );
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectCenter(ESGUI_POINT* dest, const ESGUI_RECT* r) const ESE_NOTHROW
{
  esguiRectCenter(
    r,
    dest
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT EsguiGeometryApiIntfImpl::rectCenter(const ESGUI_RECT* r) const ESE_NOTHROW
{
  ESGUI_POINT p;
  esguiRectCenter(
    r,
    &p
  );
  
  return p;
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectGrow(ESGUI_RECT* dest, int x, int y) const ESE_NOTHROW
{
  esguiRectGrow_(
    x,
    y,
    dest
  );
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectGrow(ESGUI_RECT* dest, const ESGUI_POINT* p) const ESE_NOTHROW
{
  esguiRectGrow(
    p,
    dest
  );
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectOffset(ESGUI_RECT* dest, int x, int y) const ESE_NOTHROW
{
  esguiRectOffset_(
    x,
    y,
    dest
  );
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectOffset(ESGUI_RECT* dest, const ESGUI_POINT* p) const ESE_NOTHROW
{
  esguiRectOffset(
    p,
    dest
  );
}
//----------------------------------------------------------------------------------------------

int EsguiGeometryApiIntfImpl::rectWidth(const ESGUI_RECT* r) const ESE_NOTHROW
{
  return esguiRectWidth(r);
}
//----------------------------------------------------------------------------------------------

int EsguiGeometryApiIntfImpl::rectHeight(const ESGUI_RECT* r) const ESE_NOTHROW
{
  return esguiRectHeight(r);
}
//----------------------------------------------------------------------------------------------

bool EsguiGeometryApiIntfImpl::rectIsZero(const ESGUI_RECT* r) const ESE_NOTHROW
{
  return TRUE == esguiRectIsZero(r);
}
//----------------------------------------------------------------------------------------------

bool EsguiGeometryApiIntfImpl::rectIsTall(const ESGUI_RECT* r) const ESE_NOTHROW
{
  return TRUE == esguiRectIsTall(r);
}
//----------------------------------------------------------------------------------------------

bool EsguiGeometryApiIntfImpl::rectIsWide(const ESGUI_RECT* r) const ESE_NOTHROW
{
  return TRUE == esguiRectIsWide(r);
}
//----------------------------------------------------------------------------------------------

bool EsguiGeometryApiIntfImpl::rectIsSquare(const ESGUI_RECT* r) const ESE_NOTHROW
{
  return TRUE == esguiRectIsSquare(r);
}
//----------------------------------------------------------------------------------------------

bool EsguiGeometryApiIntfImpl::rectIntersect(ESGUI_RECT* dest, const ESGUI_RECT* r1, const ESGUI_RECT* r2) const ESE_NOTHROW
{
  return TRUE == esguiRectIntersect(
    r1,
    r2,
    dest
  );
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectUnion(ESGUI_RECT* dest, const ESGUI_RECT* r1, const ESGUI_RECT* r2) const ESE_NOTHROW
{
  esguiRectUnion(
    r1,
    r2,
    dest
  );
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectWidthSet(ESGUI_RECT* dest, int cx) const ESE_NOTHROW
{
  esguiRectWidthSet(
    cx,
    dest
  );
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectHeightSet(ESGUI_RECT* dest, int cy) const ESE_NOTHROW
{
  esguiRectHeightSet(
    cy,
    dest
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT EsguiGeometryApiIntfImpl::rectExtentGet(const ESGUI_RECT* r) const ESE_NOTHROW
{
  return esguiRectExtentGet(r);
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectExtentGet(ESGUI_POINT* dest, const ESGUI_RECT* r) const ESE_NOTHROW
{
  esguiRectExtentPtrGet(
    r,
    dest
  );
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectExtentSet(ESGUI_RECT* dest, int cx, int cy) const ESE_NOTHROW
{
  esguiRectExtentSet_(
    cx,
    cy,
    dest
  );
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectExtentSet(ESGUI_RECT* dest, const ESGUI_POINT* ext) const ESE_NOTHROW
{
  esguiRectExtentSet(
    ext,
    dest
  );
}
//----------------------------------------------------------------------------------------------

int EsguiGeometryApiIntfImpl::rectXCenterGet(const ESGUI_RECT* r) const ESE_NOTHROW
{
  return esguiRectXCenterGet(r);
}
//----------------------------------------------------------------------------------------------

int EsguiGeometryApiIntfImpl::rectYCenterGet(const ESGUI_RECT* r) const ESE_NOTHROW
{
  return esguiRectYCenterGet(r);
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT EsguiGeometryApiIntfImpl::rectCenterGet(const ESGUI_RECT* r) const ESE_NOTHROW
{
  return esguiRectCenterGet(r);
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectCenterGet(ESGUI_POINT* dest, const ESGUI_RECT* r) const ESE_NOTHROW
{
  esguiRectCenterPtrGet(
    r,
    dest
  );
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectCenterSet(ESGUI_RECT* dest, int x, int y) const ESE_NOTHROW
{
  esguiRectCenterSet_(
    x,
    y,
    dest
  );
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectCenterSet(ESGUI_RECT* dest, const ESGUI_POINT* c) const ESE_NOTHROW
{
  esguiRectCenterSet(
    c,
    dest
  );
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT EsguiGeometryApiIntfImpl::rectTopRightGet(const ESGUI_RECT* r) const ESE_NOTHROW
{
  return esguiRectTopRightGet(r);
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT EsguiGeometryApiIntfImpl::rectBottomLeftGet(const ESGUI_RECT* r) const ESE_NOTHROW
{
  return esguiRectBottomLeftGet(r);
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectTopRightSet(ESGUI_RECT* dest, int x, int y) const ESE_NOTHROW
{
  esguiRectTopRightSet_(
    x,
    y,
    dest
  );
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectTopRightSet(ESGUI_RECT* dest, const ESGUI_POINT* p) const ESE_NOTHROW
{
  esguiRectTopRightSet(
    p,
    dest
  );
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectBottomLeftSet(ESGUI_RECT* dest, int x, int y) const ESE_NOTHROW
{
  esguiRectBottomLeftSet_(
    x,
    y,
    dest
  );
}
//----------------------------------------------------------------------------------------------

void EsguiGeometryApiIntfImpl::rectBottomLeftSet(ESGUI_RECT* dest, const ESGUI_POINT* p) const ESE_NOTHROW
{
  esguiRectBottomLeftSet(
    p,
    dest
  );
}
//----------------------------------------------------------------------------------------------

bool EsguiGeometryApiIntfImpl::rectInRect(const ESGUI_RECT* subj, const ESGUI_RECT* r) const ESE_NOTHROW
{
  return TRUE == esguiRectInRect(
    subj,
    r
  );
}
//----------------------------------------------------------------------------------------------

float EsguiGeometryApiIntfImpl::degToRad(float deg) const ESE_NOTHROW
{
  return esguiMathDegToRad(deg);
}
//----------------------------------------------------------------------------------------------

float EsguiGeometryApiIntfImpl::radToDeg(float rad) const ESE_NOTHROW
{
  return esguiMathRadToDeg(rad);
}
//----------------------------------------------------------------------------------------------

ESGUI_POINT EsguiGeometryApiIntfImpl::pointRotate(const ESGUI_POINT* p, float angle, esU32 flags) const ESE_NOTHROW
{
  ESGUI_POINT out;
  esguiMathPointRotatePtr(
    p,
    angle,
    flags,
    &out
  );
  
  return out;
}
//----------------------------------------------------------------------------------------------

bool EsguiGeometryApiIntfImpl::lineIntersect(ESGUI_POINT* beg1, ESGUI_POINT* end1, const ESGUI_POINT* beg, const ESGUI_POINT* end, const ESGUI_RECT* r) const ESE_NOTHROW
{
  return TRUE == esguiMathLineIntersect(
    beg,
    end,
    r,
    beg1,
    end1
  );
}
//----------------------------------------------------------------------------------------------
