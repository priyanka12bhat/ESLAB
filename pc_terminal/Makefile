
CC=gcc
CFLAGS = -g -Wall -lm
EXEC = ./pc-terminal

all:
	$(CC) $(CFLAGS) pc_terminal.c ../protocol/packet.c ../protocol/crc16.c joystick.c -o $(EXEC)

run: all
	$(EXEC)
