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

ifeq ($(OS), Windows_NT)
	CLEAN_O = del /f *.o
	CLEAN_BIN = del /f $(OBJ_NAME).exe $(LIB_NAME)
else
	CLEAN_O = rm -f *.o
	CLEAN_BIN = rm -f $(OBJ_NAME) $(LIB_NAME)
endif

build:
	$(CC) -c $(OBJS) $(COMPILER_FLAGS)
	$(CC) -shared -o $(LIB_NAME) *.o
	$(CC) -o $(OBJ_NAME) $(TEST_SOURCE) $(LIB_NAME) $(COMPILER_FLAGS)
	$(CLEAN_O)

run:
	$(OBJ_NAME)

clean:
	$(CLEAN_BIN)
#MinGW
	del /f $(OBJ_NAME).exe $(LIB_NAME)
