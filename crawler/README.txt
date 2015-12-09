 Created by: Chua Zheng Fu Edrei
 COSC 50 Summer 2015 
 Lab Assignment 4: README for crawler
 07/29/2015

##########################################################################################

Instructions for used: 

1) type "make" in the command line to make files

2) To run crawler, typed in:
./crawler [SEED URL] [TARGET DIRECTORY WHERE TO PUT THE DATA] [MAX CRAWLING DEPTH]
eg: ./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 1

3) To test crawler, typed in: 
crawlerTest.sh &> crawlerTestlog.`date +"%a_%b_%d_%T_%Y"`

make sure to create a directory folder "data" in the same directory as lab4 before running 
crawlerTest.sh

Examples can be found in TESTING.txt and crawlerTest.sh

Special considerations:

1) I define an invalid URL as any URL that returns a http status code of 400 and above.

2) The parser in web.c has some issues that are not resolved at the time of writing.
I posted a question regarding this in piazza and below is Prof Zhou's reply:

The GetNextURL() function can return relative URLs incorrectly. There are three types that it
 doesn't deal with well. Here's a nice summary done by a previous student. 

My suggestion is to fix them if you get time. We won't deduct your points if you don't fix them. 

a) URLs with fragments on the end

Example:

Found on page http://old-www.cs.dartmouth.edu/~cs50/tse/wiki/Computer_science.html

Need to strip trailing fragments from links.

b) URLs that use // to try to indicate http:// are interpreted as relative URLs

from the page: http://old-www.cs.dartmouth.edu/~cs50/tse/wiki/Human.html

c) Inconsistency between relative URLs that include /wiki/ in their path and those that don't

With (from http://old-www.cs.dartmouth.edu/~cs50/tse/wiki/Human.html):
From http://old-www.cs.dartmouth.edu/~cs50/tse/wiki/Computer_science.html

A note on this one: relative links that begin with / are treated as top-level paths, and so 
result in broken links from a web browser also (to test, go to http://old-www.cs.dartmouth.edu/~cs50/tse/wiki/Human.html 
and click "Human (disambiguation)" at the top. it will link to http://old-www.cs.dartmouth.edu/wiki/Human_(disambiguation), 
which is a broken link.

3) Remember to create a directory folder "data" in the same directory as lab4 before running 
crawlerTest.sh
