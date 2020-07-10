#ifndef _es_gui_rect_h_
#define _es_gui_rect_h_

#ifdef __cplusplus
  extern "C" {
#endif

// rect type
typedef struct tagESGUI_RECT
{
  ESGUI_POINT org;
  ESGUI_POINT edge;

} ESGUI_RECT;

extern const ESGUI_RECT c_zeroRect;

// normalize rect
void esguiRectNormalize(ESGUI_RECT* r);

// assign rect
void esguiRectSet(const ESGUI_POINT* org, const ESGUI_POINT* edge, ESGUI_RECT* dest);
void esguiRectSet_(int x0, int y0, int x1, int y1, ESGUI_RECT* dest);

// empty rectangle
void esguiRectZeroSet( ESGUI_RECT* r );

// equality compare for rects
esBL esguiRectsEqual(const ESGUI_RECT* r1, const ESGUI_RECT* r2);
esBL esguiRectsEqual_(int r1x0, int r1y0, int r1x1, int r1y1, int r2x0, int r2y0, int r2x1, int r2y1);

// return center of rectangle. rectangle should be normalized
void esguiRectCenter(const ESGUI_RECT* r, ESGUI_POINT* dest);

// grow|shrink rect
void esguiRectGrow_(int x, int y, ESGUI_RECT* dest);
void esguiRectGrow(const ESGUI_POINT* p, ESGUI_RECT* dest);

// shift rectangle
void esguiRectOffset_(int x, int y, ESGUI_RECT* dest);
void esguiRectOffset(const ESGUI_POINT* p, ESGUI_RECT* dest);

// retrieve width and height of rect
int esguiRectWidth(const ESGUI_RECT* r);
int esguiRectHeight(const ESGUI_RECT* r);

// rect characterization
esBL esguiRectIsZero(const ESGUI_RECT* r);
esBL esguiRectIsTall(const ESGUI_RECT* r);
esBL esguiRectIsWide(const ESGUI_RECT* r);
esBL esguiRectIsSquare(const ESGUI_RECT* r);

// rects intersection. rectengles should be normalized first
// if dest is not null, assign intersection result to it
esBL esguiRectIntersect(const ESGUI_RECT* r1, const ESGUI_RECT* r2, ESGUI_RECT* dest);

// calculate union of two rectangles
void esguiRectUnion(const ESGUI_RECT* r1, const ESGUI_RECT* r2, ESGUI_RECT* dest);

// additional rect utilities
void esguiRectWidthSet(int cx, ESGUI_RECT* r);
void esguiRectHeightSet(int cy, ESGUI_RECT* r);

ESGUI_POINT esguiRectExtentGet(const ESGUI_RECT* r);
void esguiRectExtentPtrGet(const ESGUI_RECT* r, ESGUI_POINT* dest);

void esguiRectExtentSet_(int cx, int cy, ESGUI_RECT* r);
void esguiRectExtentSet(const ESGUI_POINT* ext, ESGUI_RECT* r);

int esguiRectXCenterGet(const ESGUI_RECT* r);
int esguiRectYCenterGet(const ESGUI_RECT* r);

ESGUI_POINT esguiRectCenterGet(const ESGUI_RECT* r);
void esguiRectCenterPtrGet(const ESGUI_RECT* r, ESGUI_POINT* dest);

void esguiRectCenterSet_(int x, int y, ESGUI_RECT* r);
void esguiRectCenterSet(const ESGUI_POINT* c, ESGUI_RECT* r);

ESGUI_POINT esguiRectTopRightGet(const ESGUI_RECT* r);
ESGUI_POINT esguiRectBottomLeftGet(const ESGUI_RECT* r);

void esguiRectTopRightSet_(int x, int y, ESGUI_RECT* r);
void esguiRectBottomLeftSet_(int x, int y, ESGUI_RECT* r);

void esguiRectTopRightSet(const ESGUI_POINT* p, ESGUI_RECT* r);
void esguiRectBottomLeftSet(const ESGUI_POINT* p, ESGUI_RECT* r);

// check if point is in rect
esBL esguiPointInRect_(int x, int y, const ESGUI_RECT* r);
esBL esguiPointInRect(const ESGUI_POINT* p, const ESGUI_RECT* r);

// check if rect is inside rect. NB! rect is considered to fall
// into other rect even if their right borders coinside
esBL esguiRectInRect(const ESGUI_RECT* subj, const ESGUI_RECT* r);

#ifdef __cplusplus
  }
#endif

#endif //  _es_gui_rect_h_
