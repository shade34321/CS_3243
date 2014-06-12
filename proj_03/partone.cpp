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
#include <iostream>
using namespace std;

#define NUM_CHILD 4

pid_t performFork();
void simulateBusyWork(char);

int main() {
	pid_t children[NUM_CHILD];
	int i;

	for (i = 0; i < NUM_CHILD; i++){
		cout << "PERFORMING FORK " << i << endl;
		children[i] = performFork();
		
	}

	for (i = 0; i < NUM_CHILD; i++) {
		wait(NULL);
	}
}

pid_t performFork() {
	pid_t pid;
	pid_t ppid = getpid();

	//if (ppid ==)
	//fork a process
	pid = fork();

	//determine if this process is the parent or the child
	if (pid < 0) { //error encountered
		fprintf(stderr, "Fork failed.\n");
		return 1;
	} else if (pid == 0 ) { //Child process
		(void)setsid();
		//(void)umask((mode_t)066);
		// Child Process continues further execution...
		// Become Process Group Leader
		

		simulateBusyWork('C');
	} else { //parent process
		simulateBusyWork('P');
	}

	return pid;
}

void simulateBusyWork(char ch) {
	int i, j;

	for(i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++){
			; //does nothing but waste time
		}
		cout << ch << getpid() << ": " << i << endl;
	}

	cout << "Process " << getpid() << " finished its work." << endl;
}