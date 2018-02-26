#include <stdio.h>
#include <stdlib.h>
#include "bodies.h"
#include "input.h"

const int GRAVITY = 1; //How much does gravity accelerate an object?
const int GRAVTICK = 2; //How often does gravity accelerate an object?
int gravdelay = 0;
const float FPS = 60; //The framerate is designed to never go above or below 60


body collide(body a, body b)
{
 //Let's take this one axis at a time . . .
puts("Starting new Collidenew function!");
 bool xcoll = false;
 bool ycoll = false; //These mean the two objects are about to collide in some axis if we move them any further
 bool xocup = false;
 bool yocup = false; //These mean the two objects already occupy one of the two same axes

 bool xmove;
 bool ymove; //If we are to collide in one axis but not the other, don't mess with the other's movement.

 //The object type might not collide with stuff.
 //Let's make sure we're actually colliding 2 solid objects.
 if (b.type != TYPE_BODY && b.type != TYPE_STATIC && b.type != TYPE_PLAYER && b.type != TYPE_CORPSE)//case for if the second object isn't solid
 {
  if (abs(a.x - b.x) < b.width + a.width && abs(a.y - b.y) < b.height + a.height)
  {
   if (b.type == TYPE_CHECKPOINT){a.spawnX = b.x; a.spawnY = b.y;}
   if ((b.type == TYPE_HURT || b.type == TYPE_HURT_VIS)){a.type = TYPE_CORPSE; a.yvel = -4;} //This is the easiest way to kill the player instantly
  }
  return a;//Now we won't continue on with the normal collision, because this object doesn't collide with anything.
 }

 if (a.type != TYPE_BODY && a.type != TYPE_STATIC && a.type != TYPE_PLAYER && a.type != TYPE_CORPSE)//case for if the first object is not solid
 {
  return a;
 }

 //Now to figure out the deal with x/ycoll/ocup
 //X first
 if(a.x + a.width < b.x - b.width || a.x - a.width > b.x + b.width)
 {
  xocup = false;
  puts("Objects aren't in x axis");
 }else{xocup = true; puts ("Objects ARE in x axis");}

 if (a.x + a.width + signOf(a.xvel) < b.x - b.width || a.x - a.width + signOf(a.xvel) > b.x + b.width)
 {
  xcoll = false;
  puts("Objects aren't about to collide");
 }else{xcoll = true; puts ("Objects will collide in x next frame");}
 //Now for Y
 if(a.y + a.height < b.y - b.height || a.y - a.height > b.y + b.height)
 {
  yocup = false;
  puts ("Objects aren't in y axis");
 }else{yocup = true; puts ("Objects ARE in y axis");}

 //Now to figure out if we even need to worry about the axises
 if(a.x + a.width + a.xvel < b.x - b.width || a.x - a.width + a.xvel > b.x + b.width){xmove = true; puts("No collide in X axis at all, skipping checks");}else{puts("Objects will collide in X"); xmove = false;}
 if(a.y + a.height + a.yvel < b.y - b.height || a.y - a.height + a.yvel > b.y + b.height){ymove = true; if(a.yvel != 0){a.canJump = false;} puts("No collide in y axis at all, skipping checks");}else{ymove = false; if(xocup && a.yvel >= 1){a.canJump = true;} puts("Player can jump, will collide in Y . . . ");}
 if(a.y + a.height + signOf(a.yvel) < b.y - b.height || a.y - a.height + signOf(a.yvel) > b.y + b.height)
 {
  ycoll = false;
  puts ("Objects aren't about to collide in y");
 }else{ycoll = true; puts ("Objects WILL collide in y next frame");}
 while (!xcoll)
 {
  if(xmove || !yocup){break;}
  a.x = a.x + signOf(a.xvel);
  if (a.x + a.width + signOf(a.xvel) < b.x - b.width || a.x - a.width + signOf(a.xvel) > b.x + b.width)
  {
   xcoll = false;
   puts("Objects aren't about to collide");
  }else{xcoll = true; puts ("Objects will collide in x next frame");}
 }
 if(xcoll && yocup){a.xvel = 0;}
 while (!ycoll)
 {
  if(ymove || !xocup){break;}
  a.y = a.y + signOf(a.yvel);
  if(a.y + a.height + signOf(a.yvel) < b.y - b.height || a.y - a.height + signOf(a.yvel) > b.y + b.height)
  {
   ycoll = false;
   puts ("Objects aren't about to collide in y");
  }else{ycoll = true; puts ("Objects WILL collide in y next frame");}

 }
 if(ycoll && xocup){a.yvel = 0;}
 printf("Finishing collideNew function with an xvel of %d and a yvel of %d\n", a.xvel, a.yvel);
 return a;
}

body updateBody(body b, body statics[], int staticcount, bool key[])
{
 puts("Updating a body!");
 if (b.type == TYPE_STATIC || b.type == TYPE_CHECKPOINT || b.type == TYPE_PASSIVE){return b;}
 if (b.type == TYPE_CORPSE)
 {
  if (key[KEY_DOWN])
  {
   b.type = TYPE_PLAYER;
   b.x = b.spawnX;
   b.y = b.spawnY;
   b.xvel = 0;
   b.yvel = 0;
   xscreen = b.x - SCREEN_W/2;
   yscreen = b.y - SCREEN_H/2;
   cameraFocusObject = &b;
  }
 }
 if (b.type == TYPE_PLAYER)
 {
  if(key[KEY_UP] && b.canJump)
  {
   b.yvel = -b.jumppower;
   b.canJump = false;
   puts ("Thing jumps!");
  }
  if(key[KEY_DOWN] && b.ability == ABIL_MORPH)
  {
  if(b.height == b.high){b.y = b.y + b.height/2;}
  b.height = b.high/2;
  }else if(b.height == b.high/2 && b.ability == ABIL_MORPH){b.height = b.high; b.y = b.y - b.high/2;} //if player is crouched down, get them up

  if(key[KEY_LEFT])
  {
   if (b.canJump){b.xvel = -b.speed;}
   else if(abs(b.xvel) <= 4){b.xvel = b.xvel - 1;}
   if (b.width == b.wide && b.ability == ABIL_MORPH){b.x = b.x - b.wide/2;}
   if(b.ability == ABIL_MORPH){b.width = b.wide/2;}
   b.direction = DIR_LEFT;
   puts ("Thing walks left!");
  }
  else if(key[KEY_RIGHT])
  {
   if(b.canJump){b.xvel = b.speed;}
   else if(abs(b.xvel) <= 4){b.xvel = b.xvel + 1;}
   if (b.width == b.wide && b.ability == ABIL_MORPH){b.x = b.x + b.wide/2;}
   if (b.ability == ABIL_MORPH){b.width = b.wide/2;}
   b.direction = DIR_RIGHT;
   puts ("Thing walks right!");
  }else
  {
   b.xvel -= signOf(b.xvel);
   if(b.width == b.wide/2)
   {
    if(b.direction == DIR_LEFT){b.x = b.x + b.width;}else{b.x = b.x - b.width;}
   }
   b.width = b.wide;
  }
  if (key[KEY_SPACE] && b.ability != ABIL_NONE && b.ability != ABIL_MORPH)
  {
   if(b.ability == ABIL_BOOST){b.speed = b.normSpeed * 2;}
  }else if (key[KEY_SPACE])
  {
   b.type = TYPE_CORPSE;
   cameraFocusObject = NULL;
  }else{b.speed = b.normSpeed;}
 }
 if(gravdelay == GRAVTICK)
 {
  puts ("Doing a gravity!");
  gravdelay = 0;
  b.yvel = b.yvel + GRAVITY;
  if (b.type == TYPE_CORPSE && b.canJump){b.xvel = b.xvel - signOf(b.xvel);}
  printf("%d\n", b.yvel);
 }else{gravdelay++;}

 for(int i = 0; i < staticcount; i++){body g = statics[i]; b = collide(b, g);}
 b.x = b.x + b.xvel;
 b.y = b.y + b.yvel;
 return b;
}
