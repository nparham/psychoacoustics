/****** Mikro.c ********************************************************
 *
 * Graphic LCD variables, functions, and macros
 *   For alphanumeric characters, use display strings, formatted as
 *        DisplayStr[ ] = "\RRR\CCCcharacters to be displayed";
 *   where RRR and CCC are octal row and column positions for the first
 *   character in the string     001 <= RRR <= 012  (ten rows)
 *                               001 <= CCC <= 032  (26 columns)
 *   Characters are created in 12x16 pixel matrix.
 *
 * This code was developed by Andy Harazin.
 **********************************************************************/

#include "p24FJ256GB110.h"
#include "Mikro.h"
#include "AlphaFont.h"

uint16 ROWy;              // Row and column pixel positions for
uint16 COLx;              // Display and DisplayChar functions

void SetReg(uint8 index, uint8 value);

/****** Pause **********************************************************
*
* This function pauses for 10*Param microseconds
* for any value of Param from 1 to 65535.
**********************************************************************/
void Pause(uint16 param)
{
   uint16 ctr;

   while (--param) {
      ctr = 38;
	  while (--ctr) ;
   }
   ctr = 35;
   while (--ctr) ;               // Need 9 more microseconds
}

/****** InitBackground *************************************************
 *
 * Initialize 320x240 pixel screen with royal blue background
 **********************************************************************/
void InitBackground()
{
   DrawRectangle(0, 0, 319, 239, BKGD);
}

/****** PMP_Init *******************************************************
 *
 * Initialize PMP module for use with LCD graphical display
 **********************************************************************/
void PMP_Init()
{
   _RC1 = 0;                     // Display RS Enable
   _TRISC1 = 0;                  // enable RESET line
   _TRISB15 = 0;                 // enable RS line
   _RF12 = 1;                    // Display CS Disable;
   _TRISF12 = 0;                 // enable chip select line

   // PMP setup
   PMMODE = 0;
   PMAEN = 0;
   PMCON = 0;
   PMMODEbits.MODE = 2;          // Intel 80 master interface

   PMMODEbits.WAITB = 0;         // PMP setup, wait, hold times config
   PMMODEbits.WAITM = 1;
   PMMODEbits.WAITE = 0;

   PMMODEbits.MODE16 = 0;        // 8 bit mode

   PMCONbits.PTRDEN = 1;         // enable RD line
   PMCONbits.PTWREN = 1;         // enable WR line
   PMCONbits.PMPEN = 1;          // enable PMP

   _RC1 = 1;                     // release from reset

   Delay(20);
}

/****** SetReg *********************************************************
 *
 * Set HX8347D LCD controller registers
 **********************************************************************/
void SetReg(uint8 index, uint8 value)
{
   _RF12 = 0;                    // Display CS Enable;
   WriteCommand(index);
   WriteData(value);
   _RF12 = 1;                    // Display CS Disable;
}


/****** LCD_Init *******************************************************
 *
 * Initialize HX8347D configuration
 **********************************************************************/
void LCD_Init()
{
   const uint8 lcd_reg[] = { 0xEA, 0xEB, 0xEC, 0xED, 0xE8, 0xE9, 0xF1, 0xF2, 0x27, 0x40, 0x41,
      0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x50, 0x51, 0x52,
      0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x04, 0x05, 0x08,
      0x09, 0x16, 0x1B, 0x1A, 0x24, 0x25, 0x23, 0x18, 0x19, 0x01, 0x1F, 0x1F, 0x1F, 0x1F,
      0x17, 0x36, 0x28, 0x00
   };
   const uint8 lcd_val[] = { 0x00, 0x20, 0x0C, 0xC4, 0x40, 0x38, 0x01, 0x10, 0xA3, 0x00, 0x00,
      0x01, 0x13, 0x10, 0x26, 0x08, 0x51, 0x02, 0x12, 0x18, 0x19, 0x14, 0x19, 0x2F, 0x2C,
      0x3E, 0x3F, 0x3F, 0x2E, 0x77, 0x0B, 0x06, 0x07, 0x0D, 0x1D, 0xCC, 0x01, 0x3F, 0x00,
      0xEF, 0x68, 0x1B, 0x01, 0x2F, 0x57, 0x8D, 0x36, 0x01, 0x00, 0x88, 0x80, 0x90, 0xD0,
      0x05, 0x00, 0x38, 0x3C
   };
   uint16 q = 0;
   while (lcd_reg[q])            // stops at the end, 0x00.
   {
      SetReg(lcd_reg[q], lcd_val[q]);
      if ((lcd_reg[q]) == 0x1F)  // 0x1f register needs a short delay
      {
         Delay(500);
      }
      q++;
   }
   Delay(4000);                  // 0x38 requires a long delay
   SetReg(0x28, 0x3C);
}

/****** SetLocation ****************************************************
 *
 * Set location of current LCD position, for use with write pixel.
 **********************************************************************/
void SetLocation(uint16 x, uint16 y)
{
   WriteCommand(0x02);
   WriteData(x >> 8);
   WriteCommand(0x03);
   WriteData(x);
   WriteCommand(0x06);
   WriteData(y >> 8);
   WriteCommand(0x07);
   WriteData(y);
   WriteCommand(0x22);
}

/****** DrawRectangle **************************************************
 *
 * Draws a rectangle between (X1,Y1) and (X2,Y2)
 * where 0 <= X1 < X2 <= 319    and  0 <= Y1 < Y2 <= 239
 * and color RBG
 **********************************************************************/
void DrawRectangle(uint16 X1, uint16 Y1, uint16 X2, uint16 Y2, uint16 color)
{
   uint16 counterX, counterY;

   _LATF12 = 0;                  // Display CS Enable;
   for (counterY = Y1; counterY <= Y2; counterY++)
   {
      SetLocation(X1, counterY);
      for (counterX = X1; counterX <= X2; counterX++)
      {
         WritePixel(color);
      }
   }
   _LATF12 = 1;                  // Display CS Disable;
}

/****** Display ********************************************************
 *
 * Get string and display each of its characters.
 * COLx and ROWy identify the LCD pixel position for the start of
 * the character to be displayed.
***********************************************************************/
void Display(uint16 color, uint8 *str)
{
   ROWy = (str[0] * 24) - 19;     // LCD pixel position for start of char.
   COLx = (str[1] * 12) - 7;
   str = str + 2;                 // Skip over row and column bytes
   while (*str != 0)
   {
      DisplayChar(color, *str);
      COLx += 12;                 // Move to right one char. position
      str++;
   }
}

/****** DisplayChar ****************************************************
 *
 * Display the single character having the inline ASCII code.
 * Use AlphaFont.h table to form each character.
 * COLx and ROWy identify the LCD pixel position for the start of
 * the character to be displayed.
 * OFFSETx and OFFSETy identify the offset from COLx and ROWy to
 * the pixel to be changed.
 **********************************************************************/

void DisplayChar(uint16 color, uint8 charInput)
{
   uint16 StartRow, StartCol;       // Row and column of character in AlphaFont table
   uint16 OFFSETy;                  // Pixel position within char. on LCD
   uint16 OFFSETx;

   _LATF12 = 0;                  // LCD Chip select
   // Get starting position for character in AlphaFont table
   StartRow = (charInput / 16) * 16;
   StartCol = (charInput % 16);
   for (OFFSETy = 0; OFFSETy < 16; OFFSETy++)  // Move to next row of pixels
   {
      for (OFFSETx = 0; OFFSETx < 12; OFFSETx++) // Write that row of pixels
      {
         if ((AlphaFont[(StartRow+OFFSETy)*16+StartCol]) // Word from table
              &   (1 << (16 - OFFSETx)) )    // Bit position in word
         {
            WritePixelAt(COLx + OFFSETx, ROWy + OFFSETy, RGB(255, 255, 255));
         }
         else
         {
            WritePixelAt(COLx + OFFSETx, ROWy + OFFSETy, color);
         }
      }
   }
   _LATF12 = 1;                  // LCD Chip deselect
}

