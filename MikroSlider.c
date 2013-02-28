
#include "MikroSlider.h"
/* Macros */
#define SlideInColor  RGB(190,190,190) /* Grey */
#define SlideOutColor RGB(100,100,100) /* Dark Grey */
#define SliderBoarderSize 2
#define SliderKnobSize 5
#define DetectOutline 5

/* Function Prototypes */
void DisplaySlider(Slider* MySlider);
void DrawCircle(uint16 x0, uint16 y0, uint16 rad, uint8 Color);

/*
 * Creates Slider
 * 0 < posx < 320
 * 0 < posy < 240
 * 0 < size
 * ort = hrz or vrt
 */
void StartSlider(Slider* MySlider, uint16 posx, uint16 posy, uint16 size, enum orientation ort){

    MySlider->visible = 1; /* Initialized to shown */

    MySlider->posx = posx;
    MySlider->posy = posy;
    MySlider->size = size;
    MySlider->ort = ort;

    MySlider->value = 0;
    MySlider->scaledvalue = 0;
    DisplaySlider(MySlider);
    
}

void CheckSlider(Slider* MySlider){
    if (MySlider->ort){
        if(DetectRectangle(MySlider->posx-DetectOutline, MySlider->posy - MySlider->size,
                           MySlider->posx+1+DetectOutline, MySlider->posy)){
            
            if(tsy != (MySlider->posy - MySlider->value)){
                MySlider->value = MySlider->posy - tsy;
                DisplaySlider(MySlider);
                MySlider->scaledvalue = ((float)(MySlider->value)/MySlider->size)*255;
            }
            
        }
    } else {
        if(DetectRectangle(MySlider->posx, MySlider->posy - DetectOutline,
                           MySlider->posx+MySlider->size, MySlider->posy+1+DetectOutline)){

            if(tsx != (MySlider->posx + MySlider->value)){
                MySlider->value = (tsx - MySlider->posx);
                DisplaySlider(MySlider);
                MySlider->scaledvalue = ((float)(MySlider->value)/MySlider->size)*255;
            }

        }
    }
    
}

void DisplaySlider(Slider* MySlider){

    if (MySlider->ort){
        /* Vertical Orientation */

        /* Erases Slider */
        DrawRectangle(MySlider->posx-SliderKnobSize+1, MySlider->posy - MySlider->size - SliderKnobSize,
                        MySlider->posx+SliderKnobSize, MySlider->posy + SliderKnobSize, BKGD);

        /* Draws Outline of Slider */
        DrawRectangle(MySlider->posx-SliderBoarderSize, MySlider->posy - MySlider->size,
                        MySlider->posx+1+SliderBoarderSize, MySlider->posy, SlideOutColor);

        /* Draws Inline of Slider */
        DrawRectangle(MySlider->posx, MySlider->posy - MySlider->size,
                        MySlider->posx+1, MySlider->posy, SlideInColor);

        /* Draws Slider */
        DrawRectangle(MySlider->posx-SliderKnobSize+1, MySlider->posy-SliderKnobSize - (MySlider->value),
                        MySlider->posx+SliderKnobSize, MySlider->posy+SliderKnobSize - (MySlider->value), BLACK);
    } else {
        /* Horizontal Orientation */

        /* Erases Slider */
        DrawRectangle(MySlider->posx-SliderKnobSize, MySlider->posy - SliderKnobSize-1,
                        MySlider->posx+MySlider->size+SliderKnobSize, MySlider->posy+SliderKnobSize, BKGD);

        DrawRectangle(MySlider->posx, MySlider->posy - SliderBoarderSize,
                        MySlider->posx+MySlider->size, MySlider->posy+SliderBoarderSize, SlideOutColor);

        DrawRectangle(MySlider->posx, MySlider->posy - 1,
                        MySlider->posx + MySlider->size, MySlider->posy, SlideInColor);

        DrawRectangle(MySlider->posx-SliderKnobSize+1 + (MySlider->value), MySlider->posy-SliderKnobSize,
                        MySlider->posx+SliderKnobSize + (MySlider->value), MySlider->posy+SliderKnobSize, BLACK);
    }

}

void DrawCircle(uint16 x0, uint16 y0, uint16 rad, uint8 Color){

    uint16 x = rad;
    uint16 y = 0;
    uint16 xChange = 1 - (rad << 1);
    uint16 yChange = 0;
    uint16 radError = 0;

    while (x >= y){
        WritePixelAt(x + x0, y + y0, Color);
        WritePixelAt(y + x0, x + y0, Color);
        WritePixelAt(-x + x0, y + y0, Color);
        WritePixelAt(-y + x0, x + y0, Color);
        WritePixelAt(-x + x0, -y + y0, Color);
        WritePixelAt(-y + x0, -x + y0, Color);
        WritePixelAt(x + x0, -y + y0, Color);
        WritePixelAt(y + x0, -x + y0, Color);

        y++;
        radError += yChange;
        yChange += 2;
        if(((radError << 1) + xChange) > 0)
        {
          x--;
          radError += xChange;
          xChange += 2;
        }
    }
}
