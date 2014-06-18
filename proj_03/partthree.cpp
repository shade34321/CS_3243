// CS3243 Operating Systems
// Summer 2014 
// Project 3: Forks and Threads
// Matthew Tinney and Shade Alabsa
// Date: 10 June 2014
// File: partthree.cpp

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <vector>
#include <algorithm>

using namespace std;

struct t_data{
	int sort;
	vector<long> list;
};

static int LOCK = 0;

vector<t_data*> importList(int);
int printList(vector<long> &);
void * sort(void *);
void trial1();
void trial2();

void QuickSort(vector<long> &, long, long);
void SelectionSort(vector<long> &);
void MergeSort(vector<long> &);
void BubbleSort(vector<long> &);
void ShellSort(vector<long> &);
void InsertionSort(vector<long> &);
vector<long> merge(vector<long> &, vector<long> &);


int main(){
	
	trial1();
	trial2();

	return 0;
}

void trial1(){
	vector<t_data*> thread_list;
	vector<long> final_list;
	pthread_t tid[2];
	int i, numElem, numThreads = 2;

	thread_list = importList(2);

	thread_list.at(1)->sort = 1;
	thread_list.at(2)->sort = 2;

	for (i = 0; i < numThreads; i++) {
		if (pthread_create(&tid[i], NULL, &sort, &thread_list.at(i)) != 0){
			perror("pthread_create() error");
			exit(1);
		}
	}

	for (i = 0; i < numThreads; i++) {
		while (!LOCK){
			sleep(1);
		}
		LOCK = 1;
		if (pthread_join(tid[i], (void **)&thread_list.at(i)) == 0){
			for (int j = 0; j < thread_list.at(i)->list.size(); j++){
				final_list.push_back(thread_list.at(i)->list.at(j));
			}
			LOCK = 0;
		}
		else {
			perror("pthread_join() error");
			exit(1);
		}
	}

	//You only tell us to use the sorting algorithms on the threads:)
	sort(final_list.begin(), final_list.end());

	if (printList(final_list)){
		cout << "ERROR writing to file!" << endl;
		exit(1);
	}
}

void trial2(){
	vector<t_data*> thread_list;
	vector<long> final_list;
	pthread_t tid[2];
	int i, numElem, numThreads = 4;

	thread_list = importList(4);

	thread_list.at(1)->sort = 3;
	thread_list.at(2)->sort = 4;
	thread_list.at(3)->sort = 5;
	thread_list.at(4)->sort = 6;

	for (i = 0; i < numThreads; i++) {
		if ( pthread_create(&tid[i], NULL, &sort, &thread_list.at(i)) != 0){
			perror("pthread_create() error");
			exit(1);
		}
	}

	for (i = 0; i < numThreads; i++) {
		while (!LOCK){
			sleep(1);
		}
		LOCK = 1;
		if (pthread_join(tid[i], (void **)&thread_list.at(i)) == 0){
			for (int j = 0; j < thread_list.at(i)->list.size(); j++){
				final_list.push_back(thread_list.at(i)->list.at(j));
			}
			LOCK = 0;
		}
		else {
			perror("pthread_join() error");
			exit(1);
		}
	}

	//You only tell us to use the sorting algorithms on the threads:)
	sort(final_list.begin(), final_list.end());

	if (printList(final_list)){
		cout << "ERROR writing to file!" << endl;
		exit(1);
	}
}


vector<t_data*> importList(int numThreads){
	int data;
	fstream f;
	vector<long> list;
	vector<t_data*> t_list;

	f.open("numbers.txt", fstream::in);

	if (f.is_open()) {
		while (f >> data) {
			list.push_back(data);
		}
	} else {
		cout << "Error importing file!" << endl;
		exit(1);
	}

	f.close();

	long numElements = list.size() / numThreads;
	long offset = 0;
	long lastIndex = numElements;

	for (int i = 0; i < numThreads; i++){
		t_data *p_data = (t_data *) malloc(sizeof(t_data));
				
		for (int j = offset; j != lastIndex; ++j) {
			p_data->list.push_back(list.at(j));
		}

		offset += numElements;
		lastIndex += numElements;
		t_list.push_back(p_data);
		if (p_data){
			free(p_data);
		}
	}

	return t_list;
}

int printList(vector<long> &list){
	ofstream file;
	file.open("sorted.txt");
	if (file.is_open()){
		for (vector<long>::const_iterator i = list.begin(); i != list.end(); ++i){
			file << *i << endl;
		}
	} else {
		return -1;
	}

	file.close();
	return 0;
}

void * sort(void *thread_data){
	t_data *info = (t_data *)thread_data;

	switch (info->sort) {
		case 1: {
			QuickSort(info->list, 0, (info->list.size() - 1));
			break;
		}
		case 2: {
			SelectionSort(info->list);
			break;
		}
		case 3: {
			MergeSort(info->list);
			break;
		}
		case 4: {
			BubbleSort(info->list);
			break;
		}
		case 5: {
			ShellSort(info->list);
			break;
		}
		case 6: {
			InsertionSort(info->list);
			break;
		}
	}

	return info;
}

void QuickSort(vector<long> &u_list, long left, long right) {

	long tempVal;
	long i = left;
	long j = right;
	long pivotVal = u_list.at((left + right) / 2);

	while (i <= j) {
		while (u_list.at(i) < pivotVal)
			i++;
		while (u_list.at(j) > pivotVal)
			j--;
		if (i <= j) {
			tempVal = u_list.at(i);
			u_list.at(i) = u_list.at(j);
			u_list.at(j) = tempVal;
			i++;
			j--;
		}
	}

	if (left < j) {
		QuickSort(u_list, left, j);
	}
	if (i < right) {
		QuickSort(u_list, i, right);
	}
}

void SelectionSort(vector<long> &u_list) {
	long minIndex;
	long tempVal;
	long i;
	long j;

	for (i = 0; i < u_list.size() - 1; i++) {
		minIndex = i;

		for (j = i + 1; j < u_list.size(); j++) {

			if (u_list.at(j) < u_list.at(minIndex)) {
				minIndex = j;
			}
		}

		if (minIndex != i) {
			tempVal = u_list.at(i);
			u_list.at(i) = u_list.at(minIndex);
			u_list.at(minIndex) = tempVal;
		}
	}
}

void MergeSort(vector<long> &list) {
	vector<long> sortedList;
	vector<long> mLeft;
	vector<long> mRight;

	if (list.size() > 1) {

		long mMiddle = list.size() / 2;

		for (long i = 0; i < mMiddle; i++) {
			mLeft.push_back(list.at(i));
		}

		for (long j = mMiddle; j < list.size(); j++) {
			mRight.push_back(list.at(j));
		}

		MergeSort(mLeft);
		MergeSort(mRight);
		list = merge(mLeft, mRight);
	}
}

void BubbleSort(vector<long> &u_list) {
	long tempVal, n = u_list.size();

	do {
		long newN = 0;
		for (long i = 1; i < n; ++i) {
			if (u_list.at(i - 1) > u_list.at(i)) {
				tempVal = u_list.at(i - 1);
				u_list.at(i - 1) = u_list.at(i);
				u_list.at(i) = tempVal;
				newN = i;
			}
		}

		n = newN;
	} while (n > 0);
}

void ShellSort(vector<long> &u_list) {
	int gaps[] = { 701, 301, 132, 57, 23, 10, 4, 1 };
	int g = sizeof(gaps) / sizeof(int);
	long temp, j;

	for (long k = 0; k < g; ++k) {
		for (long i = gaps[k]; i < u_list.size(); ++i) {
			temp = u_list.at(i);

			for (j = i; j >= gaps[k] && u_list.at(j - gaps[k]) > temp;
				j -= gaps[k]) {
				u_list.at(j) = u_list.at(j - gaps[k]);
			}

			u_list.at(j) = temp;
		}
	}
}

void InsertionSort(vector<long> &u_list) {
	long valueToInsert, holePos;

	for (long i = 1; i < u_list.size(); ++i) {
		valueToInsert = u_list.at(i);
		holePos = i;
		while (holePos > 0 && valueToInsert < u_list.at(holePos - 1)) {
			u_list.at(holePos) = u_list.at(holePos - 1);
			holePos = holePos - 1;
		}
		u_list.at(holePos) = valueToInsert;
	}
}

vector<long> merge(vector<long> &firstList, vector<long> &secondList) {
	vector<long> finalSortedList;

	while (firstList.size() > 0 || secondList.size() > 0) {
		if (firstList.size() > 0 && secondList.size() > 0) {
			if (firstList.at(0) <= secondList.at(0)) {
				finalSortedList.push_back(firstList.at(0));
				firstList.erase(firstList.begin());
			}
			else {
				finalSortedList.push_back(secondList.at(0));
				secondList.erase(secondList.begin());
			}
		}
		else if (firstList.size() > 0) {
			finalSortedList.push_back(firstList.at(0));
			firstList.erase(firstList.begin());
		}
		else if (secondList.size() > 0) {
			finalSortedList.push_back(secondList.at(0));
			secondList.erase(secondList.begin());
		}
	}

	return finalSortedList;
}