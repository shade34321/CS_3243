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

using namespace std;

#define MAX_PROCESSES			60 // This will not ever change
#define PROCESS_COUNT			60 // useful when debugging to limit # of procs
#define MIN_BURST				10
#define MAX_BURST				200
#define MIN_MEMORY_PER_PROC		10
#define MAX_MEMORY_PER_PROC		250
#define MAX_MEMORY				1040
#define MAX_BLOCK_PROC_RATIO	0.5
#define ENABLE_COMPACTION		1 // Boolean flag for whether compaction is on/off
#define PRINT_INTERVAL			500 // # of cpu quanta between memory map printouts
#define MAX_QUANTA				50000 // # quanta to run before ending simulation.
#define SLEEP_LENGTH			2500 // Used with the usleep()to slow down sim between
									// cycles (makes reading screen in real-time easier!)

struct process{
	int memorySize;			//Size of memory in bytes for the process
	int memStart;			//Memory Start location
	char pid;				//character to denote the process.
	int burstTime;			//Burst time for process
	int state;				//State the process is in
};

struct processMap{
	char memory[MAX_MEMORY];			//Total bytes of memory
	process processes[MAX_PROCESSES];		//array of process we can have.
	int numProcess;
	int memUsed;
	int currentQuanta;

};

enum p_State {
	WAITING = 0,
	RUNNING = 1,
	IDLE = 2
};

void createProcess();
void printMemoryMap(processMap *);

int main(){
	srand(time(NULL));	//seeding for random numbers

}

void printMemoryMap(processMap *pMap){
	int free = MAX_MEMORY - pMap->memUsed;
	double usedPercent = pMap->memUsed / ((double)MAX_MEMORY);
	double freePercent = ((double)free) / ((double)MAX_MEMORY);
	double numProcessPercent = ((double)pMap->numProcess) / ((double)MAX_PROCESSES);
	int unloadedProcess = MAX_PROCESSES - pMap->numProcess;
	double unloadedPercent = ((double)unloadedProcess) / ((double)MAX_PROCESSES);
	int freeBlocks = 0;
	int largest = 0;
	int smallest = 0;
	double blockRatio = 0.0;

	cout << "QUANTA ELAPSED: " << pMap->currentQuanta << endl;
	cout << "MEMORY: " << MAX_MEMORY << "b";
	cout << setw(10) << "USED: " << pMap->memUsed << "b (";
	cout << fixed << setprecision(2) << usedPercent << "%)";
	cout << setw(10) << "FREE: " << free << "b (" << fixed << setprecision(2) << freePercent << "%)" << endl;

	cout << "PROCESSES: " << MAX_PROCESSES << setw(10) << "LOADED: " << pMap->numProcess << "(" << fixed << setprecision(2) << numProcessPercent << "%)";
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