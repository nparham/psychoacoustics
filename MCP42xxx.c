/* SPI interface with MCP42xxx digital potentiometers */
#include "p24FJ256GB110.h"
#include "MCP42xxx.h"
#include "Mikro.h"

/* CS logic */
#define SELECT (0)
#define DESELECT (1)
/* CS pins */
#define CHIP_0 (_LATB3)
#define CHIP_1 (_LATB4)

/* Instruction set:
 * These values should be OR'd together to form a valid instruction.
 * Both potentiometers can be set by ORing both CMD_POT words.
 */
#define CMD_WRITE (0x10)
#define CMD_SHUTDOWN (0x20)
#define CMD_POT0 (0x01) // select potentiometer 0
#define CMD_POT1 (0x02) // select potentiometer 1
#define PACK_INST(inst, data) (inst << 8)|data // for packing instruction with data

void SPI_Init();
void SPI_Clock_Speed(uint8 pri_prescale, uint8 sec_prescale);
uint8 SPIwrite(uint8 data);

/*******************/
/* DEVICE-SPECIFIC */
/*******************/

void Select(struct MCP42xxx_device_handle device)
{
	switch (device) {
		case 0:
			CHIP_0 = 0;
			break;
		case 1:
			CHIP_1 = 0;
			break;
	}
}

void Deselect(struct MCP42xxx_device_handle device)
{
	switch (device) {
		case 0:
			CHIP_0 = 0;
			break;
		case 1:
			CHIP_1 = 0;
			break;
	}
}

void MCP42xxxInit()
{
	SPI_Init();

	//Max clock frequency for single unit (no daisy chain): 10MHz
	// TODO: set clock frequency with SPI_Clock_Speed()
}

void MCP42xxx_setResistance(uint8 potnum, uint8 resistance, struct MCP42xxx_device_handle device)
{
	// passing the value 2 for potnum will select both pots.

	Select(device);
	SPIwrite( PACK_INST( (CMD_WRITE|(potnum + 1)), resistance) );
	Deselect(device);
}


//TODO shutdown command for power save



/***************/
/* GENERAL SPI */
/***************/

/* Write a byte to MOSI and read a byte from MISO */
uint16 SPIwrite(uint16 data)
{
	//TODO check to see if the transition to word writes worked
   SPI2BUF = data; // fill TX buffer
   while (!(SPI2STAT & 0x0080)); // wait for SRMPT set
   return SPI2BUF; // read RX buffer
}

/* SPI module 2 initialization */
void SPI_Init()
{
   // Map pins  TODO choose pins
   _RP21R = 11; // Remap RG6 to SPI2CLK
   _RP19R = 10; // Remap RG8 to SPI2DATA OUT
   _SDI2R = 26; // Remap RP26 to SPI2DATA IN
   _TRISG6 = 0; // RG6 output for SPI2CLK
   _TRISG7 = 1; // RG7 input for SPI2 DATA IN
   _TRISG8 = 0; // RG8 output for SPI2 DATA OUT

   IFS2bits.SPI2IF = 0;    // Clear interrupt flag
   IEC2bits.SPI2IE = 0;	   // Disable interrupts

   SPI2CON1bits.MODE16 = 1; // Send 16 bits at a time 
   SPI2CON1bits.DISSCK = 0; // Enable internal spi clock 
   SPI2CON1bits.DISSDO = 0; // SDO2 pin is controlled by module
// TODO choose between SPI mode 1,1 or 0,0 
   SPI2CON1bits.SMP = 0; // Sample middle of data output time TODO
   SPI2CON1bits.CKE = 1; // Changes on active to idle TODO
   SPI2CON1bits.CKP = 0; // Clock active low TODO
   SPI2CON1bits.MSTEN = 1; // Enable master mode

   SPI2CON2bits.FRMEN = 0; // Disable frame support
   SPI2CON2bits.SPIBEN = 1; // Enable enhanced buffer mode

   SPI2STATbits.SPIROV = 0; // Clear SPIROV

   SPI2STATbits.SPIEN = 1; // Enable module
}

void SPI_Clock_Speed(uint8 pri_prescale, uint8 sec_prescale)
{
    /* Reference for SPI clock speed settings
    *   _____________________________Secondary Prescalar_________________
    *   FCY = 16 MHz______|______|__1:1__|__2:1__|__4:1__|__6:1__|__8:1__|
    *   Primary Prescalar |   1:1|Invalid|   8000|   4000|   2667|   2000|
    *                     |   4:1|   4000|   2000|   1000|    667|    500|
    *                     |  16:1|   1000|    500|    250|    167|    125|
    *                     |  64:1|    250|    125|     63|     42|     31|
    *   _________________________________________________________________
    *   FCY =  5 MHz______|______|__1:1__|__2:1__|__4:1__|__6:1__|__8:1__|
    *   Primary Prescalar |   1:1|   5000|   2500|   1250|    833|    625|
    *                     |   4:1|   1250|    625|    313|    208|    156|
    *                     |  16:1|    313|    156|     78|     52|     39|
    *                     |  64:1|     78|     39|     20|     13|     10|
    *
    * (SCKx frequencies shown in kHz)
    */
    
    SPI2CON1bits.PPRE = pri_prescale;
    SPI2CON1bits.SPRE = sec_prescale;

}

