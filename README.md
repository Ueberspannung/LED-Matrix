# LED-Matrix
low level driver for MAX7219 / MAX7221 based LED matrix


##### Table of contents
- [description](#description)
- [hardware](#hardware)
- [functions](#fuctions)
  - [begin](#begin)
  - [setIntensity](#setIntensity)
  - [setByte](#setByte)
  - [setBuffer](#setBuffer)
  - [DisplayTest](#DisplayTest)


## description
This is a simple low level driver for dot matrix displays based on MAX7219 or MAX7221 drivers for Adrduino based systems.  
it is designed to work with or without a display buffer. If the drivers are used wtih 7 segment displays one might want to add a few functions to set font translation and decimal dot.
if you need more features such as text or graphic you might want to use an other display driver or use this one as a base for your features


## hardware
I used [AZ-Delivery's](https://www.az-delivery.de/) [MAX7219 8x32 4 in 1 Dot Matrix LED Modul](https://www.az-delivery.de/products/4-x-64er-led-matrix-display?_pos=3&_psq=led+matrix&_ss=e&_v=1.0).
This is a 4x 8x8 MAX7219 base LED matrix modul.
There are many other moduls available on Amazon, Ali and ebay

if you are using a different layout you will need to change the adress mappen of the setBuffer and setByte functions

the driver uses standard SPI interface. Depending on your HW you might want to add a parameter for the SPI-Interface and use the aoutmaten CS Feature available on some devices


This is the adressing scheme of the modul

```
        +---------+-------------------------------+-------------------------------+-------------------------------+-------------------------------+
        !  Modul  !               0               !               1               !               2               !               3               !
        +---------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        !   Bit   ! 0 ! 1 ! 2 ! 3 ! 4 ! 5 ! 6 ! 7 ! 0 ! 1 ! 2 ! 3 ! 4 ! 5 ! 6 ! 7 ! 0 ! 1 ! 2 ! 3 ! 4 ! 5 ! 6 ! 7 ! 0 ! 1 ! 2 ! 3 ! 4 ! 5 ! 6 ! 7 !
        +---------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        ! DIGIT_7 !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !
        +---------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        ! DIGIT_6 !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !
        +---------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        ! DIGIT_5 !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !
        +---------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        ! DIGIT_4 !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !
DIN =>  +---------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+  => DOUT
        ! DIGIT_3 !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !
        +---------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        ! DIGIT_2 !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !
        +---------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        ! DIGIT_1 !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !
        +---------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        ! DIGIT_0 !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !   !
        +---------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
```
This corresponds to the FC16 HW-scheme

## functions

## begin

```
void begin(uint8_t moduls);
void begin(uint8_t moduls_x, uint8_t moduls_y);
```

to initialize the driver *begin* needs to be called.
there are 2 variants of this function:
- one which could be used if there are several moduls arragned as a chain
- one which could be used if there are several moduls arranged as a matrix  
  see max72xx.h for details 

the drivers are 
- set in active mode, 
- character translation is disabled,
- scan limit is set to 7 (all on)
- intensity is set to 0 (lowest)

## setIntensity
```
void setIntensity(uint8_t intensity);
void setIntensityuint8_t modul, uint8_t intensity);
void setIntensity(uint8_t modul_x, uint8_t modul_y, uint8_t intensity);
```

sets the intensity for all ar a dedicated driver

## setByte
```
void setByte(uint8_t modul_x, uint16_t row, uint8_t val);
```
sets a deicated 8 bit pattern. in matrix mode rows are jast incremented.
- 0 ... 7 for the fisrt row of moduls
- 8 ... 15 for the second row of moduls
- ...


## setBuffer
```
void setBuffer(const uint8_t * Buffer);
```
displays a buffer 
for details on the display buffer layout see max79xx.hardware


## DisplayTest
```
void DisplayTest(bool Test);
void DisplayTest(uint8_t modul,bool Test);
void DisplayTest(uint8_t modul_x, uint8_t modul_y, bool Test);
```

enables all LEDs at the desired modul at full intensity, without changing the contents of the data and intensity register.
disabling the display test returns to normal operation
