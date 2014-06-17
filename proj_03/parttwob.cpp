// CS3243 Operating Systems
// Summer 2014 
// Project 3: Forks and Threads
// Matthew Tinney and Shade Alabsa
// Date: 10 June 2014
// File: parttwob.cpp

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <iostream>
#include <cmath>
#include <sys/types.h>

using namespace std;

struct data{
	long numDarts;
	long hits;
};

long createThreads(long, long);
void * throwDart(void *);
double getPoint();
double calculatePi(long, long);
bool inside(double, double);

int main(){
	int i, numThreads[5] = { 1, 2, 4, 16, 64 };
	double pi;
	long hit, duration = 10000000;
	time_t start, end;

	srand(time(NULL));

	for (i = 0; i < 5; i++) {
		time(&start);

		hit = createThreads(duration, numThreads[i]);

		time(&end);

		pi = calculatePi(hit, duration);
		printf("Number of darts: %ld\n", duration);
		printf("Number of hits: %ld\n", hit);
		printf("Number of misses: %ld\n", (duration - hit));
		printf("Time elapsed was: %f\n", difftime(end, start));
		printf("PI ~= %f\n", pi);
	}

	return 0;
}

long createThreads(long numDarts, long numThreads){
	pthread_t threads[numThreads];
	data *returnThreads = (data *)malloc(sizeof(data)* numThreads);
	long hit = 0;
	int i;
	void *ret;

	if (!returnThreads){
		exit(-1);
	}

	for (i = 0; i < numThreads; i++){
		returnThreads[i].numDarts = numDarts / numThreads;
		if (pthread_create(&threads[i], NULL, &throwDart, &returnThreads[i]) != 0) {
			perror("pthread_create() error");

			if (returnThreads){
				free(returnThreads);
			}

			exit(1);
		}
	}

	for (i = 0; i < numThreads; i++){
		if (pthread_join(threads[i], &ret) != 0) {
			perror("pthread_join() error");

			if (returnThreads){
				free(returnThreads);
			}

			exit(3);
		}
		hit += returnThreads[i].hits;
	}

	if (returnThreads){
		free(returnThreads);
	}

	return hit;
}

void * throwDart(void *info) {
	int i, j;
	long hit = 0;
	j = ((data *)(info))->numDarts;
	double x, y;

	for (i = 0; i < j; i++) {
		x = getPoint();
		y = getPoint();

		if (inside(x, y)) {
			hit++;
		}
	}

	((data *)(info))->hits = hit;
	return NULL;
}

//Check to see if the point is inside the circle
//Center is at (5,5) with radius 5. The square is from 0-10 on the cartesian plane.
// In general a point is in a circle if it meets the condition below
// (x - center_x)^2 + (y - center_y)^2 < radius^2
// where x and y are the points coordinates and center_x and center_y are the circles center point coordinates and radius is the radius of the circle.
// If they are equal then it's a point on the circle which I will handle as being in the circle.
bool inside(double x, double y) {
	return ((pow(x, 2) + pow(y, 2) <= pow(1, 2)) ? true : false);
}

//Generate a number between 0 and 9 for  point.
double getPoint() {
	double p;
	p = (rand() % 20001 - 10000) / (double)10000;
	return p;
}

double calculatePi(long hits, long darts) {
	double pi;
	pi = ((4 * hits) / (double)darts);
	return pi;
}