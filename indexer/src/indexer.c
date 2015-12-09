/* ========================================================================== */
/* File: indexer.c - Tiny Search Engine indexer
 *
 * Author: Chua Zheng Fu Edrei			
 * Date: 9 August 2015
 *
 * Input: ./indexer [TARGET_DIRECTORY] [RESULTS FILE NAME] (1)
 * 		./indexer [TARGET_DIRECTORY] [RESULTS FILENAME] [REWRITEN FILENAME] (2)
 *
 * Command line options: (1) or (2) as shown above
 *
 * Output: 1) [RESULTS FILE NAME] with an inverted index of words in the html files stored 
 *				in [TARGET_DIRECTORY] for first command line option
 *		   2) the above and [REWRITEN FILENAME] with an inverted index of words reconstructed
 *				from [RESULTS FILE NAME]
 *		   3) Note: [RESULTS FILE NAME] and [REWRITEN FILENAME] will be saved in [TARGET_DIRECTORY]
 *				as indicated in the lab instructions
 *
 * Pseudocode: 1. Check input parameters
 *					a. Check number of arguments
 *					b. Check if directory is valid
 *			   2. Assign variable names to input parameter for readability of code
 *			   3. Fill up array of filenames using the function GetFilenamesInDir
 *			   4. Create Inverted Index
 *					a. get document ID using function GetDocumentId
 *					b. combined filename with directory path using function getFileName
 *					c. load the html file into a string using function LoadDocument
 *					d. get the words in the html string using function GetNextWord
 *					e. normalise the words using function NormalizeWord
 *					f. insert the normalised word into the inverted index using function HashTableInsert
 *					g. free memory
 *			   5. Save Inverted Index to file using function SaveIndexToFile
 *			   6. Reconstruct inverted index from file if applicable (command line option 2)
 *					a. read the file and create the index using function ReadFile
 *					b. save the reconstructed index to file using function SaveIndexToFile
 *
 * Special Considerations: NIL
 *
 */
/* ========================================================================== */

#include "common.h"

// ---------------- Local includes  e.g., "file.h"
#include "file.h"                          	// file checking functionality
#include "web.h"                            // html parsing functionality
#include "hashtable.h"                      // hashtable functionality
#include "list.h"

// ---------------- Constant definitions
#define MAXLEN 1000

// ---------------- Private prototypes
char* getFileName(char* fileName, char* dir);
char* LoadDocument(char* fileName);
int GetDocumentId (char* fileName);
void SaveIndexToFile(HashTable* index, char* filename);
int ReadFile(HashTable* index, char *file);

/* ========================================================================== */

int main(int argc, char* argv[])
{
	HashTable Index;			// the inverted index
	InitialiseHashTable(&Index);
	
	// check input parameters--------------------------------------------------------
	if (argc<3 || argc>4 ){ 	// check number of arguments
		fprintf(stderr,"Error: Incorrect number of input argument\n");
		return -1;
	}else if(!IsDir(argv[1])){	// check if directory is valid
		fprintf(stderr,"Error: Directory cannot be found\n");
		return -1;
	}

	// Assign variable names to input parameter for readability of code--------------
	char* directory = argv[1];
	char* result_file = getFileName(argv[2], directory);
	

	// Fill up array of filenames----------------------------------------------------
	char **filenames = NULL;
 	int num_files = 0;
 
	num_files = GetFilenamesInDir(directory, &filenames);
 	if(num_files < 0) {
      	fprintf(stderr,"Error: One of the files in the directory is invalid or corrupted\n");
		free(result_file); result_file = NULL;
		return -1;
  	}else if(num_files==0){
		fprintf(stderr,"Error: Directory doesn't contain a valid file\n");
		free(result_file); result_file = NULL;
		return -1;
	}
	
	// Create Inverted Index----------------------------------------------------------
	for(int i = 0; i < num_files; i++) {
		int docId = GetDocumentId(filenames[i]);
    	if(docId == -1) continue;
		char* newfilename = getFileName(filenames[i],directory); // combine filename with path
    	printf("Reading file: %s\n", newfilename);
    	char* doc = LoadDocument(newfilename); 	// Create pointer to the html document
    	if(doc == NULL){
    		free(filenames[i]); filenames[i] = NULL;
    	 	continue;
    	}
    	char *word = NULL;
    	int pos = 0;
    	while ((pos = GetNextWord(doc, pos, &word)) > 0){	// get words from the html doc
    		NormalizeWord(word);
    		HashTableInsert(word, docId, &Index);	// insert word into hashtable
        	free(word);	
        	word = NULL;
    	}  	
    	// free memory
    	free(newfilename); newfilename = NULL;
    	free(filenames[i]); filenames[i] = NULL;
    	free(doc); doc = NULL;
    }
    free(filenames);
    
    // Save Inverted Index to file----------------------------------------------------
   	SaveIndexToFile(&Index, result_file);
   	printf("Inverted index saved in %s\n", result_file);
    
    // Reconstruct inverted index from file-------------------------------------------
    if(argc == 4){
		char* rewritten_file = getFileName(argv[3], directory);
		if(!strcmp(result_file,rewritten_file)){
    		free (result_file); free(rewritten_file);
    		fprintf(stderr,"Error: file name for [RESULTS FILENAME] and [REWRITEN FILENAME] are the same. They should be different\n");
			return -1;
		}
		HashTable ReconstructIndex;	
		InitialiseHashTable(&ReconstructIndex);
		if(!ReadFile(&ReconstructIndex,result_file)){	// error if index cannot be reconstructed
			free (result_file); free(rewritten_file);
			return -1;
		}
		SaveIndexToFile(&ReconstructIndex, rewritten_file);
		printf("Reconstructed inverted index saved in %s\n", rewritten_file);
		free(rewritten_file);
	}
	free(result_file);
	
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
char* getFileName(char *fileName, char* dir){
	char* newfilename;
	if ( *(dir + (strlen(dir)-1)) == '/'){
		newfilename = calloc(strlen(fileName)+strlen(dir)+1, sizeof(char));
		sprintf(newfilename,"%s%s",dir, fileName);
	}else{
		newfilename = calloc(strlen(fileName)+strlen(dir)+2, sizeof(char));
		sprintf(newfilename,"%s/%s",dir, fileName);
	}
	return(newfilename);	
}

/*
 * LoadDocument: function to load the html file into a string
 * @fileName: the file name of the html file
 * 
 * Returns a string with the html file loaded
 * Special consideration: It is the caller's responsibility to free string
 *
 * Pseudocode:	1. Check if file exists
 *				2. Check if file is valid
 *				3. Allocate memory to string
 *				4. Copy the file into string
 *
 */
char* LoadDocument(char *fileName){
	FILE* rfile = fopen(fileName,"r");
	// Check if file exists
	if(rfile == NULL){
		fprintf(stderr,"Error: file does not exist %s\n", fileName);
		fclose(rfile);
		free(fileName); fileName = NULL;
		return NULL;
	}
	
	// Check if file is valid
	if(!IsFile(fileName)){
		fprintf(stderr,"Error: file cannot be opened %s\n", fileName);
		fclose(rfile);
		free(fileName); fileName = NULL;
		return NULL;
	}
	
	// Allocate memory for entire content
	int lSize;
	char *htmldoc;
	char junk[MAXLEN];
	struct stat st;
	stat(fileName, &st);
	lSize = st.st_size;
	htmldoc = calloc(1, lSize+1);
	
	// ignore the first 3 lines
	for(int i=0; i<2; i++){
		if(fgets(junk, MAXLEN, rfile)==NULL){  
			fprintf(stderr,"Error: file format is incorrect %s\n", fileName);
			fclose(rfile);
			free(fileName); fileName = NULL;
			return NULL;
		}
	}
	
	while(fgets(junk, MAXLEN, rfile)!=NULL){
		strcat(htmldoc,junk);
	}
		
	fclose(rfile);
    return(htmldoc);
}

/*
 * GetDocumentId: function to get the document ID from the filename
 * @fileName: the file name of the html file
 * 
 * Returns an integer that is the document ID
 * Assumption: the file name is an integer
 *
 * Pseudocode:	1. Check if file name is an integer
 *				2. Returns the integer as the document id if 1. is true
 *
 */
int GetDocumentId (char *fileName){
	int doc_id;
	if (sscanf (fileName, "%d", &doc_id)!=1) { 
		fprintf(stderr,"File name must be a number. File %s will be skipped\n", fileName);
		free(fileName); fileName = NULL;
		return -1;
	}
	return(doc_id);
}

/*
 * SaveIndexToFile: function to convert the inverted index into a file
 * @index: the inverted index
 * @fileName: the file name of the html file
 * 
 * Prints a file (filename) with the inverted index into the target directory
 *
 * Pseudocode:	for all rows in the hash table{
 *					while there are still WordNodes in the row{
 *						print the word and the number of documents with the word
 *						print the document id and frequency while there are DocumentNodes linked to the WordNode
 *					}
 *					print a new line
 *				}
 *				clean up 
 *
 */
void SaveIndexToFile(HashTable* index, char* filename){
	FILE* wfile = fopen(filename, "w");
	for(int i=0; i<MAX_HASH_SLOT; i++){
		WordNode* curr_word_node = index->table[i];
		while(curr_word_node != NULL){
			fprintf(wfile,"%s %d ",curr_word_node->word, curr_word_node->docfreq);
			DocumentNode* curr_doc_node = curr_word_node->page;
			do{
				fprintf(wfile,"%d %d ", curr_doc_node->doc_id, curr_doc_node->freq);
				curr_doc_node = curr_doc_node->next;
			}while(curr_doc_node != NULL);
			fprintf(wfile,"\n");
			curr_word_node = curr_word_node->next;
		}
	}
	CleanUpHash(index);
	fclose(wfile);
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