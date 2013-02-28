/* An app to manually control the digital potentiometers for testing. */

#include "p24FJ256GB110.h"
#include "MCP42xxx.h"
#include "Mikro.h"
#include "MikroSlider.h"

/* set up MCP42xxx device handles */
MCP42xxx_device_handle chip0 = 0;
MCP42xxx_device_handle chip1 = 1;

/****** Configuration selections **************************************/
_CONFIG1(JTAGEN_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2);
_CONFIG2(PLLDIV_DIV2 & POSCMOD_HS & FNOSC_PRIPLL & IOL1WAY_OFF);

int main()
{
   Slider slider0, slider1, slider2, slider3;

   /* initialization */
   AD1PCFGL = 0xFFFF;            // Make all ADC pins default to digital pins
   PMP_Init();                   // Configure PMP module for LCD
   LCD_Init();                   // Configure LCD controller
   InitBackground();

   TMR5 = 0;                     // Clear Timer5
   PR5 = 19999;                  // Set period of Timer5 to 10 ms
   T5CON = 0x8010;               // Clock Timer5 with Fcy/8 = 2 MHz

   MCP42xxxInit(); 

   StartSlider(&slider0, 50, 50, 150, hrz);
   StartSlider(&slider1, 50, 100, 150, hrz);
   StartSlider(&slider2, 50, 150, 150, hrz);
   StartSlider(&slider3, 50, 200, 150, hrz);

   /* end initialization*/

   while (1) {
      DetectTouch();  // Detects touch positions on the screen
      CheckSlider(&slider0);
      CheckSlider(&slider1);
      CheckSlider(&slider2);
      CheckSlider(&slider3);

	  //update digipot resistances
      MCP42xxx_setResistance(0, slider0.scaledvalue, 0); // chip 0, pot 0
      MCP42xxx_setResistance(1, slider1.scaledvalue, 0); // chip 0, pot 1
      MCP42xxx_setResistance(0, slider2.scaledvalue, 1); // chip 1, pot 0
      MCP42xxx_setResistance(1, slider3.scaledvalue, 1); // chip 1, pot 1

      while (!_T5IF) ;           // Loop time = 10 ms
      _T5IF = 0;

   }
}

// make some functions to check touch input and then set digipot output

