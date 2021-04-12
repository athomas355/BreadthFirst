#include "bfs.h"
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
#define DEBUG_ST 0
using namespace std;

//making a flag variable so we dont have repeats of outputs when doing recursion
bool rootDirFlag = true;

void BreadthFirst::breadthfirst(char* dir, Queue *queue, bool pFlag, bool Lflag, bool dFlag, bool gFlag, bool iFlag, bool sFlag, bool tFlag, bool uFlag, bool lFlag) { 
	//Queue queue;
	Queue *originalQueue = queue;

	if(queue->isEmpty()) {
		queue->enqueue(dir);
	}

	string cDir(dir);
	string output = "";
	string slash("/");
							
	char oa[2048];
	char rootOutput[2048];
	if (rootDirFlag) {
		struct stat rootST;
		if(stat(dir, &rootST) != 0) {
			perror("Error After stat call on root:	");
			exit(-1);
		}		
		
		char firstBuffer[1024];
		if(pFlag) {
			strcat(rootOutput, permissions(firstBuffer, rootST)); 
		}
		if(iFlag) {
			char* firstEOSP = rootOutput + strlen(rootOutput);
			sprintf(firstEOSP, "%d", rootST.st_nlink);
		}
		if(tFlag) {
			strcat(rootOutput, fileType(rootST));
		}
		if(uFlag) {
			strcat(rootOutput, userID(rootST));
		}
		if(gFlag) {
			strcat(rootOutput, groupID(rootST));
		}
		if(sFlag) {
			strcat(rootOutput, fileSize(firstBuffer, rootST));
		}
		if(dFlag) {
			strcat(rootOutput, timeMod(firstBuffer, rootST)); 
		}
		strcat(rootOutput, dir);
		printf("%s\n", rootOutput);	
		rootDirFlag = false;
		rootOutput[0] = '\0';
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
						struct stat st;
						struct stat lst;
						bool isLink, isDir;

						if(lstat(cw, &st) != 0) {
							perror("Error After lstat call: ");
							exit(-1);
						}
						else {	//is mode the protection of the link or the protectiong of the link the thing points to
							mode_t mode = st.st_mode;	//if -L is absent and cwCopy is a symlink then do *not* enque cwCopy
							isLink = S_ISLNK(mode);
							if(!isLink) {
								isDir = S_ISDIR(mode);
							}
							else{
								isDir = ((lstat(cw, &lst) >= 0) && S_ISDIR(lst.st_mode));
								if(stat(cw, &st) != 0) {
									perror("Error After stat call: ");
								exit(-1);
								}
								printf("debugger breakpoint");
							}
						}
						char buffer[1024];	//this is here so that temporary arguments can be stored safely until the function exuts
						if(pFlag) {					//we need other conditionals to print out the different dash methods 
							strcat(oa, permissions(buffer, st));
							//cout << permissions(cw) << " " << cw << endl;
						}
						if(iFlag) {
							char* eosp = oa + strlen(oa);	//eosp is the end of string pointer
							sprintf(eosp, "%d", st.st_nlink);
						}
						if(uFlag) {
							strcat(oa, userID(st));
						}
						if(gFlag) {
							strcat(oa, groupID(st));
						}
						if(sFlag) {
							strcat(oa, fileSize(buffer,st));
						}
						if(dFlag) {
							strcat(oa, timeMod(buffer,st));
						}
						if(tFlag) {
							strcat(oa, fileType(st));
						}
						strcat(oa, cw);

						printf("%s\n", oa);
						oa[0] = '\0';	
						
						
					/*	if(Lflag) {

						}*/
						
							if(isDir && isLink) {
								printf("problem with Lstat********");
							}	
							/*		This is gray encoding
 *							https://en.wikipedia.org/wiki/Gray_code
 *							https://en.wikipedia.org/wiki/Karnaugh_map
 *
 *										Lflag	!Lflag
 *							isDir	!isLink		enq	enq
 *							isDir	isLink		enq	!enq	When Lflag is false we are the symlinks not enq
 *							!isDir	isLink		!enq	!enq
 *							!isDir	!isLink		!enq	!enq
 *
 * 							if isDir is false we never enqueue
 * 							if isDir is true and isLink is false then enqueue
 * 							if isDir is true and isLink is true and Lflag is false then do not enqueue
 * 							if isDir is true and isLink is true and Lflag is true then enqueue
 *
 * 							(isDir && !isLink) || (isDir && isLink && Lflag) then enqueue
 * 							else do not enqueue
							printf("isDir = %u, and isLink = %u\n", isDir, isLink); 
							if(isDir) {		
								//enqueu requires a C styled pointer to a char array	
								queue->enqueue(cwCopy);
							}
							else if(Lflag && isLink) {
								queue->enqueue(cwCopy);
							}
							else if(isLink && !isDir)
							else {	//Just for debugging
								printf("Not enqueueing cwCopy %s\n", cwCopy);
							}					
						
 * 							*/
						
							printf("isDir = %u, and isLink = %u, Lflag = %u\t", isDir, isLink, Lflag);
							if((isDir && !isLink) || (isDir && isLink && Lflag)) {
								queue->enqueue(cwCopy);
								cout << endl;
							}
							else {
								printf("Not enqueuing\n");
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
		}
		else {		//If we are here then d is NULL or the opendir returned a NULL
			printf("d is NULL");
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
				breadthfirst((char*)queue->getCurrentHeadData(), queue, pFlag, Lflag, dFlag, gFlag, iFlag, sFlag, tFlag, uFlag, lFlag);	
			}
		
		//printf("the currentDir is pointing at %p\n", currentDir);
		//////////////////free(currentDir);						//this free matches up with the malloc 
		closedir(d);
		}
	}


char* BreadthFirst::permissions(char* buffer, struct stat st) {
		mode_t permissions = st.st_mode;
		buffer[0] = (permissions & S_IFDIR) ? 'd' : '-';
		buffer[1] = (permissions & S_IRUSR) ? 'r' : '-';
		buffer[2] = (permissions & S_IWUSR) ? 'w' : '-';
		buffer[3] = (permissions & S_IXUSR) ? 'x' : '-';
                buffer[4] = (permissions & S_IRGRP) ? 'r' : '-';
                buffer[5] = (permissions & S_IWGRP) ? 'w' : '-';
                buffer[6] = (permissions & S_IXGRP) ? 'x' : '-';
                buffer[7] = (permissions & S_IROTH) ? 'r' : '-';
                buffer[8] = (permissions & S_IWOTH) ? 'w' : '-';
                buffer[9] = (permissions & S_IXOTH) ? 'x' : '-';
		buffer[10] = '\t';
                buffer[11] = '\0';
		
		return buffer;
}

char* BreadthFirst::fileType(struct stat st) {
#if DEBUG_ST		
		int other = st.st_mode & S_IFMT;
		printf("Other = %u %o\t", other, other);
		if(st.st_mode & S_IFLNK) {
			printf("It is a symlink\t");
		}
		else {
			printf("It is not a symlink\t");
		}
#endif
		switch(st.st_mode & S_IFMT) {
                        case S_IFBLK: 	return((char*)"Block Device\t");				break;
                        case S_IFCHR: 	return((char*)"Character Device\t");             	break;
                        case S_IFDIR: 	return((char*)"Directory\t");             		break;
                        case S_IFIFO: 	return((char*)"FIFO/pipe\t");             		break;
                        case S_IFREG: 	return((char*)"Regular File\t");             		break;
                        case S_IFSOCK: 	return((char*)"Socket\t");             			break;
                        case S_IFLNK: 	return((char*)"SymLink\t");       break;//This is different becuase we suspect that there is a bug in one of the system libraries
			default:      	return((char*)"unkown?\t");				break;	
	}
}




char* BreadthFirst::userID(struct stat st) {
	struct passwd *pwd;
	pwd = getpwuid(st.st_uid);
	return strcat(pwd->pw_name, "\t");
}

char* BreadthFirst::groupID(struct stat st) {
	struct group *grp;
	grp = getgrgid(st.st_gid);
	return strcat(grp->gr_name, "\t");
}

char* BreadthFirst::fileSize(char* buffer, struct stat st) {
		int size = st.st_size;
		if(size < 1024) {
			sprintf(buffer, "%d\t", size);				
		}
		else if(size < 1048576) {
			sprintf(buffer, "%dK\t", size/1024);
		}
		else if(size < 1073741824) {
			sprintf(buffer, "%dM\t", size/1048576);	
		} 
		else {
			sprintf(buffer, "%dG\t", size/1073741824);
		}
		
		return buffer;
}

char* BreadthFirst::timeMod(char* buffer, struct stat st){
		strftime(buffer, 10, "%m-%d-%y\t", localtime(&(st.st_mtime)));
		return buffer;
}
