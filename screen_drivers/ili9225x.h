#ifndef _ili_9225_x_h_
#define _ili_9225_x_h_

// ili925x.h file - ILITEK a-Si TFT LCD Single Chip Driver declarations
//
#include <esgui/include/esgui.h>

#ifdef __cplusplus
  extern "C" {
#endif

// Chip ID validity check
esBL ili9225isValidId(ESGUI_HDRV hdrv);

// screen driver setup service
void ili9225xScreenDriverSetup(ESGUI_HDRV hdrv, int extx, int exty);
// misc controller - specific services
esU16 ili9225xIdRead(ESGUI_HDRV hdrv);

// Driver testing routines
#ifdef ESGUI_USE_ILI9225X_DRIVER_TEST
// Window fill for driver testing
void ili9225xWindowFillTest(ESGUI_HDRV hdrv);
#endif


#ifdef __cplusplus
  }
#endif

#endif // _ili_9225_x_h_
