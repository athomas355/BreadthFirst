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


int main(int argc, char* argv[]) {
	

	Queue queue;
	BreadthFirst bfs;
	bool pFlag = false;
	bool LFlag = false;
	bool dFlag = false;
	bool gFlag = false;
	bool iFlag = false;
	bool sFlag = false;
	bool tFlag = false;
	bool uFlag = false;
	bool lFlag = false;
	bool hFlag = false;	
	
	int option;
	while((option = getopt(argc, argv, "hLdgipstul")) != -1) {
		
		switch(option) {
			case 'h':
				//call help function	
				printf("bt [-h -L -d -g -i -p -s -t -u -l] [directory name]\n");
				exit(0);
			case 'L':
				LFlag = true;
				break;
			case 'd':
				dFlag = true;
				break;
			case 'g':
				gFlag = true;
				break;
			case 'i':
				iFlag = true;
				break;
			case 's':
				sFlag = true;
				break;
			case 't':
				tFlag = true;
				break;
			case 'u':
				uFlag = true;
				break;
			case 'l':
				lFlag = true;
				break;
			case 'p':
				pFlag = true;
				break;
			case '?':
				printf("unknown option: %c\n", optarg);
				exit(1);
		}
	}
	
	//if -l is chosen
	if(lFlag) {
		tFlag = true;
		pFlag = true;
		uFlag = true;
		iFlag = true;
		gFlag = true;
		sFlag = true;
	}
	for(; optind < argc; optind++) {
		bfs.breadthfirst(argv[optind], &queue, pFlag, LFlag, dFlag, gFlag, iFlag, sFlag, tFlag, uFlag, lFlag);
	}

	return 0;
}


