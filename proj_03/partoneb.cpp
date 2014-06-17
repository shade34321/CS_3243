// CS3243 Operating Systems
// Summer 2014 
// Project 3: Forks and Threads
// Matthew Tinney and Shade Alabsa
// Date: 10 June 2014
// File: partoneb.cpp

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

//Number of children we would like
#define NUM_CHILD 4

pid_t performFork();
void simulateBusyWork(char);

int main() {
	pid_t children[NUM_CHILD], endID;
	int i, status;

	//Create the children processes.
	for (i = 0; i < NUM_CHILD; i++){
		children[i] = performFork();
		cout << "A child process with PID " << children[i] << " was created." << endl;
	}


	simulateBusyWork('P');
	int current_children = NUM_CHILD;
	while (current_children > 0) {
		for (i = 0; i < NUM_CHILD; i++) {
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
	return 0;
}

pid_t performFork() {
	pid_t pid;

	pid = fork();

	//determine if this process is the parent or the child
	if (pid < 0) { //error encountered
		fprintf(stderr, "Fork failed.\n");
		return 1;
	}
	else if (pid == 0) { //Child process
		simulateBusyWork('C');
		exit(0);
	}
	else { //parent process
		//We handle the parent's busy work in main
		//If we did it here then the parent would be doing busy work with one child on 4 seperate occasions and not with 4 children at once.
	}

	return pid;
}

void simulateBusyWork(char ch) {
	int i, j;
    long outerIter = rand() % 9900000 + 100000;
    long innerIter = rand() % 9900000 + 100000;

	for (i = 0; i < outerIter; i++) {
		for (j = 0; j < innerIter; j++){
			; //does nothing but waste time
		}
		cout << ch << getpid() << ": " << i << endl;
	}

	cout << "Process " << getpid() << " finished its work." << endl;
}
