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
	int processID;		//Character to denote the process.
	int totalPages;		//Total pages for this process
	
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

// Methods
void initProcesses();
void createProcess(int, int);

// Print Methods
void printMemoryMap();

// Debug Methods
void printProcesses();

int main(){
	srand(time(NULL));	//seeding for random numbers

	// Init all the processes
	initProcesses();
	printProcesses();
	printMemoryMap();
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
	vector<segment> segs;
	segment temp;
	int totalPages = 0;
	
	if (processID == 64){
		temp.numPages = 20;
		temp.id = 0;
		segs.push_back(temp);
		totalPages = 20;
	}
	else {
		// Code segment
		temp.numPages = 2;
		temp.id = 0;
		segs.push_back(temp);
		totalPages += 2;
		
		// Stack segment
		temp.numPages = 3;
		temp.id = 1;
		segs.push_back(temp);
		totalPages += 3;
		
		// Heap segment
		temp.numPages = 5;
		temp.id = 2;
		segs.push_back(temp);
		totalPages += 5;
		
		// Subroutine segments
		int num_of_subs = rand() % 5 + 1;

		for(int i = 3; i < (3 + num_of_subs); i++)
		{
			temp.numPages = 2;
			temp.id = i;
			segs.push_back(temp);
			totalPages += 2;
		}
	}
	
	pMap.processes[index].pid = index;
	pMap.processes[index].processID = processID;
	pMap.processes[index].totalPages = totalPages;
	pMap.processes[index].segments = segs;
	pMap.numProcess++;
}

void printProcesses() {	
	process p;
	printf("\nProcID\tPID\tSegs\tPages\n");
	
	for(int i = 0; i < pMap.numProcess; i++)
	{
		p = pMap.processes[i];
		
		printf("%c\t%d\t%lu\t%d\n", p.processID, p.pid, p.segments.size(), p.totalPages);
	}
}

void printMemoryMap() {
	
	int framesUsed = 0;
	double framesUsedPercent = 0;
	int framesFree = 0;
	double framesFreePercent = 0;
	
	int pagesCount = 0;
	double pagesCountPercent = 0;
	int pagesLoaded = 0;
	double pagesLoadedPercent = 0;
	int pagesUnloaded = 0;
	double pagesUnloadedPercent = 0;
	int pagesFree = 0;
	double pagesFreePercent = 0;
	
	int processesLoaded = 0;
	double processesLoadedPercent = 0;
	int processesUnloaded = 0;
	double processesUnloadedPercent = 0;
	int processesDead = 0;
	double processesDeadPercent = 0;
	
	char output[120];
	
	memset(output, 0, 120);
	
	sprintf(output, "\nQUANTA ELAPSED: %d", pMap.currentQuanta);
	printf("%-25s\n", output);
	
	sprintf(output, "FRAMES: %df", MAX_FRAMES);
	printf("%-18s", output);
	sprintf(output, "USED: %df (%.2f%%)", framesUsed, framesUsedPercent);
	printf("%-20s", output);
	sprintf(output, "FREE: %df (%.2f%%)", framesFree, framesFreePercent);
	printf("%-20s\n", output);
	
	sprintf(output, "SWAP SPACE: %dp", MAX_PAGES);
	printf("%-18s", output);
	sprintf(output, "PAGES: %dp (%.2f%%)", pagesCount, pagesCountPercent);
	printf("%-20s", output);
	sprintf(output, "LOADED: %dp (%.2f%%)", pagesLoaded, pagesLoadedPercent);
	printf("%-22s", output);
	sprintf(output, "UNLOADED: %dp (%.2f%%)", pagesUnloaded, pagesUnloadedPercent);
	printf("%-22s", output);
	sprintf(output, "FREE: %dp (%.2f%%)", pagesFree, pagesFreePercent);
	printf("%-20s\n", output);

	sprintf(output, "PROCESSES: %d", MAX_PROCESSES);
	printf("%-18s", output);
	sprintf(output, "LOADED: %d (%.2f%%)", processesLoaded, processesLoadedPercent);
	printf("%-20s", output);
	sprintf(output, "UNLOADED: %d (%.2f%%)", processesUnloaded, processesUnloadedPercent);
	printf("%-22s", output);
	sprintf(output, "DEAD: %d (%.2f%%)", processesDead, processesDeadPercent);
	printf("%-25s\n", output);

	printf("%s\n", string(80, '=').c_str());
	
}