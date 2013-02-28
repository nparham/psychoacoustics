/****** MikroTouch.c ***************************************************
 *
 * Touchscreen variables and functions
 *
 * This code was developed by Andy Harazin
 * Modified by Tyler Fultz 2/8/13
 *  -Added Macros
 *  -Added DetectBox
 **********************************************************************/
#include "MikroTouch.h"

/***********************************************************************
 * Global variables
 **********************************************************************/
sint32 XADC;
sint32 YADC;
sint32 tsx = -1;
sint32 tsy = -1;

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
sint16 GetX(void)
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
   uint32 XPIXEL = ((((long) 285760)) - ((long) ADC1BUF0 * 320)) / 815;
   return XPIXEL;
}

/******GetY ************************************************************
 *
 * Get the Y position, YPIXEL, of the current touch on the touch screen
 **********************************************************************/
sint16 GetY(void)
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
   //YPIXEL = (240*YADC - 240*86)/(876-86)
   uint32 YPIXEL = (((long) ADC1BUF0 * 240) - ((long)20640)) / 790;
   return YPIXEL;
}

/******DetectRectangle ************************************************
 *
 * Returns a positive value if touch is inside box
 * Value is 1 if touched inside, 0 if not
 **********************************************************************/
uint8 DetectRectangle(int xmin,int ymin,int xmax,int ymax){

    if((tsx >= 0 && tsy >= 0) &&
       (tsx >= xmin && tsx <= xmax) &&
       (tsy >= ymin && tsy <= ymax)){

        return 1;

    } else{

        return 0;

    }

}

/******DetectChar *****************************************************
 *
 * Returns a positive value if touch is inside Char space defined in
 * Mikro.c
 * Value is 1 if touched inside, 0 if not
 **********************************************************************/
uint8 DetectChar(char* strDetect)
{
   uint8 StartCharY = strDetect[0];
   uint8 StartCharX = strDetect[1];
   uint8 NumChar = 0;
   strDetect += 2;
   
   while(*(strDetect++) != 0){
       NumChar++;
   }

   uint8 xstart = (StartCharX)*12 - 7;
   uint8 ystart = (StartCharY)*24 - 19;
   uint8 xstop  = xstart+NumChar*12;
   uint8 ystop  = ystart+1*16;

   return DetectRectangle(xstart,ystart,xstop,ystop);

}
