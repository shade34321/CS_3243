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

void test(int);
double getPoint();
double calculatePi(int, int);
bool inside(int, int);

int main() {
	srand(time(NULL));

    test(2500);
    test(500000);
    test(1000000);

	return 0;
}

void test(int duration) {
	int i, hit;
	double x, y, pi;
	time_t start, end;
	hit = 0;

	time(&start);

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
	
	time(&end);

	pi = calculatePi(hit, duration);
	printf("Number of darts: %d\n", duration);
	printf("Number of hits: %d\n", hit);
	printf("Number of misses: %d\n", (duration - hit));
	printf("Time elapsed was: %f\n", difftime(end, start));
	printf("PI ~= %f\n", pi);
}

//Check to see if the point is inside the circle
//Center is at (5,5) with radius 5. The square is from 0-10 on the cartesian plane.
// In general a point is in a circle if it meets the condition below
// (x - center_x)^2 + (y - center_y)^2 < radius^2
// where x and y are the points coordinates and center_x and center_y are the circles center point coordinates and radius is the radius of the circle.
// If they are equal then it's a point on the circle which I will handle as being in the circle.
bool inside(int x, int y) {
	return ((pow(x, 2) + pow(y, 2) < pow(100,2)) ? true : false);
}

//Generate a number between 0 and 9 for  point.
double getPoint() {
	//trying to get more points than 1,0 0,1 -1,0 0,-1
	return ((rand() % 201 - 100));
	//return rand();
}

//calculates Pi based on how many hits we have
//pi = 4 * hits / total_darts.
double calculatePi(int hits, int darts) {
	double p;
	//cout << "Hits: " << hits << endl << "Darts: " << darts << endl;
	p = ((4 * hits) / (double) darts);
	return p;
}
