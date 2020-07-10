#ifndef _s6e63d6x_h_
#define _s6e63d6x_h_

#include <esgui/include/esgui.h>

#ifdef __cplusplus
    extern "C" {
#endif

// Internal alignment offs to window commands
#define s6e63d6x_OFFS  0x20

// Chip ID validity check
esBL s6e63d6xIsValidId(ESGUI_HDRV hdrv);

// Screen driver setup service
void s6e63d6xScreenDriverSetup(ESGUI_HDRV hdrv, int extx, int exty);

// Misc controller - specific services
esU16 s6e63d6xReadId(ESGUI_HDRV hdrv);

#ifdef __cplusplus
    }
#endif

#endif
