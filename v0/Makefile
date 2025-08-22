# Compiler Used and Variables
CC= gcc
INCLUDE_DIR= -I./include
S_DEFINE = -DSERVER
C_DEFINE = -DCLIENT
CFLAGS = -Wall -Wextra

SERVER = server
CLIENT = client

S_OBJ = main.o server.o net_utility.o
C_OBJ = main.o client.o net_utility.o

all: server client

build/$(SERVER): src/$(S_OBJ)
	$(CC) -o build/$(S_OBJ) $^

build/$(S_OBJ): src/$(S_SRC)
	$(CC) $(CFLAGS) $(S_DEFINE) -c $^ -o $@

clean:

