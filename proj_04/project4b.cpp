// CS3243 Operating Systems
// // Summer 2014 
// // Project 4: Process Synchronization
// // Duncan Thomas and Shade Alabsa
// // Date: 23 June 2014
// // File: project4b.cpp

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <iostream>
#include <cmath>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string.h>
#include <fcntl.h>
using namespace std;

#define NUM_CHILD 4
#define LIST_SIZE 10000
#define PARENT "p"
#define CHILD "c"

pthread_mutex_t mem_lock;
sem_t *parent;
sem_t *child;

int *unsorted;
int *sorted;
int sorted_size;

int importList();
void printList();
void writeToFile();
void selectionSort(int *, int);
void merge(int *, int, int *, int, int *);
void merge_sort(int *, int);
void childWork(int, int);
void parentWork();
pid_t performFork(int, int);

int main(){
	pid_t children[NUM_CHILD], endID;
	unsorted = (int *)malloc(sizeof(int)* LIST_SIZE);
	sorted = (int *)malloc(sizeof(int)* LIST_SIZE);
	sorted_size = 0;

	int size = LIST_SIZE / NUM_CHILD;
	int start = 0;
	int status;

	pthread_mutex_init(&mem_lock, NULL);
	parent = sem_open(PARENT, O_CREAT, 0644, 0);
	if (parent == SEM_FAILED){
		cerr << "Parent semaphore failed to open" << endl;
		exit(1);
	}

	child = sem_open(CHILD, O_CREAT, 0644, 4);
	if (child == SEM_FAILED){
		cerr << "Child semaphore failed to open" << endl;
		exit(1);
	}
	//sem_init(&child, 1, NUM_CHILD);
	//sem_init(&parent, 1, 0);

	if (importList()){
		free(unsorted);
		free(sorted);
		exit(1);
	}

	for (int i = 0; i < NUM_CHILD; i++){
		children[i] = performFork(start, size);
		cout << "A child process with PID " << children[i] << " was created." << endl;
		start += size;
	}

	parentWork();

	int current_children = NUM_CHILD;
	while (current_children > 0) {
		for (int i = 0; i < NUM_CHILD; i++) {
			endID = waitpid(children[i], &status, WNOHANG | WUNTRACED);

			if (endID == -1) {            // error calling waitpid
				perror("waitpid error");
				exit(EXIT_FAILURE);
			}
			else {  // child ended
				current_children--;
				cout << "P" << getpid() << ": Process " << children[i] << " has exited." << endl;
			}
		}
	}
	
	//merge_sort(unsorted, LIST_SIZE);
	printList();
	writeToFile();

	pthread_mutex_destroy(&mem_lock);
	sem_close(child);
	sem_close(parent);
	sem_unlink(CHILD);
	sem_unlink(PARENT);
	sem_destroy(child);
	sem_destroy(parent);

	return 0;
}

void parentWork(){
	do{
		//cout << "sorted size is " << sorted_size << endl;
		//cout << getpid() << " Parent waiting on semaphore" << endl;
		sem_wait(parent);
		//cout << getpid() << "Waiting on lock" << endl;
		pthread_mutex_lock(&mem_lock);
		//cout << getpid() << " parent got lock and performing merge sort" << endl;
		sorted_size += 10;
		merge_sort(unsorted, sorted_size);
		pthread_mutex_unlock(&mem_lock);
		//cout << getpid() << " parent releasing lock" << endl;
		sem_post(child);
		//cout << "Parent notifying children" << endl;
	} while (sorted_size < 10000);
}

void printList(){
	for (int i = 0; i < LIST_SIZE; i++){
		cout << unsorted[i] << endl;
	}
}

void writeToFile(){
	ofstream file;
	file.open("sorted.txt");
	if (file.is_open()){
		for (int i = 0; i < LIST_SIZE; i++){
			file << unsorted[i] << endl;
		}
	}

	file.close();
}

int importList(){
	fstream f;
	int data, i = 0;

	f.open("numbers.txt", fstream::in);

	if (f.is_open()) {
		while (f >> data) {
			if (i < LIST_SIZE){
				unsorted[i] = data;
				i++;
			}
			else {
				cerr << "Imported list is greater than array size!" << endl;
				return 1;
			}
		}
	}

	f.close();

	return 0;
}

pid_t performFork(int start, int size) {
	pid_t pid;

	pid = fork();

	//determine if this process is the parent or the child
	if (pid < 0) { //error encountered
		fprintf(stderr, "Fork failed.\n");
		return 1;
	}
	else if (pid == 0) { //Child process
		childWork(start, size);
		//cout << getpid() << " about to exit!" << endl;
		exit(0);
	}
	else { //parent process
		/**
		do{
			cout << getpid() << " Parent waiting on semaphore" << endl;
			sem_wait(&parent);
			cout << getpid() << "Waiting on lock" << endl;
			pthread_mutex_lock(&mem_lock);
			cout << getpid() << " parent got lock and performing merge sort" << endl;
			merge_sort(unsorted, sorted_size);
			pthread_mutex_unlock(&mem_lock);
			cout << getpid() << " parent releasing lock" << endl;
			sem_post(&child);
			cout << "Parent notifying children" << endl;
		} while (sorted_size < 10000);
		*/
	}

	return pid;
}

void childWork(int start, int size){
	int temp[10];

	sem_t *parent = sem_open(PARENT, O_CREAT, 0644, 0);
	if (parent == SEM_FAILED){
		cerr << "Parent semaphore failed to open" << endl;
		exit(1);
	}

	sem_t *child = sem_open(CHILD, O_CREAT, 0644, 4);
	if (child == SEM_FAILED){
		cerr << "Child semaphore failed to open" << endl;
		exit(1);
	}

	//cout << getpid() << " start: " << start << endl << getpid() << " size: " << size << endl;
	//cout << getpid() << " has been forked!" << endl;
	//sleep(10);
	for (int i = start; i < (start+size); i += 10){
		cout << getpid() << " sorting 10" << endl;
		copy(&unsorted[i], &unsorted[(i + 10)], temp);
		selectionSort(temp, 10);
		//cout << getpid() << " Waiting on child semapohre" << endl;
		sem_wait(child);
		//cout << getpid() << " Waiting on lock" << endl;
		pthread_mutex_lock(&mem_lock);
		cout << getpid() << " got lock" << endl;
		copy(&temp[0], &temp[9], &sorted[(sorted_size)]);
		//memcpy(&sorted[sorted_size], &temp[0], sizeof(int)* 10);
		sorted_size += 10;
		pthread_mutex_unlock(&mem_lock);
		//cout << getpid() << " released lock" << endl;
		sem_post(parent);
		//cout << getpid() << " notified parent" << endl;
	}

	//cout << getpid() << " child done and returning back to the parent!" << endl;

	sem_close(child);
	sem_close(parent);
	sem_unlink(CHILD);
	sem_unlink(PARENT);
}

void merge(int *A, int a, int *B, int b, int *C) {
	int i, j, k;
	i = 0;
	j = 0;
	k = 0;
	while (i < a && j < b) {
		if (A[i] <= B[j]) {
			/* copy A[i] to C[k] and move the pointer i and k forward */
			C[k] = A[i];
			i++;
			k++;
		}
		else {
			/* copy B[j] to C[k] and move the pointer j and k forward */
			C[k] = B[j];
			j++;
			k++;
		}
	}
	/* move the remaining elements in A into C */
	while (i < a) {
		C[k] = A[i];
		i++;
		k++;
	}
	/* move the remaining elements in B into C */
	while (j < b)  {
		C[k] = B[j];
		j++;
		k++;
	}
}

void merge_sort(int *A, int n) {
	//cout << "Doing merge sort" << endl;
	int i;
	int *A1, *A2;
	int n1, n2;

	if (n < 2)
		return;   /* the array is sorted when n=1.*/

	/* divide A into two array A1 and A2 */
	n1 = n / 2;   /* the number of elements in A1 */
	n2 = n - n1;  /* the number of elements in A2 */
	A1 = (int*)malloc(sizeof(int)* n1);
	A2 = (int*)malloc(sizeof(int)* n2);

	/* move the first n/2 elements to A1 */
	for (i = 0; i < n1; i++) {
		A1[i] = A[i];
	}
	/* move the rest to A2 */
	for (i = 0; i < n2; i++) {
		A2[i] = A[i + n1];
	}
	/* recursive call */
	merge_sort(A1, n1);
	merge_sort(A2, n2);

	/* conquer */
	merge(A1, n1, A2, n2, A);
	free(A1);
	free(A2);
}

void selectionSort(int *temp, int size) {
	//cout << "Doing selection sort" << endl;
	for (int x = 0; x < size; x++){
		int minIndex = x;
		for (int y = x; y < size; y++){
			if (temp[minIndex] > temp[y])
			{
				minIndex = y;
			}
		}
		int t = temp[x];
		temp[x] = temp[minIndex];
		temp[minIndex] = t; 
	}
}