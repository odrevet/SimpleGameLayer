# Place files

Place files are text files that indicates which pictures and hotspot are present
at a given place.

Each place file are located in the res directory, and the file name is the id
of the place.

Structure of a place file is :

```
NUMBER_OF_IMAGES
PATH_TO_IMAGE_FILE   DRAW_LOCATION_X DRAW_LOCATION_Y     (one line per image)
NUMBER_OF_HOTSPOTS
X Y WIDTH HEIGHT TYPE PARAMETER                          (one line per hotspot)
```

For examples of place files, look at N001 in the res folder (N001 stands for
North at location one)
