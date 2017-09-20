//This is the tables branch
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "simplestuff.c" //Why is this in a separate file? It only defines ONE new function! One!

const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int GRAVITY = 1;
const int GRAVTICK = 2;
const bool MORPHY = false;
int xscreen = 0;
int yscreen = 0;
int gravdelay = 0;

enum MYKEYS {
 KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

enum MYDIR{DIR_LEFT, DIR_RIGHT};

typedef struct body
{
 int x;
 int y;
 int width;
 int height;
 int wide;
 int high; //high and wide are used for remembering how tall and wide something is supposed to be normally.
 int xvel;
 int yvel;
 int speed;
 int direction;
 int jumppower;
 bool isControlled;
 bool isActive;
 bool canJump;
} body;

body newBody(int x, int y, int width, int height, bool isControlled, int jumpheight, int speed, bool active)
{
 body f;
 f.x = x;
 f.y = y;
 f.width = width;
 f.height = height;
 f.wide = width;
 f.high = height;
 f.xvel = 0;
 f.yvel = 0;
 f.speed = speed;
 f.direction = DIR_LEFT;
 f.jumppower = jumpheight;
 f.isControlled = isControlled;
 f.isActive = active;
 f.canJump = false;
 return f;
}

body newStatic(int x1, int y1, int x2, int y2)
{
 body s;
 s.width = (x2 - x1)/2;
 s.height = (y2 - y1)/2;
 s.x = x1 + s.width ;
 s.y = y1 + s.height;
 s.wide = s.width;
 s.high = s.height;
 s.xvel = 0;
 s.yvel = 0;
 s.speed = 0;
 s.direction = DIR_RIGHT;
 s.jumppower = 0;
 s.isControlled = false;
 s.isActive = false;
 s.canJump = false;
 return s;
}

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
 if (!b.isActive){return b;}
 if (b.isControlled)
 {
  if(key[KEY_UP] && b.canJump)
  {
   b.yvel = -b.jumppower;
   b.canJump = false;
   puts ("Thing jumps!");
  }
  if(key[KEY_DOWN] && MORPHY)
  {
  if(b.height == b.high){b.y = b.y + b.height/2;}
  b.height = b.high/2;
  }else if(b.height == b.high/2 && MORPHY){b.height = b.high; b.y = b.y - b.high/2;} //if player is crouched down, get them up

  if(key[KEY_LEFT])
  {
   b.xvel = -b.speed;
   if (b.width == b.wide && MORPHY){b.x = b.x - b.wide/2;}
   if(MORPHY){b.width = b.wide/2;}
   b.direction = DIR_LEFT;
   puts ("Thing walks left!");
  }
  else if(key[KEY_RIGHT])
  {
   b.xvel = b.speed;
   if (b.width == b.wide && MORPHY){b.x = b.x + b.wide/2;}
   if (MORPHY){b.width = b.wide/2;}
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
 }
 if(gravdelay == GRAVTICK)
 {
  puts ("Doing a gravity!");
  gravdelay = 0;
  b.yvel = b.yvel + GRAVITY;
  printf("%d\n", b.yvel);
 }else{gravdelay++;}

 for(int i = 0; i < staticcount; i++){body g = statics[i]; b = collide(b, g);}
 b.x = b.x + b.xvel;
 b.y = b.y + b.yvel;
 return b;
}

void drawBody(body b)
{
 int sideLeft = b.x - b.width - SCREEN_W * xscreen;
 int sideRight = b.x + b.width - SCREEN_W * xscreen;
 int sideTop = b.y - b.height - SCREEN_H * yscreen;
 int sideBottom = b.y + b.height - SCREEN_H * yscreen; /*To scroll the screen when the player reaches the edge, we change the variable x(y)screen
and draw everything back SCREEN_W(H) pixles, times x(y)screen.
If it's 0, the player is on the first part of the level, if it's 1, they've gone right (or down) one screen, etc.*/
 if (!b.isActive)
 {
  al_draw_filled_rectangle(sideLeft, sideTop, sideRight, sideBottom, al_map_rgb(255, 0, 0));
 }
 else
 {
  al_draw_rectangle(sideLeft, sideTop, sideRight, sideBottom, al_map_rgb(255, 0, 0), 0);
 }
 if(b.isControlled)
 { //Draw a dot in the center of player objects
  al_draw_rectangle(b.x - SCREEN_W * xscreen, b.y - SCREEN_H * yscreen, b.x + 1 - SCREEN_W * xscreen, b.y + 1 - SCREEN_H * yscreen, al_map_rgb(255, 0, 0), 0);
 }
}

int main(int argc, char **argv)
{
 ALLEGRO_DISPLAY *display = NULL;
 ALLEGRO_EVENT_QUEUE *event_queue = NULL;
 ALLEGRO_TIMER *timer = NULL;

 body player = newBody(40, 40, 15, 15, true, 8, 4, true);

 bool key[4] = { false, false, false, false };
 bool redraw = true;
 bool doexit = false;

 FILE *levl;
 char ch;

 levl = fopen(argv[1], "r");
 if(levl == NULL)
 {
  printf("Please specify a filename for the level. For example, ./Shape example_level.txt should run the example level.\n");
  exit (0);
 }
 ch = fgetc(levl);
 int staticLimit = 30;//Don't store more statics than this variable allows for
 body statics[staticLimit]; //TODO: IMPLEMENT A SYSTEM THAT COUNTS THE STATIC OBJECTS IN A FILE
//RATHER THAN GIVE THE STATICS VARIABLE AN ARBITARY LIMIT
//HOW I WAS ABLE TO STORE 14 STATIC OBJECTS IN A TABLE THAT ONLY HAD ROOM FOR 10 IS BEYOND ME
 int staticcount = 0;
 while (ch != EOF)
 {
  printf("Char is not EOF!\n");
  if (ch == 's')
  {
   printf("Char is s. Beginning if statement.\n");
   fgetc(levl);
   char arg = fgetc(levl);
   char line[80];
   int numbs[4];
   int charcounter = 0;
   int numbcounter = 0;
   while(arg != '\n' && arg != EOF)
   {
    printf("Argument is not a new line!\n");
    if (isdigit(arg)){line[charcounter] = arg; charcounter = charcounter + 1; printf("Argument is a number\n");}else{printf("Argument is not a number. Possibly space, or a semi colon.\n"); charcounter = 0; numbs[numbcounter] = atoi(line); numbcounter = numbcounter + 1;
     for(int i = 0; i < 80; i++){line[i] = 0;}}
    arg = fgetc(levl);
   }
   printf("Wrapping stuff up, time to add a static body.\n");
   //if (staticcount <= 10)
   //{
   statics[staticcount] = newStatic(numbs[0], numbs[1], numbs[2], numbs[3]);
   staticcount++;
   //}else{printf("The game cannot currently store more than 10 statics. Sorry.\n");}
   printf ("So far we have %d static objects.\n", staticcount);
   ch = fgetc(levl);
  }
 }
 fclose(levl);

 /*for(int i = 2; i < argc; i++)
 {
  if (strcmp("-res", argv[i]) == 0){printf("Hey it works!\n"); SCREEN_W = atoi(argv[i+1]); SCREEN_H = atoi(argv[i+2]);}
  if (strcmp("-morph", argv[i]) == 0){MORPHY = true;}
 }*/

 if(!al_init())
 {
  fprintf(stderr, "failed to initialize allegro!\n");
  return -1;
 }

 if(!al_install_keyboard())
 {
  fprintf(stderr, "failed to initialize the keyboard!\n");
  return -1;
 }

 timer = al_create_timer(1.0 / FPS);
 if(!timer)
 {
  fprintf(stderr, "failed to create timer!\n");
  return -1;
 }

 display = al_create_display(SCREEN_W, SCREEN_H);
 if(!display) {
  fprintf(stderr, "failed to create display!\n");
  al_destroy_timer(timer);
  return -1;
 }
 bool prim = al_init_primitives_addon();
 if(!prim)
 {
  fprintf(stderr, "failed to init primitives!\n");
  al_destroy_timer(timer);
  return -1;
 }

 event_queue = al_create_event_queue();
 if(!event_queue)
 {
  fprintf(stderr, "failed to create event_queue!\n");
  al_shutdown_primitives_addon();
  al_destroy_display(display);
  al_destroy_timer(timer);
  return -1;
 }

 al_register_event_source(event_queue, al_get_display_event_source(display));

 al_register_event_source(event_queue, al_get_timer_event_source(timer));

 al_register_event_source(event_queue, al_get_keyboard_event_source());

 al_clear_to_color(al_map_rgb(0,0,0));

 al_flip_display();

 al_start_timer(timer);

 while(!doexit)
 {
  ALLEGRO_EVENT ev;
  al_wait_for_event(event_queue, &ev);

  if(ev.type == ALLEGRO_EVENT_TIMER)
  {
   player = updateBody(player, statics, staticcount, key);
   //screen switching for x
   if(player.x > SCREEN_W*(xscreen+1)){xscreen = xscreen + 1;}
   else if (player.x < SCREEN_W * xscreen){xscreen = xscreen - 1;}
   //screen switching for y
   if(player.y > SCREEN_H*(yscreen+1)){yscreen = yscreen + 1;}
   else if (player.y < SCREEN_H * yscreen){yscreen = yscreen - 1;}
   redraw = true;
  }
  else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
  {
   doexit = true;
  }
  else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
  {
   switch(ev.keyboard.keycode)
   {
    case ALLEGRO_KEY_UP:
     key[KEY_UP] = true;
     break;

    case ALLEGRO_KEY_DOWN:
     key[KEY_DOWN] = true;
     break;

    case ALLEGRO_KEY_LEFT:
     key[KEY_LEFT] = true;
     break;

    case ALLEGRO_KEY_RIGHT:
     key[KEY_RIGHT] = true;
     break;
   }
  }
  else if(ev.type == ALLEGRO_EVENT_KEY_UP)
  {
   switch(ev.keyboard.keycode)
   {
    case ALLEGRO_KEY_UP:
     key[KEY_UP] = false;
     break;

    case ALLEGRO_KEY_DOWN:
     key[KEY_DOWN] = false;
     break;

    case ALLEGRO_KEY_LEFT:
     key[KEY_LEFT] = false;
     break;

    case ALLEGRO_KEY_RIGHT:
     key[KEY_RIGHT] = false;
     break;

    case ALLEGRO_KEY_ESCAPE:
     doexit = true;
     break;
   }
  }

  if(redraw && al_is_event_queue_empty(event_queue))
  {
   redraw = false;

   al_clear_to_color(al_map_rgb(0, 255, 255));

   drawBody(player);
   int i;
   for(i = 0; i<staticcount; i++)
   {
    body g = statics[i];
    drawBody(g);
   }

   al_flip_display();
  }
 }

 al_destroy_timer(timer);
 al_destroy_display(display);
 al_destroy_event_queue(event_queue);
 al_shutdown_primitives_addon();

 return 0;
}
