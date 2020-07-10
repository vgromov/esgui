// screen driver file.
// the particular lcd driver is included depending on
// esguiConfig settings
#include <esgui/include/esgui.h>
#include <esfwxe/ese_assert.h>
#include <string.h>
//----------------------------------------------------------------------------------------------

// bus interface services implementation
//
// reset all bus internals to 0
void esguiScreenBusHandleInit(ESGUI_HBUS hbus)
{
  if( !hbus )
    return;

  hbus->init = NULL;
  hbus->reset = NULL;
  hbus->lock = NULL;
  hbus->unlock = NULL;
  hbus->selectScreen = NULL;
  hbus->deselectScreen = NULL;
  // 8 bit interface
  hbus->writeCmdB = NULL;
  hbus->writeDataB = NULL;
  // 16 bit interface
  hbus->writeCmdW = NULL;
  hbus->writeDataW = NULL;
  // 32 bit interface
  hbus->writeCmdDW = NULL;
  hbus->writeDataDW = NULL;
}
//----------------------------------------------------------------------------------------------

// initialize bus hardware
esBL esguiScreenBusInit(ESGUI_HBUS hbus)
{
  if( hbus && hbus->init )
    return hbus->init(hbus);
    
  return FALSE;
}
//----------------------------------------------------------------------------------------------

// lock|unlock bus for exclusive access
esBL esguiScreenBusLock(ESGUI_HBUS hbus)
{
  if( hbus && hbus->lock )
    return hbus->lock(hbus);
    
  return TRUE;  
}
//----------------------------------------------------------------------------------------------

void esguiScreenBusUnlock(ESGUI_HBUS hbus)
{
  if( hbus && hbus->unlock )
    hbus->unlock(hbus);
}
//----------------------------------------------------------------------------------------------

// screen bus io
//
// 8 bit
void esguiScreenBusWriteCmdB(ESGUI_HBUS hbus, esU8 cmd)
{
  if( hbus && hbus->writeCmdB )
    hbus->writeCmdB(hbus, cmd);
}
//----------------------------------------------------------------------------------------------

void esguiScreenBusWriteDataB(ESGUI_HBUS hbus, esU8 data)
{
  if( hbus && hbus->writeDataB )
    hbus->writeDataB(hbus, data);
}
//----------------------------------------------------------------------------------------------

void esguiScreenBusReadDataB(ESGUI_HBUS hbus, esU8* data)
{
  if( hbus && hbus->readDataB )
    hbus->readDataB(hbus, data);
}
//----------------------------------------------------------------------------------------------

// 16 bit
void esguiScreenBusWriteCmdW(ESGUI_HBUS hbus, esU16 cmd)
{
  if( hbus && hbus->writeCmdW )
    hbus->writeCmdW(hbus, cmd);
}
//----------------------------------------------------------------------------------------------

void esguiScreenBusWriteDataW(ESGUI_HBUS hbus, esU16 data)
{
  if( hbus && hbus->writeDataW )
    hbus->writeDataW(hbus, data);
}
//----------------------------------------------------------------------------------------------

void esguiScreenBusReadDataW(ESGUI_HBUS hbus, esU16* data)
{
  if( hbus && hbus->readDataW )
    hbus->readDataW(hbus, data);
}
//----------------------------------------------------------------------------------------------

// 32 bit
void esguiScreenBusWriteCmdDW(ESGUI_HBUS hbus, esU32 cmd)
{
  if( hbus && hbus->writeCmdDW )
    hbus->writeCmdDW(hbus, cmd);
}
//----------------------------------------------------------------------------------------------

void esguiScreenBusWriteDataDW(ESGUI_HBUS hbus, esU32 data)
{
  if( hbus && hbus->writeDataDW )
    hbus->writeDataDW(hbus, data);
}
//----------------------------------------------------------------------------------------------

void esguiScreenBusReadDataDW(ESGUI_HBUS hbus, esU32* data)
{
  if( hbus && hbus->readDataDW )
    hbus->readDataDW(hbus, data);
}
//----------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
// driver interface services implementation
//

void esguiScreenDriverHandleInit(ESGUI_HDRV handle, esU32 drvStructSize)
{
  if( handle )
  {
    memset(
      handle,
      0,
      drvStructSize
    );

    handle->rotation = ESGUI_SCREEN_ROT_INVALID;
    handle->colorFmt = esguiClrfmtInvalid;
  }
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR_FORMAT esguiScreenColorFormatGet(ESGUI_HDRV handle)
{
  if( !handle )
    return esguiClrfmtInvalid;

  return handle->colorFmt;
}
//---------------------------------------------------------------------------

int esguiScreenBitDepthGet(ESGUI_HDRV handle)
{
  if( !handle )
    return 0;

  return handle->bpp;
}
//---------------------------------------------------------------------------

ESGUI_BITOP esguiScreenSupportedBitopGet(ESGUI_HDRV handle, ESGUI_BITOP bop)
{
  if( !handle )
    return ESGUI_COPY;

  if( ESGUI_COPY == bop )
    return ESGUI_COPY;
  else if( handle->getPixel )
    return bop;

  return ESGUI_COPY;
}
//---------------------------------------------------------------------------

esBL esguiScreenInit(ESGUI_HDRV handle, ESGUI_HBUS hbus, ESGUI_COLOR_FORMAT drvColorFmt)
{
  if( !handle )
    return FALSE;
  
  handle->hbus = hbus;
  
  handle->colorFmt = drvColorFmt;
  handle->bpp = esguiBppFromColorFormat(drvColorFmt);

  if( handle->init )
    return handle->init(handle, drvColorFmt);
    
  return TRUE;
}
//----------------------------------------------------------------------------------------------

esBL esguiScreenLock(ESGUI_HDRV handle)
{
  if( !handle || !handle->hbus )
    return FALSE;
    
  return esguiScreenBusLock(handle->hbus);
}
//----------------------------------------------------------------------------------------------

void esguiScreenUnlock(ESGUI_HDRV handle)
{
  if( !handle || !handle->hbus )
    return;
  
  esguiScreenBusUnlock(handle->hbus);
}
//----------------------------------------------------------------------------------------------

// reset screen hardware. calls either bus or driver service, if set
void esguiScreenHwReset(ESGUI_HDRV handle)
{
  if( handle )
  {
    // first, try driver level
    if( handle->reset )
      handle->reset(handle);
    else if( handle->hbus && handle->hbus->reset ) // otherwise, call bus service, if any
      handle->hbus->reset(handle->hbus);
  }
}
//----------------------------------------------------------------------------------------------

// locks|unlocks screen refresh (for flicker-free draw)
void esguiScreenLockRefresh(ESGUI_HDRV handle, esBL lock)
{
  if( handle && handle->lockRefresh )
    handle->lockRefresh(handle, lock);
}
//----------------------------------------------------------------------------------------------

// set backlight level from 0 to 255
void esguiScreenSetBacklight(ESGUI_HDRV handle, esU8 level)
{
  if( handle && handle->setBacklight )
    handle->setBacklight(handle, level);
}
//----------------------------------------------------------------------------------------------

void esguiScreenSetPowerMode(ESGUI_HDRV handle, ESGUI_SCREEN_PWRMODE mode)
{
  if( handle && handle->setPowerMode )
    handle->setPowerMode(handle, mode);
}
//----------------------------------------------------------------------------------------------

// set screen rotation
void esguiScreenSetRotation(ESGUI_HDRV handle, ESGUI_SCREEN_ROTATION rotation)
{
  if( handle && handle->setRotation && handle->rotation != rotation )
  {
    handle->setRotation(handle, rotation);
    handle->rotation = rotation;
  }
}
//----------------------------------------------------------------------------------------------

// get screen rotation
ESGUI_SCREEN_ROTATION esguiScreenGetRotation(ESGUI_HDRV handle)
{
  if( handle )
    return handle->rotation;

  return ESGUI_SCREEN_ROT_INVALID;
}
//----------------------------------------------------------------------------------------------

// set screen position. consecutive memory writes
// will start at this position
void esguiScreenSetPos(ESGUI_HDRV handle, int x, int y)
{
  if( handle && handle->setPos )
    handle->setPos(handle, x, y);
}
//----------------------------------------------------------------------------------------------

// set virtual window in which all memory writes will go
void esguiScreenSetWindow(ESGUI_HDRV handle, int x0, int y0, int x1, int y1)
{
  if( handle && handle->setWindow )
    handle->setWindow(handle, x0, y0, x1, y1);
}
//----------------------------------------------------------------------------------------------

// reset screen window back to defaults
void esguiScreenResetWindow(ESGUI_HDRV handle)
{
  // if we have explicit implementation or reset window - use it
  if( handle && handle->resetWindow )
    handle->resetWindow(handle);
}
//----------------------------------------------------------------------------------------------

// set pixel to color at current position
void esguiScreenSetPixel(ESGUI_HDRV handle, ESGUI_Color_t clr)
{
  if( handle && handle->setPixel )
    handle->setPixel(handle, clr);
}
//----------------------------------------------------------------------------------------------

// get pixel from current position
ESGUI_Color_t esguiScreenGetPixel(ESGUI_HDRV handle)
{
  if( handle && handle->getPixel )
    return handle->getPixel(handle);
    
  return 0;
}
//----------------------------------------------------------------------------------------------

// fill current screen window area with specified color
void esguiScreenFillWindow(ESGUI_HDRV handle, int x0, int y0, int x1, int y1, ESGUI_Color_t clr)
{
  if( handle && handle->fillWindow )
    handle->fillWindow(handle, x0, y0, x1, y1, clr);
}
//----------------------------------------------------------------------------------------------

// clear screen screen
void esguiScreenClear(ESGUI_HDRV handle, ESGUI_Color_t clr)
{
  // if we have explicit implementation of display clear - use it
  if( handle->clear )
    handle->clear(handle, clr);
}
//----------------------------------------------------------------------------------------------

#if defined(ESGUI_USE_COLOR) && 1 == ESGUI_USE_COLOR

// color conversions
ESGUI_Color_t esguiScreenFromESGUI_COLOR(ESGUI_HDRV handle, ESGUI_COLOR clr)
{
  if( !handle )
    return 0;
  
  ESGUI_Color_t out = 0;
  ESGUI_RESULT result = esguiColorToColor(
    &out,
    handle->colorFmt,
    clr.raw,
    esguiClrfmtNative,
    NULL
  );
  ESE_ASSERT(esguiResultOk == result);

  return out;
}
//----------------------------------------------------------------------------------------------

ESGUI_COLOR esguiScreenToESGUI_COLOR(ESGUI_HDRV handle, ESGUI_Color_t clr)
{
  ESGUI_COLOR out;
  out.raw = 0;

  if( !handle )
    return out;

  ESGUI_RESULT result = esguiColorToColor(
    &out.raw,
    esguiClrfmtNative,
    clr,
    handle->colorFmt,
    NULL
  );
  ESE_ASSERT(esguiResultOk == result);

  return out;
}
#endif
//----------------------------------------------------------------------------------------------

// drawing scoping
void esguiScreenIoBatchBegin(ESGUI_HDRV drv)
{
  if( drv )
  {
    if( 0 == drv->ioBatchNesting && 
        drv->hbus && 
        drv->hbus->selectScreen )
      drv->hbus->selectScreen(drv->hbus);
    
    ++drv->ioBatchNesting;
  }
}
//----------------------------------------------------------------------------------------------

void esguiScreenIoBatchEnd(ESGUI_HDRV drv)
{
  if( drv )
  {
    if( drv->ioBatchNesting )
    {
      --drv->ioBatchNesting;
      
      if( 0 == drv->ioBatchNesting && 
          drv->hbus && 
          drv->hbus->deselectScreen )
        drv->hbus->deselectScreen(drv->hbus);
    }
  }
}
//----------------------------------------------------------------------------------------------

// specific hardware drivers code includes
//
#ifndef USE_EMULATOR

#  ifdef ESGUI_USE_HX5051
#    include <esgui/screen_drivers/hx5051.cc>
#  endif

#  ifdef ESGUI_USE_S6E63D6X
#    include <esgui/screen_drivers/s6e63d6x.cc>
#  endif

#  ifdef ESGUI_USE_SSD1306
#    include <esgui/screen_drivers/ssd1306.cc>
#  endif

# ifdef ESGUI_USE_ILI9225X
#    include <esgui/screen_drivers/ili9225x.cc>
# endif

#endif  // USE_EMULATOR
