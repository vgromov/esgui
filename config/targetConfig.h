#ifndef _target_config_h_
#define _target_config_h_

// use jtag debug (no WDT)
// remove in production code
#define SWD_JTAG_DEBUG

// frequency configuration data
//
// external Xtal frequency
#define Fosc						12000000			

#if LPC214X == 1
		
// valid PLL frequency range
#define MinFcco					156000000
#define MaxFcco					320000000			

// PLL0 divisors
#define PLL0_M					4
#define PLL0_P					2	// 1, 2, 4, or 8 is valid

// MCU frequency
#define Fmcu						((Fosc)*(PLL0_M))
// PLL0 frequency
#define F0cco						((PLL0_P)*((Fmcu)*2))	
// check PLL0 frequency
#if( F0cco < MinFcco || F0cco > MaxFcco )
	#error "F0cco is out of valid PLL frequency range"
#endif			

#ifdef USE_PLL1
#endif

// periferial frequency
#define PERIPH_DIV			1 // 0 - 1/4; 1 - Fmcu; 2 - 1/2 
#if(PERIPH_DIV == 0)
	#define Fpclk					((Fmcu)/4)
#elif(PERIPH_DIV == 1)
	#define Fpclk					(Fmcu)
#elif(PERIPH_DIV == 2)
	#define Fpclk					((Fmcu)/2)
#endif

#endif // LPC214X == 1

#if LPC23XX == 1

// valid PLL frequency range
#define MinFcco					275000000
#define MaxFcco					550000000			

#define PLL_M						40
#define PLL_N						3

#define Fcco						((2*(PLL_M)*Fosc)/(PLL_N))

// check PLL0 frequency
#if( Fcco < MinFcco || Fcco > MaxFcco )
	#error "F0cco is out of valid PLL frequency range"
#endif	

// Only 1 and even values (2, 4, 6, ..., 256) are supported
#define MCU_CLK_DIV			4
// MCU frequency
#define Fmcu						((Fcco)/(MCU_CLK_DIV))
// peripherial bus frequency
#define Fpclk						((Fmcu)/4)

#endif // LPC23XX == 1

#endif
