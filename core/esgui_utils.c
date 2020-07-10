#include <esgui/include/esgui.h>
#include <esgui/include/esgui_utils.h>
//----------------------------------------------------------------------------------------------

void esguiUtilsSwapInt(int* a, int* b)
{
  int tmp = *b;
  *b = *a;
  *a = tmp;
}
//----------------------------------------------------------------------------------------------

void esguiUtilsSwapB(esU8* pb, int count)
{
  esU8* pEnd = pb+count;
  esU8 tmp;

  while(pb < pEnd)
  {
    tmp = pb[0];
    pb[0] = pb[1];
    pb[1] = tmp;
    pb += 2;
  }
}
//----------------------------------------------------------------------------------------------
