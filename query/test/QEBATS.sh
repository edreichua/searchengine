#!/bin/bash
#
# QEBATS.sh
#
# Description: test query and output results into a log file 
# The bash script will run the following tests:
#
# 1. Test input parameters
# 2. Clean build query
# 3. Test input parameter
# 4. Test query engine
#
# It requires queryengine_test.c to run and presuppose that the datapath of the directory of
# the html files is $(datapath) (default is ../../data) and the inverted index file is
# $(indexfile) (default is ../../indexer/index.dat). This should be changed accordingly.
#
# 1. Specify the log file, data directory and the final index file
logfile="`date +"%a_%b_%d_%T_%Y"`.log"
datapath=../../data 
indexfile=../../indexer/index.dat 

echo "Test started at: `date +"%a_%b_%d_%T_%Y"`" >> $logfile
echo "Operating system and host name: `uname -a`" >> $logfile
echo "start testing query" >> $logfile
printf "\n\n" >> $logfile

# 2. Clean build the query engine
echo "1. make clean and build query engine" >> $logfile
make clean &>> $logfile
make test &>> $logfile
if [ $? -ne 0 ]; then
	echo "build failed" >> $logfile
	exit 1
fi
echo "build query engine successfully" >> $logfile
printf "\n\n" >> $logfile

# 3. Test input parameters
echo "2.1 Test incorrect number of argument" >> $logfile
echo "./query $indexfile" >> $logfile
printf "Output: ">> $logfile
./queryengine_test $indexfile &>> $logfile
if [ $? -eq 0 ]; then
	echo "test input parameters fail" >> $logfile
	exit 1
else
	echo "test input parameters successfully" >> $logfile
fi
printf "\n\n" >> $logfile

echo "2.2 Test invalid index file" >> $logfile
echo "./query invalidindex $datapath" >> $logfile
printf "Output: ">> $logfile
./queryengine_test invalidindex $datapath &>> $logfile
if [ $? -eq 0 ]; then
	echo "test input parameters fail" >> $logfile
	exit 1
else
	echo "test input parameters successfully" >> $logfile
fi
printf "\n\n" >> $logfile

echo "2.3 Test invalid directory" >> $logfile
echo "./query $indexfile invaliddir" >> $logfile
printf "Output: ">> $logfile
./queryengine_test $indexfile invaliddir &>> $logfile
if [ $? -eq 0 ]; then
	echo "test input parameters fail" >> $logfile
	exit 1
else
	echo "test input parameters successfully" >> $logfile
fi
printf "\n\n" >> $logfile

# 4. Test query engine
echo "3. Test query engine" >> $logfile
echo "./query $indexfile $datapath" >> $logfile
./queryengine_test $indexfile $datapath &>> $logfile

# queryengine_test takes over in printing the result

# 5. Clean up
make clean

printf "\n\n" >> $logfile

echo "Test ended at: `date +"%a_%b_%d_%T_%Y"`" >> $logfile

exit 0