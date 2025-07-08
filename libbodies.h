#ifndef LIBBODIES_DEF
#define LIBBODIES_DEF

#include <stdio.h>
#include <stdlib.h>
#include "bodies.h"
#include "input.h"
extern const int GRAVITY; //How much does gravity accelerate an object?
extern const int GRAVTICK; //How often does gravity accelerate an object?
extern int gravdelay;
extern const float FPS; //The framerate is designed to never go above or below 60

extern body * cameraFocusObject;
extern int xscreen;
extern int yscreen;
extern int SCREEN_W;
extern int SCREEN_H;

//this function computes a collision between two bodies.
body collide(body, body);

//updates a body, moves it along with it's velocity
body updateBody(body, body [], int, bool []);

#endif
