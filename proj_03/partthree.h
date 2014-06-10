// CS3243 Operating Systems
// Summer 2014 
// Project 3: Forks and Threads
// Matthew Tinney and Shade Alabsa
// Date: 10 June 2014
// File: partthree.h

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

typedef struct node {
	int data;
	struct node *next;
};

int getLenth(struct node*);
boolean push(struct node*, int);
boolean insertAtBeginning(struct node**, int);
boolean remove(struct node*, struct node*);
void importList(struct node*, string&);
void print(struct node*);