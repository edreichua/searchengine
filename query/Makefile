# Filename: Makefile
# Description: The make file is to build up the query
CC = gcc
CFLAGS = -Wall -pedantic -std=c11
EXEC = query
SRCS = ./src/query.c
UTILDIR=../util/
UTILFLAG=-ltseutil -lm
UTILLIB=$(UTILDIR)libtseutil.a 
UTILC=$(UTILDIR)file.c $(UTILDIR)web.c $(UTILDIR)hashtable.c $(UTILDIR)list.c

$(EXEC): $(SRCS) $(UTILLIB)
	$(CC) $(CFLAGS) -o query $(SRCS) -L$(UTILDIR) $(UTILFLAG)
$(UTILLIB): $(UTILC) $(UTILH)
	cd $(UTILDIR); make
clean:
		rm -f *~
		rm -f *#
		rm -f *.o
		cd $(UTILDIR); make clean