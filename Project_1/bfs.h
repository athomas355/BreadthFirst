#ifndef BFS_H
#define BFS_H

#include <cstring>
#include <iostream>
#include <assert.h>
using namespace std;

struct node{
                        void* data;
                        struct node* next;
           };

struct Queue {
	node *head;
	node *tail;
	Queue() {
		head = tail = NULL;
	}

	void enqueue(void *datum) {			//datum is pointing to whatever the caller wants it to point to 
		node* temp;
		temp = (node*)malloc(sizeof(node));		
		temp->data = datum;			//saving the pointer to whatever that caller allocated for datum

		//Either the list is Empty or the list is populated
		assert((head == NULL && tail == NULL) || (head != NULL && tail != NULL));
		if(head == NULL && tail == NULL) {
			head = tail = temp;		
			tail->next = NULL;		//this huarantees that it it the end of the list
			return;
		}
		
		tail->next = temp;
		tail = temp;	
		tail->next = NULL;
	}

	void* dequeue() {
		if(head != NULL) {				//checks if list is empty
			node* temp = head;
			void* data_output = temp->data;		
			head = head->next;			
			free(temp);				//undoes the malloc in method enqueue
			if(head == NULL) {			//did we just pull the last element off of the list?
				tail = NULL;			//this is going to satisfy the assert in method enqueue
			}
			return data_output;		
		}
		else {						//the list is empty
			return NULL;
		}
	}

	bool isEmpty() {
		return (head == NULL && tail == NULL);
	}

	void* getCurrentHeadData() {				//return the data that is in the head of the queue. Also does not change the state of the queue
		return head->data;						
	}

	void printQueue(const char* annot) {
		//return;
		if(head == NULL) {				//this is so it will not seg fault
			return;
		}

		node* ptr = head;
		printf("The Queue:\t, ptr at Queue->Head is %p\t, %s\t", head, annot);
		while (ptr != NULL) {
			printf("%s\t, what next is pointing to %p\t", ptr->data, ptr->next);		//this is not a good solution because we don't actually know what the data datatype it is

			ptr = ptr->next;
			assert(tail->next == NULL);
			assert(tail->next != head);
			
		}

		cout << endl;
	}
};

class BreadthFirst {
	public:
		
		void breadthfirst(char* root, Queue *queue, bool pFlag, bool Lflag, bool dFlag, bool gFlag, bool iFlag, bool sFlag, bool tFlag, bool uFlag, bool lFlag);
		bool isDir(string dir);
		void printQueue(Queue *queue);
		void help();
		void symLInks();
		char* fileType(struct stat st);
		char* permissions(char* buffer, struct stat st);
		//void inode(char *file);
		char* userID(struct stat st);
		char* groupID(struct stat st);
		char* fileSize(char* buffer, struct stat st);
		char* timeMod(char* buffer, struct stat st);
		void multiple(); 
};

#endif


