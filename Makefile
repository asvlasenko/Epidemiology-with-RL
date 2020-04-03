OBJS = 	./src/epi_lib/approx_binomial.c \
				./src/epi_lib/disease.c \
				./src/epi_lib/epi_api.c \
				./src/epi_lib/files.c \
				./src/epi_lib/population.c
TEST_SOURCE = ./src/c/main.c
CC = gcc
OBJ_NAME = ./bin/main
LIB_NAME = ./bin/epi_lib.so
INCLUDE_PATHS = ./include
LIBRARY_PATHS =
COMPILER_FLAGS = -std=c99 -Wall -Wfatal-errors
LINKER_FLAGS =

#Linux
#CLEANUP = rm -f *.o

#MinGW
CLEANUP = del /f *.o

build:
	$(CC) -c $(OBJS) $(COMPILER_FLAGS)
	$(CC) -shared -o $(LIB_NAME) *.o
	$(CC) -o $(OBJ_NAME) $(TEST_SOURCE) $(LIB_NAME) $(COMPILER_FLAGS)
	$(CLEANUP)

run:
	$(OBJ_NAME)

clean:
#Linux
#	rm -f $(OBJ_NAME) $(LIB_NAME)

#MinGW
	del /f $(OBJ_NAME).exe $(LIB_NAME)
