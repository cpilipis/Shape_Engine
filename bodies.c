#include <stdio.h>
#include <stdlib.h>
#include "simplestuff.c" //Why is this in a separate file? It only defines ONE new function! One! And it's a function that C probably comes with in some math library anyways!

typedef struct body
{
 int type;
 int x;
 int y;
 int width;
 int height;
 int wide;
 int high; //high and wide are used for remembering how tall and wide something is supposed to be normally.
 int xvel;
 int yvel;
 int speed;
 int normSpeed; //used for remembering how fast something goes normally
 int direction;
 int jumppower;
 bool canJump;
 int ability;
 int spawnX;
 int spawnY;
} body;

body *cameraFocusObject; //A pointer of the object the camera should follow
//By making it a constant, we can make sure the function to follow the object with a camera does not alter it's x and y posiiton
//but by changing the address the pointer points to, we can switch objects on the fly.


body newBody(int x, int y, int width, int height, bool isControlled, int jumpheight, int speed, bool active, int ability)
{
 body f;
 f.x = x;
 f.y = y;
 if (isControlled){f.type = TYPE_PLAYER;}else if(active){f.type = TYPE_BODY;}else{f.type = TYPE_STATIC;}
 f.width = width;
 f.height = height;
 f.wide = width;
 f.high = height;
 f.xvel = 0;
 f.yvel = 0;
 f.speed = speed;
 f.normSpeed = speed;
 f.direction = DIR_LEFT;
 f.jumppower = jumpheight;
 f.canJump = false;
 f.ability = ability;
 f.spawnX = x;
 f.spawnY = y;
 return f;
}

body newStatic(int x1, int y1, int x2, int y2)
{
 body s;
 s.width = (x2 - x1)/2;
 s.height = (y2 - y1)/2;
 s.x = x1 + s.width ;
 s.y = y1 + s.height;
 s.type = TYPE_STATIC;
 s.wide = s.width;
 s.high = s.height;
 s.xvel = 0;
 s.yvel = 0;
 s.speed = 0;
 s.normSpeed = 0;
 s.direction = DIR_RIGHT;
 s.jumppower = 0;
 s.canJump = false;
 s.ability = ABIL_NONE;
 s.spawnX = s.x;
 s.spawnY = s.y;
 return s;
}

body newCheckpoint(int x, int y)
{
 body c;
 c.x = x;
 c.y = y;
 c.type = TYPE_CHECKPOINT;
 c.width = 35;
 c.height = 35;
 c.wide = 35;
 c.high = 35;
 c.xvel = 0;
 c.yvel = 0;
 c.speed = 0;
 c.normSpeed = 0;
 c.direction = DIR_RIGHT;
 c.jumppower = 0;
 c.canJump = false;
 c.ability = ABIL_NONE;
 c.spawnX = x;
 c.spawnY = y;
 return c;
}

body newHurt(int x1, int y1, int x2, int y2, bool vis)
{
 body s;
 s.width = (x2 - x1)/2;
 s.height = (y2 - y1)/2;
 s.x = x1 + s.width ;
 s.y = y1 + s.height;
 if (vis){s.type = TYPE_HURT_VIS;}else{s.type = TYPE_HURT;}
 s.wide = s.width;
 s.high = s.height;
 s.xvel = 0;
 s.yvel = 0;
 s.speed = 0;
 s.normSpeed = 0;
 s.direction = DIR_RIGHT;
 s.jumppower = 0;
 s.canJump = false;
 s.ability = ABIL_NONE;
 s.spawnX = s.x;
 s.spawnY = s.y;
 return s;
}
