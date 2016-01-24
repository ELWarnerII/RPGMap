/**
   @file map.c
   @author Louis Warner (elwarner)
   @author David Sturgill (dbsturgi) - char **initMap( int *rows );
   This file contains helper functions for the explorer.c program. It does all of the work
   for allocating, expanding, and freeing memory for the map, as well as printing the current
   version of the map.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
char **initMap( int *rows ) {
  // Allocate enough space to store a pointer for every row.
  *rows = INITIAL_MAP_SIZE;
  char **map = (char **) malloc( *rows * sizeof( char * ) );

  // Fill in the rows with 3-character strings of spaces.
  for ( int i = 0; i < *rows; i++ ) {
    map[ i ] = (char *)malloc( INITIAL_MAP_SIZE + 1 );
    strcpy( map[ i ], "   " );
  }
  
  // Return the new map.
  return map;
}

/**
   This function frees all dynamically allocated memory that is used to store the map. It should be called before
   program successful program termination.
   @param char **map - pointer to the map array
   @param int rows - number of rows in the map
*/
void freeMap( char **map, int rows ){
  //Free each row of characters.
  for ( int i = 0; i < rows; i++ ) {
    free(map[i]);
  }
  
  //Free the columns.
  free(map);
}

/**
   This function will print the given map (of the given height in rows) to standard output.
   @param char **map - the map to be printed
   @param int rows - height of the map
   @param int rowPos - y-coordinate for the user
   @param int colPos - x-coordinate for the user
   @param int dir - direction for the user (NORTH, SOUTH, EAST, or WEST)
 */
void showMap( char **map, int rows, int rowPos, int colPos, int dir ){
  //Set directional arrow.
  if(dir == NORTH){
    map[rowPos][colPos] = '^';
  }
  if(dir == SOUTH){
    map[rowPos][colPos] = 'V';
  }
  if(dir == EAST){
    map[rowPos][colPos] = '>';
  }
  if(dir == WEST){
    map[rowPos][colPos] = '<';
  }
  
  //Print top border.
  fprintf(stdout, "+");
  for(int i = 0; i < strlen(map[0]); i++){
    fprintf(stdout, "-");
  }
  fprintf(stdout, "+\n");
  
  //Print all rows and left and right borders.
  for(int j = 0; j < rows; j++){
    fprintf(stdout, "|%s|\n", map[j]);
  }
  
  //Print bottom border.
  fprintf(stdout, "+");
  for(int k = 0; k < strlen(map[0]); k++){
    fprintf(stdout, "-");
  }
  fprintf(stdout, "+\n");
}


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
char **expandMap( char **map, int *rows, int extraRows, int extraCols, int shiftRows, int shiftCols ){
  
  //Keep old rows for iteration of old map array after making a new expanded array.
  int oldrows = *rows;
  
  //Set rows to the new amount of rows.
  *rows = *rows + extraRows;
  
  //Keep old length for same reason as oldrows.
  int oldLength = strlen(map[0]);
  
  //Obtain new length from adding any extra columns.
  int newLength = oldLength + extraCols;
  
  // Allocate enough space to store a pointer for every row
  char **newmap = (char **) malloc( *rows * sizeof( char * ) );
  
  // Fill in the rows with strings of spaces ended by null terminators.
  for ( int i = 0; i < *rows; i++ ) {
    newmap[ i ] = (char *) malloc( newLength + 1);
    for (int j = newLength - 1; j >= 0 ; j--){
      newmap[i][j] = ' ';
    }
    newmap[i][newLength] = '\0';
  }
  
  //Copy the old map into the new.
  for ( int k = 0; k < oldrows; k++ ) {
    for( int l = 0; l < oldLength; l++){
      newmap[k][l] = map[k][l];
    }
  }
  
  //Perform a row shift if necessary.
  if(shiftRows == 1){    
    for(int j = *rows - 2; j >= 0; j--){
      for(int k = 0; k < newLength; k++){
        newmap[j + 1][k] = newmap[j][k];
        newmap[j][k] = ' ';
      }
    }
  }
  
  //Perform a column shift if necessary.
  if(shiftCols == 1){ 
    for(int j = 0; j < *rows; j++){
      for(int k = newLength - 1; k > 0; k--){
        newmap[j][k] = newmap[j][k - 1];
        newmap[j][k - 1] = ' ';
      }
    }
  }
  
  // Return the new map.
  return newmap;
}


/**
   This function is used for saving one map into another. It also calls freeMap to
   prevent leaks when shifting array locations.
   @param char **map - the map to be saved
   @param char **savemap - second map (will be set to the return value in function call)
   @param int rows - height of map
   @param int oldrows - height of savemap
   @return char **newmap - saved map state (will be sent to savemap in the function call)
*/
char **copyMap( char **map, char **savemap, int rows, int oldrows) {
  
  //Get length from map.
  int length = strlen(map[0]);
  
  //Erase anything in savemap.
  freeMap(savemap, oldrows);
  
  //Create a new map.
  char **newmap = (char **) malloc( rows * sizeof( char * ) );

  // Fill in the rows with 3-character strings of spaces.
  for ( int i = 0; i < rows; i++ ) {
    newmap[ i ] = (char *)malloc(  length + 1 );
    strcpy( newmap[ i ], map[i] );
  }
  
  //Return the new map.
  return newmap;
}