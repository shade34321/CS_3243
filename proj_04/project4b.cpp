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
using namespace std;

#define NUM_CHILD 4
#define LIST_SIZE 10000

pthread_mutex_t lock;

int *unsorted;
int *sorted;

int importList();
void printList();
void selectionSort(int, int);
void merge(int *, int, int *, int, int *);
void merge_sort(int *, int);
pid_t performFork(int, int);

int main(){
	pid_t children[NUM_CHILD], endID;
	unsorted = (int *)malloc(sizeof(int)* LIST_SIZE);
	sorted = (int *)malloc(sizeof(int)* LIST_SIZE);
	int size = LIST_SIZE / NUM_CHILD;
	int start = 0;
	int status;

	pthread_mutex_init(&lock, NULL);

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
	
	merge_sort(unsorted, LIST_SIZE);
	printList();

	pthread_mutex_destroy(&lock);

	return 0;
}

void printList(){
	for (int i = 0; i < LIST_SIZE; i++){
		cout << unsorted[i] << endl;
	}
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
		selectionSort(start, size);
		exit(0);
	}
	else { //parent process
		//We handle the parent's busy work in main
		//If we did it here then the parent would be doing busy work with one child on 4 seperate occasions and not with 4 children at once.
	}

	return pid;
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

void selectionSort(int start, int size) {
	int last = (start + size)-1;

	for (int x = start; x < last; x++){
		int minIndex = x;
		for (int y = x; y < last; y++){
			if (unsorted[minIndex] > unsorted[y])
			{
				minIndex = y;
			}
		}
		int temp = unsorted[x];
		unsorted[x] = unsorted[minIndex];
		unsorted[minIndex] = temp; 
	}
}