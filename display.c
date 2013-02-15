#include "display.h"
#include "Mikro.h"

// screen size: 320x240
#define SCR_WIDTH  (320)
#define SCR_HEIGHT (240)

//TODO form an array of colors based on RESOL, ranging from blue (cold) to red (hot)
//unsigned int SD_COLORS[RESOL];
unsigned char SENSOR_DATA[GROWS * GCOLS];

void Update_sd()
{
	// TODO real implementation when GRID-EYE comm is set up

	// stand-in implementation
	static unsigned int rot = 0;
	if( rot++ >= 3 ) {
		rot = 0;
	}
	unsigned int i;
	for ( i = 0; i < GROWS * GCOLS; i++ ) {
		SENSOR_DATA[i] = (i % 3) + rot;
		if (SENSOR_DATA[i] >= 3) {
			SENSOR_DATA[i] = 0;
		}
	}
}

void Draw_sd(unsigned char grows, unsigned char gcols)
{
	// TODO parametrize drawing area limits (currently fills screen)
	// TODO this stuff should probably only be calculated once
	unsigned int sq_w = SCR_WIDTH / gcols; // individual square width
	unsigned int sq_h = SCR_HEIGHT / grows; // individual square height

	unsigned int i;
	for( i = 0; i < grows*gcols; i++ ) {
		DrawRectangle(
				sq_w * (i % gcols),
				sq_h * (i / gcols), // do not remove the parentheses you fool
				sq_w * (1 + (i % gcols)), 
				sq_h * (1 + i / gcols), 
				Sd_to_color(SENSOR_DATA[i])
				);
	}
}

unsigned int Sd_to_color(unsigned char sd)
{
	//TODO base this on RESOL and use sd_colors[]
	static unsigned int replace_this[3] = {
		RGB(0,0,0), // black
		RGB(0,0,255), // blue
		RGB(255,0,0) // red
	};
	return replace_this[sd];
}
