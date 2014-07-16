//CS3242 Operating Systems
//Summer 2014
//Project 5: Swapping and Paging, Part 1
//Shade Alabsa, Duncuan Thomas, Matthew Trussell
//Date: 6 July 2014
//File: partone.cpp
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string.h>

using namespace std;

#define MAX_PROCESSES			60 // This will not ever change
#define PROCESS_COUNT			60 // useful when debugging to limit # of procs
#define MIN_BURST				10
#define MAX_BURST				200
#define MIN_MEMORY_PER_PROC		4
#define MAX_MEMORY_PER_PROC		160
#define MAX_MEMORY              1040
#define MAX_BLOCK_PROC_RATIO	0.5
#define ENABLE_COMPACTION		1 // Boolean flag for whether compaction is on/off
#define PRINT_INTERVAL			500 // # of cpu quanta between memory map printouts
#define MAX_QUANTA				50000 // # quanta to run before ending simulation.
#define SLEEP_LENGTH			2500 // Used with the usleep()to slow down sim between
									// cycles (makes reading screen in real-time easier!)

struct process{
	int memorySize;			//Size of memory in bytes for the process
	int memStart;			//Memory Start location
	int pid;				//PID of the process
	int processID;			//character to denote the process.
	int burstTime;			//Burst time for process
	int state;				//State the process is in
};

struct processMap{
	process memory[MAX_MEMORY];			//Total bytes of memory
	process processes[MAX_PROCESSES+1];		//array of process we can have.
	int numProcess;
	int memUsed;
	int currentQuanta;
};

struct back_store{
	process bs[MAX_MEMORY];
	int capacity;
	int size;
	int head;
	int tail;
};


struct hole{
	int start;
	int size;
};

enum mem_allocation {
	FIRST = 1,
	BEST = 2,
	WORST =  3
};

enum p_State {
	WAITING = 0,
	RUNNING = 1,
	IDLE = 2
};

processMap pMap;
back_store backStore;
vector<hole> holes;

void initProcesses();
void createProcess(int, int);
void printProcess();
void printMemoryMap();
bool putInMemory(int, process *);
int getburstTime();

int main(){
	srand(time(NULL));	//seeding for random numbers

	pMap.memUsed = 0;
	pMap.numProcess = 0;
	pMap.currentQuanta = 0;
	
	backStore.capacity = MAX_MEMORY;
	backStore.size = 0;
	backStore.head = 0;
	backStore.tail = 0;

	initProcesses();
	printProcess();

	putInMemory(0, &pMap.processes[0]);
	printMemoryMap();
}

int getburstTime(){
	return (rand() % MAX_BURST + MIN_BURST);
}

bool putInBackStore(int start){
	process *p;
	p = &pMap.memory[start];

	if ((backStore.capacity - backStore.tail + 1) < p->memorySize){ //The tail 
		//memcpy(backStore.bs[backStore.tail]);
	}

	pMap.numProcess--;
	pMap.memUsed -= p->memorySize;
}

bool putInMemory(int start, process *p){
	cout << "Inside putInMemory" << endl;
	cout << p->processID << endl;

	if (p) { //if we pass in a process
		cout << "Processing process " << endl << "start is " << start << endl;

		for (int i = 0; i < p->memorySize; i++){
			pMap.memory[start] = *p;
			start++;
		}

		cout << "start is now " << start << endl;

		if (p->processID != 64) {
			pMap.numProcess++;
		}
		pMap.memUsed += p->memorySize;
	} else {
		cout << "Inside else" << endl;
		process *temp;
		temp = &pMap.memory[start]; //Get the front of the backStore

		memcpy(temp, backStore.bs, (backStore.bs[backStore.head].memorySize * sizeof(process)));
		pMap.memUsed += backStore.bs[backStore.head].memorySize;
		pMap.numProcess++;
		//Update backstore with new head
	}

	return true;
}

void initProcesses(){
	int i;
	int j = 1;

	createProcess(0, 64); //create root process

	for (i = 48; i < 58; i++){
		createProcess(j, i);
		j++;
	}

	for (i = 65; i < 91; i++){
		if (i != 73){
			createProcess(j, i);
			j++;
		}
	}

	for (i = 97; i < 123; i++){
		if (i != 108){
			createProcess(j, i);
			j++;
		}
	}
}

void createProcess(int location, int processID){
	int memSize;
	int burst;

	if (processID == 64){
		memSize = 120;
		burst = -1;
	}
	else {
		memSize = rand() % MAX_MEMORY_PER_PROC + MIN_MEMORY_PER_PROC;
		burst = getburstTime();
	}

	pMap.processes[location].memorySize = memSize;
	pMap.processes[location].memStart = 0;
	pMap.processes[location].pid = location;
	pMap.processes[location].processID = processID;
	pMap.processes[location].burstTime = burst;
	pMap.processes[location].state = IDLE;
}

void printProcess(){
	cout << "ProcessID" << setw(10) << "PID" << " Memory Size" << " Memory Start" << " Burst Time" << " State" << endl;
	printf("ProcessID\tPID\tMemory Size\tMemory Start\tBurst Time\tState\n");
	for (int i = 0; i < MAX_PROCESSES; i++){
		printf("%\t%c\t%d\t%d\t%d\t%d\n", pMap.processes[i].processID, pMap.processes[i].pid, pMap.processes[i].memorySize, pMap.processes[i].memStart, pMap.processes[i].burstTime, pMap.processes[i].state);
		//cout << pMap.processes[i].processID << setw(10) << pMap.processes[i].pid << setw(10) << pMap.processes[i].memorySize << setw(10) << pMap.processes[i].memStart << setw(10) << pMap.processes[i].burstTime << setw(10) << pMap.processes[i].state << endl;
	}
}

void printMemoryMap(){
	int free = MAX_MEMORY - pMap.memUsed;
	double usedPercent = pMap.memUsed / ((double)MAX_MEMORY) * 100;
	double freePercent = ((double)free) / ((double)MAX_MEMORY) * 100;
	double numProcessPercent = ((double)pMap.numProcess) / ((double)MAX_PROCESSES) * 100;
	int unloadedProcess = MAX_PROCESSES - pMap.numProcess;
	double unloadedPercent = ((double)unloadedProcess) / ((double)MAX_PROCESSES) * 100;
	int freeBlocks = 0;
	int largest = 0;
	int smallest = 0;
	double blockRatio = 0.0;

	cout << "QUANTA ELAPSED: " << pMap.currentQuanta << endl;
	cout << "MEMORY: " << MAX_MEMORY << "b";
	cout << setw(10) << "USED: " << pMap.memUsed << "b (";
	cout << fixed << setprecision(2) << usedPercent << "%)";
	cout << setw(10) << "FREE: " << free << "b (" << fixed << setprecision(2) << freePercent << "%)" << endl;

	cout << "PROCESSES: " << MAX_PROCESSES << setw(10) << "LOADED: " << pMap.numProcess << "(" << fixed << setprecision(2) << numProcessPercent << "%)";
	cout << setw(10) << "UNLOADED: " << unloadedProcess << " (" << fixed << setprecision(2) << unloadedPercent << "%)" << endl;
	cout << "FREE BLOCKS: " << freeBlocks << setw(10) << "LARGEST: " << largest << "b" << setw(2) << "SMALLEST: " << smallest << "b" << setw(10) << "BLOCKS/PROCS RATIO: " << blockRatio << endl;

	for (int i = 0; i < 80; i++){
		cout << "=";
	}

	cout << endl; //Add a new line after the barrier

	for (int i = 0; i < MAX_MEMORY; i+=80){
		
		for (int j = 0; j < 80; j++){ //For the top numbers
			if ((j + 1) % 10 == 0){
				cout << j+i;
			} else {
				cout << " ";
			}
		}
		
		cout << endl; //add new line to move to the next line of output

		for (int j = 0; j < 80; j++){ //For the middle symbols
			if ((j + 1) % 10 == 0){
				cout << "|";
			}
			else if ((j+1) % 5 == 0){
				cout << "+";
			} else {
				cout << "-";
			}
		}

		cout << endl; //Add new line again!

		for (int j = i; j < (i+80); j++){ //For the top numbers
			//print process ID;
		}

	}
}
