#ifndef _esgui_geometry_api_intf_h_
#define _esgui_geometry_api_intf_h_

class ESE_ABSTRACT EsguiGeometryApiIntf
{
public:
  /// Point API
  ///
  virtual const ESGUI_POINT* pointZeroGet() const ESE_NOTHROW = 0;
  virtual void pointZeroSet(ESGUI_POINT* p) const ESE_NOTHROW = 0;
  virtual bool pointIsZero(ESGUI_POINT* p) const ESE_NOTHROW = 0;
  virtual int pointsCompare(int p1x, int p1y, int p2x, int p2y) const ESE_NOTHROW = 0;
  virtual int pointsCompare(const ESGUI_POINT* p1, const ESGUI_POINT* p2) const ESE_NOTHROW = 0;
  virtual ESGUI_POINT pointsAdd(int x0, int y0, int x1, int y1) const ESE_NOTHROW = 0;
  virtual ESGUI_POINT pointsAdd(const ESGUI_POINT* p1, const ESGUI_POINT* p2) const ESE_NOTHROW = 0;
  virtual ESGUI_POINT pointsSub(int x0, int y0, int x1, int y1) const ESE_NOTHROW = 0;
  virtual ESGUI_POINT pointsSub(const ESGUI_POINT* p1, const ESGUI_POINT* p2) const ESE_NOTHROW = 0;
  virtual ESGUI_POINT pointNeg(const ESGUI_POINT* p) const ESE_NOTHROW = 0;
  virtual int pointsDistance(int p1x, int p1y, int p2x, int p2y) const ESE_NOTHROW = 0;
  virtual int pointsDistance(const ESGUI_POINT* p1, const ESGUI_POINT* p2) const ESE_NOTHROW = 0;
  virtual bool pointInRect(int x, int y, const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual bool pointInRect(const ESGUI_POINT* p, const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  
  /// Rect API
  ///
  virtual const ESGUI_RECT* rectZeroGet() const ESE_NOTHROW = 0;
  virtual void rectZeroSet( ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual void rectNormalize(ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual void rectSet(ESGUI_RECT* dest, const ESGUI_POINT* org, const ESGUI_POINT* edge) const ESE_NOTHROW = 0;
  virtual void rectSet(ESGUI_RECT* dest, int x0, int y0, int x1, int y1) const ESE_NOTHROW = 0;
  virtual bool rectsEqual(const ESGUI_RECT* r1, const ESGUI_RECT* r2) const ESE_NOTHROW = 0;
  virtual bool rectsEqual(int r1x0, int r1y0, int r1x1, int r1y1, int r2x0, int r2y0, int r2x1, int r2y1) const ESE_NOTHROW = 0;
  virtual ESGUI_POINT rectCenter(const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual void rectCenter(ESGUI_POINT* dest, const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual void rectGrow(ESGUI_RECT* dest, int x, int y) const ESE_NOTHROW = 0;
  virtual void rectGrow(ESGUI_RECT* dest, const ESGUI_POINT* p) const ESE_NOTHROW = 0;
  virtual void rectOffset(ESGUI_RECT* dest, int x, int y) const ESE_NOTHROW = 0;
  virtual void rectOffset(ESGUI_RECT* dest, const ESGUI_POINT* p) const ESE_NOTHROW = 0;
  virtual int rectWidth(const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual int rectHeight(const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual bool rectIsZero(const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual bool rectIsTall(const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual bool rectIsWide(const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual bool rectIsSquare(const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual bool rectIntersect(ESGUI_RECT* dest, const ESGUI_RECT* r1, const ESGUI_RECT* r2) const ESE_NOTHROW = 0;
  virtual void rectUnion(ESGUI_RECT* dest, const ESGUI_RECT* r1, const ESGUI_RECT* r2) const ESE_NOTHROW = 0;
  virtual void rectWidthSet(ESGUI_RECT* dest, int cx) const ESE_NOTHROW = 0;
  virtual void rectHeightSet(ESGUI_RECT* dest, int cy) const ESE_NOTHROW = 0;
  virtual ESGUI_POINT rectExtentGet(const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual void rectExtentGet(ESGUI_POINT* dest, const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual void rectExtentSet(ESGUI_RECT* dest, int cx, int cy) const ESE_NOTHROW = 0;
  virtual void rectExtentSet(ESGUI_RECT* dest, const ESGUI_POINT* ext) const ESE_NOTHROW = 0;
  virtual int rectXCenterGet(const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual int rectYCenterGet(const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual ESGUI_POINT rectCenterGet(const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual void rectCenterGet(ESGUI_POINT* dest, const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual void rectCenterSet(ESGUI_RECT* dest, int x, int y) const ESE_NOTHROW = 0;
  virtual void rectCenterSet(ESGUI_RECT* dest, const ESGUI_POINT* c) const ESE_NOTHROW = 0;
  virtual ESGUI_POINT rectTopRightGet(const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual ESGUI_POINT rectBottomLeftGet(const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  virtual void rectTopRightSet(ESGUI_RECT* dest, int x, int y) const ESE_NOTHROW = 0;
  virtual void rectTopRightSet(ESGUI_RECT* dest, const ESGUI_POINT* p) const ESE_NOTHROW = 0;
  virtual void rectBottomLeftSet(ESGUI_RECT* dest, int x, int y) const ESE_NOTHROW = 0;
  virtual void rectBottomLeftSet(ESGUI_RECT* dest, const ESGUI_POINT* p) const ESE_NOTHROW = 0;
  virtual bool rectInRect(const ESGUI_RECT* subj, const ESGUI_RECT* r) const ESE_NOTHROW = 0;
  
  /// Geometry math
  ///
  virtual float degToRad(float deg) const ESE_NOTHROW = 0;
  virtual float radToDeg(float rad) const ESE_NOTHROW = 0;
  virtual ESGUI_POINT pointRotate(const ESGUI_POINT* p, float angle, esU32 flags) const ESE_NOTHROW = 0;
  virtual bool lineIntersect(ESGUI_POINT* beg1, ESGUI_POINT* end1, const ESGUI_POINT* beg, const ESGUI_POINT* end, const ESGUI_RECT* r) const ESE_NOTHROW = 0;
};

#endif //< _esgui_geometry_api_intf_h_
