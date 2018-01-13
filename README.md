# The Shape Engine
## A simple 2d game being written in C

### Features

- Runs at any resolution
- Reads text files to generate a level
- Currently has platforms, checkpoints, and death triggers (for bottomless pits)

### Dependencies
This program uses Allegro and all it's dependencies. That's about it.

### Compilation
`gcc simple_base.c -lallegro -lallegro_primitives -o Shape`
On mac, you'll want to add a `-lallegro_main` in there. I don't know about any compilation on windows yet.

### Running it
`./Shape example_level.txt`
You can add the -res argument for changing the resolution from 640X480 to something else. For example:
`./Shape example_level.txt -res 1920 1080`

### Stuff I may want to do

- Implement visible spikes/hazards for the player to avoid other than pits
- Implement a networked multiplayer mode
- Write up a random level generator in Python. Maybe one based on the [libraryofbabel.info](http://www.libraryofbabel.info).
