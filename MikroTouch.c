/****** MikroTouch.c ***************************************************
 *
 * Touchscreen variables and functions
 *
 * This code was developed by Andy Harazin.
 **********************************************************************/

#include "p24FJ256GB110.h"       // PIC24 register and bit definitions
#include "Mikro.h"
#include "MikroTouch.h"
 
/****** Function prototypes *******************************************/
int GetX(void);
int GetY(void);

/***********************************************************************
 * Global variables
 **********************************************************************/
long XADC;
long YADC;
long tsx = -1;
long tsy = -1;
int oldScaledSliderVal = 0;
int scaledSliderVal = 0;

/****** DetectTouch ****************************************************
 *
 * Store the current touch position into global variables, tsx and tsy.
 * tsx=tsy=-1 means no current touch.
 **********************************************************************/
void DetectTouch(void)
{
   int tmp1, tmp2;

   tmp1 = GetX();                // Retrieve two consecutive values
   tmp2 = GetX();
   if ((tmp1 - tmp2 < 5) && (tmp1 - tmp2 > -5)) // Check if similar
   {
      tsx = (tmp1 + tmp2) >> 1;  // Take the average
      if (tsx < 5 || tsx > 315)  // Check for bounds
         tsx = -1;               // Invalid touch
   }
   else
   {
      tsx = -1;                  // No touch or signal not debounced
   }
   tmp1 = GetY();                // Retrieve two consecutive values
   tmp2 = GetY();
   if ((tmp1 - tmp2 < 5) && (tmp1 - tmp2 > -5)) // Check if similar
   {
      tsy = (tmp1 + tmp2) >> 1;  // Take the average
      if (tsy < 5 || tsy > 235)  // Check for bounds
         tsy = -1;               // Invalid touch
   }
   else
   {
      tsy = -1;                  // No touch or signal not debounced
   }
}

/****** ADCsetup *******************************************************
 *
 * Configure the analog-digital converter.
 * With AD1CON3=0x183F, auto-conversion takes 150 us.  Any faster doesn't work.
 **********************************************************************/
void ADCsetup(void)
{
   AD1CON1 = 0x80E0;             // Turn on, auto-convert
   AD1CON2 = 0;                  // AVdd, AVss, int every conversion, MUXA only
   AD1CON3 = 0x183F;             // Use CPU clock as conversion clock
   AD1CSSL = 0;                  // No scanned inputs
}

/****** GetX ***********************************************************
 *
 * Get the X position, XPIXEL, of the current touch on the touch screen
 **********************************************************************/
int GetX(void)
{
   ADCsetup();
   _PCFG13 = 0;                  // Set up vertical pins for reading analog inputs
   _PCFG11 = 0;
   _TRISB13 = 1;                 // Float both (digital) port inputs for reading
   _TRISB11 = 1;

   _PCFG12 = 1;                  // Set up horizontal axis for digital outputs
   _PCFG10 = 1;
   _TRISB12 = 0;
   _TRISB10 = 0;
   _LATB12 = 1;                  // Make left edge = 3.3V
   _LATB10 = 0;                  // Make right edge = 0V
   AD1CHS = 0x0D;                // Sample XL input
   _SAMP = 1;
   while (!_DONE) ;              // Wait for conversion to be completed

// XPIXEL = (893x320 - 320*XADC)/(893-78)
   XPIXEL = ((((long) 285760)) - ((long) ADC1BUF0 * 320)) / 815;
   return XPIXEL;
}

/******GetY ************************************************************
 *
 * Get the Y position, YPIXEL, of the current touch on the touch screen
 **********************************************************************/
int GetY(void)
{
   ADCsetup();

   _PCFG12 = 0;                  // Set up horizontal pins for reading analog inputs
   _PCFG10 = 0;
   _TRISB12 = 1;
   _TRISB10 = 1;

   _PCFG13 = 1;                  // Set up vertical axis for digital outputs
   _PCFG11 = 1;
   _TRISB13 = 0;
   _TRISB11 = 0;
   _LATB13 = 1;                  // Make top edge = 3.3V
   _LATB11 = 0;                  // Make bottom edge = 0V

   AD1CHS = 0x0C;                // Sample YU input
   _SAMP = 1;
   while (!_DONE) ;              // Wait for conversion to be completed
// YPIXEL = (240*YADC - 240*86)/(876-86)
   YPIXEL = (((long) ADC1BUF0 * 240) - 20640) / 790;
   return YPIXEL;
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

