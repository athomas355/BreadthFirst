#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <array>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include "bfs.h"
using namespace std;

void help() {
	//Print Help Message and Exit
	
}

void symLinks() {
	//Follow Symbloic Links
}

void fileType(char *file) {
	//printf("Print information on file type");
	struct stat st;
	if(stat(file, &st) == 0) {
		printf("File Type:	");

		switch(st.st_mode & S_IFMT) {
			case S_IFBLK: printf("Block Device\n");		    break;
			case S_IFCHR:  printf("character device\n");        break;
    			case S_IFDIR:  printf("directory\n");               break;
    			case S_IFIFO:  printf("FIFO/pipe\n");               break;
    			case S_IFLNK:  printf("symlink\n");                 break;
    			case S_IFREG:  printf("regular file\n");            break;
    			case S_IFSOCK: printf("socket\n");                  break;
    			default:       printf("unknown?\n");                break;
    		}		
	}
}


char* permissions(char *file) {
	//Print permissions bits as rwxrwxrwx
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

void inode(char *file) {
	//Pring number of links to file in inode table
	struct stat st;
	if(stat(file, &st) == 0) {
		int linkCount = st.st_nlink;
		printf("%i\n", linkCount);
	}	
}

void userID(char *file) {
	//Print UID associated with file
	
	struct stat st;
	struct passwd *pwd;
	if(stat(file, &st) == 0) {
		pwd = getpwuid(st.st_uid);
		printf("File's UID: %s\n", pwd->pw_name);
	}
}

void groupID(char *file) {
	//Print group ID associated with file
	struct stat st;
        struct group *grp;
        if(stat(file, &st) == 0) {
                grp = getgrgid(st.st_gid);
                printf("File's UID: %s\n", grp->gr_name);
        }

}

void fileSize(char *file) {
	//Print size of file in bytes
	struct stat st;
	if(stat(file, &st) == 0) {
        	double size = st.st_size;
        	if (size < 1024) {
                	cout << "File Size:\t" << size << "bytes" << endl;
        	}
        	if (size >= 1024 && size < 1049000) {
			cout << "File Size:\t" << size/1000000 << "MB" << endl; 
		} 
		if (size >= 1074000000) {
			cout << "File Size:\t" << size/1000000000 << "GB" << endl;
		}
	}
}

void timeMod(char *file) {
	//Print time of last modification
	struct stat st;
	if(stat(file, &st) == 0) {
		char date[10];
		strftime(date, 10, "%m-%d-%y", localtime(&(st.st_ctime)));
		printf("%s was last modified at %s\n", file, date);
		date[0] = 0;
	}
}	

void multiple() {
	//-l command
}

int main(int argc, char* argv[]) {
	

	Queue queue;
	BreadthFirst bfs;
	bool pFlag = false;

	int option;
	while((option = getopt(argc, argv, "p")) != -1) {
		switch(option) {
			case 'p':
				pFlag = true;
				break;
			case '?':
				printf("unknown option: %c\n", optarg);
				exit(1);
		}
	}
	for(; optind < argc; optind++) {
		bfs.breadthfirst(argv[optind], &queue, pFlag);
	}

	return 0;
}


