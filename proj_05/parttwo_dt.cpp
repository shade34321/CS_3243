//CS3242 Operating Systems
//Summer 2014
//Project 5: Swapping and Paging, Part 2
//Shade Alabsa, Duncuan Thomas, Matthew Trussell
//Date: 19 July 2014
//File: partone.cpp

//the single overall page table is broken right now and completely circumvented.
//no method for inserting into memory, deciding what to take out of memory, or printing



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string.h>

using namespace std;


#define MAX_PROCESSES 52	// This will not ever change 
#define PROCESS_COUNT 23	// useful when debugging to limit # of procs 
#define MIN_DEATH_INTERVAL 20 
#define MAX_DEATH_INTERVAL 300 
#define MAX_FRAMES 280 
#define MAX_PAGES 720 
#define SHIFT_INTERVAL 10 
#define PRINT_INTERVAL 500 // # of cpu quanta between memory map printouts
#define MAX_QUANTA 50000	// # quanta to run before ending simulation. 
#define SLEEP_LENGTH 2500	// Used with the usleep()to slow down sim between# of cpu quanta between memory map printouts
// cycles (makes reading screen in real-time easier!)

struct page {
	char processID;
	int suffix;
	char valid;
	//int reference;
};

struct process{
	char processID;			//character to denote the process
	int timeAlive;
	page pageTable[20];
	//int pageTable2[20];
};

struct main_Mem {
	page * memory[MAX_FRAMES]; 
};

struct back_store {
	page * backMem[MAX_PAGES];
};

struct page_table {
	page* pTable[MAX_PAGES];
	int pTable2[MAX_PAGES];
	bool valid[MAX_PAGES];
	bool ref[MAX_PAGES];
};



void initProcesses();
void initPageTable();
process createKernal();
process createProcess(char);
void touchProcess();
void killProcess(process *);

void printMemory();
void printBackStore();
void printProcess();
void printMemoryMap();

bool putInMemory(int, process *);
void putInBackStore(process *, int);

process processList[PROCESS_COUNT];
page_table pTable;
back_store backStore;
main_Mem mainMem;

//because I realized that I created all processes at the start and had no way of dealing with dead ones
process aliveProcess[PROCESS_COUNT]; //(and didn't want to change it)

int main(){//#######MAIN########
	//srand(time(NULL)); //Turn off for debug.
	initProcesses();
	initPageTable();
	for (int i=0; i<23; i++) {
		cout << aliveProcess[i].processID << endl;
	}
	for (int i=0; i<23; i++) {
		touchProcess();
	}
		touchProcess();
	for (int i=0; i<PROCESS_COUNT; i++) {
		cout << processList[i].pageTable[1].processID << processList[i].pageTable[1].suffix << endl;
	}
	cout << "#########################################\n";
	for (int i=0; i<23; i++) {
		cout << aliveProcess[i].processID<< endl;
	}
	cout << backStore.backMem[2] << endl;
	
	
}


void initProcesses(){ //creates all processes for all letters; puts them into processList
	processList[22]=createKernal();
	// Messy and gross way of doing things, but it works...I think...
	char * letters = "ABCDEFGHIJKLMNOPQRSTUV";
	for(int i=0; i<PROCESS_COUNT-1; i++){
		processList[i]=createProcess(*letters);
		letters++;
	}
	for (int i=0; i<PROCESS_COUNT; i++) {
		aliveProcess[i]=createProcess('-');
	}
}

void initPageTable(){
	for (int i=0; i<MAX_PAGES; i++) {
		pTable.ref[i]=false;
		pTable.pTable2[i]=-1;
	}
}

page createPage(char ID, int suf, char val){ //used by createProcess()
	page temp;
	temp.processID=ID;
	temp.suffix=suf;
	temp.valid=val;
	return temp;
}

process createKernal(){ //basically a clone of createProcess() with special stuff for kernal; used by initProcesses()
	process temp;
	temp.processID='@';
	temp.timeAlive=100000;
	for (int i=0; i<20; i++) {
		if (i<2) {
			temp.pageTable[i]=createPage('@', 0, 'v');//code segment
		}else if (i<5) {
			temp.pageTable[i]=createPage('@', 1, 'v');//stack segment
		}else if (i<10) {
			temp.pageTable[i]=createPage('@', 2, 'v');//heap sesgment
		}else {
			temp.pageTable[i]=createPage('@', 3, 'v');//subroutine segments
		}		
	}
	return temp;
}

process createProcess(char IDletter){ //returns a process with ID; used by initProcesses()
	int rand5=(rand()%5)+1;
	int randAlive=(rand()%(MAX_DEATH_INTERVAL-MIN_DEATH_INTERVAL))+MIN_DEATH_INTERVAL;
	
	
	process temp;	
	temp.processID=IDletter;
	temp.timeAlive=randAlive;
	for (int i=0; i<10+rand5; i++) {
		if (i<2) {
			temp.pageTable[i]=createPage(IDletter, 0, 'v');//code segment
		}else if (i<5) {
			temp.pageTable[i]=createPage(IDletter, 1, 'v');//stack segment
		}else if (i<10) {
			temp.pageTable[i]=createPage(IDletter, 2, 'v');//heap segment
		}else {
			temp.pageTable[i]=createPage(IDletter, 3, 'v');//subroutine segment
		}
	}
	for (int i=10+rand5; i<20; i++) {
		temp.pageTable[i]=createPage('-', -1, 'i');//fills rest of array as invalid
	}
	

	return temp;
}

void putInMemory(/*process *, page number*/){
	//take location in backstore
	//check for room in mainMem 
		//if not, use alg to decide what to putInBackStore() using 1 of 3 algorithms
	//copy from backstore to mainMem
	//update pagetable
}

void putInBackStore(process * proc, int pNum){
	process temp = *proc;
	for (int i=0; i<MAX_PAGES; i++) {
		if (backStore.backMem[i]==0) {
			backStore.backMem[i]=&temp.pageTable[pNum];
		}
	}
}



void touchProcess(){
	//gen num 0-21 inclusive (22 is @ and always in mem). pick process from processList array that == [rand()] 
	int randTemp = rand()%(PROCESS_COUNT-1);
	cout << "rand temp= " << randTemp << endl; //DEBUG
	bool isAlive = false;
	process temp1 = processList[randTemp];
	page * temp2;
	for (int i=0; i<PROCESS_COUNT; i++) {
		if(aliveProcess[i].processID==temp1.processID){
			isAlive = true;
			break;
		}
	}if (!isAlive) { //if process hasn't been "created"(all have been, but is not technically created until in aliveProcess[])
		//copy to aliveProcesses[]
		for (int i=0; i<PROCESS_COUNT; i++) {
			if (aliveProcess[i].processID == '-') { //'-'indicates empty spot
				aliveProcess[i]=processList[randTemp];
				break;
			}
		}
		//load all pages into page table
		for (int i=0; processList[randTemp].pageTable[i].valid == 'v'; i++) {
			for (int j=0; i<MAX_PAGES; j++) {
				if (!pTable.ref[j]) {
					temp2=&(temp1.pageTable[i]);
					pTable.pTable[j]=temp2;
					pTable.valid[j]=true;
					break;
				}
			}
		}
		//load all into backingStore
		for (int i=0; temp1.pageTable[i].valid == 'v'; i++) {
			putInBackStore(&temp1, i);
		}
	}
	//search inner pageTable for pages 0-9 and one random valid page >9
	//aliveProcess[i]=processList[randTemp];
	//check pagetable to see if they're 
	/*process temp555;
	for (int i=0; i<MAX_PAGES; i++) {
		temp555 = *pTable.pTable[i];
		if (temp555.processID==processList[randTemp].processID && pTable.pTable2[i] != -1) {
			;
		}
	}*/
	//put missing shit in mainMem
	bool exists=false;
	for (int j=0; j<11; j++) {
		for (int i=0; i<MAX_FRAMES; i++) {
			if (mainMem.memory[i]==&(processList[randTemp].pageTable[j])) {
				exists=true;
				break;
			}
		}
		if(!exists){
			putInMemory();
		}
	}
	
	//update pageTable (pagetable is so broken right now it's not even funny)
	//--timeAlive
	processList[randTemp].timeAlive--;
	//if timeAlive<1 killProcess()
	if (processList[randTemp].timeAlive==0) {
		killProcess(&(processList[randTemp]));
	}
}
void killProcess(process * proc){
	//overwrite with '-' in aliveProcesses[]
	for (int i=0; i<MAX_PROCESSES; i++) {
		if ((*proc).processID == aliveProcess[i].processID) {
			aliveProcess[i]=createProcess('-');
		}
	}
	//give new timeAlive
	(*proc).timeAlive=20+rand()%280;
}
