
/* *** ***** *** **
   REMOVE THIS FILE
   ***** *** ******/



#include "p24FJ256GB110.h"       // PIC24 register and bit definitions
#include "Mikro.h"               // LCD variables, functions, macros
#include "MikroTouch.h"          // Touchscreen variables, functions
#include "MikroPot.h"
#include "MikroMeasureTime.c"    // Start, Stop, Send, ASCIIn, Blankn functions

/****** Configuration selections **************************************/
_CONFIG1(JTAGEN_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2);
_CONFIG2(PLLDIV_DIV2 & POSCMOD_HS & FNOSC_PRIPLL & IOL1WAY_OFF);

/****** Global variables **********************************************/
char HexStr[] = "\001\0010x0000";
char Potstr[] = "\010\030000";

char XPIXELstr[] = "\006\010000";
char YPIXELstr[] = "\006\020000";


//////// Main program //////////////////////////////////////////////////

int main()
{
   Initial();
   while (1)                     // Looptime without Sleep = 20.35 ms
   {
      DetectTouch();
      MoveSlider();
      while (!_T5IF);            // Loop time = 10 ms
      _T5IF = 0;
   }
}

/****** Initial ********************************************************
 *
 * Initialize LCD Screen (PMP + configuration + initial display).
 * Initialize Timer5 for a loop time of 10 ms.
 **********************************************************************/
void Initial()
{
   AD1PCFGL = 0xFFFF;            // Make all ADC pins default to digital pins
   PMP_Init();                   // Configure PMP module for LCD
   LCD_Init();                   // Configure LCD controller
   InitBackground();             // Paint screen royal blue
   InitPot();                    // Create pot
   _TRISA3 = 0;                  // Make RA3 an output (pin 50 of Mikro board)
   TMR5 = 0;                     // Clear Timer5
   PR5 = 19999;                  // Set period of Timer5 to 10 ms
   T5CON = 0x8010;               // Clock Timer5 with Fcy/8 = 2 MHz
}

