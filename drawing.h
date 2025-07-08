#ifndef DRAWING
#define DRAWING
#include <stdio.h>
#include <stdlib.h>
#include "bodies.h"
#include <allegro5/allegro_primitives.h>

extern int xscreen;
extern int yscreen; //Coordinates of the scrolling camera
extern int scrollBoundaries;
extern int SCREEN_W; //Screen width and heights by default
extern int SCREEN_H;
extern int backCols [3];
extern int foreCols [3];


//draws a body
void drawBody(body b);

#endif
