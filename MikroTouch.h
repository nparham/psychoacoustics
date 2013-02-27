#ifndef MIKROTOUCH_H
#define MIKROTOUCH_H

/* call this first */
void ADCsetup(void);
void InitPot(void);
void MoveSlider(void);

//TODO return tsx, tsy from DetectTouch
void DetectTouch(void);

// until globals are removed
extern long tsx;
extern long tsy;


#endif // MIKROTOUCH_H
