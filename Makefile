# Extra options for the default compile rule to use.
CC = gcc
CFLAGS = -Wall -std=c99 -g
LDLIBS = -lm

# Drawing depends on this object.
explorer: map.o

# Object file dependencies
explorer.o: map.h