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
<<<<<<< HEAD
									// cycles (makes reading screen in real-time easier!)

#define MAX_PAGE_PER_PROC	20		// The max number of pages per process
				
struct page{
	int frame_id;
	char valid;
	// int reference;
=======
									// cycles (makes reading screen in real-time easier!)					
struct segment{
	int id;				// ID for this segment
	int numPages;		// Number of pages this seg should have
	vector<int> pages;  // List of pages owned by this segment
>>>>>>> master
};

struct process{
	int pid;			//PID of the process
	int processID;		//Character to denote the process.
	int totalPages;		//Total pages for this process
<<<<<<< HEAD
	int lifetime;		//Number of quanta this process lives for
	int state;	
	
	page pageTable[MAX_PAGE_PER_PROC]; 		
};

struct processMap{
	int pages[MAX_PAGES];				//Total pages of memory
=======
	
	vector<segment> segments; // List of the segments of this process
};

struct processMap{
	int memory[MAX_PAGES];				//Total pages of memory
>>>>>>> master
	process processes[MAX_PROCESSES];	//array of process we can have.
	int numProcess;
	int pagesUsed;
	int currentQuanta;
};

<<<<<<< HEAD
enum p_State {
	ALIVE = 0,
	DEAD = 1,
	REMOVED = 2
};

processMap pMap;


// Methods
void initProcesses();
void createProcess(int, int);
int getLifeTime();
int countProcWithState(int);

// Print Methods
void printAllTheThings(); 	// prints all the things//TODO
void printStats();  		// prints only the memory stats
void printFrames();       	// prints only the memory frames//TODO
void printSwapSpace();    	// prints only the swap space pages//TODO
void printPageTable();    	// prints only the page table//TODO

// Debug Methods
=======
processMap pMap;

void initProcesses();
void createProcess(int, int);
>>>>>>> master
void printProcesses();

int main(){
	srand(time(NULL));	//seeding for random numbers

	// Init all the processes
	initProcesses();
	printProcesses();
<<<<<<< HEAD
	printStats();
	printFrames();
	printSwapSpace();
}

int getLifeTime(){
	return rand() % MAX_DEATH_INTERVAL + MIN_DEATH_INTERVAL;
}

int countProcWithState(int state) {
	int count = 0;
	
	for(int i = 0; i < PROCESS_COUNT; i++) {
		if (pMap.processes[i].state == state) {
			count++;
		}
	}
	
	return count;
=======
>>>>>>> master
}

void initProcesses(){
	int i;
	int j = 1;

	createProcess(0, 64); //create kernel process

	for (i = 65; i < 87; i++){	
<<<<<<< HEAD
			createProcess(j, i); //create A-V
=======
			createProcess(j, i);
>>>>>>> master
			j++;
	}
}

void createProcess(int index, int processID){
<<<<<<< HEAD
	int totalPages = 0;
	int life;
	
	if (processID == 64){
		totalPages = 20;
		life = -1;
	}
	else {
		life = getLifeTime();
		
		// Code segment
		totalPages += 2;
		
		// Stack segment
		totalPages += 3;
		
		// Heap segment
=======
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
>>>>>>> master
		totalPages += 5;
		
		// Subroutine segments
		int num_of_subs = rand() % 5 + 1;

		for(int i = 3; i < (3 + num_of_subs); i++)
		{
<<<<<<< HEAD
=======
			temp.numPages = 2;
			temp.id = i;
			segs.push_back(temp);
>>>>>>> master
			totalPages += 2;
		}
	}
	
	pMap.processes[index].pid = index;
	pMap.processes[index].processID = processID;
	pMap.processes[index].totalPages = totalPages;
<<<<<<< HEAD
	pMap.processes[index].state = ((processID == 64) ? (ALIVE) : (REMOVED));
	pMap.processes[index].lifetime = life;	
=======
	pMap.processes[index].segments = segs;
	pMap.numProcess++;
>>>>>>> master
}

void printProcesses() {	
	process p;
<<<<<<< HEAD
	printf("\nProcID\tPID\tPages\tState\tLife\n");
	
	for(int i = 0; i < PROCESS_COUNT; i++)
	{
		p = pMap.processes[i];
		std::string st;
		switch(p.state) {
			case ALIVE:
			st = "ALIVE";
			break;
			case DEAD:
			st = "DEAD";
			break;
			case REMOVED:
			default:
			st = "REMOVED";
			break;
		}
		
		printf("%c\t%d\t%d\t%s\t%d\n", p.processID, p.pid, p.totalPages, st.c_str(), p.lifetime);
	}
}

void printStats() {
	
	int framesUsed = pMap.pagesUsed;
	double framesUsedPercent = framesUsed / MAX_FRAMES;
	int framesFree = MAX_FRAMES - framesUsed;
	double framesFreePercent = framesFree / MAX_FRAMES;
	
	int pagesCount = 0;
	double pagesCountPercent = 0;
	int pagesLoaded = 0;
	double pagesLoadedPercent = 0;
	int pagesUnloaded = 0;
	double pagesUnloadedPercent = 0;
	int pagesFree = 0;
	double pagesFreePercent = 0;
	
	int processesLoaded = pMap.numProcess;
	double processesLoadedPercent = processesLoaded / MAX_PROCESSES;
	int processesUnloaded = MAX_PROCESSES - processesLoaded;
	double processesUnloadedPercent = processesUnloaded / MAX_PROCESSES;
	int processesDead = countProcWithState(DEAD);
	double processesDeadPercent = processesDead / MAX_PROCESSES;;
	
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

	printf("%s\n", string(120, '=').c_str());	
}

void printFrames() {
	const int OUT_MAX = 120;
	const int NUM_PER_LINE = 12;
	const int SEG_PER_LINE = 6;
	
	char output[OUT_MAX];
	memset(output, 0, OUT_MAX);

		printf("\nPhysical Memory (Frames)\n");
		printf("%s\n", string(OUT_MAX, '=').c_str());

		for (int i = 0; i < MAX_FRAMES; i += (OUT_MAX/2)){

			// print the top numbers
			for (int j = (i + 4); j < (i + (OUT_MAX/2)); j += 5) {
				if(j <= 280) {
					sprintf(output, "%02d", j);
				} 
				else {
					sprintf(output, "%s", "--");
				}
			
				if ((j + 1) % OUT_MAX == 0){
					printf("%10s\n", output);
				}
				else {
					printf("%10s", output);
				}
			}

			// print the fancy lines
			for (int j = 0; j < SEG_PER_LINE; j++){
				if ((j + 1) % SEG_PER_LINE == 0){
					printf("--------++--------||\n");
				}
				else {
					printf("--------++--------||");
				}
			}

			// print the process ids
			//for (int j = i; j < (i + OUT_MAX); j++){
				/*if (pMap.memory[j] != -1){
					sprintf(output, "%c", (char)pMap.processes[pMap.memory[j]].processID);
				}
				else {
					sprintf(output, " ");
				}

				if ((j + 1) % OUT_MAX == 0){
					printf("%s\n", output);
				}
				else {
					printf("%s", output);
				}*/
			//}
		}

		printf("%s\n", string(OUT_MAX, '=').c_str());
}

void printSwapSpace() {
	const int OUT_MAX = 120;
	const int NUM_PER_LINE = 12;
	const int SEG_PER_LINE = 6;
	
	char output[OUT_MAX];
	memset(output, 0, OUT_MAX);

		printf("\nSwap Space (Pages)\n");
		printf("%s\n", string(OUT_MAX, '=').c_str());
		

		for (int i = 0; i < MAX_PAGES; i += (OUT_MAX/2)){
			
			// print the top numbers
			for (int j = (i + 4); j < (i + (OUT_MAX/2)); j += 5) {
		
				sprintf(output, "%02d", j);

				if ((j + 1) % OUT_MAX == 0){
					printf("%10s\n", output);
				}
				else {
					printf("%10s", output);
				}
			}

			// print the fancy lines
			for (int j = 0; j < SEG_PER_LINE; j++){
				if ((j + 1) % SEG_PER_LINE == 0){
					printf("--------++--------||\n");
				}
				else {
					printf("--------++--------||");
				}
			}

			// print the process ids
			//for (int j = i; j < (i + OUT_MAX); j++){
				/*if (pMap.memory[j] != -1){
					sprintf(output, "%c", (char)pMap.processes[pMap.memory[j]].processID);
				}
				else {
					sprintf(output, " ");
				}

				if ((j + 1) % OUT_MAX == 0){
					printf("%s\n", output);
				}
				else {
					printf("%s", output);
				}*/
			//}
		}

		printf("%s\n", string(OUT_MAX, '=').c_str());
=======
	printf("\nProcID\tPID\tSegs\tPages\n");
	
	for(int i = 0; i < pMap.numProcess; i++)
	{
		p = pMap.processes[i];
		
		printf("%c\t%d\t%lu\t%d\n", p.processID, p.pid, p.segments.size(), p.totalPages);
	}
>>>>>>> master
}