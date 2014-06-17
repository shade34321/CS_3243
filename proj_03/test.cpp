#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <time.h>

using namespace std;

struct data {
	int numDarts;
	int hits;
};

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

	return 0;
}
