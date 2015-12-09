# Filename: Makefile
# Description: The make file is to test the query
CC = gcc
CFLAGS = -Wall -pedantic -std=c11
EXEC = queryengine_test
SRCS = ./queryengine_test.c
UTILDIR=../../util/
UTILFLAG=-ltseutil -lm
UTILLIB=$(UTILDIR)libtseutil.a 
UTILC=$(UTILDIR)file.c $(UTILDIR)web.c $(UTILDIR)hashtable.c $(UTILDIR)list.c

test: $(SRCS) $(UTILLIB)
	$(CC) $(CFLAGS) -o queryengine_test $(SRCS) -L$(UTILDIR) $(UTILFLAG)
$(UTILLIB): $(UTILC) $(UTILH)
	cd $(UTILDIR); make
clean:
		rm -f *~
		rm -f *#
		rm -f *.o
		rm -f $(EXEC)
		cd $(UTILDIR); make clean