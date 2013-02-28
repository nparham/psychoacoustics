/* 
 * File:   MikroTouch.h
 * Author: Tyler
 *
 * Created on February 15, 2013, 11:09 AM
 */

#ifndef MIKROTOUCH_H
#define	MIKROTOUCH_H

#include "Mikro.h"
#include "p24FJ256GB110.h"

/****** Macros ********************************************************/
#define x_pixels (320)
#define y_pixels (240)

/****** Function prototypes *******************************************/
void DetectTouch(void);
void ADCsetup(void);
sint16 GetX(void);
sint16 GetY(void);
uint8 DetectRectangle(int xmin, int ymin, int xmax, int ymax);
uint8 DetectChar(char* strDetect);

/***********************************************************************
 * Global variables
 **********************************************************************/
extern sint32 tsx;
extern sint32 tsy;

#endif	/* MIKROTOUCH_H */

