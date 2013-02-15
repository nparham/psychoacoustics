/* 
 * This code was developed by Andy Harazin.
 */

#ifndef MIKRO_H
#define	MIKRO_H

/* datatypes */
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned long uint32;
typedef signed char sint8;
typedef signed short int sint16;
typedef signed long sint32;

uint16 DELAY;

#define RGB(red, green, blue) (uint16) (((((uint16)(red) & 0xF8) >> 3) << 11) | ((((uint16)(green) & 0xFC) >> 2) << 5) | (((uint16)(blue) & 0xF8) >> 3))
//#define BKGD RGB(0,0,255)
#define BKGD BLACK
#define WHITE RGB(255,255,255)
#define BLACK RGB(0,0,0)
#define GREEN RGB(0,255,0)

// Delay macro provides a pause of (10 x parameter) microseconds
#define Delay(x) DELAY = x; while(--DELAY){ uint8 counter=42; while(--counter);}

#define WriteToDevice(data) PMDIN1 = data; while(!PMMODEbits.BUSY)
#define WriteCommand(cmd) 	_RB15 = 0; WriteToDevice(cmd)
#define WriteData(data)		_RB15 = 1; WriteToDevice(data)
#define WritePixel(data) WriteData(data>>8); PMDIN1 = data; while(!PMMODEbits.BUSY);
#define WritePixelAt(x, y, data) SetLocation(x,y); WritePixel(data)

void Initial(void);
void PMP_Init(void);
void InitBackground(void);
void LCD_Init(void);
void SetLocation(uint16 x, uint16 y);
void DrawRectangle(uint16 X1, uint16 Y1, uint16 X2, uint16 Y2, uint16 color);
void Display(uint16 color, uint8 *str);
void DisplayChar(uint16 color, uint8 charInput);

#endif	/* #ifndef MIKRO_H */

