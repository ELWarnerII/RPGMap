/**
   @file map.h
   @author Louis Warner (elwarner)
   This file contains helper declarations of helper functions for the explorer.c program. 
   These functions are defined in map.c.
 */
#define INITIAL_MAP_SIZE 3
#define NORTH 8
#define SOUTH 2
#define EAST 6
#define WEST 4

/**
   This function allocates memory for the initial map which is in the form of a 3x3 array of characters.
   @param int *rows - pointer to the number of rows (which will be set to 3 anytime this function is called)
   @return char **map - pointer to an array of strings representing the initial map
*/
char **initMap( int *rows );


/**
   This function frees all dynamically allocated memory that is used to store the map. It should be called before
   program successful program termination.
   @param char **map - pointer to the map array
   @param int rows - number of rows in the map
*/
void freeMap( char **map, int rows );


/**
   This function will print the given map (of the given height in rows) to standard output.
   @param char **map - the map to be printed
   @param int rows - height of the map
   @param int rowPos - y-coordinate for the user
   @param int colPos - x-coordinate for the user
   @param int dir - direction for the user (NORTH, SOUTH, EAST, or WEST)
 */
void showMap( char **map, int rows, int rowPos, int colPos, int dir );


/**
   This function will return an expanded version of the map, expanding either the rows or columns of
   the map based on parameter criteria. 
   @param char **map - the original map
   @param int *rows - height of the map
   @param int extraRows - number of rows to increase map by
   @param int extraCols - number of columns to increase map by
   @param int shiftRows - amount of rows that current rows need to be shifted downward
   @param int shiftCols - amount of rows that current columns need to be shifted rightward
   @return char **newmap - the resulting expanded map
 */
char **expandMap( char **map, int *rows, int extraRows, int extraCols, int shiftRows, int shiftCols );


/**
   This function is used for saving one map into another. It also calls freeMap to
   prevent leaks when shifting array locations.
   @param char **map - the map to be saved
   @param char **savemap - second map (will be set to the return value in function call)
   @param int rows - height of map
   @param int oldrows - height of savemap
   @return char **newmap - saved map state (will be sent to savemap in the function call)
*/
char **copyMap( char **map, char **savemap, int rows, int oldrows);