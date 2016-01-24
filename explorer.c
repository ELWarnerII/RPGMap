/**
   @file explorer.c
   @author Louis Warner (elwarner)
   This program creates a map for basic RPG based on text file input.
   The player position and direction is indicated by <,>,^, or V. The player can move by
   turning right or left or taking a step forward. The player can move forward onto spaces with the
   '.' character or spaces filled by lower-case letters (items). '#' represents a wall that cannot be passed through.
   Spaces the player has not yet seen are represented by ' ', and the map edges are represented with '+' on the corners
   and '-' on the edges. The current map will be printed after every valid move.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
 
//Variables for number of rows (set by map initialization) and columns (set to constant).
int rows;
int oldrows;
int cols = INITIAL_MAP_SIZE;

//Declaration for the map, initialized in build phase.
char **map = NULL;
char **savemap = NULL;

//Player always begins facing north, at the center of the array.
int dir = NORTH;
int rowPos = 1;
int colPos = 1;

//The last space the player was on.
char last = ' ';


/**
   Checks for a valid line of sight for the character.
   @param String this - max of 4 characters (3 and a null terminator)
   @return int valid - 0 for false or 1 for true
 */
int isValidSequence(char this[4]){
  //Set valid to 1 (true).
  int valid = 1;
  //Check all characters except the null terminator.
  for(int i = 0; i < 3; i ++){
    //Check if it is a valid common character. If not, set valid to 0 (false).
    if(this[i] != '.' && this[i] != '#'){
       valid = 0;
    }
    //If the first test failed, but the character is actually a valid lowercase letter this will set valid back to 1 (true).
    if(this[i] >= 97 && this[i] <= 122){
       valid = 1;
    }
  }
  return valid;  
}


/**
   Checks for a valid forward movement by determining whether a wall is in the way.
   @return int valid - 0 for false or 1 for true
 */
int validForward(){
  int valid = 1;
  if(dir == NORTH){
    if(map[rowPos - 1][colPos] == '#'){
      valid = 0;
    }
  }
  if(dir == SOUTH){
    if(map[rowPos + 1][colPos] == '#'){
      valid = 0;
    }
  }
  if(dir == EAST){
    if(map[rowPos][colPos + 1] == '#'){
      valid = 0;
    }
  }
  if(dir == WEST){
    if(map[rowPos][colPos - 1] == '#'){
      valid = 0;
    }
  }
  return valid;
}


/**
   Makes an attempt at moving forward. If the player is going to a map edge,
   the map is expanded to make room for the new strings. The map state is saved
   before attempting the move, and will revert to the saved state if an illegal
   move is attempted.
   @param string this - the sequence of chars the user sees after a forward move
 */
void moveForward(char this[4]){
  //Integer for whether or not an expansion has occurred.
  int expansion = 0;
  
  //Save map state.
  savemap = copyMap(map, savemap, rows, oldrows); 
  oldrows = rows;
  
  //Save current user space.
  map[rowPos][colPos] = last;
  
  //Check direction
  if(dir == NORTH){
    
    //Make a move, save new char into last.
    rowPos--;
    last = map[rowPos][colPos];
    
    //Check if an expansion is needed.
    if(rowPos == 0){
      map = expandMap( map, &rows, 1, 0, 1, 0 );
      expansion++;
      rowPos++;
    }
    //Procedure for making a northward move.
    for(int i = 0; i < 3; i++){
      if(map[rowPos - 1][colPos - 1 + i] == ' ' || map[rowPos - 1][colPos - 1 + i] == this[i]){
        map[rowPos - 1][colPos - 1 + i] = this[i];
      } else {
        fprintf(stderr, "Inconsistent map\n");        
        map = copyMap(savemap, map, oldrows, rows);
        if(expansion){
          rows = oldrows;
          expansion--;
        }
        rowPos++;      
        last = map[rowPos][colPos];
        return;
      }
    }
    if(expansion){
      expansion--;
    }
    
    //Check direction
  } else if(dir == SOUTH){
    rowPos++;
    last = map[rowPos][colPos];
    
    //Check if an expansion is needed.
    if(rowPos == rows - 1){
      map = expandMap( map, &rows, 1, 0, 0, 0 );  
      expansion++;
    }
    //Procedure for making a southward move.
    for(int i = 0; i < 3; i++){
      if(map[rowPos + 1][colPos + 1 - i] == ' ' || map[rowPos + 1][colPos + 1 - i] == this[i]){
        map[rowPos + 1][colPos + 1 - i] = this[i];
      } else {
        fprintf(stderr, "Inconsistent map\n");       
        map = copyMap(savemap, map, oldrows, rows);
        if(expansion){
          rows = oldrows;
          expansion--;
        }
        rowPos--;  
        last = map[rowPos][colPos];
        return;
      }      
    }
    if(expansion){
      expansion--;
    }
    
  //Check direction
  } else if(dir == EAST){
    colPos++;
    last = map[rowPos][colPos];
    
    //Check if an expansion is needed.
    if(colPos == cols - 1){
      map = expandMap( map, &rows, 0, 1, 0, 0 );
      cols++;
    }
    //Procedure for making an eastward move.
    for(int i = 0; i < 3; i++){
      if(map[rowPos - 1 + i][colPos + 1] == ' ' || map[rowPos - 1 + i][colPos + 1] == this[i]){
        map[rowPos - 1 + i][colPos + 1] = this[i];
      } else {
        fprintf(stderr, "Inconsistent map\n");
        map = copyMap(savemap, map, oldrows, rows);
        colPos--;
        last = map[rowPos][colPos];
        return;        
      }
    }
  
  //Check direction
  } else if(dir == WEST){
    colPos--;
    last = map[rowPos][colPos];
    
    //Check if an expansion is needed.
    if(colPos == 0){
      map = expandMap( map, &rows, 0, 1, 0, 1 ); 
      colPos++;
      cols++;
    }
    //Procedure for making a westward move.
    for(int i = 0; i < 3; i++){
      if(map[rowPos + 1 - i][colPos - 1] == ' ' || map[rowPos + 1 - i][colPos - 1] == this[i]){
        map[rowPos + 1 - i][colPos - 1] = this[i];
      } else {
        fprintf(stderr, "Inconsistent map\n");
        map = copyMap(savemap, map, oldrows, rows);
        colPos++;
        last = map[rowPos][colPos];
        return;
      }
    }
  }
  
  //Display the map.
  showMap(map, rows, rowPos, colPos, dir);
}

/**
   Makes an attempt to turn left.
   @param string this - the sequence the player sees after a successful turn
 */
void turnLeft(char this[4]){
  //Save current state
  savemap = copyMap(map, savemap, rows, oldrows); 
  oldrows = rows;
  
  //Check direction
  if(dir == NORTH){
    dir = WEST;
    for(int i = 0; i < 3; i++){
      if(map[rowPos + 1 - i][colPos - 1] == ' ' || map[rowPos + 1 - i][colPos - 1] == this[i]){
        map[rowPos + 1 - i][colPos - 1] = this[i];
      } else {
        fprintf(stderr, "Inconsistent map");
        map = copyMap(savemap, map, oldrows, rows);
        dir = NORTH;
        return;
      }
    }
  //Check direction
  } else if(dir == SOUTH){
    dir = EAST;
    for(int i = 0; i < 3; i++){
      if(map[rowPos - 1 + i][colPos + 1] == ' ' || map[rowPos - 1 + i][colPos + 1] == this[i]){
        map[rowPos - 1 + i][colPos + 1] = this[i];
      } else {
        fprintf(stderr, "Inconsistent map");
        map = copyMap(savemap, map, oldrows, rows);
        dir = SOUTH;
        return;
      }
    }
  //Check direction
  } else if(dir == EAST){
    dir = NORTH;
    for(int i = 0; i < 3; i++){
      if(map[rowPos - 1][colPos - 1 + i] == ' ' || map[rowPos - 1][colPos - 1 + i] == this[i]){
        map[rowPos - 1][colPos - 1 + i] = this[i];
      } else {
        fprintf(stderr, "Inconsistent map");
        map = copyMap(savemap, map, oldrows, rows);
        dir = EAST;
        return;
      }
    }
  //Check direction
  } else if(dir == WEST){
    dir = SOUTH;
    for(int i = 0; i < 3; i++){
      if(map[rowPos + 1][colPos + 1 - i] == ' ' || map[rowPos + 1][colPos + 1 - i] == this[i]){
        map[rowPos + 1][colPos + 1 - i] = this[i];
      } else {
        fprintf(stderr, "Inconsistent map");
        map = copyMap(savemap, map, oldrows, rows);
        dir = WEST;
        return;
      }
    }
  }
  //Display the map.
  showMap(map, rows, rowPos, colPos, dir);
}

/**
   Makes an attempt to turn right.
   @param string this - the sequence the player sees after a successful turn
 */
void turnRight(char this[4]){  
  //Save current state
  savemap = copyMap(map, savemap, rows, oldrows);
  oldrows = rows;
  
  //Check direction
  if(dir == NORTH){
    dir = EAST;
    for(int i = 0; i < 3; i++){
      if(map[rowPos - 1 + i][colPos + 1] == ' ' || map[rowPos - 1 + i][colPos + 1] == this[i]){
        map[rowPos - 1 + i][colPos + 1] = this[i];
      } else {
        fprintf(stderr, "Inconsistent map");
        map = copyMap(savemap, map, oldrows, rows);
        dir = NORTH;
        return;
      }
    }
  //Check direction
  } else if(dir == SOUTH){
    dir = WEST;
    for(int i = 0; i < 3; i++){
      if(map[rowPos + 1 - i][colPos - 1] == ' ' || map[rowPos + 1 - i][colPos - 1] == this[i]){
        map[rowPos + 1 - i][colPos - 1] = this[i];
      } else {
        fprintf(stderr, "Inconsistent map");
        map = copyMap(savemap, map, oldrows, rows);
        dir = SOUTH;
        return;
      }
    }
  //Check direction
  } else if(dir == EAST){
    dir = SOUTH;
    for(int i = 0; i < 3; i++){
      if(map[rowPos + 1][colPos + 1 - i] == ' ' || map[rowPos + 1][colPos + 1 - i] == this[i]){
        map[rowPos + 1][colPos + 1 - i] = this[i];
      } else {
        fprintf(stderr, "Inconsistent map");
        map = copyMap(savemap, map, oldrows, rows);
        dir = EAST;
        return;
      }
    }
  //Check direction
  } else if(dir == WEST){
    dir = NORTH;
    for(int i = 0; i < 3; i++){
      if(map[rowPos - 1][colPos - 1 + i] == ' ' || map[rowPos - 1][colPos - 1 + i] == this[i]){
        map[rowPos - 1][colPos - 1 + i] = this[i];
      } else {
        fprintf(stderr, "Inconsistent map");
        map = copyMap(savemap, map, oldrows, rows);
        dir = WEST;
        return;
      }
    }
  }
  //Display the map.
  showMap(map, rows, rowPos, colPos, dir);
}




/**
   This program is used if an input file is given for building the map.
   @param FILE *input - the input file
 */
void buildFromFile(FILE *input){
  //Strings to hold the next command and sequence. They are large enough to hold valid strings and a null terminator.
  char command[8];
  char sequence[4];

  //Character helpful for discarding lines.
  char c;
  
  //Initialize map and savemap.
  map = initMap(&rows);
  savemap = initMap(&oldrows);
  
  //Read initial map sequence
  while(!feof(input)){
    fscanf(input, "%4s", sequence);
    if(isValidSequence(sequence)){
      for(int i = 0; i < 3; i++){
        map[rowPos - 1][colPos - 1 + i] = sequence[i];
      }
      savemap = copyMap(map, savemap, rows, oldrows);
      showMap(map, rows, rowPos, colPos, dir);
      break;
    } else {
      fprintf(stderr, "Invalid command\n");
      c = getc(input);
      while(c != '\n'){
        c = getc(input);
      }
    }
  }
  //Read and process commands
  while(!feof(input)){
    fscanf(input, "%8s", command);
    if(!strncmp(command, "forward", 7)){
      fscanf(input, "%4s", sequence);
      if(isValidSequence(sequence)){
        if(validForward()){
          moveForward(sequence);
        } else if (!validForward()){
          fprintf(stderr, "Blocked\n");
        }
      } else {
        fprintf(stderr, "Invalid command\n");
        c = getc(input);
        while(c != '\n'){
          c = getc(input);
        }
      }
    } else if(!strncmp(command, "right", 5)){
      fscanf(input, "%4s", sequence);
      if(isValidSequence(sequence)){
        turnRight(sequence);
      } else {
        fprintf(stderr, "Invalid command\n");
        c = getc(input);
        while(c != '\n'){
          c = getc(input);
        }
      }
    } else if(!strncmp(command, "left", 4)){
      fscanf(input, "%4s", sequence);
      if(isValidSequence(sequence)){
        turnLeft(sequence);
      } else {
        fprintf(stderr, "Invalid command\n");
        c = getc(input);
        while(c != '\n'){
          c = getc(input);
        }
      }
    } else if(!strncmp(command, "quit", 4)){
      break;
    } else{
      fprintf(stderr, "Invalid command\n");
      c = getc(input);
      while(c != '\n'){
        c = getc(input);
      }
    }
  }
  return;
}


/**
   This program is used to build the map from standard input.
 */
void buildFromStandard(){
  //Strings to hold the next command and sequence. They are large enough to hold valid strings and a null terminator.
  char command[8];
  char sequence[4];

  //Character helpful for discarding lines.
  char c;
  
  //Initialize map and savemap.
  map = initMap(&rows);
  savemap = initMap(&oldrows);
  
  //Read initial map sequence
  while(!feof(stdin)){
    fscanf(stdin, "%4s", sequence);
    if(isValidSequence(sequence)){
      for(int i = 0; i < 3; i++){
        map[rowPos - 1][colPos - 1 + i] = sequence[i];
      }
      savemap = copyMap(map, savemap, rows, oldrows);
      showMap(map, rows, rowPos, colPos, dir);
      break;
    } else {
      fprintf(stderr, "Invalid command\n");
      c = getc(stdin);
      while(c != '\n'){
        c = getc(stdin);
      }
    }
  }
  //Read and process commands
  while(!feof(stdin)){
    fscanf(stdin, "%8s", command);
    if(!strncmp(command, "forward", 7)){
      fscanf(stdin, "%4s", sequence);
      if(isValidSequence(sequence)){
        if(validForward()){
          moveForward(sequence);
        } else if (!validForward()){
          fprintf(stderr, "Blocked\n");
        }
      } else {
        fprintf(stderr, "Invalid command\n");
        c = getc(stdin);
        while(c != '\n'){
          c = getc(stdin);
        }
      }
    } else if(!strncmp(command, "right", 5)){
      fscanf(stdin, "%4s", sequence);
      if(isValidSequence(sequence)){
        turnRight(sequence);
      } else {
        fprintf(stderr, "Invalid command\n");
        c = getc(stdin);
        while(c != '\n'){
          c = getc(stdin);
        }
      }
    } else if(!strncmp(command, "left", 4)){
      fscanf(stdin, "%4s", sequence);
      if(isValidSequence(sequence)){
        turnLeft(sequence);
      } else {
        fprintf(stderr, "Invalid command\n");
        c = getc(stdin);
        while(c != '\n'){
          c = getc(stdin);
        }
      }
    } else if(!strncmp(command, "quit", 4)){
      break;
    } else{
      fprintf(stderr, "Invalid command\n");
      c = getc(stdin);
      while(c != '\n'){
        c = getc(stdin);
      }
    }
  }
  return;
}


/**
   The main program can run with either 1 or 0 command line arguments. The function determines whether there is a valid
   number of command line arguments and then chooses whether to process from a file or from standard input.
   @param argc - count of command line arguments
   @param *argv[] - array of command line arguments
   @return exit statues - either successful or unsuccessful
 */
int main( int argc, char *argv[] ){
  
  //Check for correct amount of arguments
  if(argc > 2){
    fprintf(stderr, "usage: explorer [script_file]\n");
    exit (1);
  }
  
  //Attempt to open the input file
  if(argc == 2){  
    FILE *input = fopen(argv[1], "r");
    if( !input ){
      fprintf(stderr, "Can't open movement script: %s\nusage: explorer [script_file]\n", argv[1]);
      exit (1);
    }
    buildFromFile(input);
    fclose(input);
  } else{
    buildFromStandard();
  }
  
  //Free up remaining allocated memory.
  freeMap(map, rows);
  freeMap(savemap, oldrows);
  
  //Successful return.
  exit(0);
}