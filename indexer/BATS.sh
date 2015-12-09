#!/bin/bash
#
# test.sh
#
# Description: test indexer and output results into a log file 
# The bash script will run the following tests:
#
# 1. Test input parameters
# 2. Clean build indexer
# 3. Test indexer in both the original and testing mode and compare the index files
#

# 1. Specify the log file, data directory and the final index file
logfile="`date +"%a_%b_%d_%T_%Y"`.log"
datapath=../data 
indexfile=index.dat 
echo "Test started at: `date +"%a_%b_%d_%T_%Y"`" >> $logfile
echo "Operating system and host name: `uname -a`" >> $logfile
echo "start testing indexer" >> $logfile
printf "\n\n" >> $logfile

# 2. clean build the indexer
echo "1. make clean and build indexer" >> $logfile
make clean &>> $logfile
make indexer &>> $logfile
if [ $? -ne 0 ]; then
	echo "build failed" >> $logfile
	exit 1
fi
echo "build indexer successfully" >> $logfile
printf "\n\n" >> $logfile

# 3. test input parameters
echo "2.1 Test input parameters: insufficient number of arguments" >> $logfile
echo "./indexer $datapath" >> $logfile
printf "Output: ">> $logfile
./indexer $datapath 2>> $logfile
if [ $? -eq 0 ]; then
	echo "test input parameters fail" >> $logfile
	exit 1
else
	echo "test input parameters successfully" >> $logfile
fi

printf "\n\n" >> $logfile

echo "2.2 Test input parameters: Invalid data path" >> $logfile
echo "./indexer ./datapath index.dat" >> $logfile
printf "Output: ">> $logfile
./indexer ./datapath index.dat 2>> $logfile
if [ $? -eq 0 ]; then
	echo "test input parameters fail" >> $logfile
	exit 1
else
	echo "test input parameters successfully" >> $logfile
fi

printf "\n\n" >> $logfile


# 4. test indexer in the testing mode and compare the results
echo "3. Test indexer in the testing mode and compare the results" >> $logfile
echo "./indexer $datapath tmpindex tmpnewindex" >> $logfile
printf "Output: ">> $logfile
./indexer $datapath tmpindex tmpnewindex 2>> $logfile

tmpindex="${datapath}/tmpindex"
tmpnewindex="${datapath}/tmpnewindex"

sort $tmpindex >> tmpindex2
sort $tmpnewindex >> tmpnewindex2
diff tmpindex2 tmpnewindex2
if [ $? -eq 0 ]; then
    echo "inverted index reconstructed from file successfully" >> $logfile
    echo "Indexes have been built, read and rewritten correctly!" >> $logfile
else
    echo "inverted index reconstruction fail"
fi

# 5. Clean up
rm $tmpindex
rm $tmpnewindex
rm tmpnewindex2
rm tmpindex2
make clean

printf "\n\n" >> $logfile

echo "Test ended at: `date +"%a_%b_%d_%T_%Y"`" >> $logfile

exit 0