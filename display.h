#ifndef DISPLAY_H
#define DISPLAY_H

#define GROWS (8) // grid rows
#define GCOLS (8) // grid cols
#define RESOL (3) // number of heat levels displayed

void Update_sd(); // get new sensor data and fill in sensor_data with it.
void Draw_sd(unsigned char grows, unsigned char gcols);
unsigned int Sd_to_color(unsigned char sd); // map sensor datum to a color.

#endif // DISPLAY_H
