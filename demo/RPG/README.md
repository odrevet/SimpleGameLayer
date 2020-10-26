# RPG

    make && ./RPG

A minimalisic RPG with :

* tilemap
* dialog
* music

# Level Editor

Create or modify a .map file.

## build

    make -f Makefile_level_editor

## run

-l level_path

    ./level_editor -l data/overworld.map

Create and edit level files.

Command line argument:

* -l: level to load (.map file)

If the file do not existe, it may be created and intialize in the editor's console.

Controles:

* arrow key: move
* t: tile selection mode
* enter: past selected tile
* s: save
* c: add a column
* r: add a row
* d: shift down
* i: make the tile under the cursor the selected tile
* backspace: clear tile under cursor
* l: cicle throught layer
* q: quit

# Sprite

Load and animate NPCs

# Credits

## Graphics

Graphics from open game art

## Music

* https://modarchive.org/index.php?request=view_by_moduleid&query=96987

* https://modarchive.org/index.php?request=view_by_moduleid&query=35443
