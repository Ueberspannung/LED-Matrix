#include "MAX72xx.h"
#include <Arduino.h>




/* ********************
 * public
 * ******************** */

MAX72xx_c::MAX72xx_c(void)
{ // MAX72xx_c
	ModulsX=0;
	ModulsY=0;
} // MAX72xx_c
 
 
void MAX72xx_c::begin(uint8_t moduls)
{	// begin
	begin(moduls,1);
}	// begin

void MAX72xx_c::begin(uint8_t moduls_x, uint8_t moduls_y)
{	// begin
	uint8_t reg;

	ModulsX=moduls_x;
	ModulsY=moduls_y;
	
	pinMode(PIN_SPI_MOSI,OUTPUT);
	pinMode(PIN_SPI_SCK,OUTPUT);
	pinMode(PIN_SPI_SS,OUTPUT);
	digitalWrite(PIN_SPI_SS,HIGH);

	SPI.begin();

	// wake up
	setRegister(REG_SHUTDOWN,1);

	// scan limit
	setRegister(REG_SCAN_LIMIT,0xFF);
	
	// clear data
	for (reg=0;reg<8;reg++)
	{
		setRegister((REG_et)(REG_DIGIT0+reg),0);
	}

	setIntensity(0);

	// test off
	DisplayTest(false);	
}	// begin

void MAX72xx_c::setIntensity(uint8_t intensity)
{	// setIntensity
	setRegister(REG_INTENSITY,intensity);
}	// setIntensity

void MAX72xx_c::setIntensity(uint8_t modul, uint8_t intensity)
{	// setIntensity
	if (test(modul))
		setRegister(modul,REG_INTENSITY,intensity);
}	// setIntensity

void MAX72xx_c::setIntensity(uint8_t modul_x, uint8_t modul_y, uint8_t intensity)
{	// setIntensity
	if (test(modul_x,modul_y))
		setRegister(modul_x,modul_y,REG_INTENSITY,intensity);
}	// setIntensity


void MAX72xx_c::DisplayTest(bool Test)
{	// DisplayTest
	setRegister(REG_DISPLAYTEST,Test);
}	// DisplayTest

void MAX72xx_c::DisplayTest(uint8_t modul,bool Test)
{	// DisplayTest
	if (test(modul))
		setRegister(modul,REG_DISPLAYTEST,Test);
}	// DisplayTest

void MAX72xx_c::DisplayTest(uint8_t modul_x, uint8_t modul_y, bool Test)
{	// DisplayTest
	if (test(modul_x,modul_y))
		setRegister(modul_x,modul_y,REG_DISPLAYTEST,Test);
}	// DisplayTest

void MAX72xx_c::setByte(uint8_t modul, uint16_t row, uint8_t val)
{	// setByte
	if (test(modul,row>>3))
		setRegister(modul,row>>3,(REG_et)(REG_DIGIT0+(row & 7)),val);
}	// setByte


void MAX72xx_c::setBuffer(const uint8_t * Buffer)
{
	uint8_t row;
	uint8_t modul_x;
	uint8_t modul_y;
	uint16_t index;
	row=7;
	for(row=0;row<8;row++)
	{	// row loop
		SPI.beginTransaction(SPI_set);
		setCS(false);
		modul_y=ModulsY;
		while (modul_y--)
		{	// ModulY
			modul_x=ModulsX;
			while (modul_x--)
			{	// ModulX
				// modul_y*ModulsX*8+row*ModulsX+modul_x
				index=modul_y;	
				index*=ModulsX;
				index<<=3;
				index+=(uint16_t)row*(uint16_t)ModulsX;
				index+=modul_x;
				write_reg((REG_et)((uint8_t)REG_DIGIT0+row),Buffer[index]);
			}	// ModulX
		}	// ModulY
		setCS(true);
		SPI.endTransaction();
	}	// row loop
}

					
/* ********************
 * private
 * ******************** */
	
void MAX72xx_c::setRegister(REG_et reg,uint8_t val)
{	// setRegister
	uint8_t n;
	
	SPI.beginTransaction(SPI_set);
	setCS(false);
	for (n=0;n<ModulsX*ModulsY;n++)
		write_reg(reg,val);
	setCS(true);
	SPI.endTransaction();
}	// setRegister

void MAX72xx_c::setRegister(uint8_t modul, REG_et reg, uint8_t val)
{	// setRegister
	uint8_t n;
	
	modul=ModulsX*ModulsY-1-modul;
	SPI.beginTransaction(SPI_set);
	setCS(false);
	for (n=0;n<ModulsX*ModulsY;n++)
		if (n==modul) 
			write_reg(reg,val);
		else
			write_reg(REG_NOP,val);
	setCS(true);
	SPI.endTransaction();
}	// setRegister

void MAX72xx_c::setRegister(uint8_t modul_x, uint8_t modul_y, REG_et reg, uint8_t val)
{	// setRegister
	setRegister(ModulsX*modul_y+modul_x,reg,val);
}	// setRegister

void MAX72xx_c::setCS(bool val)
{	// setCS
	digitalWrite(PIN_SPI_SS,val);
}	// setCS

void MAX72xx_c::write_reg(REG_et reg,uint8_t val)
{	// write_reg
	SPI.transfer(reg);
	SPI.transfer(val);
}	// write_reg

bool MAX72xx_c::test(uint16_t n)
{	// test
	return n<(uint16_t)ModulsX*(uint16_t)ModulsY;
}	// test


bool MAX72xx_c::test(uint8_t x, uint8_t y)
{	// test
	return (x<ModulsX) && (y<ModulsY);
}	// test
	
