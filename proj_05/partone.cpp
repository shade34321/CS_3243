//CS3242 Operating Systems
//Summer 2014
//Project 5: Swapping and Paging, Part 1
//Shade Alabsa, Duncuan Thomas, Mathew Trussell
//Date: 6 July 2014
//File: partone.cpp
// http://pastebin.com/b13sQ5tt
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
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

bool firstFit(process *p);
bool worstFit(process *p);
bool bestFit(process *p);
bool putProcessInHole(process *p);
bool compareBestFit(const hole &, const hole &);

int getUserInput();

int main(){
	srand(time(NULL));	//seeding for random numbers
	
	// Get input from the user to determine the process allocation algorithm
	mem_allocation algorithm = (mem_allocation) getUserInput();
	
	// Init the memory map
	pMap.memUsed = 0;
	pMap.numProcess = 0;
	pMap.currentQuanta = 0;
	memset(pMap.memory, -1, sizeof(pMap.memory));

	// Init the backing store
	backStore.capacity = MAX_MEMORY;
	backStore.size = 0;
	backStore.head = 0;
	backStore.tail = 0;
	memset(backStore.bs, -1, sizeof(backStore.bs));

	// Init all the processes
	initProcesses();
	
	putInMemory(0, &pMap.processes[0]);
	putInMemory(120, &pMap.processes[1]);
	putInMemory(300, &pMap.processes[2]);
	putInMemory(500, &pMap.processes[5]);
	putInMemory(800, &pMap.processes[6]);
	putInMemory(940, &pMap.processes[7]);

	/*
	printf("Inserted process @, 0, and 4 into memory\n");
	printf("%c: start: %d\tsize: %d\n", (char)pMap.processes[0].processID, pMap.processes[0].memStart, pMap.processes[0].memorySize);
	printf("%c: start: %d\tsize: %d\n", (char)pMap.processes[1].processID, pMap.processes[1].memStart, pMap.processes[1].memorySize);
	printf("%c: start: %d\tsize: %d\n", (char)pMap.processes[5].processID, pMap.processes[5].memStart, pMap.processes[5].memorySize);
	printMemory();
	*/

	
	firstFit(&pMap.processes[22]);
	//firstFit(&pMap.processes[23]);
		
	bestFit(&pMap.processes[41]);
	//bestFit(&pMap.processes[42]);
	
	worstFit(&pMap.processes[55]);
	//worstFit(&pMap.processes[56]);
	
	/* This would be the basic structure to use the user selected alogrithm each time, once we get it set up
	to loop for MAX_QUANTA durations 
	*/
	/*switch(algorithm) {
		case FIRST:
			firstFit(&pMap.processes[48]);
			break;
		case WORST:
			worstFit(&pMap.processes[48]);
			break;
		case BEST:
			bestFit(&pMap.processes[48]);
			break;
		default:
			// Do nothing
			break;	
	}*/

	printMemoryMap();
}

int getUserInput() {
	int input = -1;
	
	while(input < 1 || input > 3) {
		cout << "Choose the allocation algorithm: (1) First, (2) Worst, (3) Best: ";
		cin >> input;
		cout << "\n";
		
		if (input < 1 || input > 3)
		{
			input = -1;
		}
	}
	
	return input;	
}

int getburstTime(){
	return (rand() % MAX_BURST + MIN_BURST);
}

void findHoles(){
	int i, j = backStore.head;
	hole *h = (hole *)malloc(sizeof(hole));
	holes.clear();

	printf("Inside findHoles\n");
	printMemory();

	h->size = 0;

	for (i = 0; i < MAX_MEMORY; i++){
		if (pMap.memory[i % MAX_MEMORY] == -1){ //should handle wrapping holes - need to test
			h->start = i;
			while (pMap.memory[i] == -1){
				h->size++;
				i++;
			}
			holes.push_back(*h);
		}
		h->size = 0;
	}
}

void printHoles(){
	if (holes.size() == 0){
		printf("No possible holes\n");
	} else {
		printf("Possible holes\n");
		for (int i = 0; i < holes.size(); i++){
			printf("Hole: %d\tstart: %d\tend: %d\tsize: %d\n", i, holes.at(i).start, holes.at(i).start + holes.at(i).size - 1, holes.at(i).size);
		}
	}
}

bool worstFit(process *p){
	printf("\nWORST FIT\n");
	
	process *temp;
	findHoles();  //Get updated list of holes
	printHoles();

	sort(holes.begin(), holes.end(), compareBestFit);
	reverse(holes.begin(), holes.end());

	printHoles();

	return putProcessInHole(p);
	/*
	//Do we have a hole big enough?
	for (int i = 0; i < holes.size(); i++){
		if (!p && holes.at(i).size >= pMap.processes[backStore.bs[backStore.head]].memorySize){ //we are pulling from the backstore
			printf("%c needs: %d and is being put at %d\n", (char)pMap.processes[backStore.bs[backStore.head]].processID, pMap.processes[backStore.bs[backStore.head]].memorySize, holes.at(i).start);
			putInMemory(holes.at(i).start, NULL);
			return true;
		}
		else if (p && holes.at(i).size >= p->memorySize) { //we were given a process to put in memory
			printf("%c needs: %d and is being put at %d\n", (char)p->processID, p->memorySize, holes.at(i).start);
			putInMemory(holes.at(i).start, p);
			return true;
		}
	}

	//check for IDLE Processes next
	for (int i = 0; i < MAX_MEMORY; i += pMap.processes[pMap.memory[i]].memorySize){
		temp = &pMap.processes[pMap.memory[i]];
		if (!p && temp->state == IDLE && temp->memorySize >= pMap.processes[backStore.bs[backStore.head]].memorySize){
			printf("Idle process at %d, it's the process %c and it has a state of %d\n", i, (char)pMap.processes[pMap.memory[i]].processID, pMap.processes[pMap.memory[i]].state);
			putInBackStore(i, NULL);
			printf("%c needs: %d and is being put at %d\n", (char)pMap.processes[backStore.bs[backStore.head]].processID, pMap.processes[backStore.bs[backStore.head]].memorySize, holes.at(i).start);
			putInMemory(i, NULL);
			return true;
		} else if (p && temp->state == IDLE && temp->memorySize >= p->memorySize){
			printf("Idle process at %d, it's the process %c and it has a state of %d\n", i, (char)pMap.processes[pMap.memory[i]].processID, pMap.processes[pMap.memory[i]].state);
			putInBackStore(i, NULL);
			printf("%c needs: %d and is being put at %d\n", (char)p->processID, p->memorySize, holes.at(i).start);
			putInMemory(i, p);
			return true;
		}
	}

	return false;
	*/
}

bool compareBestFit(const hole &a, const hole &b){
	return a.size < b.size;
}

/*
// Get All holes then sort from lowest to highest.
// Loop over the various holes to find a hole small enough to fit what we need
// Insert memory/process in memory
*/
bool bestFit(process *p){
	printf("\nBEST FIT\n");
	
	findHoles();  //Get updated list of holes
	printHoles();

	sort(holes.begin(), holes.end(), compareBestFit);

	printHoles();

	return putProcessInHole(p);
	/*
	//Do we have a hole big enough?
	for (int i = 0; i < holes.size(); i++){
		if (!p && holes.at(i).size >= pMap.processes[backStore.bs[backStore.head]].memorySize){ //we are pulling from the backstore
			printf("%c needs: %d and is being put at %d\n", (char)pMap.processes[backStore.bs[backStore.head]].processID, pMap.processes[backStore.bs[backStore.head]].memorySize, holes.at(i).start);
			putInMemory(holes.at(i).start, NULL);
			return true;
		}
		else if (p && holes.at(i).size >= p->memorySize) { //we were given a process to put in memory
			printf("%c needs: %d and is being put at %d\n", (char)p->processID, p->memorySize, holes.at(i).start);
			putInMemory(holes.at(i).start, p);
			return true;
		}
	}

	//check for IDLE Processes next
	for (int i = 0; i < MAX_MEMORY; i += pMap.processes[pMap.memory[i]].memorySize){
		temp = &pMap.processes[pMap.memory[i]];
		if (!p && temp->state == IDLE && temp->memorySize >= pMap.processes[backStore.bs[backStore.head]].memorySize){
			printf("Idle process at %d, it's the process %c and it has a state of %d\n", i, (char)pMap.processes[pMap.memory[i]].processID, pMap.processes[pMap.memory[i]].state);
			putInBackStore(i, NULL);
			printf("%c needs: %d and is being put at %d\n", (char)pMap.processes[backStore.bs[backStore.head]].processID, pMap.processes[backStore.bs[backStore.head]].memorySize, holes.at(i).start);
			putInMemory(i, NULL);
			return true;
		} else if (p && temp->state == IDLE && temp->memorySize >= p->memorySize){
			printf("Idle process at %d, it's the process %c and it has a state of %d\n", i, (char)pMap.processes[pMap.memory[i]].processID, pMap.processes[pMap.memory[i]].state);
			putInBackStore(i, NULL);
			printf("%c needs: %d and is being put at %d\n", (char)p->processID, p->memorySize, holes.at(i).start);
			putInMemory(i, p);
			return true;
		}
	}

	return false;
	*/
}

bool firstFit(process *p){
	printf("\nFIRST FIT\n");
	process *temp;
	findHoles();   //Get updated list of holes
	printHoles();

	return putProcessInHole(p);
}

bool putProcessInHole(process *p){
	process *temp;
	//Do we have a hole big enough?
	for (int i = 0; i < holes.size(); i++){
		if (!p && holes.at(i).size >= pMap.processes[backStore.bs[backStore.head]].memorySize){ //we are pulling from the backstore
			printf("%c needs: %d and is being put at %d\n", (char)pMap.processes[backStore.bs[backStore.head]].processID, pMap.processes[backStore.bs[backStore.head]].memorySize, holes.at(i).start);
			putInMemory(holes.at(i).start, NULL);
			return true;
		}
		else if (p && holes.at(i).size >= p->memorySize) { //we were given a process to put in memory
			printf("%c needs: %d and is being put at %d\n", (char)p->processID, p->memorySize, holes.at(i).start);
			putInMemory(holes.at(i).start, p);
			return true;
		}
	}

	//check for IDLE Processes next
	for (int i = 0; i < MAX_MEMORY; i += pMap.processes[pMap.memory[i]].memorySize){
		temp = &pMap.processes[pMap.memory[i]];
		if (!p && temp->state == IDLE && temp->memorySize >= pMap.processes[backStore.bs[backStore.head]].memorySize){
			printf("Idle process at %d, it's the process %c and it has a state of %d\n", i, (char)pMap.processes[pMap.memory[i]].processID, pMap.processes[pMap.memory[i]].state);
			putInBackStore(i, NULL);
			printf("%c needs: %d and is being put at %d\n", (char)pMap.processes[backStore.bs[backStore.head]].processID, pMap.processes[backStore.bs[backStore.head]].memorySize, holes.at(i).start);
			putInMemory(i, NULL);
			return true;
		} else if (p && temp->state == IDLE && temp->memorySize >= p->memorySize){
			printf("Idle process at %d, it's the process %c and it has a state of %d\n", i, (char)pMap.processes[pMap.memory[i]].processID, pMap.processes[pMap.memory[i]].state);
			putInBackStore(i, NULL);
			printf("%c needs: %d and is being put at %d\n", (char)p->processID, p->memorySize, holes.at(i).start);
			putInMemory(i, p);
			return true;
		}
	}

	return false;
}

bool putInBackStore(int start, process *p){
	if (p){ //process is going straight to the back store - will probably never happen but during testing:)
		for (int i = 0; i < p->memorySize; i++){
			backStore.bs[backStore.tail] = p->pid;
			backStore.tail = (backStore.tail + 1) % backStore.capacity; //set the new tail
		}
	} else { //pull it from the back store
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
		pMap.processes[backStore.tail - 1].memStart = tail;
		pMap.processes[backStore.tail - 1].state = IDLE;
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

// Prints the memory stats, then calls printMemory() to print the actual memory "array"
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
	
	sprintf(output, "\nQUANTA ELAPSED: %d", pMap.currentQuanta);
	printf("%-25s\n", output);
	sprintf(output, "MEMORY: %d b", MAX_MEMORY);
	printf("%-20s", output);
	sprintf(output, "USED: %d b (%.2f%%)", pMap.memUsed, usedPercent);
	printf("%-25s", output);
	sprintf(output, "FREE: %d b (%.2f%%)", free, freePercent);
	printf("%-25s\n", output);
	sprintf(output, "PROCESS: %d", MAX_PROCESSES);
	printf("%-20s", output);
	sprintf(output, "LOADED: %d b (%.2f%%)", pMap.numProcess, numProcessPercent);
	printf("%-25s", output);
	sprintf(output, "UNLOADED: %d b (%.2f%%)", unloadedProcess, unloadedPercent);
	printf("%-25s\n", output);

	printf("%s\n", string(80, '=').c_str());

	printMemory();
}
