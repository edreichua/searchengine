#!/bin/bash
# Script name: crawlerTest.sh
#
# Description: Test bench for crawler
#
# Special Consideration: To test the function Crawler using this test script, create a
# 	directory named "data" in the same folder as Lab4
#
# Execute this line: crawlerTest.sh &> 

# Test 1: More than 3 input parameters
printf "\n\nTest 1: More than 3 input parameters\n"
printf "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 3 4\n"
printf "Expected output: Error: Number of input argument needs to be exactly 3\n"
printf "Test output: \n"
./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 3 4


# Test 2: Less than 3 input parameters
printf "\n\nTest 2: Less than 3 input parameters\n"
printf "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data\n"
printf "Expected output: Error: Number of input argument needs to be exactly 3\n"
printf "Test output: \n"
./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data


# Test 3: Directory doesn't exist
printf "\n\nTest 3: Directory doesn't exist\n"
printf "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./doesnotexist 3\n"
printf "Expected output: Error: Directory cannot be found\n"
printf "Test output: \n"
./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./doesnotexist 3


# Test 4: Exceeds maximum depth
printf "\n\nTest 4: Exceeds maximum depth\n"
printf "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 5\n"
printf "Expected output: Error: Depth exceeds the maximum limit\n"
printf "Test output: \n"
./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 5


# Test 5: invalid website (doesn't begin with "http://old-www.cs.dartmouth.edu/~cs50/tse")
printf "\n\nTest 5: invalid website (doesn't begin with \"http://old-www.cs.dartmouth.edu/~cs50/tse\")\n"
printf "./crawler http://old-www.cs.dartmouth.edu/~cs50/123.html  ./data 1\n"
printf "Expected output: Error: Invalid URL. Can only crawl URL with URL prefix\n"
printf "Test output: \n"
./crawler http://old-www.cs.dartmouth.edu/~cs50/123.html  ./data 1


# Test 6: invalid website (404)
printf "\n\nTest 6: invalid website (404)\n"
printf "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/jklmn.html ./data 1\n"
printf "Expected output:Error: Invalid URL\n"
printf "Test output: \n"
./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/jklmn.html ./data 1


# Test 7: crawl at depth 0
printf "\n\nTest 7: crawl at depth 0\n"
printf "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 0\n"
printf "Test output: \n"
./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 0


# Test 8: crawl at depth 1
printf "\n\nTest 8: crawl at depth 1\n"
printf "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 1\n"
printf "Test output: \n"
./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 1


# Test 9: crawl at depth 2
printf "\n\nTest 9: crawl at depth 2\n"
printf "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 2\n"
printf "Test output: \n"
./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 2


# Test 10: crawl at depth 3
printf "\n\nTest 10: crawl at depth 3\n"
printf "./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 3\n"
printf "Test output: \n"
./crawler http://old-www.cs.dartmouth.edu/~cs50/tse/ ./data 3