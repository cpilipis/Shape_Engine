# The Shape Engine
A 2d game engine with built in bounding box collision, built on Allegro 5.
Built mainly so I could practice my C, it can be used to make the simplest of platformers. At least for now. Eventually I will add bitmap support and sound support so that one could use this to make more than what I've got as of right now...

## Current features
* Uses Allegro's Primitives library for drawing shapes.
* Has Pixel Perfect bounding box collision.
* Reads levels from a file specified as the first command line argument to the program
* Can use more arguments to toggle more options
* The screen smoothly scrolls based on where the player is, allowing for traversal of a large level
* Can be adjusted to any resoluton
* Uses Arrow Keys for input
* Runs at 60 Frames Per Second
* Is small in size

## Dependencies
All you need to compile this is Allegro 5, and all it's dependencies. If you're on Linux, you should install pkg-config if you don't already have it. Go to [the website](liballeg.org) for Allegro for information on how to install it other than what I can provide here. On Debian I was able to install it through the package manager on the terminal, although I don't know about other distributions. On Mac, I had to instal Brew and then get it from there, although if you like Headaches you can probably install Allegro 5 without Brew and just get it and all it's dependencies from scratch. On windows, you're on your own.

## Compilation
On linux, I ran `gcc simple_base.c -Wall -o Shape $(pkg-config --cflags --libs allegro-5 allegro_primitives-5)`. Sometimes you'll need to add in `simplestuff.c` between `simple_base.c` and `-Wall`.
On Mac, I got Allegro 5 and all its dependencies from Brew, and then ran `clang simple_base.c -o Shape_mac -lallegro -lallegro_main -lallegro_primitives`. At least on my Mac it had clang by default, and running `gcc` would actually run `clang` without telling me. But maybe I'm wrong and it's the other way around. Maybe they're practically the same and clang just has 2 extra bells and whistles. I can't tell at this point.
On Windows, you're on your own for compilation. I don't know what the deal is on windows, and I won't shame you for installing a Linux virtual machine to compile and run this.

<<<<<<< HEAD
##Running it
This repository comes with an example level that you can fire up right after compilation! If you're on linux or Mac, just type `./Shape example_level.txt` and it'll run in a 640X480 window. After that, just use the arrow keys to move around. If you want a larger or smaller resolution, you can specify with the -res argument and provide 2 numbers as different arguments, like so:
`./Shape example_level.txt -res 1920 1080` (please note all arguments go after the level file is provide. Doing `./Shape -res 1920 1080` it will look for a file called `-res`.)
You can also get an alternate movement system by toggling "abilities" - different abilities regarding movement. `./Shape example_level -ability morph` makes the player morph around and `./Shape example_level -ability boost` adds the ability to move twice as fast by holding the space key.

##Planned features/stuff I want to try at some point
* Add more to the level engine than just changing static body location. For example, it would be cool to be able to define player spawns within the level file, or an entity to send the player to another part of the level.
* Add an option for parts of the level to loop around, as seen in the game X-pilot.
* Network the game so that two players can race eachother across a set of levels to get to the exit
* Make a program that reads books from [The Library of Babel](libraryofbabel.info) and uses the gibberish to generate a level file, although that's separate from this game base. 
* Make a separate program to build levels within a GUI
