// CS3243 Operating Systems
// Summer 2014 
// Project 3: Forks and Threads
// Matthew Tinney and Shade Alabsa
// Date: 10 June 2014
// File: partone.cpp

#include "partone_h"

int main() {
	performFork();
}

pid_t performFork() {
	pid_t pid;

	//fork a process

	//determine if this process is the parent or the child
	if (pid < 0) { //error encountered
		fprintf(stderr, "Fork failed.\n");
		return 1;
	}
	else if (pid == 0) { //Child process
		simulateBusyWork('C');
	} else { //parent process

	}

	return pid;
}

void simulateBusyWork(char ch) {
	int i, j;

	for(i = 0; i < 5; i++) {
		for (j = 0; j < 1000000; j++){
			; //does nothing but waste time
		}
		cout << i << endl;
	}

	cout << "Process finished its work." << endl;
}