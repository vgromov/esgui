#ifndef _es_gui_screen_driver_h_
#define _es_gui_screen_driver_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <esgui/include/esgui_defs.h>
#include <esgui/include/esgui_color.h>

// screen power mode
typedef enum {
  ESGUI_SCREEN_PWRINVALID = -1,
  ESGUI_SCREEN_PWROFF,
  ESGUI_SCREEN_PWRSTDBY,
  ESGUI_SCREEN_PWRON
  
} ESGUI_SCREEN_PWRMODE;

// device bus interface. certain functionality, reset, for instance, may be implemented either
// on bus, or on driver level, depending on hardware and platform
//
typedef struct tagESGUI_BUS 
{
  /// bus initializer
  esBL (*init)(struct tagESGUI_BUS*);
  /// optional screen hardware reset
  void (*reset)(struct tagESGUI_BUS*);  
  /// possible MT bus usage
  esBL (*lock)(struct tagESGUI_BUS*);
  void (*unlock)(struct tagESGUI_BUS*);
  /// optional screen device selection|release
  void (*selectScreen)(struct tagESGUI_BUS*);
  void (*deselectScreen)(struct tagESGUI_BUS*);
  
  /// read-write screen device
  ///
  /// 8 bit interface
  void (*writeCmdB)(struct tagESGUI_BUS*, esU8);
  void (*writeDataB)(struct tagESGUI_BUS*, esU8);
  void (*readDataB)(struct tagESGUI_BUS*, esU8*);
  /// 16 bit interface
  void (*writeCmdW)(struct tagESGUI_BUS*, esU16);
  void (*writeDataW)(struct tagESGUI_BUS*, esU16);
  void (*readDataW)(struct tagESGUI_BUS*, esU16*);
  /// 32 bit interface
  void (*writeCmdDW)(struct tagESGUI_BUS*, esU32);
  void (*writeDataDW)(struct tagESGUI_BUS*, esU32);
  void (*readDataDW)(struct tagESGUI_BUS*, esU32*);
  
} ESGUI_BUS; 
typedef ESGUI_BUS* ESGUI_HBUS;

// screen interface
typedef struct tagESGUI_DRIVER
{
  // screen bus
  ESGUI_HBUS hbus;
  // physical screen extent
  int width;
  int height;
  // currently set screen rotation
  ESGUI_SCREEN_ROTATION rotation;
  // Driver color format
  ESGUI_COLOR_FORMAT colorFmt;
  // Driver color bit depth
  int bpp;
  // Screen iobatch nesting counter
  int ioBatchNesting;
  // Screen controller chip ID (Not all displays anr controllers support this)
  // Chip ID is usually read once during initialization sequence
  esU16 chipId;
  // screen hardware initializer
  esBL (*init)(struct tagESGUI_DRIVER*, ESGUI_COLOR_FORMAT);
  // optional screen hardware reset
  void (*reset)(struct tagESGUI_DRIVER*);
  // locks|unlocks screen device refresh
  void (*lockRefresh)(struct tagESGUI_DRIVER*, esBL);
  // set backlight level from 0 to 255
  void (*setBacklight)(struct tagESGUI_DRIVER*, esU8);
  // control screen power mode
  void (*setPowerMode)(struct tagESGUI_DRIVER*, ESGUI_SCREEN_PWRMODE);
  // set lcd screen  rotation
  void (*setRotation)(struct tagESGUI_DRIVER*, ESGUI_SCREEN_ROTATION);
  // set screen position. consecutive lcd memory writes
  // will start at this position
  void (*setPos)(struct tagESGUI_DRIVER*, int x, int y);
  // set virtual window in which all lcd memory writes will go
  void (*setWindow)(struct tagESGUI_DRIVER*, int x0, int y0, int x1, int y1);
  // reset virtual window back to defaults
  void (*resetWindow)(struct tagESGUI_DRIVER*);
  // set pixel to color at current position
  void (*setPixel)(struct tagESGUI_DRIVER*, ESGUI_Color_t);
  // get pixel from current position
  ESGUI_Color_t (*getPixel)(struct tagESGUI_DRIVER*);
  // fill current screen window area with specified color
  void (*fillWindow)(struct tagESGUI_DRIVER*, int x0, int y0, int x1, int y1, ESGUI_Color_t clr);
  // clear screen
  void (*clear)(struct tagESGUI_DRIVER*, ESGUI_Color_t);
  
} ESGUI_DRV;

// GUI Driver handle
typedef ESGUI_DRV* ESGUI_HDRV;

// Screen bus services
//

// Reset all bus internals to 0
void esguiScreenBusHandleInit(ESGUI_HBUS hbus);

// Initialize bus hardware
esBL esguiScreenBusInit(ESGUI_HBUS hbus);

// Lock|unlock bus for exclusive access
esBL esguiScreenBusLock(ESGUI_HBUS hbus);
void esguiScreenBusUnlock(ESGUI_HBUS hbus);

// Screen bus io
//
// 8 bit
void esguiScreenBusWriteCmdB(ESGUI_HBUS hbus, esU8 cmd);
void esguiScreenBusWriteDataB(ESGUI_HBUS hbus, esU8 data);
void esguiScreenBusReadDataB(ESGUI_HBUS hbus, esU8* data);

// 16 bit
void esguiScreenBusWriteCmdW(ESGUI_HBUS hbus, esU16 cmd);
void esguiScreenBusWriteDataW(ESGUI_HBUS hbus, esU16 data);
void esguiScreenBusReadDataW(ESGUI_HBUS hbus, esU16* data);

// 32 bit
void esguiScreenBusWriteCmdDW(ESGUI_HBUS hbus, esU32 cmd);
void esguiScreenBusWriteDataDW(ESGUI_HBUS hbus, esU32 data);
void esguiScreenBusReadDataDW(ESGUI_HBUS hbus, esU32* data);

// Screen driver services
//

// Reset screen driver handle internals to 0
void esguiScreenDriverHandleInit(ESGUI_HDRV handle, esU32 drvStructSize);

// Initialize screen hardware, but do not turn display on, so far, if possible
// driver handle must be set-up first, somewhere in specific driver code
//
esBL esguiScreenInit(ESGUI_HDRV handle, ESGUI_HBUS hbus, ESGUI_COLOR_FORMAT drvColorFmt);

// Lock|unlock screen driver exclusive access
esBL esguiScreenLock(ESGUI_HDRV handle);
void esguiScreenUnlock(ESGUI_HDRV handle);

// Return ESGUI driver color format
ESGUI_COLOR_FORMAT esguiScreenColorFormatGet(ESGUI_HDRV handle);

// Return ESGUI driver color bit depth
int esguiScreenBitDepthGet(ESGUI_HDRV handle);

// Return a driver - supported BITOP given requested BITOP
ESGUI_BITOP esguiScreenSupportedBitopGet(ESGUI_HDRV handle, ESGUI_BITOP bop);

// Reset screen hardware. calls either bus or driver service, if set
void esguiScreenHwReset(ESGUI_HDRV handle);

// Locks|unlocks screen refresh (for flicker-free draw)
void esguiScreenLockRefresh(ESGUI_HDRV handle, esBL lock);

// Set backlight level from 0 to 255
void esguiScreenSetBacklight(ESGUI_HDRV handle, esU8 level);
void esguiScreenSetPowerMode(ESGUI_HDRV handle, ESGUI_SCREEN_PWRMODE mode);

// Set screen screen rotation
void esguiScreenSetRotation(ESGUI_HDRV handle, ESGUI_SCREEN_ROTATION rotation);

// Set screen screen rotation
ESGUI_SCREEN_ROTATION esguiScreenGetRotation(ESGUI_HDRV handle);

// Set screen screen position. consecutive lcd memory writes
// will start at this position
//
void esguiScreenSetPos(ESGUI_HDRV handle, int x, int y);

// Set virtual window in which all lcd memory writes will go
void esguiScreenSetWindow(ESGUI_HDRV handle, int x0, int y0, int x1, int y1);

// Reset screen window back to defaults
void esguiScreenResetWindow(ESGUI_HDRV handle);

// Set pixel to color at current position
void esguiScreenSetPixel(ESGUI_HDRV handle, ESGUI_Color_t clr);

// Get pixel from current position
ESGUI_Color_t esguiScreenGetPixel(ESGUI_HDRV handle);

// Fill current screen window area with specified color
void esguiScreenFillWindow(ESGUI_HDRV handle, int x0, int y0, int x1, int y1, ESGUI_Color_t clr);

// Clear screen screen
void esguiScreenClear(ESGUI_HDRV handle, ESGUI_Color_t clr);

// Color conversions
//

#if defined(ESGUI_USE_COLOR) && 1 == ESGUI_USE_COLOR

ESGUI_Color_t esguiScreenFromESGUI_COLOR(ESGUI_HDRV drv, ESGUI_COLOR sclr);
ESGUI_COLOR esguiScreenToESGUI_COLOR(ESGUI_HDRV drv, ESGUI_Color_t sclr);

#endif

// Drawing scoping
void esguiScreenIoBatchBegin(ESGUI_HDRV drv);
void esguiScreenIoBatchEnd(ESGUI_HDRV drv);

// Specific driver includes
//
#ifdef ESGUI_USE_HX5051
#  include <esgui/screen_drivers/hx5051.h>
#endif

#ifdef ESGUI_USE_S6E63D6X
#  include <esgui/screen_drivers/s6e63d6x.h>
#endif

#ifdef ESGUI_USE_SSD1306
#  include <esgui/screen_drivers/ssd1306.h>
#endif

#ifdef ESGUI_USE_ILI9225X
#  include <esgui/screen_drivers/ili9225x.h>
#endif

#ifdef __cplusplus
  }
#endif

#endif // _es_gui_screen_driver_h_

