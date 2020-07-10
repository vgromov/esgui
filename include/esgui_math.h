#ifndef _es_gui_math_h_
#define _es_gui_math_h_

#ifdef __cplusplus
  extern "C" {
#endif

//pi const
#define ESGUI_PI  3.14159265358979323846f

// Rotation. flags is a one or more OR-ed rotationFlagXXX
enum {
  esguiMathFlagRotateClockwise  = 0x01,
  esguiMathFlagRotateAngleInDeg = 0x02,  // angle is in RAD otherwise
  esguiMathFlagRotateInverseX   = 0x04,
  esguiMathFlagRotateInverseY   = 0x08
};

//polar helpers
float esguiMathDegToRad(float deg);
float esguiMathRadToDeg(float rad);

ESGUI_POINT esguiMathPointRotate(const ESGUI_POINT* p, float angle, esU32 flags);
void esguiMathPointRotatePtr(const ESGUI_POINT* p, float angle, esU32 flags, ESGUI_POINT* dest);

// arbitrary lines
//
// clip line segment intersection calculation. return TRUE if line segment 
// intersects rectangle window. return TURE if intersection is not empty.
// if beg1 and end1 not NULL - return calculated intersection points for new segment
esBL esguiMathLineIntersect(
  const ESGUI_POINT* beg, 
  const ESGUI_POINT* end, 
  const ESGUI_RECT* r, 
  ESGUI_POINT* beg1, 
  ESGUI_POINT* end1
);

#ifdef __cplusplus
  }
#endif

#endif // _es_gui_math_h_
