# Filename: Makefile
# Description: The make file is to build up the indexer
CC = gcc
CFLAGS = -Wall -pedantic -std=c11
UTILDIR=../data
EXEC = indexer
OBJS = indexer.o web.o list.o hashtable.o file.o -lcurl
SRCS = ./src/indexer.c ./src/list.c ./src/web.c ./src/hashtable.c ./src/file.c
$(EXEC): $(OBJS)
		$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)
$(OBJS): $(SRCS)
		$(CC) $(CFLAGS) -c $(SRCS)
debug: $(SRCS)
		$(CC) $(CFLAGS) -g -ggdb -c $(SRCS)
		$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(OBJS)
clean:
		rm -f *~
		rm -f *#
		rm -f *.o
		cd $(UTILDIR); rm -f *.dat;