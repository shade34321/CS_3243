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

int buffer[10];
//pthread_mutex_t lock;

void doWork();
int insert_item(int *);
int remove_item(int *);

int main(){
	srand(time(NULL));


	return 0;
}

void consumer(){
	int num = (rand() % 200 + 1);
	if ((rand() % 20001 - 10000) % 3 == 0){
		if (insert_item(num)){
			cout << "Error inserting into buffer!" << endl;
		}
	}
}

void producer(){

}

int insert_item(int data){

}

int remove_item(){

}