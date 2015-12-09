/* ========================================================================== */
/* File: crawler.c - Tiny Search Engine web crawler
 *
 * Author: Chua Zheng Fu Edrei			
 * Date: 29 July 2015
 *
 * Input: ./crawler [SEED URL] [TARGET DIRECTORY WHERE TO PUT THE DATA] [MAX CRAWLING DEPTH]
 *
 * Command line options: NIL
 *
 * Output: html files of all the unique webpages crawled
 *
 * Error Conditions: Note that the parser provided does not eliminate all duplicate websites
 *		(it included some internal reference)
 *
 * Special Considerations: I define an invalid URL as one with http status code >= 400
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>                           // printf
#include <curl/curl.h>                       // curl functionality
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

// ---------------- Local includes  e.g., "file.h"
#include "common.h"                          // common functionality
#include "web.h"                             // curl and html functionality
#include "list.h"                            // webpage list functionality
#include "hashtable.h"                       // hashtable functionality
#include "utils.h"                           // utility stuffs

// ---------------- Constant definitions
#define MAXLEN 1000

// ---------------- Macro definitions

// ---------------- Structures/Types
struct WebPage* seedPage;

// ---------------- Private variables
char* seedURL;									// the url of the seed webpage
char* webPageDirectory;							// directory to write file
int maxWebPageDepth; 							// maximum depth specified by user
int filenum;									// labeling of filenumber

// ---------------- Private prototypes
int CheckURL(char* url);
int CheckDirectory(char* directory);
int CheckDepth( const char* depth);
void WriteFile(WebPage webpage, int pageid);
void CrawlPage(WebPage webpage);

/* ========================================================================== */


int main(int argc, char* argv[])
{	
	filenum = 1;
	
    // check command line arguments
	if(argc != 4){		// check number of arguments
		fprintf(stderr,"Error: Number of input argument needs to be exactly 3\n");
		return -1;
	}else if (CheckDirectory(argv[2])){		// check if directory exist
		return -1;
	}else if(CheckDepth(argv[3])){			// check depth
		return -1;
	}else if(CheckURL(argv[1])){			// check url
		fprintf(stderr,"Error: Invalid URL. Can only crawl URL with URL prefix %s\n",URL_PREFIX);
		return -1;
	}
	
    // init curl
    curl_global_init(CURL_GLOBAL_ALL);

    // setup seed page
	seedPage = (WebPage*)calloc(1,sizeof(WebPage));
	NormalizeURL(seedURL);
    seedPage->url = (char*)calloc(strlen(seedURL)+1,sizeof(char));
	strcpy(seedPage->url,seedURL);
	seedPage->depth = 0;
	
    // get seed webpage
    if(!GetWebPage(seedPage)){				// clean up and exit if url is invalid
    	fprintf(stderr,"Error: Invalid URL\n");
		curl_global_cleanup();
		return -1;
	}

    // write seed file
	WriteFile(*seedPage, filenum++);
	
	// Exit if maxWebPageDepth = 0
	if(maxWebPageDepth == 0){				// clean up and exit if max webpage depth is 0
		printf("\n[crawler]: Crawling - %s\n\n",seedPage->url);
		printf("1 page has been crawled \n\n");
		CleanUpPage(seedPage);	
		CleanUpHash(URLsVisited);	
		curl_global_cleanup();
		return 0;
	}	
	
    // add seed page to hashtable
   	InitialiseHashTable(URLsVisited);
    HashTableInsert(seedURL);

    // add seed node to list
    WebPageList = (List*)calloc(1,sizeof(List));
    struct ListNode* seednode = (ListNode*)calloc(1,sizeof(ListNode));
    seednode->page = seedPage;
    WebPageList->head = seednode;
    WebPageList->tail = seednode;
    
    // extract urls from seed page
    CrawlPage(*seedPage);
    WebPageList->head = RemoveNode(WebPageList->head);
    
    // while there are urls to crawl
   	while(WebPageList->head != NULL && WebPageList->head->page->depth < maxWebPageDepth){
        // get next url from list, get webpage for url, write page file and extract urls from webpage
		CrawlPage(*(WebPageList->head->page));
		WebPageList->head = RemoveNode(WebPageList->head);
	}
	
	// cleanup memory
	CleanUpList(WebPageList);
	CleanUpHash(URLsVisited);

    // cleanup curl
    curl_global_cleanup();
	
	printf("\n\n %d webpages have been crawled\n\n", filenum-1);
    return 0;
}

/* ==================================   Functions   ================================== */

/* 	CheckURL				 ............................................................	
*
*	Function to check if URL is a valid URL
*	Input: url (string)
*	Output: -1 if url is invalid, 0 if url is valid
*/

int CheckURL(char* url){
	char dummy[MAXLEN];
	
	if(strlen(url)<strlen(URL_PREFIX)){
		return -1;
	}else{ 
		strcpy(dummy, url);
		dummy[strlen(URL_PREFIX)]='\0';
		if(strcmp(dummy,URL_PREFIX)){
			return -1;
		}
	}
	NormalizeURL(url);
	seedURL = url;
	
	return 0;
}

/* 	CheckDirectory				 .........................................................	
*
*	Function to check if filename is a directory that exists
*	Input: directory (string)
*	Output: -1 if directory cannot be found, 0 if directory can be found
*/

int CheckDirectory(char* directory){
	struct stat dir;

	if(!stat(directory, &dir) && S_ISDIR(dir.st_mode)){
		webPageDirectory = directory;
		return 0;
	}else{
		fprintf(stderr,"Error: Directory cannot be found\n");
		return -1;
	}
}

/* 	CheckDepth					 .........................................................	
*
*	Function to check if depth exceeds the MAX_DEPTH
*	Input: depth (string)
*	Output: -1 if depth exceeds MAX_DEPTH, 0 otherwise
*/

int CheckDepth(const char* depth){
	int depthnum;
	
	if (sscanf (depth, "%d", &depthnum)!=1) { 
		fprintf(stderr,"Error: Input argument for depth must be a number\n");
		return -1;
	}else if(depthnum > MAX_DEPTH){
		fprintf(stderr,"Error: Depth exceeds the maximum limit\n");
		return -1;	
	}	
	*(&maxWebPageDepth) = depthnum;
	return 0;
}

/* 	WriteFile					 .........................................................	
*
*	Function to write webpage to directory
*	Input: webpage (WebPage), pageid (int)
*	Output: NIL
*/

void WriteFile(WebPage webpage, int pageid){
	char filename[MAXLEN];
	FILE * wfile;
	
	sprintf(filename, "%s/%d", webPageDirectory, pageid);
	wfile = fopen(filename,"w");
	fprintf(wfile, "%s\n%d\n%s",webpage.url,webpage.depth,webpage.html);	
	fclose(wfile);
}

/* 	CrawlPage					 .........................................................	
*
*	Function to crawl page
*	Input: webpage (WebPage)
*	Output: NIL
*/

void CrawlPage(WebPage webpage){
	char* nexturl= NULL;
	int lastpos = 0;
	int depth = webpage.depth + 1;
	
	if(depth > maxWebPageDepth) return;
	
	printf("\n\n[crawler]: Crawling - %s\n\n",webpage.url);
	
	while((lastpos = GetNextURL(webpage.html, lastpos, webpage.url, &nexturl))>0){
		NormalizeURL(nexturl);
		if(!CheckURL(nexturl)){
			// setup new page
			struct WebPage* newwebpage = (WebPage*)calloc(1,sizeof(WebPage));
			newwebpage->url = (char*)calloc(strlen(nexturl)+1, sizeof(char));
			strcpy(newwebpage->url,nexturl);
			newwebpage->depth = depth;
			
			// get new webpage
    		if(GetWebPage(newwebpage)){
    			if(HashTableInsert(nexturl)){											 // If not found in hash table, add to hash table
    				printf("[crawler]: Parser found new link - %s\n",nexturl);
    				struct ListNode* listentry = (ListNode*)calloc(1,sizeof(ListNode));							
    				listentry->page = newwebpage;									     // then add to list
					WebPageList->tail = InsertNode(WebPageList->tail,listentry);
					
	   				WriteFile(*newwebpage, filenum++); 									 // then write file
    			} else{
    				CleanUpPage(newwebpage);
    			}
			}
		}
		free(nexturl);
		nexturl = NULL;
		// Sleep for a second 
		sleep(INTERVAL_PER_FETCH);
	}
}