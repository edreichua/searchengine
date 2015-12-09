# Filename: Makefile
# Description: The make file is to build up the entire TinySearchEngine
CC = gcc
CFLAGS = -Wall -pedantic -std=c11
EXEC = tse
UTILDIR=./util/
CRAWLERDIR=./crawler/
INDEXERDIR=./indexer/
QUERYDIR=./query/

$(EXEC):
	cd $(UTILDIR); make lib
	cd $(CRAWLERDIR); make crawler
	cd $(INDEXERDIR); make indexer
	cd $(QUERYDIR); make query
	
clean:
		rm -f *~
		rm -f *#
		rm -f *.o
		cd $(UTILDIR); make clean
		cd $(CRAWLERDIR); make clean
		cd $(INDEXERDIR); make clean
		cd $(QUERYDIR); make clean