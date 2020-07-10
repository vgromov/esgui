#ifndef _es_gui_point_h_
#define _es_gui_point_h_

#ifdef __cplusplus
  extern "C" {
#endif

// point type
typedef struct tagESGUI_POINT
{
  int x;
  int y;

} ESGUI_POINT;

extern const ESGUI_POINT c_zeroPoint;

// zero point variable
void esguiPointZeroSet(ESGUI_POINT* p);

esBL esguiPointIsZero(ESGUI_POINT* p);

// comparison. returned is -1 if p1 < p2, 0 if equal, 1 if p1 > p2
int esguiPointsCompare_(int p1x, int p1y, int p2x, int p2y);
int esguiPointsCompare(const ESGUI_POINT* p1, const ESGUI_POINT* p2);

// addition|subtraction
ESGUI_POINT esguiPointsAdd(const ESGUI_POINT* p1, const ESGUI_POINT* p2);
ESGUI_POINT esguiPointsSub(const ESGUI_POINT* p1, const ESGUI_POINT* p2);

// negation
ESGUI_POINT esguiPointNeg(const ESGUI_POINT* p);

// distance calculation
int esguiPointsDistance_(int p1x, int p1y, int p2x, int p2y);
int esguiPointsDistance(const ESGUI_POINT* p1, const ESGUI_POINT* p2);

#ifdef __cplusplus
  }
#endif

#endif // _es_gui_point_h_
