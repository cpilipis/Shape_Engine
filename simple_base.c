#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

#include "bodies.h"
#include "drawing.c"
#include "libbodies.h"
#include "input.h"
#include "loading.c"

int main(int argc, char **argv)
{
 ALLEGRO_DISPLAY *display = NULL;
 ALLEGRO_EVENT_QUEUE *event_queue = NULL;
 ALLEGRO_TIMER *timer = NULL;
 ALLEGRO_COLOR pColor = al_map_rgb(foreCols[0], foreCols[1], foreCols[2]);
 int playerAbility = ABIL_NONE;
 bool key[5] = {false, false, false, false, false};
 bool redraw = true;
 bool doexit = false;

 body players[3];
 int playerCount = 0;

 FILE *levl;
 char ch;
 int charAbility = ABIL_NONE;
 bool trippy = false;
 for(int i = 2; i < argc; i++)
 {
  if (strcmp("-res", argv[i]) == 0){SCREEN_W = atoi(argv[i+1]); SCREEN_H = atoi(argv[i+2]);}
  if (strcmp("-trippy", argv[i]) == 0){trippy = true;}
  if (strcmp("-ability", argv[i]) == 0)
  {
   if(strcmp(argv[i + 1], "morph") == 0){playerAbility = ABIL_MORPH;}
   if(strcmp(argv[i + 1], "boost") == 0){playerAbility = ABIL_BOOST;}
  }
  if (strcmp(argv[i], "-backgroundColor") == 0)
  {
   for (int n = 0; n < 3; n++)
   {
    backCols[n] = atoi(argv[i + 1 + n]);
   }
  }
  if (strcmp(argv[i], "-foregroundColor") == 0)
  {
   for (int n = 0; n < 3; n++)
   {
    foreCols[n] = atoi(argv[i + 1 + n]);
   }
  }
 }

 levl = fopen(argv[1], "r");
 if(levl == NULL)
 {
  printf("Please specify a filename for the level. For example, ./Shape example_level.txt should run the example level.\n");
  exit (0);
 }
 ch = fgetc(levl);
 int staticLimit = 50;//Don't store more statics than this variable allows for
 body statics[staticLimit]; //TODO: IMPLEMENT A SYSTEM THAT COUNTS THE STATIC OBJECTS IN A FILE
//RATHER THAN GIVE THE STATICS VARIABLE AN ARBITARY LIMIT
//HOW I WAS ABLE TO STORE 14 STATIC OBJECTS IN A TABLE THAT ONLY HAD ROOM FOR 10 IS BEYOND ME
//IN FACT, THE COMPILER DIDNT EVEN THROW ANY WARNINGS AND THE GAME RAN FINE
//THE ONLY REASON I NOTICED WAS THAT THE PROGRAM THREW AN ERROR WHEN I EXITED OUT OF IT
 int staticcount = 0;
 while (ch != EOF)
 {
  printf("Char is not EOF!\n");
  while(ch == ' ' || ch == '\n' || ch == ';'){ch = fgetc(levl); printf("Looks like we've got some dead space, or a stray semicolon.\n");}
  if (ch == 's')
  {
   printf("Let's add a static object.\n");
   statics[staticcount] = spoutStatic(levl, 4, TYPE_STATIC);
   staticcount++;
   ch = fgetc(levl);
  }
  if (ch == 'p')
  {
   printf("Let's add a player.\n");
   players[playerCount] = spoutStatic(levl, 4, TYPE_PLAYER);
   cameraFocusObject = &players[playerCount];
   playerCount++;
   ch = fgetc(levl);
  }
  if (ch == 'c')
  {
   printf("Let's add a checkpoint.\n");
   statics[staticcount] = spoutStatic(levl, 2, TYPE_CHECKPOINT);
   staticcount++;
   ch = fgetc(levl);
  }
  if (ch == 'h')
  {
   printf("Let's add a hurt trigger.\n");
   statics[staticcount] = spoutStatic(levl, 4, TYPE_HURT_VIS);
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
 int colorTrip = 1;
 int colorSkip = 2;
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

   if (cameraFocusObject != NULL) //this pertains to players but isn't in the for loop
   {//because we don't want to make the screen scroll for every player, just the first one.
    if(cameraFocusObject->x > SCREEN_W + xscreen - scrollBoundaries && cameraFocusObject->xvel > 0){xscreen = xscreen + cameraFocusObject->xvel;}
    if(cameraFocusObject->x < xscreen + scrollBoundaries && cameraFocusObject->xvel < 0){xscreen = xscreen + cameraFocusObject->xvel;}
    if(cameraFocusObject->y > SCREEN_H + yscreen - scrollBoundaries && cameraFocusObject->yvel > 0){yscreen = yscreen + cameraFocusObject->yvel;}
    if(cameraFocusObject->y < yscreen + scrollBoundaries && cameraFocusObject->yvel <= 0){yscreen = yscreen + cameraFocusObject->yvel;}
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
   if (trippy == true)
   { //color shifting code
    for (int f = 1; f <= colorSkip; f++)
    {
     for (int a = 0; a < 3; a++){foreCols[a] = foreCols[a] + colorTrip;}
     if (foreCols[1] + colorTrip > 255 || foreCols[1] + colorTrip < 0){break;}
    }
    if (foreCols[1] >= 250){colorTrip = -1;}
    if (foreCols[1] <= 5){colorTrip = 1;}
   }

   al_clear_to_color(al_map_rgb(backCols[0], backCols[1], backCols[2]));

   for (int i = 0; i < playerCount; i++){drawBody(players[i]);}
   for(int i = 0; i<staticcount; i++){drawBody(statics[i]);} //draw everything

   al_flip_display();
  }
 }

 al_destroy_timer(timer);
 al_destroy_display(display);
 al_destroy_event_queue(event_queue);
 al_shutdown_primitives_addon();
 printf("Size of a single body in memory: %d\n", sizeof(body));
 printf("Size of the statics table in total: %d\n", sizeof(statics));
 return 0;
}
