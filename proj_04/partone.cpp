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
#include <signal.h>
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
	int sleep;
};

pthread_mutex_t lock;
sem_t full;
sem_t empty;
int done;


void push(queue *, int);
void pop(queue *);
void printQ(queue *);
void * producer(void *);
void * consumer(void *);
void emptyQ(queue *);

int main(int argc, char* argv[]){
	if (argc != 4 && argc != 5){
		cerr << "You supplied " << argc << " arguments and we need 5" << endl;
		cerr << "Incorrect Usage!" << endl;
		cerr << "You need to supply the number of producer threads, consumer threads, and how long to sleep for like so" << endl;
		cerr << "You can also supply how long you want the main thread to sleep before exiting by adding a number to the end" << endl;
		cerr << "./partone numProducers numConsumers secondsToSleep mainSleep" << endl;		
		exit(1);
	}

	int numProd = atoi(argv[1]);
	int numCons = atoi(argv[2]);
	int ssleep = atoi(argv[3]);
	int msleep = atoi(argv[3]);
	int quit;

	if (argc == 5){
		msleep = atoi(argv[4]);
		quit = 1;
	}

	done = 1; //condition variable to quit

	pthread_t prod[numProd];
	pthread_t cons[numCons];
	
	queue *q;

	void *ret;

	srand(time(NULL));
		
	q = (queue *)malloc(sizeof(queue));
	q->size = 0;
	q->capacity = MAX_ELEMENTS;
	q->sleep = ssleep;

	q->head = NULL;
	q->tail = NULL;

	pthread_mutex_init(&lock, NULL);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, 1000);

	for (int i = 0; i < numProd; i++){
		pthread_create(&prod[i], NULL, &producer, (void *)q);
	}

	sleep(2);

	for (int i = 0; i < numCons; i++){
		pthread_create(&cons[i], NULL, &producer, (void *)q);
	}

	if (quit){
		sleep(msleep);

		done = 0;

		for (int i = 0; i < numProd; i++) {
			pthread_join(prod[i], &ret);
		}

		for (int i = 0; i < numProd; i++) {
			pthread_join(cons[i], &ret);
		}
	}

	printQ(q);

	pthread_mutex_destroy(&lock);
	sem_destroy(&full);
	sem_destroy(&empty);
	emptyQ(q);

	return 0;
}

void emptyQ(queue * q){
	cout << "Emptying out the queue so I can free it!" << endl;
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

	printf("Current queue data looks like!\n");
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
	do{
		sem_wait(&full);
		pthread_mutex_lock(&lock);
		printf("------> [Process %d] consuming ", pthread_self());
		pop(q);
		pthread_mutex_unlock(&lock);
		sem_post(&empty);
		//if ((rand() % 10 + 1) % 2 == 0) {
			sleep(q->sleep);
		//}
	} while (done);

	pthread_exit(NULL);
}

void * producer(void *s){
	queue *q = (queue *)s;
	int i = 0;
	do {
		int num = (rand() % 200 + 1);
		sem_wait(&empty);
		pthread_mutex_lock(&lock);
		printf("[Process %d] producing ", pthread_self());
		push(q, num);
		pthread_mutex_unlock(&lock);
		sem_post(&full);
		//if ((rand() % 10 + 1) % 2 == 1) {
			sleep(q->sleep);
		//}
	} while (done);

	pthread_exit(NULL);
}