/* An app to manually control the digital potentiometers for testing. */

#include "p24FJ256GB110.h"
#include "MCP42xxx.h"
#include "Mikro.h"

/* set up MCP42xxx device handles */
MCP42xxx_device_handle chip0 = 0;
MCP42xxx_device_handle chip1 = 1;

/****** Configuration selections **************************************/
_CONFIG1(JTAGEN_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2);
_CONFIG2(PLLDIV_DIV2 & POSCMOD_HS & FNOSC_PRIPLL & IOL1WAY_OFF);

int main()
{

   unsigned int looptime = 0;

   /* initialization */
   AD1PCFGL = 0xFFFF;            // Make all ADC pins default to digital pins
   PMP_Init();                   // Configure PMP module for LCD
   LCD_Init();                   // Configure LCD controller
   InitBackground();

   TMR5 = 0;                     // Clear Timer5
   PR5 = 19999;                  // Set period of Timer5 to 10 ms
   T5CON = 0x8010;               // Clock Timer5 with Fcy/8 = 2 MHz

   MCP42xxxInit(); 
   /* end initialization*/


   while (1) {
      while (!_T5IF) ;           // Loop time = 10 ms
      _T5IF = 0;

   }
}

// make some functions to check touch input and then set digipot output
