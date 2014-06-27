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
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

using namespace std;

#define MAX_ELEMENTS 1000

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

pthread_mutex_t lock;
sem_t full;
sem_t empty;


void push(queue *, int);
void pop(queue *);
void printQ(queue *);
void * producer(void *);
void * consumer(void *);
void emptyQ(queue *);

int main(){
	/**
	if (argc != 4){
		cerr << "Incorrect Usage!" << endl;
		cerr << "You need to supply the number of producer threads, consumer threads, and how long to sleep for" << endl;

	}
	*/
	pthread_t t1;
	pthread_t t2;
	
	queue *q;

	void *ret;

	srand(time(NULL));
		
	q = (queue *)malloc(sizeof(queue));
	q->size = 0;
	q->capacity = MAX_ELEMENTS;

	q->head = NULL;
	q->tail = NULL;

	pthread_mutex_init(&lock, NULL);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, 1000);

	/**
	for (int i = 0; i < MAX_ELEMENTS+1; i++){
		int r = rand();
		printf("random number is %d\n", r);
		push(q, r);
	}

	printQ(q);

	for (int i = 0; i < MAX_ELEMENTS; i++){
		pop(q);
	}

	pop(q);
	
	printQ(q);
	*/

	pthread_create(&t1, NULL, &producer, (void *)q);
	sleep(2);
	pthread_create(&t2, NULL, &consumer, (void *)q);

	pthread_join(t1, &ret);
	pthread_join(t2, &ret);

	printQ(q);

	pthread_mutex_destroy(&lock);
	sem_destroy(&full);
	sem_destroy(&empty);
	emptyQ(q);

	return 0;
}

void emptyQ(queue * q){
	for (int i = 0; i < q->size; i++){
		pop(q);
	}

	free(q);
}

void push(queue *q, int data){
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

		printf("%d!\n", q->tail->data);
		q->size++;
	} else {
		printf("The queue is full!\n");
	}
}

void pop(queue *q){
	if (q->size != 0){
		node *temp = q->head;
		q->size--;
		q->head = temp->next;
		printf("%d!\n", temp->data);
		free(temp);
	} else {
		printf("The queue is empty!\n");
	}
}

void printQ(queue *q){
	int i;

	printf("Queue Capacity %d\nQueue Size %d\n", q->capacity, q->size);
	
	node *temp = q->head;
	
	for (i = 0; i < q->size; i++) {
		printf("%d->", temp->data);
		temp = temp->next;
	}

	printf("\n");
}

void * consumer(void *s){
	queue *q = (queue *)s;
	int i = 0;
	while (i < 50){
		//if ((rand() % 20001 - 10000) % 3 == 0) {
		if (1) {
			wait(&empty);
			pthread_mutex_lock(&lock);
			printf("------> [Process %d] consuming ", pthread_self());
			pop(q);
			pthread_mutex_unlock(&lock);
			signal(&full);
			sleep(1);
		} else {
			sleep((rand() % 10 + 1));
		}
		i++;
		printf("On iteration %d\n", i);
	}

	pthread_exit(NULL);
}

void * producer(void *s){
	queue *q = (queue *)s;
	int i = 0;
	while (i < 50) {
		int num = (rand() % 200 + 1);
		//if ((rand() % 20001 - 10000) % 7 == 0) {
		if (1) {
			wait(&full);
			pthread_mutex_lock(&lock);
			printf("[Process %d] producing ", pthread_self());
			push(q, num);
			pthread_mutex_unlock(&lock);
			signal(&full);
			sleep(1);
		} else {
			sleep((rand() % 10 + 1));
		}
		i++;
		printf("On iteration %d\n", i);
	}

	pthread_exit(NULL);
}