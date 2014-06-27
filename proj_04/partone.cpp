// CS3243 Operating Systems
// Summer 2014 
// Project 4: Process Synchronization
// Duncan Thomas and Shade Alabsa
// Date: 23 June 2014
// File: partone.cpp

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <iostream>
#include <cmath>
#include <sys/types.h>

using namespace std;

#define MAX_ELEMENTS 10

struct node {
	int data;
	node *next;
};

struct queue{
	int size;
	int capacity;
	node *buffer;
	node *head;
	node *tail;
};

queue *q;
//pthread_mutex_t lock;

void push(int);
void pop();
void printQ();
//void doWork();
//int insert_item(int *);
//int remove_item(int *);

int main(){
	srand(time(NULL));

	q = (queue *)malloc(sizeof(queue));
	q->buffer = (node *)malloc(sizeof(node)*MAX_ELEMENTS);
	q->size = 0;
	q->capacity = MAX_ELEMENTS;

	q->head = NULL;
	q->tail = NULL;

	for (int i = 0; i < MAX_ELEMENTS+1; i++){
		int r = rand();
		printf("random number is %d\n", r);
		push(r);
	}

	printQ();

	for (int i = 0; i < MAX_ELEMENTS; i++){
		pop();
	}

	pop();
	
	printQ();

	return 0;
}

void push(int data){
	if (q->size != q->capacity || q->size == 0) {
		node *temp = (node *)malloc(sizeof(node));
		temp->data = data;
		if (q->size == 0){
			q->head = temp;
			q->tail = temp;
		} else {
			q->tail->next = temp;
			q->tail = temp;
		}

		q->size++;
	} else {
		printf("The queue is full!\n");
	}
}

void pop(){
	if (q->size != 0){
		node *temp = q->head;
		q->size--;
		q->head = temp->next;
		free(temp);
	} else {
		printf("The queue is empty!\n");
	}
}

void printQ(){
	int i;

	printf("Queue Capacity %d\nQueue Size %d\n", q->capacity, q->size);
	
	node *temp = q->head;
	
	for (i = 0; i < q->size; i++) {
		printf("%d->", temp->data);
		temp = temp->next;
	}

	printf("\n");
}

/**
void consumer(){
	if (buffer[0]){

	}
}

void producer(){
	int num = (rand() % 200 + 1);
	if ((rand() % 20001 - 10000) % 3 == 0){
		if (insert_item(num)){
			cout << "Error inserting into buffer!" << endl;
		}
	}
}

int insert_item(int data){

}

int remove_item(){

}
*/