// CS3243 Operating Systems
// Summer 2014 
// Project 3: Forks and Threads
// Matthew Tinney and Shade Alabsa
// Date: 10 June 2014
// File: partthree.cpp

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <vector>

using namespace std;

int importList(vector<int> *);

int main(){

	return 0;
}

int importList(vector<int> *list){
	int data;
	fstream f;

	f.open("numbers.txt", fstream::in);

	if (f.is_open()) {
		while (f >> data) {
			if (insert(list, list->head, data)) {
				return -1;
			}
		}
	}

	return 0;
}