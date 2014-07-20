//CS3242 Operating Systems
//Summer 2014
//Project 5: Swapping and Paging, Part 2
//Shade Alabsa, Duncan Thomas, Mathew Trussell
//Date: 6 July 2014
//File: parttwo.cpp

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string.h>

using namespace std;

#define MAX_PROCESSES 		52 // This will not ever change
#define PROCESS_COUNT 		23 // useful when debugging to limit # of procs
#define MIN_DEATH_INTERVAL 	20
#define MAX_DEATH_INTERVAL 	300
#define MAX_FRAMES 			280
#define MAX_PAGES 			720
#define SHIFT_INTERVAL 		10
#define PRINT_INTERVAL 		500 	// # of cpu quanta between memory map printouts
#define MAX_QUANTA 			50000 	// # quanta to run before ending simulation.
#define SLEEP_LENGTH 		2500 	// Used with the usleep()to slow down sim between
									// cycles (makes reading screen in real-time easier!)					
struct segment{
	int id;				// ID for this segment
	int numPages;		// Number of pages this seg should have
	vector<int> pages;  // List of pages owned by this segment
};

struct process{
	int pid;			//PID of the process
	int processID;		//character to denote the process.
	
	vector<segment> segments; // List of the segments of this process
};

struct processMap{
	int memory[MAX_PAGES];				//Total pages of memory
	process processes[MAX_PROCESSES];	//array of process we can have.
	int numProcess;
	int pagesUsed;
	int currentQuanta;
};

processMap pMap;

void initProcesses();
void createProcess(int, int);
void printProcesses();

int main(){
	srand(time(NULL));	//seeding for random numbers

	// Init all the processes
	initProcesses();
	printProcesses();
}

void initProcesses(){
	int i;
	int j = 1;

	createProcess(0, 64); //create kernel process

	for (i = 65; i < 87; i++){	
			createProcess(j, i);
			j++;
	}
}

void createProcess(int index, int processID){
	printf("\nCreating process %c", (char)processID);
	vector<segment> segs;
	segment temp;
	
	if (processID == 64){
		temp.numPages = 20;
		temp.id = 0;
		segs.push_back(temp);
	}
	else {
		// Code segment
		temp.numPages = 2;
		temp.id = 0;
		segs.push_back(temp);
		
		// Stack segment
		temp.numPages = 3;
		temp.id = 1;
		segs.push_back(temp);
		
		// Heap segment
		temp.numPages = 5;
		temp.id = 2;
		segs.push_back(temp);
		
		// Subroutine segments
		int num_of_subs = rand() % 5 + 1;
		printf("\nnum subs: %d\n", num_of_subs);
		for(int i = 3; i < (3 + num_of_subs); i++)
		{
			temp.numPages = 2;
			temp.id = i;
			segs.push_back(temp);
		}
	}
	
	pMap.processes[index].pid = index;
	pMap.processes[index].processID = processID;
	pMap.processes[index].segments = segs;
	pMap.numProcess++;
}

void printProcesses() {	
	process p;
	for(int i = 0; i < pMap.numProcess; i++)
	{
		p = pMap.processes[i];
		
		printf("\nProcessID: %c", p.processID);
		printf("\nPID: %d", p.pid);
		printf("\nSEGMENTS: %lu\n", p.segments.size());
	}
}