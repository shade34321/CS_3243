#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <time.h>
#include <vector>

using namespace std;

struct data {
	int numDarts;
	int hits;
};

int importList(vector<int> &);

int main() {
	int i;
	double p;

	int j = 0;
	int k = 0;

	srand(time(NULL));

	for (i = 0; i< 10; i++){
		p = (rand() % 201 - 100) / 100.0;
		printf("%f\n", p);
		cout << p << endl;
		if (p == 100){
			k++;
		}
		else if (p == -100){
			j++;
		}

	}

	cout << "j: " << j << endl;
	cout << "k: " << k << endl;

	int numThreads = 10;
	int numDarts = 200;
	data *returnThreads = (data *)malloc(sizeof(data)* numThreads);

	for (i = 0; i < numThreads; i++){
		returnThreads[i].numDarts = numDarts / numThreads;
		returnThreads[i].hits = i;
	}

	for (i = 0; i < numThreads; i++){
		cout << returnThreads[i].numDarts << endl;
		cout << returnThreads[i].hits << endl;
	}

	vector<int> list;

	if (importList(list)){
		return -1;
	}

	for (vector<int>::const_iterator i = list.begin(); i != list.end(); ++i){
		cout << *i << ' ';
	}

	return 0;
}

int importList(vector<int> &list){
	int data;
	fstream f;

	f.open("numbers.txt", fstream::in);

	if (f.is_open()) {
		while (f >> data) {
			list.insert(list.begin(), data);
		}
	}
	else {
		cout << "Error importing file!" << endl;
		return -1;
	}

	return 0;
}