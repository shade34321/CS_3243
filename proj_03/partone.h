// CS3243 Operating Systems
// Summer 2014 
// Project 3: Forks and Threads
// Matthew Tinney and Shade Alabsa
// Date: 10 June 2014
// File: partone.h

#include <sys/types.h>
#include <unistd.h>
#include <iostream>
using namespace std;

pid_t performFork();
void simulateBusyWork(char);