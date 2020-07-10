#ifndef _hx5051_h_
#define _hx5051_h_

#include <esgui/include/esgui.h>

#ifdef __cplusplus
    extern "C" {
#endif

// NB! this hardware has problem setting gram window, so 
// this operation is emulated in software, to avoid 
// unneccessary delays
//

typedef struct
{
    ESGUI_DRV cmn;
    // for software emulation of gram window
    ESGUI_RECT wnd;
    ESGUI_POINT pos;
    
} hx5051_DRV;

void hx5051ScreenDriverSetup(ESGUI_HDRV hdrv, int extx, int exty);

#ifdef __cplusplus
    }
#endif

#endif
