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
	int memory[MAX_MEMORY];			//Total bytes of memory
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
void printMemory();
void printProcess();
void printMemoryMap();
bool putInMemory(int, process *);
int getburstTime();

int main(){
	srand(time(NULL));	//seeding for random numbers

	pMap.memUsed = 0;
	pMap.numProcess = 0;
	pMap.currentQuanta = 0;
	memset(pMap.memory, -1, sizeof(pMap.memory));

	backStore.capacity = MAX_MEMORY;
	backStore.size = 0;
	backStore.head = 0;
	backStore.tail = 0;

	initProcesses();
	//printProcess();
	//printMemory();
	printMemoryMap();
	putInMemory(0, &pMap.processes[0]);
	printMemory();
	printMemoryMap();
}

int getburstTime(){
	return (rand() % MAX_BURST + MIN_BURST);
}

bool putInBackStore(int start){
	/*
	process *p;
	p = &pMap.processes[start];

	if ((backStore.capacity - backStore.tail + 1) < p->memorySize){ //The tail 
		//memcpy(backStore.bs[backStore.tail]);
	}

	pMap.numProcess--;
	pMap.memUsed -= p->memorySize;
	*/
}

bool putInMemory(int start, process *p){
	cout << "Inside put Memory" << endl;

	for (int i = 0; i < p->memorySize; i++){
		pMap.memory[start] = p->pid;
		start++;
	}

	if (p->processID != 64) {
		pMap.numProcess++;
	}
	pMap.memUsed += p->memorySize;
	/*
	if (p) { //if we pass in a process
		for (int i = 0; i < p->memorySize; i++){
			pMap.memory[start] = *p;
			start++;
		}

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
	*/
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
	pMap.processes[location].state = ((processID == 64) ? (RUNNING) : (IDLE));
}

void printMemory(){
	char output[80];
	bool test = true;
	memset(output, 0, 80);

	for (int j = 0; j < MAX_MEMORY; j++){ //For the top numbers
		//print process ID;
		if (pMap.memory[j] != -1){
			sprintf(output, "%c", (char)pMap.processes[pMap.memory[j]].processID);
		}
		else {
			sprintf(output, "{");
		}

		if ((j + 1) % 80 == 0){
			printf("%s\n", output);
		}
		else {
			printf("%s", output);
		}
	}
	/*
	for (int i = 0; i < MAX_MEMORY; i++){
		if ((i+1) % 80 == 0){
			printf("%d\n", pMap.memory[i]);
		} else {
			printf("%d", pMap.memory[i]);
		}
	}
	printf("\n");
	*/
}

void printProcess(){
	//cout << "ProcessID" << setw(10) << "PID" << " Memory Size" << " Memory Start" << " Burst Time" << " State" << endl;
	printf("ProcessID\tPID\tMemory Size\tMemory Start\tBurst Time\tState\n");
	for (int i = 0; i < MAX_PROCESSES + 1; i++){
		printf("%c\t\t%d\t%d\t\t%d\t\t%d\t\t%d\n", pMap.processes[i].processID, pMap.processes[i].pid, pMap.processes[i].memorySize, pMap.processes[i].memStart, pMap.processes[i].burstTime, pMap.processes[i].state);
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
	char output[80];
	
	memset(output, 0, 80);
	
	sprintf(output, "QUANTA ELAPSED: %d", pMap.currentQuanta);
	printf("%-25s\n", output);
	sprintf(output, "MEMORY: %d b", MAX_MEMORY);
	printf("%-25s", output);
	sprintf(output, "USED: %d b (%.2f%%)", pMap.memUsed, usedPercent);
	printf("%-25s", output);
	sprintf(output, "FREE: %d b (%.2f%%)", free, freePercent);
	printf("%-25s\n", output);
	sprintf(output, "PROCESS: %d", MAX_PROCESSES);
	printf("%-25s", output);
	sprintf(output, "LOADED: %d b (%.2f%%)", pMap.numProcess, numProcessPercent);
	printf("%-25s", output);
	sprintf(output, "UNLOADED: %d b (%.2f%%)", unloadedProcess, unloadedPercent);
	printf("%-25s\n", output);

	printf("%s\n", string(80, '=').c_str());

	for (int i = 0; i < MAX_MEMORY; i+=80){
		
		for (int j = (i+9); j < (i+80); j+=10){ //For the top numbers
			sprintf(output, "%d", j);
			if ((j + 1) % 80 == 0){
				printf("%10s\n", output);
			} else {
				printf("%10s", output);
			}
		}
		
		for (int j = 0; j < 8; j++){
			if ((j + 1) % 8 == 0){
				printf("----+----|\n");
			} else {
				printf("----+----|");
			}
		}

		for (int j = i; j < (i+80); j++){ //For the top numbers
			//print process ID;
			if (pMap.memory[j] != -1){
				sprintf(output, "%c", (char)pMap.processes[pMap.memory[j]].processID);
			} else {
				sprintf(output, " ");
			}

			if ((j + 1) % 80 == 0){
				printf("%s\n", output);
			} else {
				printf("%s", output);
			}
		}
	}

	printf("%s\n", string(80, '=').c_str());
}
