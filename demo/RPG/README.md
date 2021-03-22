# RPG

    make && ./RPG

A minimalisic RPG with :

* tilemap
* dialog
* music
* events

# Level Editor

* Create or modify a .map file.

* See animations of a .tileset file. 

## build

    make -f Makefile_level_editor

## run

    ./level_editor -i data/overworld.map

Create and edit level files.

Command line argument:

* -i: level to load (.map or .tileset file)


Controles:

* arrow key: move
* t: tile selection
* a: animated tile selection
* enter: past selected tile
* s: save
* c: add a column
* r: add a row
* d: shift down
* i: make the tile under the cursor the selected tile
* backspace: clear tile under cursor
* l: cicle throught layer
* q: quit / close menu

## File formats

### .map

```
Path to .tileset file
Path to music file
Path to .tiles file
Tile width : tile height
Number of tile for width : Number of tiles for height : Number of layers
Ids of tiles separated by a space character
```

### .tileset

```
Path to tileset image file
Number of animations
Number of frames List Animation speed in frame Tile id for each frame
```

### .tiles

The tiles file define the frames for the animated tiles and the properties associated for each tile id. In the RPG demo, there are only one property per tile which define if the hero can walk on the tile (1) or not (0).

```
Number of tile properties Number of properties per tile
List of Tile id Properties
```

# Sprite

Load and animate NPCs


# UML diagram

![Alt text](doc/uml_class_diagram.png?raw=true "UML Diagram")


# Credits

## Graphics

Graphics by ArMM1998 from open game art

Licensed under the CC0 1.0 Universal (CC0 1.0) license 

https://opengameart.org/content/zelda-like-tilesets-and-sprites

## Musics

* AFTERNOON by GLYPH 
  
Licensed under the Mod Archive Distribution license

https://modarchive.org/index.php?request=view_by_moduleid&query=96987


* Cavern by melf/westower
  
Licensed under the Mod Archive Distribution license

https://modarchive.org/index.php?request=view_by_moduleid&query=35443
