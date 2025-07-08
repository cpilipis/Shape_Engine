#ifndef BODY_DEF
#define BODY_DEF
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "simplestuff.h" //Why is this in a separate file? It only defines ONE new function! One! And it's a function that C probably comes with in some math library anyways!

typedef struct s_body {
	int type;
	int x;
	int y;
	int width;
	int height;
	int wide;
	int high; //remembering the original values of width and height, used for crouching
	int xvel;
	int yvel;
	int speed;
	int normSpeed;
	int direction;
	int jumppower;
	bool canJump;
	int ability;
	int spawnX;
	int spawnY;
} body;

//TODO: the above struct highlights a need for a "intvec2" struct
//just a 2D vector of ints, it's a common need and would simplify the structure
//greatly.



enum MYDIR{DIR_LEFT, DIR_RIGHT};

enum ABIL{ABIL_NONE, ABIL_TELE, ABIL_BOOST, ABIL_MORPH};

enum TYPE{TYPE_STATIC, TYPE_PASSIVE, TYPE_BODY, TYPE_PLAYER, TYPE_CHECKPOINT, TYPE_CORPSE, TYPE_HURT, TYPE_HURT_VIS};

body newBody(int x, int y, int width, int height, bool isControlled, int jumpheight, int speed, bool active, int ability);

body newStatic(int x1, int y1, int x2, int y2);

body newCheckpoint(int x, int y);

body newHurt(int x1, int y1, int x2, int y2, bool vis);

#endif
