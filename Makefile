target=netstat
CC=gcc
CFLAGS=-Wall -O2 -o

$(target):$(target).c
	$(CC) $< $(CFLAGS) $@
