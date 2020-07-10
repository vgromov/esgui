// ssd1289 lcd driver interface implementation
// isron is2102b part is commented out
#ifndef _ssd_1289_cc_
#define _ssd_1289_cc_

#include <utils.h>

// initialize lcd hardware, but do not turn display on, so far if possible
esBL lcdInit(void)
{
	// lcdBusInit and lcdBacklightInit are hardware platform (MCU, board etc.) dependent and
	// should be defined separately in the particular project
	if( lcdInitBusHW() && lcdInitBacklightHW() )	
	{
		LCD_CMD_WRITE(LCD_REG_OSCSTART, 1); // osc start
	  LCD_CMD_WRITE(LCD_REG_OUTCTL, OUTCTL_LINECNT|OUTCTL_RGBORD|OUTCTL_GATE_SHDIR|OUTCTL_CLR_REV); // driver output control
	  LCD_CMD_WRITE(LCD_REG_DRV_WFMCTL, WFMCTL_BC|WFMCTL_EOR); // LCD drive waveform control
	  LCD_CMD_WRITE(LCD_REG_PWRCTL1, PWRCTL1_DCT8(FOSC_4)|PWRCTL1_BT(8)|PWRCTL1_DCT256K(FOSC_4)|PWRCTL1_AP(AP_LargeMax)); //Power Control (1)  0xA8A6
	 	LCD_CMD_WRITE(LCD_REG_LCDCTL, LCDCTL_GATEON|LCDCTL_DTE|LCDCTL_SHOW|LCDCTL_ON); //Display Control
		LCD_CMD_WRITE(LCD_REG_ENTRY, ENTRY_COLORS65K|ENTRY_OEDEF|ENTRY_DMODE(DMODE_RAM)|ENTRY_ADDRINC_MODE(ADDR_HINC_VINC)); //Set Display color mode for 65k color
		LCD_CMD_WRITE(LCD_REG_PWRCTL2, PWRCTL2_VRC(5)); //Power Control (2)
	  LCD_CMD_WRITE(LCD_REG_PWRCTL3, PWRCTL3_VRH(11)); //Power Control (3)
	  LCD_CMD_WRITE(LCD_REG_PWRCTL4, PWRCTL4_VCOMG); //Power Control (4)
	  LCD_CMD_WRITE(LCD_REG_SLEEP, 0); //Exit Sleep Mode
		msDelay(30);                //delay ~30ms
	  LCD_CMD_WRITE(LCD_REG_PWRCTL5, PWRCTL5_OTP|PWRCTL5_VCM(40)); //Power Control (5)

	  return TRUE;
	}

	return FALSE;
}

// locks|unlocks lcd refresh (for flicker-free draw)
void lcdLockRefresh(esBL bLock)
{
	esU16 wReg = LCDCTL_GATEON|LCDCTL_DTE;

	if( bLock )
		wReg |= LCDCTL_SHOW;
	else
		wReg |= LCDCTL_SHOW | LCDCTL_ON;

	LCD_CMD_WRITE(LCD_REG_LCDCTL, wReg);
}

// set lcd power mode
void lcdSetPowerMode(LCD_PWRMODE nMode)
{
	esU16 wReg = LCDCTL_GATEON|LCDCTL_DTE;
	
	switch( nMode )
	{
	case LCD_PWROFF:
		wReg &= ~(LCDCTL_SHOW | LCDCTL_ON);
		break;
	case LCD_PWRSTDBY:
		wReg = (wReg | LCDCTL_SHOW ) & ~LCDCTL_ON;
		break;
	default: // ON mode is default
		wReg |= LCDCTL_ON | LCDCTL_SHOW;
	}

	LCD_CMD_WRITE(LCD_REG_LCDCTL, wReg);
}

// set\get lcd screen	rotation
void lcdSetScreenRotation(LCD_SCREEN_ROTATION nRotation)
{
	switch( nRotation )
	{
	case LCD_ROT_0:
		LCD_CMD_WRITE(LCD_REG_OUTCTL, OUTCTL_LINECNT|OUTCTL_RGBORD|OUTCTL_CLR_REV|OUTCTL_GATE_SHDIR);
		// set h and v increment, horizontal orientation
		LCD_CMD_WRITE(LCD_REG_ENTRY, ENTRY_COLORS65K|ENTRY_OEDEF|ENTRY_DMODE(DMODE_RAM)|ENTRY_ADDRINC_MODE(ADDR_HINC_VINC) );
		break;
	case LCD_ROT_180:
		LCD_CMD_WRITE(LCD_REG_OUTCTL, OUTCTL_LINECNT|OUTCTL_RGBORD|OUTCTL_CLR_REV|OUTCTL_SRC_SHDIR);
		// set h and v increment, horizontal orientation
		LCD_CMD_WRITE(LCD_REG_ENTRY, ENTRY_COLORS65K|ENTRY_OEDEF|ENTRY_DMODE(DMODE_RAM)|ENTRY_ADDRINC_MODE(ADDR_HINC_VINC) );
		break;
	case LCD_ROT_90CW:
		LCD_CMD_WRITE(LCD_REG_OUTCTL, OUTCTL_LINECNT|OUTCTL_RGBORD|OUTCTL_CLR_REV|OUTCTL_GATE_SHDIR|OUTCTL_SRC_SHDIR);
		// set h and v increment, vertical orientation
		LCD_CMD_WRITE(LCD_REG_ENTRY, ENTRY_COLORS65K|ENTRY_OEDEF|ENTRY_DMODE(DMODE_RAM)|ENTRY_VORIENTATION|ENTRY_ADDRINC_MODE(ADDR_HINC_VINC));
		break; 
	case LCD_ROT_90CCW:
		LCD_CMD_WRITE(LCD_REG_OUTCTL, OUTCTL_LINECNT|OUTCTL_RGBORD|OUTCTL_CLR_REV);
		// set h and v increment, vertical orientation
		LCD_CMD_WRITE(LCD_REG_ENTRY, ENTRY_COLORS65K|ENTRY_OEDEF|ENTRY_DMODE(DMODE_RAM)|ENTRY_VORIENTATION|ENTRY_ADDRINC_MODE(ADDR_HINC_VINC));		
		break;
	}
}

// set lcd screen position. consecutive lcd memory writes
// will start at this position
void lcdSetPos(int x, int y)
{
	LCD_CMD_WRITE(LCD_REG_RAMADDR_HPOS, x); // starting x address
	LCD_CMD_WRITE(LCD_REG_RAMADDR_VPOS, y); // starting y address 
}

// set virtual window in which all lcd memory writes will go
void lcdSetWindow(int x0, int y0, int x1, int y1)
{
	// set window
	LCD_CMD_WRITE(LCD_REG_HRAMADDR_POS, HRAMADDR_POS_BEG(x0)|HRAMADDR_POS_END(x1));
	LCD_CMD_WRITE(LCD_REG_VRAMADDR_POS_START, y0);
	LCD_CMD_WRITE(LCD_REG_VRAMADDR_POS_END, y1);
	// set position
	lcdSetPos(x0, y0);
}

// reset lcd window back to defaults
void lcdResetWindow(void)
{
	lcdSetWindow(0, 0, LCD_EXT_X-1, LCD_EXT_Y-1);
}

// set pixel to color at current position
void lcdSetPixel(lcdColor clr)
{
	LCD_DATA_WRITE( clr.raw );
}

// get pixel from current position
lcdColor lcdGetPixel(void)
{
	lcdColor clrResult;
	LCD_DATA_READ( clrResult.raw );

	return clrResult;  // read data 
}

// fill current lcd window area with specified color
void lcdFillWindow(int x0, int y0, int x1, int y1, lcdColor clr)
{
	int iTmp = x0;

	lcdSetWindow( x0, y0, x1, y1 );

	// perform fill operation
	while(y0 <= y1)
	{
		while(x0 <= x1)
		{
			LCD_DATA_WRITE(clr.raw);
			++x0;
		}

		x0 = iTmp;
		++y0;
	} 
}

/******************************************************************************
**                            ISRON commands. to be implemented separately.
******************************************************************************/
  /* set Display Window */
//  LCD_COMMAND = 0x0110;
//  LCD_COMMAND = 0x0500;
//  LCD_COMMAND = 0x4200;
//  LCD_COMMAND = 0x4300;
//  LCD_COMMAND = 0x4400;
//  LCD_COMMAND = 0x4500;
//  mdelay(10);
//  LCD_COMMAND = 0x46EF;
//  LCD_COMMAND = 0x4700;
//  LCD_COMMAND = 0x4800;
//  LCD_COMMAND = 0x4901;
//  LCD_COMMAND = 0x4A3F;
//  LCD_COMMAND = 0x0200;
//  LCD_COMMAND = 0x0D00;
//  LCD_COMMAND = 0x0E00;
//  LCD_COMMAND = 0x0F00;
//
//  LCD_COMMAND = 0x1000;
//  LCD_COMMAND = 0x1100;
//  LCD_COMMAND = 0x1200;
//  LCD_COMMAND = 0x1300;
//  LCD_COMMAND = 0x1400;
//  LCD_COMMAND = 0x1500;
//  LCD_COMMAND = 0x1600;
//  LCD_COMMAND = 0x1700;
//  LCD_COMMAND = 0x1D08;
//  mdelay(10);
//  LCD_COMMAND = 0x2300;
//  mdelay(10);
//  LCD_COMMAND = 0x2D01;
//  LCD_COMMAND = 0x3301;
//  LCD_COMMAND = 0x3401;
//  LCD_COMMAND = 0x3500;
//  LCD_COMMAND = 0x3701;
//  mdelay(10);
//  LCD_COMMAND = 0x3E01;
//  LCD_COMMAND = 0x3F3F;
//  LCD_COMMAND = 0x4008;
//  LCD_COMMAND = 0x410A;
//  LCD_COMMAND = 0x4C00;
//  LCD_COMMAND = 0x4D01;
//  LCD_COMMAND = 0x4E3F;
//  LCD_COMMAND = 0x4F00;
//  LCD_COMMAND = 0x5000;
//  LCD_COMMAND = 0x7600;
//  LCD_COMMAND = 0x8600;
//  LCD_COMMAND = 0x8736;
//  LCD_COMMAND = 0x8806;
//  LCD_COMMAND = 0x8904;
//  LCD_COMMAND = 0x8B3F;
//  LCD_COMMAND = 0x8D01;

  /* adjust GAMMA */
//  LCD_COMMAND = 0x8F00;
//  LCD_COMMAND = 0x9022;
//  LCD_COMMAND = 0x9167;
//  LCD_COMMAND = 0x9240;
//  LCD_COMMAND = 0x9307;
//  LCD_COMMAND = 0x9412;
//  LCD_COMMAND = 0x9522;
//  LCD_COMMAND = 0x9600;
//  LCD_COMMAND = 0x9707;
//  LCD_COMMAND = 0x9873;
//  LCD_COMMAND = 0x9901;
//  LCD_COMMAND = 0x9A21;
//  LCD_COMMAND = 0x9B24;
//  LCD_COMMAND = 0x9C42;
//  LCD_COMMAND = 0x9D01;
//  mdelay(100);
//  LCD_COMMAND = 0x2494;
//  mdelay(10);
//  LCD_COMMAND = 0x256F;
//  mdelay(110);

  /* power on sequence, step 2 */
//  LCD_COMMAND = 0x2812;
//  mdelay(10);
//  LCD_COMMAND = 0x1900;
//  mdelay(10);
//  LCD_COMMAND = 0x2110;
//  mdelay(10);
//  LCD_COMMAND = 0x1e00;
//  mdelay(50);
//  LCD_COMMAND = 0x18f7;
//  mdelay(100);
//  LCD_COMMAND = 0x2100;
//  mdelay(10);
//  LCD_COMMAND = 0x2812;
//  mdelay(10);
//  LCD_COMMAND = 0x1a00;
//  mdelay(10);
//  LCD_COMMAND = 0x197c;
//  mdelay(10);
//  LCD_COMMAND = 0x1f51;
//  LCD_COMMAND = 0x2060;
//
//  mdelay(10);
//  LCD_COMMAND = 0x1e80;
//  mdelay(10);
//  LCD_COMMAND = 0x1b0b;
//  mdelay(10);

  /* start display */
//  LCD_COMMAND = 0x0020;
//  mdelay(10);
//  LCD_COMMAND = 0x3b01;

  /****************************************************************
   * Check if contact with Lcd controller (read register R49 & R50)
   ****************************************************************/
  /* read register R49, should be 0x10 */
//  LCD_COMMAND = 0x3100;
//  result = LCD_COMMAND;
//  if (result != 0x10)
//  {
//    return( FALSE );
//  }
//
//  /* read register R50, should be 0x02 */
//  LCD_COMMAND  = 0x3200;
//  result = LCD_COMMAND;
//  if (result != 0x02)
//  {
//    return( FALSE );
//  }
//  

  /****************************************************************
   * Initialize Lcd controller (long sequence) 
   ****************************************************************/
  /* system power on */
//  LCD_COMMAND = 0x0301;
//  mdelay(20);
//  LCD_COMMAND = 0x0111;
//  mdelay(10);
//  LCD_COMMAND = 0x0301;
//  mdelay(20);
//  LCD_COMMAND = 0x0028;
//  mdelay(10);
//  LCD_COMMAND = 0x2201;
//  mdelay(10);
//  LCD_COMMAND = 0x0020;
//  mdelay(10);

#endif // _ssd_1289_cc_
