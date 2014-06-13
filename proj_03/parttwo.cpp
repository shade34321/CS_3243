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

int getPoint();
double calculatePi(int, int);
bool inside(int, int);

int main() {
	srand(time(NULL));
	int x, y, i, duration, hit;
	double pi;
	time_t start, end;
	duration = 25000;
	hit = 0;

	time(&start);

	for (i = 0; i < duration; i++) {
		x = getPoint();
		y = getPoint();
		
		if (inside(x, y)) {
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

	return 0;
}

//Check to see if the point is inside the circle
//Center is at (5,5) with radius 5. The square is from 0-10 on the cartesian plane.
// In general a point is in a circle if it meets the condition below
// (x - center_x)^2 + (y - center_y)^2 < radius^2
// where x and y are the points coordinates and center_x and center_y are the circles center point coordinates and radius is the radius of the circle.
// If they are equal then it's a point on the circle which I will handle as being in the circle.
bool inside(int x, int y) {
	return ((pow((x - 5), 2) + pow((y - 5), 2) <= pow(5, 2)) ? true : false);
}

//Generate a number between 1 and 10 for  point.
int getPoint() {
	return (rand() % 10 + 1);
}

//calculates Pi based on how many hits we have
//pi = 4 * hits / total_darts.
double calculatePi(int hits, int darts) {
	double p;	cout << "Hits: " << hits << endl << "Darts: " << darts << endl;	p = ((4 * hits) / (double) darts);	return p;
}