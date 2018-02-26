#include <stdio.h>
#include <stdlib.h>
#include "bodies.h"
#include <allegro5/allegro_primitives.h>

int xscreen = 0;
int yscreen = 0; //Coordinates of the scrolling camera
int scrollBoundaries = 200;
int SCREEN_W = 640; //Screen width and heights by default
int SCREEN_H = 480;
int backCols [3] = {0, 0, 0};
int foreCols [3] = {255, 255, 255};


void drawBody(body b)
{
 int sideLeft;
 int sideRight;
 int sideTop;
 int sideBottom;
 int centx;
 int centy;
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
 if (b.type == TYPE_STATIC || b.type == TYPE_CORPSE)
 {
  al_draw_filled_rectangle(sideLeft, sideTop, sideRight, sideBottom, al_map_rgb(foreCols[0], foreCols[1], foreCols[2]));
 }
 else if (b.type == TYPE_BODY || b.type == TYPE_PLAYER)
 {
  al_draw_rectangle(sideLeft, sideTop, sideRight, sideBottom, al_map_rgb(foreCols[0], foreCols[1], foreCols[2]), 0);
 }
 else if(b.type == TYPE_CHECKPOINT)
 {
  //draw an X that is 7 pixels wide and high (one in the middle, some others on the sides)
  al_draw_line(centx - 35, centy - 35, centx + 35, centy + 35, al_map_rgb(foreCols[0], foreCols[1], foreCols[2]), 1);
  al_draw_line(centx - 35, centy + 35, centx + 35, centy - 35, al_map_rgb(foreCols[0], foreCols[1], foreCols[2]), 1);

 }
 if(b.type == TYPE_PLAYER)
 { //Draw a dot in the center of player objects
  al_draw_rectangle(centx, centy, centx + 1, centy + 1, al_map_rgb(foreCols[0], foreCols[1], foreCols[2]), 0);
 }
 if (b.type == TYPE_HURT_VIS)
 {
  int spikeEveners[25];
  printf("Let's draw some spikes!\n");
  int widthOfSpike = 15;
  int numberOfSpikes = (b.width*2)/widthOfSpike;
  printf("Width of spike is %d and number of spikes is %d.\n", widthOfSpike, numberOfSpikes);

  if (b.width * 2 <= widthOfSpike)
  {
   al_draw_line(sideLeft, sideBottom, sideLeft + widthOfSpike/2, sideTop, al_map_rgb(foreCols[0], foreCols[1], foreCols[2]), 0);
   al_draw_line(sideLeft + widthOfSpike/2, sideTop, sideRight, sideBottom, al_map_rgb(foreCols[0], foreCols[1], foreCols[2]), 0);
  }
  for (int spikeCount = 0; spikeCount < numberOfSpikes; spikeCount++)
  {
   int nextSpikeX = sideLeft + spikeCount * widthOfSpike;
   al_draw_line(nextSpikeX, sideBottom, nextSpikeX + widthOfSpike/2, sideTop, al_map_rgb(foreCols[0], foreCols[1], foreCols[2]), 0);
   al_draw_line(nextSpikeX + widthOfSpike/2, sideTop, nextSpikeX + widthOfSpike, sideBottom, al_map_rgb(foreCols[0], foreCols[1], foreCols[2]), 0);
   if (spikeCount == numberOfSpikes - 1)
   {
    printf("Looks like we're doing the last spike!\n");
    int nextSpikeX = sideLeft + spikeCount * widthOfSpike + widthOfSpike;
    int distToLastX = sideRight - nextSpikeX;
    al_draw_line(nextSpikeX, sideBottom, nextSpikeX + distToLastX/2, sideTop, al_map_rgb(foreCols[0], foreCols[1], foreCols[2]), 0);
    al_draw_line(nextSpikeX + distToLastX/2, sideTop, sideRight, sideBottom, al_map_rgb(foreCols[0], foreCols[1], foreCols[2]), 0);
   }
  }
 }
 if(b.type == TYPE_CORPSE)
 {
  //draw some text telling the player that they can push up to respawn
 }
}
