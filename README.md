# The Shape Engine
## A simple 2d game being written in C

### Features

- Runs at any resolution (but won't display right below 400 by 400; there's no visual glitches, but the player will tend to stay in a corner of the screen, rather than give you a good view of the rest of the level.)
- Reads text files to generate a level
- Currently has platforms, checkpoints, and death triggers that, for now, always display as spikes.

### Dependencies
This program uses [Allegro5](http://liballeg.org/) and all it's dependencies. That's about it. 
It's available for Windows, Mac, Linux, IOS, and Android.

### Compilation
`gcc simple_base.c -lallegro -lallegro_primitives -oShape`
On mac, you'll want to add a `-lallegro_main` in there. I don't know about any compilation on windows yet. It's likely possible if you fool around long enough in MinGW. If you figure out how to compile it on Windows.

### Running it
`./Shape example_level.txt`
You can add the -res argument for changing the resolution from 640X480 to something else. For example:
`./Shape example_level.txt -res 1920 1080`

The controls are as follows:
- Arrow keys to move
- Push space to trigger your own death
- If you died, your player will turn a solid red. Push the down arrow key to respawn

### Stuff I may want to do

- Implement visible spikes/hazards for bottomless pits. I have already programmed an invisible type of hurt trigger for bottomless pits, I just want to make it so that you can differentiate between visible and invisible triggers in the level file. They'll not only look different, but also behave a little different, too (the player's corpse currently bounces off of spikes. This is cool for spikes, but not for bottomless pits. Also, the camera shouldn't move with the corpse if it falls down a bottomless pit, so that the player flies off screen and thus the player doesn't see the end of the pit).
- Write a program to graphically build levels. Editing text files is boring. Maybe something in game would be cool, too. I could have a way to build stuff in game and a seperate "God edit" program for more powerful editing.
- Write documentation for every argument this game supports on the command line and every identifier used in mapping.
- Implement a networked multiplayer mode
- Write up a random level generator in Python. Maybe one based on the [libraryofbabel.info](http://www.libraryofbabel.info). It would be pretty cool to be able to choose any page in any book off that site and have the level be consistent via the book to level algorithm. Kinda like minecraft seeds for worlds.
