#include <stdio.h>
#include <stdlib.h>
#include "bodies.h"

body spoutStatic(FILE *inlevl, int argNumber, int typeOfThing)
{
 fgetc(inlevl);
 char arg = fgetc(inlevl);
 char line[80];
 int numbs[argNumber];
 int charcounter = 0;
 int numbcounter = 0;
 bool negate = false;
 while(arg != '\n' && arg != EOF)
 {
  printf("Argument is not a new line!\n");
  if (isdigit(arg))
  {
   line[charcounter] = arg; charcounter = charcounter + 1; printf("Argument is a number\n");
  }
  else if (arg == '-')
  {
   printf("Looks like a negative number!\n");
   negate = true;
  }
  else
  {
   printf("Argument is not a number. Possibly space, or a semi colon.\n");
   charcounter = 0;
   numbs[numbcounter] = atoi(line);
   if (negate){numbs[numbcounter] = -numbs[numbcounter]; negate = false;}
   numbcounter = numbcounter + 1;
   for(int i = 0; i < 80; i++){line[i] = 0;}
  }
  arg = fgetc(inlevl);
 }
 body thisNewBody;
 if (typeOfThing == TYPE_CHECKPOINT)
 {
  thisNewBody = newCheckpoint(numbs[0], numbs[1]);
 }
 else if(typeOfThing == TYPE_PLAYER)
 {
  thisNewBody = newBody(numbs[0], numbs[1], numbs[2], numbs[3], true, 8, 4, true, ABIL_NONE);
 }
 else if(typeOfThing == TYPE_HURT)
 {
  thisNewBody = newHurt(numbs[0], numbs[1], numbs[2], numbs[3], false);
 }
 else if(typeOfThing == TYPE_HURT_VIS)
 {
  thisNewBody = newHurt(numbs[0], numbs[1], numbs[2], numbs[3], true);
 }
 else
 {
  thisNewBody = newStatic(numbs[0], numbs[1], numbs[2], numbs[3]);
 }
 return thisNewBody;
}
