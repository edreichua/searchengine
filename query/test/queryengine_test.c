/* ========================================================================== */
/* File: queryengine_test.c - Tiny Search Engine query test
 *
 * Author: Chua Zheng Fu Edrei			
 * Date: 14 August 2015
 *
 * Input: Automated test.
 *
 * Command line options: NA
 *
 * Output: Results of the search, ranked in descending order of importance
 *
 * Pseudocode:    Check input parameters
 *			      Reconstruct inverted index from [INDEX FILE] using the ReadFile function
 *			      while (true){
 *						unionflag <- 0; flag <- 0;
 *						scan for words and operators (AND, OR) and store in an array
 *
 *						for all elements i in the array
 *							if the first element in the array is not an operator
 *								normalize word and store it in the intersect array; continue;
 *							else if first element is an operator
 *								input is invalid; flag = 1; break;
 *							else if unionflag == 1 (the previous word is a OR)
 *								if the word is not an operator
 *									normalize word and store it in the intersect array; unionflag <-0; continue;
 *								else
 *									input is invalid; flag = 1; break;
 *
 *							if the operator is AND
 *								if operator is valid
 *									normalize next word and store it in the intersect array; i++; continue;
 *								else
 *									input is invalid; flag = 1; break;
 *							else if operator is OR
 *								if operator is valid
 *									find the union of the final array and the intersect array; continue;
 *								else
 *									input is invalid; flag = 1; break;	
 *							else (is not an operator, therefore	acts as AND)
 *								normalize word and store it in the intersect array; continue;	
 *											
 *						find the union of the final array and the intersect array
 *
 *						if flag == 1
 *							invalid syntax; 
 *						if size of final array == 0
 *							no results found;
 *  					else
 *							sort final array by frequency
 *							print results				
 *
 * Special Considerations: 1. query returns the top 50 results (or less if there are fewer
 *								URLs that meet the search criteria). The user can change the
 *								maximum number of URLs return by changing the constant
 *								MAXRESULT (the default is 50).
 *						   2. query assumes that there are less than MAXSIZE unique urls. The
 *								default for MAXSIZE is 1705. If there are more urls, the value of 
 *								MAXSIZE should be changed accordingly.
 *						   3. query allows the user to key in a maximum of MAXLEN characters for
 *								the search criteria and operators. Entries that contain more than
 *								MAXLEN characters will be truncated. The default for MAXLEN is 1000
 *								and can be adjusted accordingly.
 *
 */
/* ========================================================================== */

#include "header.h"

// ---------------- Constant definitions
#define MAXLEN 1000
#define MAXSIZE 1705
#define MAXRESULT 5

// ---------------- Private prototypes
char* getFileName(int fileName, char* dir);		// function from Indexer
int ReadFile(HashTable* index, char *file);		// function from Indexer
int FindHash(char* word, int array[][2], HashTable index);
int FindIntersection(int array1[][2], int size1, int array2[][2], int size2);
int FindUnion(int array1[][2], int size1, int array2[][2], int size2);
void Sort(int array[][2], int size, int criteria);
int BinarySearch(int array[][2], int min, int max, int doc);
int StringToWord(char wordarray[][MAXLEN], char text[MAXLEN]);
int CheckOperator(char wordarray[][MAXLEN], int check, int count);
int ReturnURL(char url[MAXLEN], int doc_id, char* dir);
/* ========================================================================== */

int main(int argc, char* argv[]){
	// Declare variables-------------------------------------------------------------
	HashTable Index;				// Inverted index
	InitialiseHashTable(&Index);
	char text[MAXLEN];
	int test = 0;
	
	// 1. Check input parameters--------------------------------------------------------
	if (argc != 3 ){ 				// check number of arguments
		fprintf(stderr,"Error: Incorrect number of input argument\n");
		return -1;
	}else if(!IsFile(argv[1])){		// check if file is valid
		fprintf(stderr,"Error: File %s is invalid\n", argv[1]);
		return -1;
	}else if(!IsDir(argv[2])){		// check if directory is valid
		fprintf(stderr,"Error: Directory %s cannot be found\n", argv[2]);
		return -1;
	}
	
	// 2. Reconstruct Inverted Index-----------------------------------------------------
	printf("Please wait while the query engine is loading. It might take a few minutes... \n");
	if(!ReadFile(&Index, argv[1])){
		CleanUpHash(&Index);
		return -1;
	}
	
	
	// 3. Command Line interface and query -----------------------------------------------
	
	for(int j=0; j<9; j++){
		
		// Create text array for automated testing
		
		switch (j){
			case 0:
				printf("\n3.%d Test invalid input syntax\n",j+1);
				printf("QUERY :> AND dog\n");
				strcpy(text,"AND dog\n");
				break;
			case 1:
				printf("\n3.%d Test invalid input syntax\n", j+1);
				printf("QUERY :> cat OR AND dog\n");
				strcpy(text,"cat OR AND dog\n");
				break;
			case 2:
				printf("\n3.%d Test no result\n", j+1);
				printf("QUERY :> thisisrandom\n");
				strcpy(text,"thisisrandom\n");
				break;
			case 3:
				printf("\n3.%d Test single entry\n", j+1);
				printf("QUERY :> incredible\n");
				strcpy(text,"incredible\n");
				break;
			case 4:
				printf("\n3.%d Test uppercase\n", j+1);
				printf("QUERY :> Incredible\n");
				strcpy(text,"Incredible\n");
				break;
			case 5:
				printf("\n3.%d Test AND\n", j+1);
				printf("QUERY :> Dartmouth AND College AND Computer AND Science\n");
				strcpy(text,"Dartmouth AND College AND Computer AND Science\n");
				break;
			case 6:
				printf("\n3.%d Test space as AND\n", j+1);
				printf("QUERY :> Dartmouth College Computer Science\n");
				strcpy(text,"Dartmouth College Computer Science\n");
				break;
			case 7:
				printf("\n3.%d Test OR\n", j+1);
				printf("QUERY :> Dartmouth OR Computer\n");
				strcpy(text,"Dartmouth OR Computer\n");
				break;
			case 8:
				printf("\n3.%d Test combined\n", j+1);
				printf("QUERY :> Dartmouth College AND Hanlon OR Mathematics AND Computer Science AND Philosophy OR incredibles Pixar\n");
				strcpy(text,"Dartmouth College AND Hanlon OR Mathematics AND Computer Science AND Philosophy OR incredibles Pixar\n");
				break;
		
		}
		// a) declare variables
		int unionflag, flag, size_temp, size_intersect, size_final, count;
		char wordarray[MAXLEN][MAXLEN];
		int temparray[MAXSIZE][2], intersect[MAXSIZE][2], final[MAXSIZE][2];
		
		// b) instantiate variables
		size_temp = size_intersect = size_final = unionflag = flag = 0;
		count = StringToWord(wordarray,text);
		
		// c) query
		for(int i=0; i<count; i++){
			if(i==0 && strcmp(wordarray[i],"AND") && strcmp(wordarray[i],"OR")){ 	// if it's the first word and is not invalid
				NormalizeWord(wordarray[i]);
				size_intersect = FindHash(wordarray[i], intersect, Index);
				continue;
			}else if(i==0){ 	// if it is first word and invalid
				flag = 1; break;
			}else if(unionflag){
				if(strcmp(wordarray[i],"AND") && strcmp(wordarray[i],"OR")){
					NormalizeWord(wordarray[i]);
					size_intersect = FindHash(wordarray[i], intersect, Index);
					unionflag = 0;
					continue;
				}else{
					flag = 1; break;
				}
			}
			
			if (!strcmp(wordarray[i],"AND")){	// if it's AND
				if(CheckOperator(wordarray,i,count)){
					NormalizeWord(wordarray[i+1]);
					size_temp = FindHash(wordarray[i+1], temparray, Index);
					size_intersect = FindIntersection(intersect, size_intersect, temparray, size_temp);
					i++;
					continue;
				}else{
					flag = 1; break;
				}
			}else if(!strcmp(wordarray[i],"OR")){ // if it's OR
				if(CheckOperator(wordarray,i,count)){
					size_final = FindUnion(final, size_final, intersect, size_intersect);
					size_intersect = 0;
					unionflag = 1;
					continue;
				}else{
					flag = 1; break;
				}
			}else{
				NormalizeWord(wordarray[i]);
				size_temp = FindHash(wordarray[i], temparray, Index);
				size_intersect = FindIntersection(intersect, size_intersect, temparray, size_temp);
				continue;
			}
		}
		size_final = FindUnion(final, size_final, intersect, size_intersect);
		
		// c) print result
		if(flag){
			printf("Error: Invalid input syntax\n");
			if(j==0 || j==1 ){
				printf("\nTest 3.%d for query engine successfully\n",j+1);
				test++;
			}else{
				printf("\nTest 3.%d for query engine fail\n",j+1);
			}
		}else if(!size_final){
			printf("No results found.\n");
			if(j==2 ){
				printf("\nTest 3.%d for query engine successfully\n",j+1);
				test++;
			}else{
				printf("\nTest 3.%d for query engine fail\n",j+1);
			}
		}else{
			Sort(final, size_final, 1); // sort by frequency
			printf("\nPrinting the top %d results or less for testing:\n",MAXRESULT);
			for(int i=size_final-1; i>=0 && i> size_final-MAXRESULT-1; i--){
				char url[MAXLEN];
				if(!ReturnURL(url, final[i][0],argv[2])){
					strcpy(url,"Not Found\n");
				}	
				printf("Document ID:%d Importance:%d URL:%s", final[i][0], final[i][1], url);
			}
			if(j>2 ){
				printf("\nTest 3.%d for query engine successfully\n",j+1);
				test++;
			}else{
				printf("\nTest 3.%d for query engine fail\n",j+1);
			}
		}		
	}
	
	printf("\n%d/9 tests are successful\n",test);
	if(test==9){
		printf("All tests are successful. Query engine test success!\n");
	}
	
	// clean up
	CleanUpHash(&Index);
	return 0;
}

/* ==================================   Functions   ================================== */

/*
 * getFileName: function to get file name combined with directory path
 * @fileName: the file name
 * @dir: the directory to store the file
 * 
 * Returns the new file name (file name combined with directory path)
 * Special consideration: It is the caller's responsibility to free the filename
 *
 * Pseudocode:	1. If directory ends with a '\', append filename with directory path
 *				2. If directory does not end with a '\', append filename with directory
 *					path and include a '\' in between
 *
 */
char* getFileName(int fileName, char* dir){
	char* newfilename;
	if ( *(dir + (strlen(dir)-1)) == '/'){
		newfilename = calloc(floor(log10(abs((double)fileName)))+strlen(dir)+2, sizeof(char));
		sprintf(newfilename,"%s%d",dir, fileName);
	}else{
		newfilename = calloc(floor(log10(abs((double)fileName)))+strlen(dir)+3, sizeof(char));
		sprintf(newfilename,"%s/%d",dir, fileName);
	}
	return(newfilename);	
}

/*
 * ReadFile: function to read the file and reconstruct the inverted index
 * @index: the reconstructed inverted index (should be NULL when passed into the function)
 * @file: the file name of the file with the inverted index
 * 
 * Returns the reconstructed inverted index
 * Special consideration: index should be declared NULL before being passed into the function
 * Assumptions: the file is in the format specified in the documentation
 *
 * Pseudocode:	1. Open the file and check if it exist
 *				2. Scan for the word, the number of documents with the word, the document id
 *					and the frequency of word occurrences
 *				3. Input the parameters to reconstruct the index using HashTableInsert
 *
 */
int ReadFile(HashTable* index, char *file){
	FILE* rfile = fopen(file,"r");
	char word[MAXLEN];
	int num_of_doc, id, fre, flag;
	
	if(rfile == NULL){
		fclose(rfile);
		fprintf(stderr,"Error: file %s does not exist\n", file);
		return 0;
	}
	
	if(!IsFile(file)){
		fclose(rfile);
		fprintf(stderr,"Error: file %s is not a regular file\n", file);
		return 0;
	}
	
	while((flag = fscanf(rfile,"%s %d ", word, &num_of_doc))!=EOF){
		if(flag!=2){
			fprintf(stderr,"Error: Format of file (%s) content is incorrect\n", file);
			fclose(rfile);
			return 0;
		}
		for(int i=0; i<num_of_doc; i++){
			if(fscanf(rfile,"%d %d ", &id, &fre)!=2){
				fprintf(stderr,"Error: Format of file (%s) content is incorrect\n",file);
				fclose(rfile);
				return 0;
			}else{
				for(int j=0; j<fre; j++) HashTableInsert(word, id, index);
			}
		}
	}
	fclose(rfile);
	return 1;
}

/*
 * FindHash: function to find the word in the inverted index and return with an array of
 *			the document ID with the corresponding frequency of documents that contain the word
 * 
 * @word: the word to be searched
 * @array: the array to be filled in
 * @index: the inverted index
 * 
 * Returns an array of document ID with the corresponding frequency of documents that contain the word
 * Returns the size of the array
 *
 * Pseudocode:	1. Calculate the hashnum for the word, look through the table
 *				2. Find the WordNode that matches the word using a while loop
 *				3. Record all the Document ID in the WordNode in the array
 *				4. Sort the array by document ID in ascending order
 *				5. Return the size of the array if documents are found, return 0 otherwise
 *
 */
int FindHash(char* word, int array[][2], HashTable index){
	
	unsigned long hashnum = JenkinsHash(word, MAX_HASH_SLOT);
	WordNode *wnode = index.table[hashnum];
	
	while(wnode != NULL){
		if(!strcmp(wnode->word,word)){
			DocumentNode *dnode = wnode->page;
			for(int i=0; i<wnode->docfreq && i<MAXSIZE; i++){
				array[i][0] = dnode->doc_id;
				array[i][1] = dnode->freq;
				dnode = dnode->next;
			}
			Sort(array, wnode->docfreq, 0);
			return (wnode->docfreq);
		}
		wnode = wnode->next;
	}
	return 0;
}

/*
 * FindIntersection: function to find the intersection between 2 arrays
 * 
 * @array1: the first array 
 * @size1: the size of the first array
 * @array2: the second array
 * @size2: the size of the second array
 * 
 * Modify array1 to be the intersection between array1 and array2
 * Returns the size of the modified array that is the intersection between array1 and array2
 *
 * Special Consideration: It is the caller's responsibility to ensure that array1 and
 *					array2 have already been sorted before passing them into the function
 *
 * Pseudocode:	for all element i in array1
 *					for all elements j in array2 that are less than i
 *						if doc_id of i is same as j
 *							store the doc_id and combined freq in a dummy array intersect
 *							increment the number of element k in the intersect array
 *							remember the previous j value so we can start from that value
 *							break
 *				Transfer the content of the dummy array to array1
 *				Sort array1
 *				Returns the size of array1		
 *
 */
int FindIntersection(int array1[][2], int size1, int array2[][2], int size2){
	int k = 0;
	int jindex = 0;
	int intersect[MAXSIZE][2];
	for(int i=0; i<size1 && i<MAXSIZE; i++){
		for(int j=jindex; j<size2 && array2[j][0] <= array1[i][0]; j++){
			if(array1[i][0] == array2[j][0]){
				intersect[k][0] = array1[i][0];
				intersect[k][1] = array1[i][1] + array2[j][1];
				k++;
				jindex = j;
				break;
			}
		}
	}
	for(int i=0; i<k; i++){
		array1[i][0] = intersect[i][0];
		array1[i][1] = intersect[i][1];
	}
	Sort(array1, k, 0);
	return(k);
}

/*
 * FindUnion: function to find the union between 2 arrays
 * 
 * @array1: the first array 
 * @size1: the size of the first array
 * @array2: the second array
 * @size2: the size of the second array
 * 
 * Modify array1 to be the union between array1 and array2
 * Returns the size of the modified array that is the union between array1 and array2
 *
 * Special Consideration: It is the caller's responsibility to ensure that array1 and
 *					array2 have already been sorted before passing them into the function
 *
 * Pseudocode:	size <- size1
 *				for all element i in array2
 *					if element i cannot be found in array1 (using a binary search)
 *							store the doc_id and freq as a new element in array 1
 *							increment size
 *					else
 *					 	ensure that the element i is storing the greater frequency
 *					
 *				Sort array1
 *				Returns the size of array1		
 *
 */
int FindUnion(int array1[][2], int size1, int array2[][2], int size2){
	int size = size1;
		
	for(int i=0; i<size2 && i<MAXSIZE; i++){						 // for all element in array 2
		int index = BinarySearch(array1, 0, size1-1, array2[i][0]);  // search for that element in array1
		if (index == -1 ){ 											 // if an element in array2 cannot be found array1
			array1[size][0] = array2[i][0];
			array1[size][1] = array2[i][1];
			size++;
		}else{
			if(array1[index][1]< array2[i][1]) array1[index][1]=array2[i][1]; // the greater freq is stored
		}
	}
	Sort(array1, size, 0);
	return (size);
}

/*
 * Sort: function to sort the array in ascending order of document ID or document frequency
 * 
 * @array: the array to be sorted 
 * @size: the size of the array
 * @criteria: the criteria to sort the array. 0 to sort by document ID and 1 to sort by frequency
 * 
 * Modify array to be the sorted array
 *
 * Pseudocode:	Classic bubble sort	
 *
 */
void Sort(int array[][2], int size, int criteria){ // bubble sort
	int flag = 0;
	int dummydoc, dummyfreq, previous, current;
	do{
       flag = 0;	 /*Initialise flag before each bubble sort*/
         for(int i=1; i<size && i<MAXSIZE; i++){
            previous = array[i-1][criteria]; 
            current = array[i][criteria];
            if(previous > current){ 
              dummydoc = array[i][0];
              dummyfreq = array[i][1];
              array[i][0] = array[i-1][0];
              array[i][1] = array[i-1][1];
              array[i-1][0] = dummydoc;
              array[i-1][1] = dummyfreq;
              flag++;
            }
          }
    }while(flag); /*Repeat bubble sort until array are in ascending order*/
}

/*
 * BinarySearch: function to search for an element in an array using binary search
 * 
 * @array: the array
 * @min: the minimum index to start from
 * @max: the maximum index to end with
 * @doc: the document ID to be searched for
 * 
 * Returns the corresponding index of the search entry in the array if it is found, return 
 *	-1 otherwise
 *
 * Pseudocode:	Classic binary search using recursion
 *
 */
int BinarySearch(int array[][2], int min, int max, int doc){
	if(max<min) return -1;
	else{
		int mid = (max + min)/2;
		if(array[mid][0]>doc) return BinarySearch(array, min, mid-1, doc);
		else if(array[mid][0]<doc) return BinarySearch(array, mid+1, max, doc);
		else return mid;
	}
}

/*
 * StringToWord: function to convert a string of words to an array of words
 * 
 * @wordarray: the array of words to be created
 * @text: the string of words to be converted
 * 
 * Returns the size of the word array
 *
 * Pseudocode:	Read the word from the string using the function GetNextWord from indexer
 *					then store it in the array and free the memory
 *
 */
int StringToWord(char wordarray[][MAXLEN], char text[MAXLEN]){
	int i,pos;
	
	char* word = NULL;
	i = pos = 0;
    while ((pos = GetNextWord(text, pos, &word)) > 0){	// get words from the text
		strcpy(wordarray[i++],word);
		free(word);
		word = NULL;
	}
	
	return i;
}

/*
 * CheckOperator: function to check if an operator (AND, OR) is being used correctly
 * 
 * @wordarray: the array of words
 * @check: the index of the operator in wordarray
 * @count: the size of wordarray
 * 
 * Returns 1 if operator is used correctly, returns 0 otherwise
 *
 * Pseudocode:	Check if the word before the operator and the word after is an operator
 *					if that is the case, return 1
 *					else return 0
 *
 */
int CheckOperator(char wordarray[][MAXLEN], int check, int count){
	if( check>0 && check<count-1){
		if(strcmp(wordarray[check-1],"AND") && strcmp(wordarray[check-1],"OR")
		&& strcmp(wordarray[check+1],"AND") && strcmp(wordarray[check+1],"OR")){
			NormalizeWord(wordarray[check-1]);
			NormalizeWord(wordarray[check+1]);
			return 1;
		}
	}
	return 0;	
}

/*
 * ReturnURL: function to return the url by reading the html doc
 * 
 * @url: the url to be returned
 * @doc_id: the document ID
 * @dir: the directory
 * 
 * Returns 1 if URL is found, returns 0 otherwise
 *
 * Pseudocode:	get file name from the dir and doc_id using the getFileName function
 *				if url is obtained, return 1
 *				else return 0
 *	
 *				file is closed and memory is freed at the same time
 */
int ReturnURL(char url[MAXLEN], int doc_id, char* dir){
	char* filename = getFileName(doc_id, dir);
	FILE* rfile = fopen(filename, "r");
	if(rfile == NULL || !IsFile(filename)){
		fclose(rfile);
		free (filename);
		return 0;
	}
	
	if(fgets(url,MAXLEN, rfile)!=NULL){
		fclose(rfile);
		free (filename);
		return 1;
	}else{
		fclose(rfile);
		free (filename);
		return 0;
	}
}