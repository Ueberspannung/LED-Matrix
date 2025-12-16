#include "MAX72xx.h"

MAX72xx_c matrix;


#define BITS_PER_BYTE 8
#define PIXELS_X 32
#define PIXELS_Y 8
#define PIXEL_CNT (PIXELS_X * PIXELS_Y)
#define MODULS_X ((PIXELS_X+7)>>3)
#define MODULS_Y ((PIXELS_Y+7)>>3)

#define BUFFER_LEN (PIXEL_CNT + BITS_PER_BYTE - 1) / BITS_PER_BYTE 

uint8_t buffer[BUFFER_LEN]={0};

uint16_t bytePos(uint8_t x, uint8_t y)
{
	return (y * PIXELS_X + x) / BITS_PER_BYTE;
}

uint8_t bitPos(uint8_t x, uint8_t y)
{
	return (y * PIXELS_X + x) % BITS_PER_BYTE;
}


bool valid( uint8_t pos_x,  uint8_t pos_y)
{
	return (pos_x < PIXELS_X) && (pos_y < PIXELS_Y);
}


bool Get(uint8_t * feld,  uint8_t pos_x,  uint8_t pos_y)
{
	if (valid(pos_x, pos_y))
		return feld[bytePos(pos_x,pos_y)] & (1<<bitPos(pos_x, pos_y));
	else
		return false;
}

void Set(uint8_t* feld, bool val,  uint8_t pos_x,  uint8_t pos_y)
{
	if (valid(pos_x, pos_y))
		if (val)
			feld[bytePos(pos_x, pos_y)] |= 1 << bitPos(pos_x, pos_y);
		else
			feld[bytePos(pos_x, pos_y)] &= ~(1 << bitPos(pos_x, pos_y));
}


void setup(void)
{
	Serial.begin(115200);
	
	matrix.begin(4);
}

uint8_t n=0;

void loop(void)
{
	uint8_t x,y;
	
	for ( y=0; y<PIXELS_Y; y++)
		for ( x=0; x<PIXELS_X; x++)
		{
			Set(buffer,true,x,y);
			matrix.setBuffer(buffer);
			delay(50);
			Set(buffer,false,x,y);
		}
}
