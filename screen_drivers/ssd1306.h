#ifndef _ssd_1306_h_
#define _ssd_1306_h_

#ifdef __cplusplus
  extern "C" {
#endif

// addressing modes
typedef enum {
  ssd1306AddrModeHorizontal     = 0,
  ssd1306AddrModeVertical       = 1,
  ssd1306AddrModePage           = 2
  
} ssd1306AddrMode;

typedef enum {
  ssd1306SegmapNormal           = 0,
  ssd1306SegmapRemap            = 1
  
} ssd1306Segmap;

typedef enum {
  ssd1306ComscanInc             = 0,
  ssd1306ComscanDec             = 0x08
  
} ssd1306Comscan;

typedef enum {
  ssd1306ComPinsCfgSequential   = 0,
  ssd1306ComPinsCfgAlternative  = 0x10,
  ssd1306ComPinsCfgNoLR_Remap   = 0,
  ssd1306ComPinsCfgLR_Remap     = 0x20
  
} ssd1306ComPinsCfg;

typedef enum {
  ssd1306BitOrderNormal,
  ssd1306BitOrderInverse
  
} ssd1306BitOrder;

// screen driver setup service
void ssd1306ScreenDriverSetup(ESGUI_HDRV hdrv, int extx, int exty);
    
// display-specific  stuff
//
// switch display on|off
void ssd1306DisplayOn(ESGUI_HDRV hdrv, esBL on);
// charge pump enable|disable
void ssd1306ChargePumpEnable(ESGUI_HDRV hdrv, esBL enable);
// set pre-charge period for phase1 1..15 and phase 2 1..15
void ssd1306PrechargePeriodSet(ESGUI_HDRV hdrv, esU8 pp1, esU8 pp2);
// set display clock ratio: 1..16 and osc frequency: 0..15
void ssd1306ClkRatioOscFreqSet(ESGUI_HDRV hdrv, esU8 clkRatio, esU8 oscFreq);
// set multiplex ratio: 16..64
void ssd1306MuxRatioSet(ESGUI_HDRV hdrv, esU8 ratio);
// set vcomh deselect level 0..7
void ssd1306VcomhDeselectLevelSet(ESGUI_HDRV hdrv, esU8 level);
// set display start line 0..63
void ssd1306StartLineSet(ESGUI_HDRV hdrv, esU8 line);
// set display offset
void ssd1306DisplayOffsetSet(ESGUI_HDRV hdrv, esU8 offs);
// set-reset display inversed mode
void ssd1306InversedModeSet(ESGUI_HDRV hdrv, esBL inversed);
// resume display output from RAM
void ssd1306DisplayRamResume(ESGUI_HDRV hdrv, esBL resume);
// set display contrast
void ssd1306ContrastSet(ESGUI_HDRV hdrv, esU8 contrast);
// set col segment remap
void ssd1306ColSegRemap(ESGUI_HDRV hdrv, esBL remap);
// set com pins hardware configuration
void ssd1306ComPinsConfigSet(ESGUI_HDRV hdrv, esU8 cfg);
// set com output scan direction
void ssd1306ComOutNormalScanSet(ESGUI_HDRV hdrv, esBL normal);
// set addressing mode
void ssd1306AddrModeSet(ESGUI_HDRV hdrv, ssd1306AddrMode am);

// page mode stuff
//
// set start column
void ssd1306StartColSet(ESGUI_HDRV hdrv, esU8 colStart);
// set start page
void ssd1306StartPageSet(ESGUI_HDRV hdrv, esU8 pgStart);

// vertical-horizontal mode stuff
//
// set column range
void ssd1306ColRangeSet(ESGUI_HDRV hdrv, esU8 colStart, esU8 colEnd);
// set page range
void ssd1306PageRangeSet(ESGUI_HDRV hdrv, esU8 pgStart, esU8 pgEnd);

// RAM Window
void ssd1306SetWindow(ESGUI_HDRV hdrv, int x0, int y0, int x1, int y1);
void ssd1306ResetWindow(ESGUI_HDRV hdrv);

// direct bit blitting
void ssd1306BitBlt(ESGUI_HDRV hdrv, const esU8* data, esU32 len, ssd1306BitOrder bitOrder);
// direct display RAM fill
void ssd1306Fill(ESGUI_HDRV hdrv, esU8 val, esU32 len, ssd1306BitOrder bitOrder);

#ifdef __cplusplus
  }
#endif
    
#endif // _ssd_1306_h_
