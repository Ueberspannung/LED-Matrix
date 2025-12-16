#ifndef _MAX72xx_h_
#define _MAX72xx_h_

#include <SPI.h>
#include <stdint.h>

/* *******************************************************************
 * this class is a low leve driver for MAX7219 / 7221 based displays
 * 
 * it is designed for AZ-Delivery (FC16) type displays.
 * These are 32x8 dot displays using 4 LED drivers
 * when the DIN connector is on the left side this is the addressing scheme:
 * 
 *  chip !   0    !   1    !   2    !   3    !
 * ------+--------+--------+--------+--------+
 *  bit  !01234567!01234567!01234567!01234567!
 * ------+--------+--------+--------+--------+
 * DIGIT7!        !        !        !        !
 * DIGIT6!        !        !        !        !
 * DIGIT5!        !        !        !        !
 * DIGIT4!        !        !        !        !
 * DIGIT3!        !        !        !        !
 * DIGIT2!        !        !        !        !
 * DIGIT1!        !        !        !        !
 * DIGIT0!        !        !        !        !
 * 
 * when extending to multiple Modules it is assumed that the DIN connector
 * is at the lower left corner
 *
 *               +----+-------+----+  +----+-------+----+  +----+-------+----+
 * controller -> ! DIN!modul 6!DOUT!->! DIN!modul 7!DOUT!->! DIN!modul 8!DOUT!-> to DIN4
 *               +----!-------+----*  +----!-------+----*  +----!-------+----*
 *  
 *               +----+-------+----+  +----+-------+----+  +----+-------+----+
 * controller -> ! DIN!modul 3!DOUT!->! DIN!modul 4!DOUT!->! DIN!modul 5!DOUT!-> to DIN6
 *               +----!-------+----*  +----!-------+----*  +----!-------+----*
 * 
 *               +----+-------+----+  +----+-------+----+  +----+-------+----+
 * controller -> ! DIN!modul 0!DOUT!->! DIN!modul 1!DOUT!->! DIN!modul 2!DOUT!-> to DIN3
 *               +----!-------+----*  +----!-------+----*  +----!-------+----*
 *
 * Any 8 pixels can be reference by row and modul. starting with 0,0 in the lower left corner
 * 
 * for any other scheme the bit, digit and modul orientation needs to be adjustet
 * 
 * 
 * Buffer:
 * This is a byte buffer byte 0,bit 0 ist the lower left pixel 
 * Pixels are ordered in rows
 * 
 * sample of 4x2 (16x32)
 * 
 * 
 * row	index
 * (y)  (x/8)
 *  15  60 61 62 63
 *  14  56 57 58 59
 *  13  52 53 54 55
 *  12  48 49 50 51
 *  11  44 45 46 47 
 *  10  40 41 42 43
 *   9  36 37 38 39
 *   8  32 33 34 35
 *   7  28 29 30 31
 *   6  24 25 26 27
 *   5  20 21 22 23
 *   4  15 17 18 19
 *   3  12 13 14 15
 *   2   8  9 10 11
 *   1   4  5  6  7
 *   0   0  1  2  3
 * 
 * The Module needs to be connected to standard SPI and SS pins
 * 
 */


class MAX72xx_c
{
	public:
		
		MAX72xx_c(void);
		
		void begin(uint8_t moduls);
		void begin(uint8_t moduls_x, uint8_t moduls_y);
		
		void setIntensity(uint8_t intensity);
		void setIntensity(uint8_t modul, uint8_t intensity);
		void setIntensity(uint8_t modul_x, uint8_t modul_y, uint8_t intensity);
		
		void setByte(uint8_t modul_x, uint16_t row, uint8_t val);
		void setBuffer(const uint8_t * Buffer);
		
		void DisplayTest(bool Test);
		void DisplayTest(uint8_t modul,bool Test);
		void DisplayTest(uint8_t modul_x, uint8_t modul_y, bool Test);
		
	private:

		typedef enum:uint8_t { 	REG_NOP, 
								REG_DIGIT0, 
								REG_DIGIT1, 
								REG_DIGIT2, 
								REG_DIGIT3, 
								REG_DIGIT4, 
								REG_DIGIT5, 
								REG_DIGIT6, 
								REG_DIGIT7,
								REG_DECODE_MODE,
								REG_INTENSITY,
								REG_SCAN_LIMIT,
								REG_SHUTDOWN,
								REG_NN_D,
								REG_NN_E,
								REG_DISPLAYTEST
							} REG_et;

		SPISettings SPI_set{8000000, MSBFIRST, SPI_MODE0};
		
		uint8_t ModulsX,ModulsY;
	
		void setRegister(REG_et reg,uint8_t val);
		void setRegister(uint8_t modul, REG_et reg, uint8_t val);
		void setRegister(uint8_t modul_x, uint8_t modul_y, REG_et reg, uint8_t val);
		
		void setCS(bool val);
		void write_reg(REG_et reg,uint8_t val);
		
		bool test(uint16_t n);
		bool test(uint8_t x, uint8_t y);

};

#endif // _MAX72xx_h_
