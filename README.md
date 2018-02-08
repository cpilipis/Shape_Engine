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
On mac, you'll want to add a `-lallegro_main` in there. I don't know about any compilation on windows yet. It's likely possible if you fool around long enough in MinGW. If you figure out how to compile it on Windows and you know your instructions will work for most recent Windows machines (Ideally the method should work for Windows 7 and beyond, although Windows XP support would be kinda cool. Anything older than that might be redundant.), please click the button to edit this file, type them here, and send me a pull request about it.

### Running it
Just run the executable that pops out of the compliler, typically "Shape" if you used the command above. You have to specify a level as the first argument, which is why even the source code of the game also comes with an example level called example_level.txt.
`./Shape example_level.txt`
You can add the -res argument for changing the resolution from 640X480 to something else. For example:
`./Shape example_level.txt -res 1920 1080`
The game by default runs with a black background and white foreground, but you can change this by specifying the -backgroundColor and -foregroundColor arguments and then providing RGB values for each. For example, This next one sets the background cyan and the foreground red.
`./Shape example_level.txt -backgroundColor 0 255 255 -foregroundColor 255 0 0`

The controls for actually playing the game are as follows:
- Arrow keys to move
- Push space to trigger your own death
- If you died, your player will turn a solid red. Push the down arrow key to respawn

### Stuff I may want to do
- Implement invisible death triggers. They'll also behave differently from spikes.
- Write documentation for every argument this game supports on the command line and every identifier used in mapping.
- Write a program to graphically build levels. Editing text files is boring, and it's hard to predict the results of the level you'll end up with. Maybe something in game would be cool, too. I could have a way to build stuff in game and a seperate "God edit" program for more powerful editing.
- Implement a Lua interpreter so that the player can implement their own items and platforms.
- Implement a networked multiplayer mode
- Write up a random level generator in Python. Maybe one based on the [libraryofbabel.info](http://www.libraryofbabel.info). It would be pretty cool to be able to choose any page in any book off that site and have the level be consistent via the book to level algorithm. Kinda like minecraft seeds for worlds.
