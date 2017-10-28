#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "simplestuff.c" //Why is this in a separate file? It only defines ONE new function! One! And it's a function that C probably comes with in some math library anyways!

const float FPS = 60;
int SCREEN_W = 640;
int SCREEN_H = 480;
const int GRAVITY = 1;
const int GRAVTICK = 2;
int xscreen = 0;
int yscreen = 0;
bool doScroll = true;
int scrollBoundaries = 200;
int gravdelay = 0;

enum MYKEYS {
 KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE
};

enum MYDIR{DIR_LEFT, DIR_RIGHT};

enum ABIL{ABIL_NONE, ABIL_TELE, ABIL_BOOST, ABIL_MORPH};

enum TYPE{TYPE_STATIC, TYPE_PASSIVE, TYPE_BODY, TYPE_PLAYER, TYPE_CHECKPOINT};

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
 if (b.type != TYPE_BODY && b.type != TYPE_STATIC)
 {
  if(b.type == TYPE_CHECKPOINT && abs(a.x - b.x) < 35 && abs(a.y - b.y) < 35){a.spawnX = b.x; a.spawnY = b.y;}
  return a;//Now we won't continue on with the normal collision, because this object doesn't collide with anything.
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
   b.xvel = -b.speed;
   if (b.width == b.wide && b.ability == ABIL_MORPH){b.x = b.x - b.wide/2;}
   if(b.ability == ABIL_MORPH){b.width = b.wide/2;}
   b.direction = DIR_LEFT;
   puts ("Thing walks left!");
  }
  else if(key[KEY_RIGHT])
  {
   b.xvel = b.speed;
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
   b.x = b.spawnX;
   b.y = b.spawnY;
   b.xvel = 0;
   b.yvel = 0;
   xscreen = b.x - SCREEN_W/2;
   yscreen = b.y - SCREEN_H/2;
  }else{b.speed = b.normSpeed;}
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
 int sideLeft;
 int sideRight;
 int sideTop;
 int sideBottom;
 int centx;
 int centy;
 if(doScroll)
 {
  sideLeft = b.x - b.width - xscreen;
  sideRight = b.x + b.width - xscreen;
  sideTop = b.y - b.height - yscreen;
  sideBottom = b.y + b.height - yscreen;
  centx = b.x - xscreen;
  centy = b.y - yscreen;

  /*to scroll as the player moves, we simply shift everything by the scroll variables. No more, no less.
  The most complex part is the actual shifting of the scroll variables, in which we make a variable to store the value
  that causes the screen to scroll when the player passes it in either direction. Then, we modify xscreen/yscreen when the
  player passes that point.*/
 }else{
  sideLeft = b.x - b.width - SCREEN_W * xscreen;
  sideRight = b.x + b.width - SCREEN_W * xscreen;
  sideTop = b.y - b.height - SCREEN_H * yscreen;
  sideBottom = b.y + b.height - SCREEN_H * yscreen;
  centx = b.x - SCREEN_W * xscreen;
  centy = b.y - SCREEN_H * yscreen;

  /*To scroll the screen when the player reaches the edge, we change the variable x(y)screen
  and draw everything back SCREEN_W(H) pixles, times x(y)screen.
  If it's 0, the player is on the first part of the level, if it's 1, they've gone right (or down) one screen, etc.*/
 }
 if (b.type == TYPE_STATIC)
 {
  al_draw_filled_rectangle(sideLeft, sideTop, sideRight, sideBottom, al_map_rgb(255, 0, 0));
 }
 else if (b.type == TYPE_BODY || b.type == TYPE_PLAYER)
 {
  al_draw_rectangle(sideLeft, sideTop, sideRight, sideBottom, al_map_rgb(255, 0, 0), 0);
 }
 else if(b.type == TYPE_CHECKPOINT)
 {
  //draw an X that is 7 pixels wide and high (one in the middle, some others on the sides)
  al_draw_line(centx - 35, centy - 35, centx + 35, centy + 35, al_map_rgb(255, 0, 0), 1);
  al_draw_line(centx - 35, centy + 35, centx + 35, centy - 35, al_map_rgb(255, 0, 0), 1);

 }
 if(b.type = TYPE_PLAYER)
 { //Draw a dot in the center of player objects
  al_draw_rectangle(centx, centy, centx + 1, centy + 1, al_map_rgb(255, 0, 0), 0);
 }
}

int main(int argc, char **argv)
{
 ALLEGRO_DISPLAY *display = NULL;
 ALLEGRO_EVENT_QUEUE *event_queue = NULL;
 ALLEGRO_TIMER *timer = NULL;
 ALLEGRO_COLOR pColor = al_map_rgb(255, 0, 0);
 int playerAbility = ABIL_NONE;
 bool key[5] = {false, false, false, false, false};
 bool redraw = true;
 bool doexit = false;

 body players[3];
 int playerCount = 0;

 FILE *levl;
 char ch;
 int charAbility = ABIL_NONE;

 for(int i = 2; i < argc; i++)
 {
  if (strcmp("-res", argv[i]) == 0){printf("Hey it works!\n"); SCREEN_W = atoi(argv[i+1]); SCREEN_H = atoi(argv[i+2]);}
  if (strcmp("-snapscreen", argv[i]) == 0){doScroll = false;}
  if (strcmp("-ability", argv[i]) == 0)
  {
   if(strcmp(argv[i + 1], "morph") == 0){playerAbility = ABIL_MORPH;}
   if(strcmp(argv[i + 1], "boost") == 0){playerAbility = ABIL_BOOST;}
  }
 }

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
//IN FACT, THE COMPILER DIDNT EVEN THROW ANY WARNINGS AND THE GAME RAN FINE
//THE ONLY REASON I NOTICED WAS THAT THE PROGRAM THREW AN ERROR WHEN I EXITED OUT OF IT
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
    if (isdigit(arg)){line[charcounter] = arg; charcounter = charcounter + 1; printf("Argument is a number\n");
    }else{
     printf("Argument is not a number. Possibly space, or a semi colon.\n");
     charcounter = 0;
     numbs[numbcounter] = atoi(line);
     numbcounter = numbcounter + 1;
     for(int i = 0; i < 80; i++){line[i] = 0;}
    }
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
  if (ch == 'p')
  {
   printf("Char is p. Beginning if statement.\n");
   fgetc(levl);
   char arg = fgetc(levl);
   char line[80];
   int numbs[4];
   int charcounter = 0;
   int numbcounter = 0;
   while(arg != '\n' && arg != EOF)
   {
    printf("Argument is not a new line!\n");
    if (isdigit(arg)){line[charcounter] = arg; charcounter = charcounter + 1; printf("Argument is a number\n");
    }else{
     printf("Argument is not a number. Possibly space, or a semi colon.\n");
     charcounter = 0;
     numbs[numbcounter] = atoi(line);
     numbcounter = numbcounter + 1;
     for(int i = 0; i < 80; i++){line[i] = 0;}
    }
    arg = fgetc(levl);
   }
   printf("Wrapping stuff up, time to add a player.\n");
   players[playerCount] = newBody(numbs[0], numbs[1], numbs[2], numbs[3], true, 8, 4, true, playerAbility);
   playerCount++;

   ch = fgetc(levl);
  }
  if (ch == 'c')
  {
   printf("Char is c. Beginning if statement.\n");
   fgetc(levl);
   char arg = fgetc(levl);
   char line[80];
   int numbs[2];
   int charcounter = 0;
   int numbcounter = 0;
   while(arg != '\n' && arg != EOF)
   {
    printf("Argument is not a new line!\n");
    if (isdigit(arg)){line[charcounter] = arg; charcounter = charcounter + 1; printf("Argument is a number\n");
    }else{
     printf("Argument is not a number. Possibly space, or a semi colon.\n");
     charcounter = 0;
     numbs[numbcounter] = atoi(line);
     numbcounter = numbcounter + 1;
     for(int i = 0; i < 80; i++){line[i] = 0;}
    }
    arg = fgetc(levl);
   }
   printf("Wrapping stuff up, time to add a checkpoint.\n");
   statics[staticcount] = newCheckpoint(numbs[0], numbs[1]);
   staticcount++;

   ch = fgetc(levl);
  }
 }
 fclose(levl);




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
   for(int i = 0; i < playerCount; i++)
   {
    players[i] = updateBody(players[i], statics, staticcount, key);
   }

   if (doScroll) //this pertains to players but isn't in the for loop
   {//because we don't want to make the screen scroll for every player, just the first one.
    if(players[0].x > SCREEN_W + xscreen - scrollBoundaries && players[0].xvel > 0){xscreen = xscreen + players[0].xvel;}
    if(players[0].x < xscreen + scrollBoundaries && players[0].xvel < 0){xscreen = xscreen + players[0].xvel;}
    if(players[0].y > SCREEN_H + yscreen - scrollBoundaries && players[0].yvel > 0){yscreen = yscreen + players[0].yvel;}
    if(players[0].y < yscreen + scrollBoundaries && players[0].yvel <= 0){yscreen = yscreen + players[0].yvel;}
   }else{
    //screen switching for x
    if(players[0].x > SCREEN_W*(xscreen+1)){xscreen = xscreen + 1;}
    else if (players[0].x < SCREEN_W * xscreen){xscreen = xscreen - 1;}
    //screen switching for y
    if(players[0].y > SCREEN_H*(yscreen+1)){yscreen = yscreen + 1;}
    else if (players[0].y < SCREEN_H * yscreen){yscreen = yscreen - 1;}
   }

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

    case ALLEGRO_KEY_SPACE:
     key[KEY_SPACE] = true;
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

    case ALLEGRO_KEY_SPACE:
     key[KEY_SPACE] = false;
     break;
   }
  }

  if(redraw && al_is_event_queue_empty(event_queue))
  {
   redraw = false;

   al_clear_to_color(al_map_rgb(0, 255, 255));

   for (int i = 0; i < playerCount; i++){drawBody(players[i]);}
   for(int i = 0; i<staticcount; i++){drawBody(statics[i]);} //draw everything

   al_flip_display();
  }
 }

 al_destroy_timer(timer);
 al_destroy_display(display);
 al_destroy_event_queue(event_queue);
 al_shutdown_primitives_addon();

 return 0;
}
