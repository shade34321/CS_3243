// CS3243 Operating Systems
// Summer 2014 
// Project 3: Forks and Threads
// Matthew Tinney and Shade Alabsa
// Date: 10 June 2014
// File: parttwo.cpp

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <iostream>
#include <cmath>

using namespace std;

struct dartRatio {
    long duration;
    long* hit;
};

long legion(long, int);
void *throwDart(void*);
double getPoint();
double calculatePi(long, long);
bool inside(double, double);

int main() {
    int i, numThreads[3] = {1, 1, 1};
    double pi;
    long hit, duration[3] = {25000000, 500000000, 1000000000};
	time_t start, end;
    
    srand(time(NULL));
    
    for (i = 0; i < 3; i++) {
        time(&start);
        
        hit = legion(duration[i], numThreads[i]);
        
        time(&end);
        
        pi = calculatePi(hit, duration[i]);
        printf("Number of darts: %ld\n", duration[i]);
        printf("Number of hits: %ld\n", hit);
        printf("Number of misses: %ld\n", (duration[i] - hit));
        printf("Time elapsed was: %f\n", difftime(end, start));
        printf("PI ~= %f\n", pi);
    }
    

	return 0;
}

long legion(long duration, int numThreads) {
	pthread_t threads[numthreads];
    long hit;
    int returnThread[numThreads];
    struct dartRatio dr;
    dr.hit = &hit;
    
    if(numThreads == 1) {
        dr.duration = duration;
        return throwDart(duration);
    } else {
        int i;
        
        dr.duration = (duration/numThreads);
        
        for(i = 0; i < numThreads; i++){
            returnThread[i] = pthread_create( &threads[i], NULL, throwDart(), &dr);
        }
        
        for(i = 0; i < numThreads; i++) {
            pthread_join(threads[i], NULL);
        }
    }
	
	return hit;
}

void *throwDart(void* duration) {
    int i;
    long hit = 0;
	double x, y, pi;
    
	for (i = 0; i < duration; i++) {
		x = getPoint();
		y = getPoint();
		
		//printf("x: %f\n", x);
		//printf("y: %f\n", y);
        
		if (inside(x, y)) {
			//printf("Inside\n");
			hit++;
		}
	}
    
    duration->hitß
}

//Check to see if the point is inside the circle
//Center is at (5,5) with radius 5. The square is from 0-10 on the cartesian plane.
// In general a point is in a circle if it meets the condition below
// (x - center_x)^2 + (y - center_y)^2 < radius^2
// where x and y are the points coordinates and center_x and center_y are the circles center point coordinates and radius is the radius of the circle.
// If they are equal then it's a point on the circle which I will handle as being in the circle.
bool inside(double x, double y) {
	return ((pow(x, 2) + pow(y, 2) <= pow(100,2)) ? true : false);
}

//Generate a number between 0 and 9 for  point.
double getPoint() {
	//trying to get more points than 1,0 0,1 -1,0 0,-1
	return (((rand() % 201) - 100));
	//return rand();
}

//calculates Pi based on how many hits we have
//pi = 4 * hits / total_darts.
double calculatePi(long hits, long darts) {
	double p;
	//cout << "Hits: " << hits << endl << "Darts: " << darts << endl;
	p = ((4 * hits) / (double) darts);
	return p;
}
