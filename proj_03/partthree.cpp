// CS3243 Operating Systems
// Summer 2014 
// Project 3: Forks and Threads
// Matthew Tinney and Shade Alabsa
// Date: 10 June 2014
// File: partthree.cpp

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

using namespace std;

struct Node {
	int data;
	Node *next;
	Node *prev;
};

struct List {
	int size;
	Node *head;
	Node *tail;
};

void remove(List *);
void print(List *);
int insert(List *, Node *, int);
int importList(List *, Node *, string);

int main() {

	List list;
	
	list.size = 0;
	list.head = NULL;
	list.tail = NULL;

	/*
	if (importList(&list, "numbers250.txt")){
		cout << "ERROR: Error trying to import list." << endl;
	}
	*/

	int i;
	for (i = 0; i < 6; i++) {
		if(i == 3)
            i++;
        if (insert(&list, list.head, i)) {
			cout << "ERROR: Error inserting element " << i << " into list." << endl;
			return -1;
		}
	}

	Node *t;
	t = list.head;

	for (i = 0; i < 6; i++){
		if (i == -1) {
			if (insert(&list, t, i)) {
				cout << "ERROR: Error inserting element " << i << " into list." << endl;
				return -1;
			}
		}
		//t = t->next;
	}

	print(&list);

	cout << "list.size: " << list.size << endl;

	remove(&list);
	
	return 0;
}

int importList(List *list, string file){
	int data;
	fstream f;
	
	f.open(file.c_str(), fstream::in);

	if (f.is_open()) {
		while (f >> data) {
			if (insert(list, list->head, data)) {
				return -1;
			}
		}
	}

	return 0;
}

int insert(List *list, Node *n, int data) {
	Node *t;

	if ((t = (Node *)malloc(sizeof(Node))) == NULL) {
		return -1;
	}

	t->data = data;
	t->next = NULL;
	t->prev = NULL;

	if (n == list->head || n == NULL) { //n == NULL) {
		if (list->head == NULL){
			list->head = t;
			list->tail = t;
			list->size++;
		}
		else {
			t->next = list->head;
			list->head->prev = t;
			list->head = t;
			list->size++;
		}

		/**For inserting at the back and having an empty list
		if (list->head == NULL) {
		list->head = t;
		list->head->next = list->tail;
		list->tail = t;
		list->tail->prev = list->head;
		list->size++;
		}
		else {
		t->prev = list->tail;
		list->tail = list->tail->next = t;

		list->size++;
		}*/

	} else if (n == list->tail) {
		  t->prev = list->tail;
		  list->tail = list->tail->next = t;
	} else {
		  t->next = n; //Set the new nodes next to the element we're inserting before
		  t->prev = n->prev; //Set the new nodes previous to the element we're inserting before previous
		  n->prev->next = t; //set n's previous node next element to the new element we're inserting
		  n->prev = t; //set n's previous to the new element we're inserting.
	  }
	return 0;
}

void print(List *list) {
	Node *t;
	int i;

	t = list->head;

	for (i = 0; i < list->size; i++) {
		cout << t->data << "<=> ";
		t = t->next;
	}
	cout << endl;

	t = list->tail;

	for (i = 0; i < list->size; i++) {
		cout << t->data << "<=> ";
		t = t->prev;
	}
	cout << endl;
}

void remove(List *list){
	Node *t;

	while (list->head != NULL) {
		t = list->head; //Get the head
		list->head = list->head->next; //Get Next head
		free(t);
		list->size--;
	}
}
