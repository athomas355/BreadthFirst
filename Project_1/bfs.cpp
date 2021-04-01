#include "bfs.h"
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
using namespace std;

//Making the Queu global
//Queue *queue = new Queue();

bool rootDirFlag = true;

void BreadthFirst::breadthfirst(char* dir, Queue *queue, bool pFlag) { 
	//Queue queue;
	Queue *originalQueue = queue;

	if(queue->isEmpty()) {
		queue->enqueue(dir);
	}

	string cDir(dir);
	string output = "";
	string slash("/");

	if (rootDirFlag) {
		printf("%s\n", dir);
		rootDirFlag = false;
	}
	char cw[2048];
	while(!queue->isEmpty()) {
		//queue->printQueue("Before Dequeue");
		//dequeue is returning a pointer to the head
		char* currentDir = (char*) queue->dequeue();
		//printf("The current directory is at %p\n", currentDir);
		//queue->printQueue("After Dequeue");
		//printf("=%s\n", currentDir);	
		strcpy (cw, currentDir);

		struct dirent *entry;
		DIR *d = opendir(currentDir);

		if (d != NULL) {
			while ((entry = readdir(d)) != NULL) {
				
				//these 2 if statments are used to not show the . and .. file don't know why
				if(strcmp(entry->d_name, ".")) {
					if(strcmp(entry->d_name, "..")) {
						
						strcat(cw, "/");
						strcat(cw, entry->d_name);

						//made a copy of cw so that there isn't a pointer that points at othe elements in the queue
						char *cwCopy = (char*)malloc(2048);			//this matches with free at the end.
						//printf("cwCopy is pointing to %p\n", cwCopy);
						strcpy(cwCopy, cw);
						//string x = currentDir+ string("/") + string(entry->d_name);
						//const char *c = x.c_str();
						//We are doing this because enqueue has trouble with c++ string objects and turning it into a c styled buffer
						//strcpy(cw, x.c_str());
						
						//printf("***************%s\n", x);
						if(pFlag) {					//we need other conditionals to print out the different dash methods 
							cout << permissions(cw) << " " << cw << endl;
						}
						else {
							cout << cw << endl;
						}
						struct stat st;
						if (stat(cw, &st) == 0) {
							mode_t mode = st.st_mode;
							if(S_ISDIR(mode)) {
								
								//enqueu requires a C styled pointer to a char array	
								queue->enqueue(cwCopy);
								//queue->printQueue("After Enqueue");

								
							}
						//	else {	//this is not a directory you shouldn't see this
						//		printf("%s This is not a directory\n", c);
						//	}	
						}
						else {	//come here if the stat has an error
							perror("Error ");
						}
						
						//loop to remove last entry in char array
						char *slashPtr;
						slashPtr = strrchr(cw, '/');
						if(slashPtr != NULL) {
							*slashPtr = '\0';
						}	
						
					}
				}	
			}
			//queue->printQueue("After iterating through everything in current directory");
			if(!queue->isEmpty()) {
			//	cDir = cDir + slash + queue->head->directory; 
			//	char nextDir[1024];
			//	strncpy(nextDir, cDir.c_str(), sizeof(nextDir));
			//	nextDir[sizeof(nextDir) - 1] = 0;
				//printf("We are going to this directory %s\n", cDir);
				//cout << "We are going to this directory " << queue->getCurrentHeadData() << endl;
				//char* nextDir = cDir.c_str();
				breadthfirst((char*)queue->getCurrentHeadData(), queue, pFlag);	
			}
		}
		//printf("the currentDir is pointing at %p\n", currentDir);
		//////////////////free(currentDir);						//this free matches up with the malloc 
		closedir(d);
	}
}

char* BreadthFirst::permissions(char *file) {
	struct stat st;
	char *permArr = (char*)malloc(sizeof(char) * 10 + 1);
	if(stat(file, &st) == 0) {
		mode_t permissions = st.st_mode;
		permArr[0] = (permissions & S_IFDIR) ? 'd' : '-';
		permArr[1] = (permissions & S_IRUSR) ? 'r' : '-';
                permArr[2] = (permissions & S_IWUSR) ? 'w' : '-';
                permArr[3] = (permissions & S_IXUSR) ? 'x' : '-';
                permArr[4] = (permissions & S_IRGRP) ? 'r' : '-';
                permArr[5] = (permissions & S_IWGRP) ? 'w' : '-';
                permArr[6] = (permissions & S_IXGRP) ? 'x' : '-';
                permArr[7] = (permissions & S_IROTH) ? 'r' : '-';
                permArr[8] = (permissions & S_IWOTH) ? 'w' : '-';
                permArr[9] = (permissions & S_IXOTH) ? 'x' : '-';
                permArr[10] = '\0';
		
		return permArr;
	}
	else {
		perror("Error: ");
		exit(-1);
	}
}
