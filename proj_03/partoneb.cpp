// CS3243 Operating Systems
// Summer 2014 
// Project 3: Forks and Threads
// Matthew Tinney and Shade Alabsa
// Date: 10 June 2014
// File: partone.cpp

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;

//Number of children we would like
#define NUM_CHILD 4

pid_t performFork();
void simulateBusyWork(char);

int main() {
	pid_t children[NUM_CHILD];
	int i;

	//Generate Random number seed
	srand(time(NULL));

	//Create the children processes.
	for (i = 0; i < NUM_CHILD; i++){
		cout << "PERFORMING FORK " << i << endl;
		children[i] = performFork();		
	}


	simulateBusyWork('P');

	for (i = 0; i < NUM_CHILD; i++) {
		wait(NULL);
	}

    return 0;
}

pid_t performFork() {
	pid_t pid;
	
	pid = fork();

	//determine if this process is the parent or the child
	if (pid < 0) { //error encountered
		fprintf(stderr, "Fork failed.\n");
		return 1;
	} else if (pid == 0 ) { //Child process
		simulateBusyWork('C');
		exit(0);
	} else { //parent process
		//We handle the parent's busy work in main
		//If we did it here then the parent would be doing busy work with one child on 4 seperate occasions and not with 4 children at once.
	}

	return pid;
}

void simulateBusyWork(char ch) {
	int i, j;
    long outerIter = rand() % 9900000 + 100000;
    long innerIter = rand() % 9900000 + 100000;

    cout << "OuterIter: " << outerIter << endl;
    cout << "InnerIter: " << innerIter << endl;

	for (i = 0; i < outerIter; i++) {
		for (j = 0; j < innerIter; j++){
			; //does nothing but waste time
		}
		//only print out each 1000 - it got a bit cumbersome.
        if((i%1000) == 0) {
    		cout << ch << getpid() << ": " << i << endl;
        }
	}

	cout << "Process " << getpid() << " finished its work." << endl;
}
