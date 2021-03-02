SimpleGameLayer 2D game utility functions build upon SDL2:

* Image
* Sprites with animations
* Sound
* Font
* TileMap
* Basic UI elements: Menu and dialog box

The following demo are available:

* Template: Can be copied to start a new project
* Menu
* point and click: Display images, callbacks on 'hotspot' areas.
* Tetris
* RPG
  - Sprite with user input
  - Level Editor
  - RPG with map loading, scrolling, tile collision, dialog box

# Dependencies

The build engine is Make and the Compiler used is GCC

Under ubuntu, they both can be installed with

	sudo apt-get install build-essential

The Game Engine depends on SDL2 SDL2_Image and SDL2_Mixer.

To install under ubuntu:

	sudo apt install libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev

# Build

Each game under demo/ has it's own Makefile. Use the Make command to build.

	make

You may use the DEBUG=yes parameter to the Makefiles to enable debug mode: Define the DEBUG preprocessor variable and build with debug symboles.

	make DEBUG=yes

Each demo include from the src/ directory only the files it needs.

# Run a demo

The demo have the name of it's directory.

A readme file is present under each directory to explain details.

Each demo can be exited by pressing the `q` key.

# UML diagram

![Alt text](doc/uml_class_diagram.png?raw=true "UML Diagram")

# License

 MIT license
