/* 
 * File:   MikroSlider.h
 * Author: Tyler
 *
 * Easy Slider Functionality
 * Created on February 26, 2013, 6:30 PM
 */

#ifndef MIKROSLIDER_H
#define	MIKROSLIDER_H

#include "Mikro.h"
#include "MikroTouch.h"

enum orientation {hrz, vrt};


typedef struct  {
    uint16 posx; /* Minimum x Center Position */
    uint16 posy; /* Minimum y Center Position */
    enum orientation ort; /* hrz or vrt */
    uint16 size; /* Horizontal or Vertical size in Pixels */
    uint8 visible; /* 1 if shown, 0 if not shown */
    uint8 value; /* Value from 0 to size*/
    uint8 scaledvalue; /* Value from 0 to 255*/
} Slider;

void StartSlider(Slider* MySlider, uint16 posx, uint16 posy, uint16 size, enum orientation ort); /* Initialize Slider */
void CheckSlider(Slider* MySlider); /* Update Slider */


#endif	/* MIKROSLIDER_H */

