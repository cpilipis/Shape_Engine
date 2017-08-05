#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "simplestuff.c"

const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int GRAVITY = 1;
const int GRAVTICK = 2;
int gravdelay = 0;
enum MYKEYS {
 KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

typedef struct body
{
 int x;
 int y;
 int width;
 int height;
 int xvel;
 int yvel;
 int speed;
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
 f.xvel = 0;
 f.yvel = 0;
 f.speed = speed;
 f.jumppower = jumpheight;
 f.isControlled = isControlled;
 f.isActive = active;
 f.canJump = false;
 return f;
}

body collide(body a, body b, bool last)
{
 //let's define some side values to make the if statements and loops slightly more readable
 puts ("Beginning collide test!");
 if (!(a.x + a.width + a.xvel < b.x - b.width || a.x - a.width + a.xvel > b.x + b.width) && !(a.y - a.height + a.yvel > b.y + b.height || a.y + a.height + a.yvel < b.y - b.height))
 { //To handle collision, we'll quit moving object A when it's about to hit object B.
  puts("Yup, things will collide normally, let's step in and do this pixel perfect.");
  bool xcoll = false;
  bool ycoll = false; //These mean the two objects are about to collide in some axis if we move them any further
  bool xocup = false;
  bool yocup = false; //These mean the two objects already occupy one of the two same axes

  bool xmove;
  bool ymove; //If we are to collide in one axis but not the other, don't mess with the other's movement.

  if(a.x + a.width + a.xvel < b.x - b.width || a.x - a.width + a.xvel > b.x + b.width){xmove = true; puts("No collide in X axis");}else{xmove = false;}
  if(a.y + a.height + a.yvel < b.y - b.height || a.y - a.height + a.yvel > b.y + b.height){ymove = true; puts("No collide in y axis");}else{ymove = false;}
  while(!(xcoll && ycoll))
  {
   if(xmove){xocup = false; xcoll = false; puts("Skipping X test");}else{
   if(a.x + a.width < b.x - b.width || a.x - a.width > b.x + b.width)
   {
    xocup = false;
    puts("Objects aren't in x axis");
   }else{xocup = true; puts ("Objects ARE in x axis");}

   if (a.x + a.width + signOf(a.xvel) < b.x - b.width || a.x - a.width + signOf(a.xvel) > b.x + b.width)
   {
    xcoll = false;
    puts("Objects aren't about to collide");
   }else{xcoll = true; puts ("Objects will collide in x next frame");}}

   if(a.y + a.height < b.y - b.height || a.y - a.height > b.y + b.height)
   {
    yocup = false;
    puts ("Objects aren't in y axis");
   }else{yocup = true; puts ("Objects ARE in y axis");}



  if(a.y + a.height + signOf(a.yvel) < b.y - b.height || a.y - a.height + signOf(a.yvel) > b.y + b.height)
  {
   ycoll = false;
   puts ("Objects aren't about to collide in y");
  }else{ycoll = true; puts ("Objects WILL collide in y next frame");}

  printf ("About to try to move, with xvel of %d and yvel of %d\n", a.xvel, a.yvel);
  if((!xcoll || !yocup) && a.xvel != 0 && !xmove) {a.x = a.x + signOf(a.xvel);}else if(!ymove){a.xvel = 0;}
  if((!ycoll || !xocup) && a.yvel != 0 && !ymove) {a.y = a.y + signOf(a.yvel);}else if(!ymove){a.yvel = 0;}
 }
 if(xmove){}//a.x = a.x + a.xvel;}
 if(ymove){}//a.y = a.y + a.yvel;}
 if(xocup && a.y + a.height == b.y - b.height - 1){a.canJump = true; printf("Player can jump!\n");}else{a.canJump = false;}
}else{
puts ("Things aren't running into eachother, machine says . . . ");
bool xocup = false;
if(a.x + a.width < b.x - b.width || a.x - a.width > b.x + b.width)
{
 xocup = false;
 puts("Objects aren't in x axis");
}else{xocup = true; puts ("Objects ARE in x axis");}
printf("%d, %d\n", a.y + a.height, b.y + b.height);
if(xocup && a.y + a.height == b.y + b.height + 4){a.canJump = true; printf("Player can jump!\n");}else{a.canJump = false;}}
return a;
}

body updateBody(body b, body g, body w, bool key[])
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

  if(key[KEY_LEFT])
  {
   b.xvel = -b.speed;
   puts ("Thing walks left!");
  }
  else if(key[KEY_RIGHT])
  {
   b.xvel = b.speed;
   puts ("Thing walks right!");
  }else{b.xvel = 0;}
 }
 if(gravdelay == GRAVTICK)
 {
  puts ("Doing a gravity!");
  gravdelay = 0;
  b.yvel = b.yvel + GRAVITY;
  printf("%d\n", b.yvel);
 }else{gravdelay++;}
 b = collide(b, g, false);
 b = collide(b, w, true);
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
 {
  al_draw_rectangle(b.x, b.y, b.x + 1, b.y + 1, al_map_rgb(255, 0, 0), 0);
 }
}

int main(int argc, char **argv)
{
 ALLEGRO_DISPLAY *display = NULL;
 ALLEGRO_EVENT_QUEUE *event_queue = NULL;
 ALLEGRO_TIMER *timer = NULL;
 body player = newBody(40, 40, 10, 10, true, 8, 4, true);
 body ground = newBody(150, 400, 300, 80, false, 0, 0, false);
 body wall = newBody(300, 275, 40, 40, false, 0, 0, false);
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
   player = updateBody(player, ground, wall, key);
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

   drawBody(ground);

   drawBody(wall);

   al_flip_display();
  }
 }

 al_destroy_timer(timer);
 al_destroy_display(display);
 al_destroy_event_queue(event_queue);
 al_shutdown_primitives_addon();

 return 0;
}