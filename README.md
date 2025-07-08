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
If you're on Linux and you have Allegro 5 installed system-wide, running the makefile should work. If you don't want to install allegro system-wide, I suppose you could compile it, then modify the makefile to point to the directory that the library file resides in to compile the final executable. 

### Running it
Just run the executable that pops out of the compliler, typically "Shape" if you used the command above. You have to specify a level as the first argument, which is why even the source code of the game also comes with an example level called example_level.txt. Loading the level HAS to be the first argument for the program.
`./Shape example_level.txt`
there are optional arguments which go after the executable and the level. If you want to use one of these, you HAVE to put it after the level.
You can add the -res argument for changing the resolution from 640X480 to something else. For example:
`./Shape example_level.txt -res 1920 1080`
The game by default runs with a black background and white foreground, but you can change this by specifying the -backgroundColor and -foregroundColor arguments and then providing RGB values for each. For example, This next one sets the background cyan and the foreground red.
`./Shape example_level.txt -backgroundColor 0 255 255 -foregroundColor 255 0 0`

You can add the -trippy argument, but it isn't as cool as it sounds, seems I didn't program this one right back in the day.
You can add the -abililty argument, followed by a space and then one of two abilities: morph and boost.
Controls for these are described later in the controls section of this readme.
To clarify, the arguments would look like this:
`./Shape example_level.txt -ability boost`
`./Shape example_level.txt -ability morph`

### Controls
The controls for actually playing the game are as follows:
- Arrow keys to move
- Push space to trigger your own death (if you don't have the boost ability)
- If you died, your player will turn a solid red. Push the down arrow key to respawn
- If you have the morph ability, your character will squish sideways as they move in either direction sideways, and you can hold the down arrow key to squish downwards (crouch).
- If you have the boost ability, your character will get a speed boost in whatever direction you move as long as you hold space and are on the ground. Doesn't seem to work in mid-air though.

### abitious ideas I had as a 15 year old that shall never pan out on this decrepit codebase
- Implement invisible death triggers. They'll also behave differently from spikes.
- Write documentation for every argument this game supports on the command line and every identifier used in mapping.
- Write a program to graphically build levels. Editing text files is boring, and it's hard to predict the results of the level you'll end up with. Maybe something in game would be cool, too. I could have a way to build stuff in game and a seperate "God edit" program for more powerful editing.
- Implement a Lua interpreter so that the player can implement their own items and platforms.
- Implement a networked multiplayer mode
- Write up a random level generator in Python. Maybe one based on the [libraryofbabel.info](http://www.libraryofbabel.info). It would be pretty cool to be able to choose any page in any book off that site and have the level be consistent via the book to level algorithm. Kinda like minecraft seeds for worlds.
