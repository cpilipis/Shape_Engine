//This is the tables branch
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "simplestuff.c"

const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int GRAVITY = 1;
const int GRAVTICK = 2;
const bool MORPHY = false;
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
 if (!b.isActive)
 {
  al_draw_filled_rectangle(b.x - b.width, b.y - b.height, b.x + b.width, b.y + b.height, al_map_rgb(255, 0, 0));
 }
 else
 {
  al_draw_rectangle(b.x - b.width, b.y - b.height, b.x + b.width, b.y + b.height, al_map_rgb(255, 0, 0), 0);
 }
 if(b.isControlled)
 { //Draw a dot in the center of player objects
  al_draw_rectangle(b.x, b.y, b.x + 1, b.y + 1, al_map_rgb(255, 0, 0), 0);
 }
}

int main(int argc, char **argv)
{
 ALLEGRO_DISPLAY *display = NULL;
 ALLEGRO_EVENT_QUEUE *event_queue = NULL;
 ALLEGRO_TIMER *timer = NULL;
 body player = newBody(40, 40, 30, 30, true, 8, 4, true);
 body ground = newBody(150, 400, 300, 40, false, 0, 0, false);
 body wall = newBody(335, 200, 75, 1, false, 0, 0, false);
 body wallwall = newBody(260, 240, 1, 40, false, 0, 0, false);
 body otherwall = newBody(320, 200, 1, 75, false, 0, 0, false);
 body superotherwall = newBody(325, 200, 1, 75, false, 0, 0, false);
 body plat = newBody(250, 300, 20, 20, false, 0, 0, false);
 body poo = newBody(500, 100, 40, 1, false, 0, 0, false);
 body statics [] = {ground, wall, plat, poo, wallwall, otherwall, superotherwall};
 int staticcount = 7;
 bool key[4] = { false, false, false, false };
 bool redraw = true;
 bool doexit = false;

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
   redraw = true;
  }
  else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
  {
   break;
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
    puts ("Is this for loop even working???");
   }
   //body g = statics[0];
   //drawBody(g);

   al_flip_display();
  }
 }

 al_destroy_timer(timer);
 al_destroy_display(display);
 al_destroy_event_queue(event_queue);
 al_shutdown_primitives_addon();

 return 0;
}
