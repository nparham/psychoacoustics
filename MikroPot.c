/****** MikroPot.c *****************************************************
 *
 * Andy Hazazin's touchscreen pot with 0-255 output
 **********************************************************************/
#include "p24FJ256GB110.h"       // PIC24 register and bit definitions
#include "AlphaFont.h"           // 12x16-pixel font set
#include "Mikro.c"               // LCD variables, functions, macros
#include "MikroTouch.c"          // Touchscreen variables, functions
#include "MikroMeasureTime.c"    // Start, Stop, Send, ASCIIn, Blankn functions

/****** Configuration selections **************************************/
_CONFIG1(JTAGEN_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2);
_CONFIG2(PLLDIV_DIV2 & POSCMOD_HS & FNOSC_PRIPLL & IOL1WAY_OFF);

/****** Global variables **********************************************/
char HexStr[] = "\001\0010x0000";
unsigned int ALIVECNT = 0;
char Potstr[] = "\010\030000";

char XPIXELstr[] = "\006\010000";
char YPIXELstr[] = "\006\020000";

/****** Function prototypes *******************************************/
void Initial(void);
void InitPot(void);
void BlinkAlive(void);

//////// Main program //////////////////////////////////////////////////

int main()
{
   Initial();
   while (1)                     // Looptime without Sleep = 20.35 ms
   {
      Start();                   // Measure pot execution time
      _LATA3 ^= 1;               // Toggle pin 50 of Mikro board
      DetectTouch();
      MoveSlider();
      Stop();
      Send(1, 18);
      BlinkAlive();
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

/****** InitPot ********************************************************
 *
 * Create initial screen potentiometer
 *
 **********************************************************************/
void InitPot()
{
   DrawRectangle(0, 205, 320, 240, WHITE);
   // rectangle in bottom part to be black
   DrawRectangle(10, 215, 310, 230, BLACK);
   // initializes value to zero for reading
   DrawRectangle(10, 215, 25, 230, BKGD);
}

/****** BlinkAlive *****************************************************
 *
 * This function toggles a square of pixels every second.
 * With a loop time of 10 ms, this is every 100 looptimes.
 **********************************************************************/
void BlinkAlive()
{
   ALIVECNT++;
   if (ALIVECNT == 100)           // Write black square
   {
      DrawRectangle(0, 0, 5, 5, BLACK);
   }
   if (ALIVECNT >= 200)           // Clear black square
   {
      ALIVECNT = 0;
      DrawRectangle(0, 0, 5, 5, BKGD);
   }
}

/****** MoveSlider *****************************************************
 *
 * Moves the slider around on touch
 *
 **********************************************************************/
void MoveSlider()
{
   static int sliderVal = 25;
   static int oldSliderVal = 25;

   if (tsy >= 205 && tsy <= 230)
   {
      if (tsx != -1)
      {
         sliderVal = tsx;
         if (sliderVal < 25)
            sliderVal = 25;
         if (sliderVal > 310)
            sliderVal = 310;
      }
   }
   if (sliderVal != oldSliderVal)
   {
      _LATF12 = 0;               // Enable display;
      DrawRectangle(oldSliderVal - 15, 215, oldSliderVal, 230, BLACK);
      DrawRectangle(sliderVal - 15, 215, sliderVal, 230, BKGD);
      _LATF12 = 1;               // Disable display;
      scaledSliderVal = (((long) sliderVal * 255) - 7650) / 270;
      if (scaledSliderVal < 0)
         scaledSliderVal = 0;
      else if (scaledSliderVal > 0xFF)
         scaledSliderVal = 0xFF;
      LONGNUM = scaledSliderVal;
      ASCIIn(3, Potstr);
      Display(Potstr);           // Display pot value

      oldSliderVal = sliderVal;
      oldScaledSliderVal = scaledSliderVal;
   }
}

