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
Node * get_nth_element(List *, int);
void quick_sort(List *, int);
Node * get_nth_element(List *, int);

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

	int a[] = { 4, 65, 2, -31, 0, 99, 2, 83, 782, 1 };
	int i;

	for (i = 0; i < 10; i++) {
        if (insert(&list, list.head, a[i])) {
			cout << "ERROR: Error inserting element " << i << " into list." << endl;
			return -1;
		}
	}

	print(&list);

	quick_sort(&list, list.size);

	cout << "list.size: " << list.size << endl;

	Node *t;
	t = get_nth_element(&list, 2);

	cout << "The second element is: " << t->data << endl;

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

Node * get_nth_element(List *list, int elementNum) {
	int i;
	Node *t;
	t = list->head;

	for (i = 0; i < elementNum; i++){
		if (t->next == NULL){
			t = NULL;
			return t;
		}

		t = t->next;
	}

	return t;
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

void quick_sort(List *list, int n) {
	if (list->size < 2) {
		return; //Nothing to do
	}

	Node *p = get_nth_element(list, (n / 2));
	Node *left = list->head;
	Node *right = list->tail;

	while (left->data < right->data) {
		if (left->data < p->data) {
			left = left->next;
			continue;
		}

		if (right->data > p->data) {
			right = right->prev;
			continue;
		}

		int t = left->data;
		left->data = right->data;
		left = left->next;
		right->data = t;
		right = right->prev;
	}

	quick_sort(list, (n / 2 - 1));
	quick_sort(list, (n / 2 + 1));
}
