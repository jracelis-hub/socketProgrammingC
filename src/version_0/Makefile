# Compiler Used and Variables
CC= gcc
BUILD_DIR= ./build
SRC_DIR= ./src/
INCLUDE_DIR= ./include
S_DEFINE = -DSERVER
C_DEFINE = -DCLIENT
CFLAGS = -Wall -Wextra

SERVER = server
CLIENT = client

S_OBJ = main.o server.o s_net_utility.o
C_OBJ = main.o client.o c_net_utility.o

all: server client

build/$(SERVER): 

build/$(S_OBJ): src/$(S_SRC)
	$(CC) $(CFLAGS) $(S_DEFINE) -c $^ -o $@

clean:

