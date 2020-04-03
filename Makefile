OBJS = ./src/epi_lib/*.c
CC = gcc
OBJ_NAME = ./bin/main
INCLUDE_PATHS =
LIBRARY_PATHS =
COMPILER_FLAGS = -std=c99 -Wall -Wfatal-errors
LINKER_FLAGS =

build:
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

run:
	$(OBJ_NAME)

clean:
	del /f $(OBJ_NAME).exe
