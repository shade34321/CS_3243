//CS3242 Operating Systems
//Summer 2014
//Project 5: Swapping and Paging, Part 1
//Shade Alabsa, Duncuan Thomas, Matthew Trussell
//Date: 6 July 2014
//File: partone.cpp
// http://pastebin.com/b13sQ5tt
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
	int bs[MAX_MEMORY];
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
void printBackStore();
void printProcess();
void printMemoryMap();
bool putInMemory(int, process *);
bool putInBackStore(int, process *);
void findHoles();
void printHoles();
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
	memset(backStore.bs, -1, sizeof(backStore.bs));

	initProcesses();
	
	putInMemory(0, &pMap.processes[0]);
	putInMemory(120, &pMap.processes[1]);
	putInMemory(400, &pMap.processes[5]);

	/*
	printf("Inserted process @, 0, and 4 into memory\n");
	printf("%c: start: %d\tsize: %d\n", (char)pMap.processes[0].processID, pMap.processes[0].memStart, pMap.processes[0].memorySize);
	printf("%c: start: %d\tsize: %d\n", (char)pMap.processes[1].processID, pMap.processes[1].memStart, pMap.processes[1].memorySize);
	printf("%c: start: %d\tsize: %d\n", (char)pMap.processes[5].processID, pMap.processes[5].memStart, pMap.processes[5].memorySize);
	printMemory();

	printf("Inserted process 1 into the backstore\n");
	printf("%c: start: %d\tsize: %d\n", (char)pMap.processes[2].processID, pMap.processes[2].memStart, pMap.processes[2].memorySize);
	printf("backstore: tail %d\n", backStore.tail);
	*/
	putInBackStore(0, &pMap.processes[2]);
	/*
	printBackStore();
	printMemory();

	printf("Inserted process 3 into the backstore\n");
	printf("%c: start: %d\tsize: %d\n", (char)pMap.processes[4].processID, pMap.processes[4].memStart, pMap.processes[4].memorySize);
	printf("backstore: tail %d\n", backStore.tail);
	*/
	putInBackStore(0, &pMap.processes[4]);
	printBackStore();
	printMemory();

	printf("Inserted process 4 into the backstore\n");
	printf("%c: start: %d\tsize: %d\n", (char)pMap.processes[5].processID, pMap.processes[5].memStart, pMap.processes[5].memorySize);
	printf("backstore: tail %d\n", backStore.tail);
	putInBackStore(400, NULL);
	printMemory();
	printBackStore();

	findHoles();
	printHoles();

	printf("Pulling from backstore\n");
	printf("%c: start: %d\tsize: %d\n", (char)pMap.processes[backStore.bs[backStore.head]].processID, pMap.processes[backStore.bs[backStore.head]].memStart, pMap.processes[backStore.bs[backStore.head]].memorySize);
	printf("backstore head: %d\n", backStore.head);
	putInMemory(300, NULL);
	printMemory();
	printBackStore();
	printf("backstore head: %d\n", backStore.head);
	findHoles();
	printHoles();

	//printMemoryMap();
}

int getburstTime(){
	return (rand() % MAX_BURST + MIN_BURST);
}

void findHoles(){
	int i, j = 0;
	hole *h = (hole *)malloc(sizeof(hole));
	holes.clear();

	h->size = 0;

	printf("Inside findHoles\n");

	for (i = 0; i < MAX_MEMORY; i++){
		if (pMap.memory[i] == -1){
			h->start = i;
			while (pMap.memory[i] == -1){
				h->size++;
				i++;
			}
			holes.push_back(*h);
		}
	}
}

void printHoles(){
	if (holes.size() == 0){
		printf("No possible holes\n");
	} else {
		printf("Possible holes\n");
		for (int i = 0; i < holes.size(); i++){
			printf("Hole: %d\tstart: %d\tsize: %d\n", i, holes.at(i).start, holes.at(i).size);
		}
	}
}

bool putInBackStore(int start, process *p){
    if (p){ //process is going straight to the back store - will probably never happen but during testing:)
		for (int i = 0; i < p->memorySize; i++){
			backStore.bs[backStore.tail] = p->pid;
			backStore.tail = (backStore.tail + 1) % backStore.capacity; //set the new tail
		}
	}
	else { //pull it from the back store
		//Doing this first since I need to know where the tail is at
		//printf("Taking from memory\n");
		//printf("start: %d\t tail: %d\t Size: %d\n", start, backStore.tail, pMap.processes[pMap.memory[start]].memorySize);
		int mem = pMap.processes[pMap.memory[start]].memorySize;
        int tail = backStore.tail; //Temp variable so we can assign where the memory is at later
		for (int i = 0; i < mem; i++){
			//printf("i: %d\t taking: %d\n", i, (start + i));
			backStore.bs[backStore.tail] = pMap.memory[start + i]; //copy the memory over
			//printf("backStore tail value: %d\t", backStore.bs[backStore.tail]);
			pMap.memory[start + i] = -1; //set the memory as free
			//printf("previous memory: %d\t", pMap.memory[start + i]);
			backStore.tail = (backStore.tail + 1) % backStore.capacity; //set the new tail
			//printf("New tail: %d\n", backStore.tail);
		}

		pMap.numProcess--;
		pMap.memUsed -= pMap.processes[backStore.tail-1].memorySize;
		pMap.processes[pMap.memory[start]].memStart = tail;
		pMap.processes[pMap.memory[start]].state = IDLE;
	}

	return true;
}

bool putInMemory(int start, process *p){
	if (p){ //process is just starting and is not in the back store
		for (int i = 0; i < p->memorySize; i++){
			pMap.memory[start + i] = p->pid;
		}
	}
	else { //pull it from the back store
		int mem = pMap.processes[backStore.bs[backStore.head]].memorySize;
		for (int i = 0; i < mem; i++){
			pMap.memory[start + i] = backStore.bs[backStore.head];
			backStore.bs[backStore.head] = -1;
			backStore.head = (backStore.head + 1) % backStore.capacity;
		}

		pMap.numProcess++;
	}

	pMap.memUsed += pMap.processes[pMap.memory[start]].memorySize;
	pMap.processes[pMap.memory[start]].memStart = start;
	pMap.processes[pMap.memory[start]].state = RUNNING;

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

	printf("Memory Map\n");
	printf("%s\n", string(80, '=').c_str());

	for (int i = 0; i < MAX_MEMORY; i += 80){

		for (int j = (i + 9); j < (i + 80); j += 10){ //For the top numbers
			sprintf(output, "%d", j);
			if ((j + 1) % 80 == 0){
				printf("%10s\n", output);
			}
			else {
				printf("%10s", output);
			}
		}

		for (int j = 0; j < 8; j++){
			if ((j + 1) % 8 == 0){
				printf("----+----|\n");
			}
			else {
				printf("----+----|");
			}
		}

		for (int j = i; j < (i + 80); j++){ //For the top numbers
			//print process ID;
			if (pMap.memory[j] != -1){
				sprintf(output, "%c", (char)pMap.processes[pMap.memory[j]].processID);
			}
			else {
				sprintf(output, " ");
			}

			if ((j + 1) % 80 == 0){
				printf("%s\n", output);
			}
			else {
				printf("%s", output);
			}
		}
	}

	printf("%s\n", string(80, '=').c_str());
}

void printBackStore(){
	char output[80];
	bool test = true;
	memset(output, 0, 80);

	printf("Map of Backstore\n");
	printf("%s\n", string(80, '=').c_str());

	for (int i = 0; i < MAX_MEMORY; i += 80){

		for (int j = (i + 9); j < (i + 80); j += 10){ //For the top numbers
			sprintf(output, "%d", j);
			if ((j + 1) % 80 == 0){
				printf("%10s\n", output);
			}
			else {
				printf("%10s", output);
			}
		}

		for (int j = 0; j < 8; j++){
			if ((j + 1) % 8 == 0){
				printf("----+----|\n");
			}
			else {
				printf("----+----|");
			}
		}

		for (int j = i; j < (i + 80); j++){ //For the top numbers
			//print process ID;
			if (backStore.bs[j] != -1){
				sprintf(output, "%c", (char)pMap.processes[backStore.bs[j]].processID);
			}
			else {
				sprintf(output, " ");
			}

			if ((j + 1) % 80 == 0){
				printf("%s\n", output);
			}
			else {
				printf("%s", output);
			}
		}
	}

	printf("%s\n", string(80, '=').c_str());
}

void printProcess(){
	printf("ProcessID\tPID\tMemory Size\tMemory Start\tBurst Time\tState\n");
	for (int i = 0; i < MAX_PROCESSES + 1; i++){
		printf("%c\t\t%d\t%d\t\t%d\t\t%d\t\t%d\n", pMap.processes[i].processID, pMap.processes[i].pid, pMap.processes[i].memorySize, pMap.processes[i].memStart, pMap.processes[i].burstTime, pMap.processes[i].state);
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
