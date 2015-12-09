CC = gcc
CFLAGS = -Wall -pedantic -std=c11
EXEC = crawler
OBJS = crawler.o web.o hashtable.o list.o -lcurl
SRCS = ./src/crawler.c ./src/web.c ./src/hashtable.c ./src/list.c
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
