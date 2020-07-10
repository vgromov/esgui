#ifndef _ssd1289_types_h_
#define _ssd1289_types_h_

#include <include/esgui_utils.h>

// lcd hardware - dependent types and conversions should go here
//

// native ssd1289 color type (16 bit interface mode, 65k colors)
// order in which controller interprets it is depending on value of OUTCTL_ESGUI_RGBORD bit
typedef union {
  esU16 raw;
  struct {
    esU16 b : 5;
    esU16 g : 6;
    esU16 r : 5;
  } rgb;
} lcdColor;

#define LCD_MAX_R  0x1F
#define LCD_MAX_G  0x3F
#define LCD_MAX_B   0x1F

// native type conversion to-from standard color type ESGUI_COLOR
__inline lcdColor ESGUI_COLOR2lcdColor( ESGUI_COLOR clr )  
{
  lcdColor lcdClr;
  // assign system color to lcd color, regarding gamut clipping
  lcdClr.rgb.b = (clr.rgba.b * LCD_MAX_B) / 0xFF;
  lcdClr.rgb.g = (clr.rgba.g * LCD_MAX_G) / 0xFF;
  lcdClr.rgb.r = (clr.rgba.r * LCD_MAX_R) / 0xFF;
  return lcdClr;
}
  
__inline ESGUI_COLOR lcdColor2ESGUI_COLOR(lcdColor lcdClr)
{
  // assign lcd color to system color, regarding gamut clipping
  return ESGUI_RGB((lcdClr.rgb.r * 0xFF) / LCD_MAX_R, 
    (lcdClr.rgb.g * 0xFF) / LCD_MAX_G, (lcdClr.rgb.b * 0xFF) / LCD_MAX_B);
}

// ssd1289 register definitions
#define LCD_REG_OSCSTART        (0x00)                // OSC start register. write esU16(1) to enable, esU16(0) to disable
#define LCD_REG_OUTCTL          (0x01)                // DRV output control register. bit masks follow
  #define OUTCTL_SRC_SHDIR  ((esU16)0x4000)            // Selects the output shift direction of the source driver.  
                                                      // set- S0 shifts to S719 and <R><G><B> color is assigned from S0.  
                                                      // clear S719 shifts to S0 and <R><G><B> color is assigned from S719.  
                                                      // Set RL bit and BGR bit when changing the dot order of R, G and B.  
                                                      // RL setting will be ignored when display with RAM 
  #define OUTCTL_CLR_REV    ((esU16)0x2000)            // HW REVERSE. set - reverse, cleared - normal
  #define OUTCTL_RETCAP      ((esU16)0x1000)            // retention capasitor config. set - Cs on gate, clear - Cs on Common
  #define OUTCTL_RGBORD      ((esU16)0x0800)            // Selects the order from RGB to BGR in writing 18-bit pixel data in the GDDRAM.  
                                                      // crear - <R><G><B> color is assigned from S0.  
                                                      // set - <B><G><R> color is assigned from S0. 
  #define OUTCTL_REV_GATE_SCAN ((esU16)0x0400)          // Change scanning order of gate driver.  
                                                      // clear - G0, G1, G2, G3……G219 (left and right gate interlaced) 
                                                      // set - G0, G2, ……G318, G1, G3, ……G319 
  #define OUTCTL_GATE_SHDIR ((esU16)0x0200)            // Selects the output shift direction of the gate drive
                                                      // set - G0 shifts to G319.  
                                                      // clear - G319 shifts to G0. 
  #define OUTCTL_LINECNT    ((esU16)(LCD_EXT_Y-1))      // Specify number of lines for the LCD driver. value cannot exceed 319.  
                                                      //Remark: When using the partial display, the output for non-display area will be minimum voltage

#define LCD_REG_DRV_WFMCTL        (0x02)              // LCD-Driving-Waveform Control
  #define WFMCTL_INTERLACE    ((esU16)0x1000)           // Set display in interlace drive mode to protect from flicker. It splits one frame into 3 fields and drive.  
                                                      // set - it is 3 field driving, which also limit VPORCH = 1 and cannot be used for RETCAP on gate panel type. 
                                                      // That is DRV_OUTCTL_RETCAP set may not coexist with this bit set.  
                                                      // When clear, it is normal driving. 
  #define WFMCTL_ENWSYNC      ((esU16)0x0800)          // when set, it enables WSYNC output pin. Mode1 or Mode2 is selected by WSMD. 
                                                      // when clear, it disables WSYNC feature, the WSYNC output pin will be high-impedance. 
  #define WFMCTL_BC            ((esU16)0x0400)          // Select the liquid crystal drive waveform VCOM.  
                                                      // When B/C = 0, frame inversion of the LCD driving signal is enabled.  
                                                      // When B/C = 1, a N-line inversion waveform is generated and alternates in a N-line equals to NW[7:0]+1. 
  #define WFMCTL_EOR          ((esU16)0x0200)           // When B/C = 1 and EOR = 1, the odd/even frame-select signals and the N-line inversion signals are EORed for 
                                                      // alternating drive. EOR is used when the LCD is not alternated by combining the set values of the lines of the LCD driven 
                                                      // and the N-lines.
  #define WFMCTL_WSYNC_MODE    ((esU16)0x0100)          // clear - For fast write MCU: MCU should start to write new frame of ram data just after rising edge of long WSYNC pulse and 
                                                      // should be finished well before the rising edge of the next long WSYNC pulse. 
                                                      // e.g. 5MHz 8 bit parallel write cycle for 18 bit color depth, or 3MHz 8 bit parallel write cycle for 16 bit color depth. 
                                                      // set - For slow write MCU (Half the write speed of fast write): MCU should start to write new frame ram data after the rising 
                                                      // edge of the first short WSYNC pulse and must be finished within 2 frames time. 
                                                      // e.g. 2.5MHz 8 bit parallel write cycle for 18 bit color depth. 

#define LCD_REG_PWRCTL1            (0x03)              // Power control 1 (R03h)
typedef enum {
  FLINEx24, 
  FLINEx16, 
  FLINEx12,
  FLINEx8, 
  FLINEx6,
  FLINEx5,
  FLINEx4,
  FLINEx3,
  FLINEx2,
  FLINEx1, 
  FOSC_4, 
  FOSC_6,
  FOSC_8, 
  FOSC_10, 
  FOSC_12, 
  FOSC_16 
} PWRCTL1_DCT;

  #define PWRCTL1_DCT8_MASK    ((esU16)0xF000)            // mask for the following value
  #define PWRCTL1_DCT8(clkVal)  ((esU16)(clkVal) << 12)  // Set the step-up cycle of the step-up circuit for 8-color mode (CM = VDDIO). When the cycle is accelerated, the 
                                                      // driving ability of the step-up circuit increases, but its current consumption increases too. Adjust the cycle taking into 
                                                      // account the display quality and power consumption.

                                                      //   BT2 BT1 BT0  VGH output        VGL output          VGH booster ratio VGL booster ratio 
                                                      //  0   0   0    (VCIX2 +4) x VCI  -(VCIX2 x 3) + VCI   +6                -5 
                                                      //  0   0   1    (VCIX2 +4) x VCI  -(VGH) + VCix2       +6                -4 
                                                      //  0   1   0    (VCIX2 +4) x VCI  -(VGH)               +6                -6 
                                                      //  0   1   1    VCI x 5            -(VGH)               +5                -5 
                                                      //  1   0   0    VCI x 5            -(VGH) + VCI         +5                -4 
                                                      //  1   0   1    VCI x 5            -(VGH) + VCix2       +5                -3 
                                                      //  1   1   0    VCI x 4            -(VGH)               +4                -4 
                                                      //  1   1   1    V  x 4            -(V ) + V            +4                -3 
  #define PWRCTL1_BT_MASK      ((esU16)0x0E00)            // mask for the following value
  #define PWRCTL1_BT(btVal)    ((esU16)(btVal) << 9)      // BT[2:0]:  Control the step-up factor of the step-up circuit. Adjust the step-up factor according to the power-supply 
                                                      // voltage to be used. 
  #define PWRCTL1_DCT256K_MASK  ((esU16)0x00F0)         // mask for the following value
  #define PWRCTL1_DCT256K(clkVal) ((esU16)(clkVal) << 4)// Set the step-up cycle of the step-up circuit for 262k-color mode (CM = VSS). When the cycle is accelerated, the 
                                                      // driving ability of the step-up circuit increases, but its current consumption increases too. Adjust the cycle taking into 
                                                      // account the display quality and power consumption. 
                                                
typedef enum {
  AP_Least,
  AP_Small,
  AP_SmallMed,
  AP_Med,
  AP_MedLarge,
  AP_Large,
  AP_LargeMax,
  AP_Max
} PWRCTL1_AP;

  #define PWRCTL1_AP_MASK    ((esU16)0x000E)                // mask for the following values
  #define PWRCTL1_AP(apVal)  ((esU16)(apVal) << 1)          // Adjust the amount of current from the stable-current source in the internal operational amplifier circuit.
                                                        // Adjust the amount of current from the stable-current source in the internal operational amplifier circuit. When 
                                                        // the amount of current becomes large, the driving ability of the operational-amplifier circuits increase. Adjust the current 
                                                        // taking into account the power consumption. During times when there is no display, such as when the system is in a sleep mode. 
                                                        // AP2 AP1 AP0 Op-amp power 
                                                        //  0   0   0    Least 
                                                        //  0   0   1    Small 
                                                        //  0    1    0    Small to medium 
                                                        //  0   1   1    Medium 
                                                        //  1    0    0    Medium to large 
                                                        //  1   0   1    Large 
                                                        //  1    1    0    Large to Maximum
                                                        //  1   1   1    Maximum

#define LCD_REG_LCDCTL      (0x07)                       // Display Control (R07h)
  #define LCDCTL_NORMALIZE_MASK  ((esU16)0x1800)            // mask for the following values
  #define LCDCTL_NORMALIZE(normVal) ((esU16)(normVal) << 11)  // PT[1:0]: Normalize the source outputs when non-displayed area of the partial display is driven. 
 
  #define LCDCTL_VSCROLL_MASK    ((esU16)0x0600)           // mask for the following values
  #define LCDCTL_VSCROLL(scrlVal) ((esU16)(scrlVal) << 9 ) // VLE[2:1]: When VLE1 = 1 or VLE2 = 1, a vertical scroll is performed in the 1st screen by taking data VL17-0 in R41h
                                                        // register. When VLE1 = 0 and VLE2 = 0, a vertical scroll is performed in the 1st and 2nd screen by VL1[8:0] and VL2[8:0
                                                        // respectively. 
 
  #define LCDCTL_SPLIT      ((esU16)0x0100)                // When set, the 2-division LCD drive is performed. 
  #define LCDCTL_GATEON      ((esU16)0x0020)                // Gate off level becomes VGH when GON = “0”. 
  #define LCDCTL_DTE        ((esU16)0x0010)               // When GON = “1” and DTE = “0”, all gate outputs become VGL. When GON = “1” and DTE = “1”, selected gate
                                                        // wire becomes VGH, and non-selected gate wires become VGL. 
  #define LCDCTL_8CLRMODE    ((esU16)0x0008)               // 8-color mode setting. set - 8-color mode is selected. clear -  8-color mode is disabled.  

  #define LCDCTL_SHOW       ((esU16)0x0002)                // D[1:0]: Display is on when D1 = “1” and off when D1 = “0”. When off, the display data remains in the GDDRAM, and
                                                        // can be displayed instantly by setting D1 = “1”. When D1= “0”, the display is off with all of the source outputs set to the
                                                        // GND level. Because of this, the driver can control the charging current for the LCD with AC driving. 
  #define LCDCTL_ON          ((esU16)0x0001)                // When D[1:0] =10, the internal display is performed although the display  is off. When D[1:0] = “00”, the internal display operation
                                                        // halts and the display is off. Control the display on/off while control GON and DTE. 

#define LCD_REG_FCCTL        (0x0B)                      // Frame Cycle Control (R0Bh)
  #define FCCTL_NOVR_MASK    ((esU16)0xC000)                // mask for the following value
  #define FCCTL_NOVR(novrVal)  ((esU16)(novrVal) << 14)    // Sets amount of non-overlap of the gate output.
  #define FCCTL_DLY_MASK    ((esU16)0x3000)                // mask for the following value
  #define FCCTL_DLY(dlyVal)  ((esU16)(dlyVal) << 12)        // Set delay amount from the gate output signal falling edge of the source outputs. 
  #define FCCTL_EQDLY_MASK  ((esU16)0x0300)                // mask for the following value
  #define FCCTL_EQDLY(dlyVal) ((esU16)(dlyVal) << 8)      // Sets the equalizing period.

typedef enum {
  FCDIV_1,
  FCDIV_2,
  FCDIV_4,
  FCDIV_8    
} FCCTL_DIV;
  #define FCCTL_DIV_MASK    ((esU16)0x00C0)                // mask for the following value
  #define FCCTL_DIV(divVal) ((esU16)(divVal) << 6)        // Set the division ratio of clocks for internal operation. Internal operations are driven by clocks which 
                                                        // frequency is divided according to the DIV1-0 setting.
  #define FCCTL_SDIV        ((esU16)0x0020)                // When SDIV = 1, DIV1-0 value will be count. When SDIV = 0, DIV1-0 value will be auto determined. 
  #define FCCTL_SRTN        ((esU16)0x0010)                // SRTN: When SRTN =1, RTN3-0 value will be count. When SRTN = 0, RTN3-0 value will be auto determined.
  #define FCCTL_RTN_MASK    ((esU16)0x000F)                // mask for the following value
  #define FCCTL_RTN(rtnVal)  ((esU16)(rtnVal))              // RTN[3:0]: Set the no. of clocks in each line. The total number will be the decimal value of RTN3-0 plus 16. e.g. if 
                                                        // RTN3-0 = “1010h”, the total number of clocks in each line = 10 +16 = 26 clocks. 
#define LCD_REG_PWRCTL2      (0x0C)                      // power control 2
  #define PWRCTL2_VRC_MASK  ((esU16)0x0007)                // mask for the following value
  #define PWRCTL2_VRC(vrcVal) ((esU16)(vrcVal))           // VRC[2:0]: Adjust VCIX2 output voltage. The adjusted level is indicated in the chart below VRC2-0 setting.  
                                                        //VRC2 VRC1 VRC0 VCIX2 voltage 
                                                        //  0    0   0      5.1V 
                                                        //  0   0   1      5.2V 
                                                        //  0   1     0      5.3V 
                                                        //  0   1   1      5.4V 
                                                        //  1   0   0      5.5V 
                                                        //  1   0   1      5.6V 
                                                        //  1   1   0      5.7V 
                                                        //  1   1   1      5.8V 
                                                        //  Note: The above setting is valid when VCI has high enough voltage supply for boosting up the required voltage.  
                                                        //  The above setting is assumed 100% booster efficiency. Please refer to DC Characteristics for detail.   

#define LCD_REG_PWRCTL3      (0x0D)                      // Power Control 3 (R0Dh)
  #define PWRCTL3_VRH_MASK  ((esU16)0x000F)                // mask for the following value
  #define PWRCTL3_VRH(vrhVal) ((esU16)(vrhVal))            // VRH[3:0]: Set amplitude magnification of VLCD63. These bits amplify the VLCD63 voltage 1.54 to 2.725 times the Vref
                                                        //voltage set by VRH[3:0]. 
                                                        //  VRH3 VRH2 VRH1 VRH0 VLCD63 Voltage 
                                                        //  0      0    0      0    Vref x 1.540 
                                                        //  0      0    0      1    Vref x 1.620 
                                                        //  0      0    1      0    Vref x 1.700 
                                                        //  0      0    1      1    Vref x 1.780 
                                                        //  0      1    0      0    Vref x 1.850 
                                                        //  0      1    0      1    Vref x 1.930 
                                                        //  0      1    1      0    Vref x 2.020 
                                                        //  0      1    1      1    Vref x 2.090 
                                                        //  1      0    0      0    Vref x 2.165 
                                                        //  1      0    0      1    Vref x 2.245 
                                                        //  1      0    1      0    Vref x 2.335 
                                                        //  1      0    1      1    Vref x 2.400  
                                                        //  1      1    0      0    Vref x 2.500 
                                                        //  1      1    0      1    Vref x 2.570 
                                                        //  1      1    1      0    Vref x 2.645 
                                                        //  1      1    1      1    Vref x 2.725 
                                                        //  *Vref is the internal reference voltage equals to 2.0V. 

#define LCD_REG_PWRCTL4    (0x0E)                        // Power Control 4 (R0Eh)
  #define PWRCTL4_VCOMG    ((esU16)0x2000)                  // VcomG: When VcomG = “1”, it is possible to set output voltage of VcomL to any level, and the instruction (VDV4-0) 
                                                        // becomes available. When VcomG = “0”, VcomL output is fixed to Hi-z level, VCIM output for VcomL power supply 
                                                        // stops, and the instruction (VDV4-0) becomes unavailable. Set VcomG according to the sequence of power supply setting 
                                                        // flow as it relates with power supply operating sequence. 
  #define PWRCTL4_VDV_MASK ((esU16)0x1F00)                // mask for the following value
  #define PWRCTL4_VDV(vdvVal) ((esU16)(vdvVal) << 8)      //VDV[4:0]: Set the alternating amplitudes of Vcom at the Vcom alternating drive. These bits amplify 0.6 to 1.23 times 
                                                        // the VLCD63 voltage. When VcomG = “0”, the settings become invalid. External voltage at VcomR is referenced when 
                                                        // VDH = “01111”. 
                                                        // VCOML = 0.9475*VCOMH - VCOMA 
                                                        //VDV4 VDV3 VDV2 VDV1 VDV0 Vcom Amplitude 
                                                        //0     0   0     0   0      VLCD63 x 0.60 
                                                        //0     0   0     0   1      VLCD63 x 0.63 
                                                        //0     0   0     1   0      VLCD63 x 0.66 
                                                        //        Step = 0.03 
                                                        //0     1   1     0   1      VLCD63 x 0.99 
                                                        //0     1   1     1   0      VLCD63 x 1.02 
                                                        //0     1   1     1   1     Reference from external variable resistor 
                                                        //1     0    0     0   0      VLCD63 x 1.05 
                                                        //1     0   0     0   1      VLCD63 x 1.08 
                                                        //        Step = 0.03 
                                                        //1     0   1     0   1      VLCD63 x 1.20 
                                                        //1     0   1     1   0      VLCD63 x 1.23 
                                                        //1     0   1     1   1      Reserved 
                                                        //1     1    *      *    *      Reserved 
                                                        //      Note: Vcom amplitude < 5.5V 

#define LCD_REG_GATEPOS     (0x0F)                      // Gate Scan Position (R0Fh) 
  #define GATEPOS_MASK      ((esU16)0x01FF)                // mask for the following value
  #define GATEPOS(val)      ((esU16)(val))                // SCN[8:0]: Set the scanning starting position of the gate driver. The valid range is from 0 to 319. 

#define LCD_REG_SLEEP        (0x10)                       // Sleep mode (R10h)

#define LCD_REG_ENTRY        (0x11)                      // Entry Mode (R11h) 
  #define ENTRY_VSYNC        ((esU16)0x8000)               // VSMode: When VSMode = 1 at DMode[1:0] = “00”, the frame frequency will be dependent on VSYNC.
  #define ENTRY_COLORS65K   ((esU16)0x6000)               // DFM[1:0]: Set the color display mode. 
  #define ENTRY_COLORS262K  ((esU16)0x4000)                // DFM1 DFM0 Color mode 
                                                        // 1      1    65k color (POR) 
                                                        // 1       0    262k color 
  #define ENTRY_TRANSP      ((esU16)0x1000)                // When TRANS = 1, transparent display is allowed during DMode[1:0] = “1x”. 
  #define ENTRY_OEDEF        ((esU16)0x0800)                // When OEDef = 1, OE defines the display window. 
                                                        // When OEDef = 0, the display window is defined by R4Eh and R4Fh. 
  #define ENTRY_WMODE       ((esU16)0x0400)                // WMode: Select the source of data to write in the RAM. 
                                                        ///WMode  Write RAM from 
                                                        //0        Normal data bus (POR) 
                                                        //1       Generic interface 
typedef enum {
  DMODE_RAM,
  DMODE_Gen,
  DMODE_RAMGen_ShowGen,
  DMODE_RAMGen_ShowRAM
} DMODE;
  #define ENTRY_DMODE_MASK  ((esU16)0x0300)                // mask for the following value
  #define ENTRY_DMODE(dmodeVal) ((esU16)(dmodeVal) << 8)  // DMode[1:0]: SSD1289 allows data display from RAM data or from generic input data. When DMode[1:0] = “00”, it 
                                                        // displays the ram content. When DMode[1:0] = “01”, it displays from generic input data. 
                                                        //DMode1 DMode0  Display  Remark 
                                                        //0        0        Ram (POR)  Frame frequency depends on Fosc (POR) 
                                                        //0        1        Generic input  Frame frequency depends on VSYNC, default DMode setting in ESGUI_RGB interface 
                                                        //1        0        RAM and Generic data  In this case, generic data is shown in  the display window 
                                                        //1        1        RAM and Generic data  In this case, RAM data is shown in the display window 
typedef enum {
  ADDR_HDEC_VDEC,
  ADDR_HINC_VDEC,
  ADDR_HDEC_VINC,
  ADDR_HINC_VINC

} ADDR_mode;
  #define ENTRY_ADDRINC_MASK              ((esU16)0x0030)              // mask for the following value
  #define ENTRY_ADDRINC_MODE(addrModeVal)  ((esU16)(addrModeVal) << 4)  // address counter increment mode
  #define ENTRY_VORIENTATION              ((esU16)0x0008)              // address space orientation

#define LCD_REG_HPORCH    (0x16)                         // Horizontal Porch (R16h)
  #define HPORCH_PPL_MASK  ((esU16)0xFF00)                  // mask for the following value
  #define HPORCH_PPL(ppl)  ((esU16)(ppl) << 8)              // XL[7:0]: Set the number of valid pixel per line. Number of valid pixel per line is equal to XL[7:0] + 1 
  #define HPORCH_HSYNC_DLY_MASK ((esU16)0x00FF)            // mask for the following value
  #define HPORCH_HSYNC_DLY(dlyVal) ((esU16)(dlyVal))      // HBP[7:0]: Set the delay period from falling edge of HSYNC signal to first valid data. The pixel data exceed the range 
                                                        //set by XL[7:0] and before the first valid data will be treated as dummy data.  
                                                        //HBP7 HBP6 HBP5 HBP4 HBP3 HBP2 HBP1 HBP0   No. of clock cycle of DOTCLK 
                                                        //0     0     0   0   0     0     0  0      2 
                                                        //0 0 0 0 0 0 0  1  3 
                                                        // 
                                                        //Step = 1 
                                                        // 
                                                        //0 0 0 1 1 0 1  0  28 
                                                        //0 0 0 1 1 0 1  1  29 
                                                        //0 0 0 1 1 1 0  0  30 (POR) 
                                                        //0 0 0 1 1 1 0  1  31 
                                                        //0 0 0 1 1 1 1  0  32 
                                                        // 
                                                        //Step = 1 
                                                        // 
                                                        //1 1 1 1 1 1 1  0  256 
                                                        //1 1 1 1 1 1 1  1  257 

#define LCD_REG_VPORCH    (0x17)                         // Vertical Porch (R17h) (POR = 0103h)  
  #define VPORCH_VFPDLY_MASK ((esU16)0x7F00)              // mask for the following value
  #define VPORCH_VFPDLY(dlyVal) ((esU16)(dlyVal) << 8)    // VFP[6:0]:  Set the delay period from the last valid line to the falling edge of VSYNC of the next frame. The line data 
                                                        // within this delay period will be treated as dummy line.  
                                                        //VFP6 VFP5 VFP4 VFP3 VFP2 VFP1 VFP0 No. of clock cycle of HSYNC 
                                                        //0 0 0 0 0 0 0  1  
                                                        //0 0 0 0 0 0 1  2 (POR) 
                                                        // 
                                                        //Step = 1 
                                                        // 
                                                        //1 1 1 1 1 1 0  127 
                                                        //1 1 1 1 1 1 1  128 
  #define VPORCH_VBPDLY_MASK ((esU16)0x00FF)               // mask for the following value
  #define VPORCH_VBPDLY(dlyVal) ((esU16)(dlyVal))          // VBP[7:0]: Set the delay period from falling edge of VSYNC to first valid line. The line data within this delay period 
                                                        //will be treated as dummy line.  
                                                        //VBP7 VBP6 VBP5 VBP4 VBP3 VBP2 VBP1 VBP0 No. of clock cycle of HSYNC 
                                                        //0 0 0 0 0 0 0 0  1 
                                                        //0 0 0 0 0 0 0 1  2 
                                                        //0 0 0 0 0 0 1 0  3 
                                                        //0 0 0 0 0 0 1 1  4 (POR) 
                                                        // 
                                                        //Step = 1 
                                                        // 
                                                        //1 1 1 1 1 1 1 0  255 
                                                        //1 1 1 1 1 1 1 1  256 

#define LCD_REG_PWRCTL5    (0x1E)                         // Power Control 5 (R1Eh) (POR = 0029h) 
  #define PWRCTL5_OTP      ((esU16)0x0080)                  // nOTP: nOTP equals to “0” after power on reset and VcomH voltage equals to programmed OTP value. When nOTP set 
                                                        // to “1”, setting of VCM[5:0] becomes valid and voltage of VcomH can be adjusted.  
   #define PWRCTL5_VCM_MASK ((esU16)0x003F)                // mask for the following value
  #define PWRCTL5_VCM(vcmVal) ((esU16)(vcmVal))            // VCM[5:0]:  Set the VcomH voltage if nOTP = “1”. These bits amplify the VcomH voltage 0.35 to 0.99 times the 
                                                        // VLCD63 voltage. Default value is “101001” when power on reset. 
                                                        //VCM5 VCM4 VCM3 VCM2 VCM1 VCM0 VcomH 
                                                        //0 0 0 0 0 0 VLCD63 x 0.35 
                                                        //0 0 0 0 0 1 VLCD63 x 0.36 
                                                        // 
                                                        //Step = 0.01 
                                                        // 
                                                        //1 1 1 1 1 0 VLCD63 x 0.98 
                                                        //1 1 1 1 1 1 VLCD63 x 0.99  

#define LCD_REG_GRAMRW        (0x22)                    // read\write data from\to Graphics RAM (interface mapping-dependent)

#define LCD_REG_GRAMWMASK_LO   (0x23)                     // RAM write data mask (R23h – R24h) (POR = 0000h) 
#define LCD_REG_GRAMWMASK_HI   (0x24)

#define LCD_REG_HRAMADDR_POS  (0x44)                     // Horizontal RAM address position (R44h) (POR = EF00h) 
  #define HRAMADDR_POS_BEG_MASK  ((esU16)0x00FF)          // mask for the following value
  #define HRAMADDR_POS_BEG(begVal) ((esU16)(begVal))     //HSA[7:0]/HEA[7:0]: Specify the start/end positions of the window address in the horizontal direction by an address unit. 
  #define HRAMADDR_POS_END_MASK  ((esU16)0xFF00)          //Data are written to the GDDRAM within the area determined by the addresses specified by HEA[7:0] and HSA[7:0]. 
  #define HRAMADDR_POS_END(endVal) ((esU16)(endVal) << 8)//These addresses must be set before the RAM write. In setting these bits, make sure that “00”h <= HSA[7:0] <= HEA[7:0] <= 
                                                        //“EF”h. 
 
#define LCD_REG_VRAMADDR_POS_START   (0x45)              // Vertical RAM address position (R45h-R46h)  
#define LCD_REG_VRAMADDR_POS_END    (0x46)              // VSA[8:0]/VEA[8:0]: Specify the start/end positions of the window address in the vertical direction by an address unit. 
                                                        // Data are written to the GRAM within the area determined by the addresses specified by VEA[8:0] and VSA[8:0]. These 
                                                        // addresses must be set before the RAM write.  In setting these bits, make sure that “00”h  <= VSA[8:0]  <= VEA[8:0]  <= 
                                                        // “13F”h. 

#define LCD_REG_RAMADDR_HPOS         (0x4E)              // RAM address set (R4Eh-R4Fh) 
#define LCD_REG_RAMADDR_VPOS        (0x4F)              // XAD[7:0]: Make initial settings for the GDDRAM X address in the address counter (AC). 
                                                        // YAD[8:0]: Make initial settings for the GDDRAM Y address in the address counter (AC). 


#endif // _ssd1289_types_h_

