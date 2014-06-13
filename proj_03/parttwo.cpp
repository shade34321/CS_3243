// CS3243 Operating Systems
// Summer 2014 
// Project 3: Forks and Threads
// Matthew Tinney and Shade Alabsa
// Date: 10 June 2014
// File: parttwo.cpp

// In general a point is in a circle if it meets the condition below
// (x - center_x)^2 + (y - center_y)^2 < radius^2
// where x and y are the points coordinates and center_x and center_y are the circles center point coordinates and radius is the radius of the circle.
// If they are equal then it's a point on the circle which I will handle as being in the circle.

#include <stdio.h>
#include <stdlib.h>
#include <pthreads.h>
#include <time.h>

using namespace std;

double calculatePi(int, int)
int getPoint();

int main(){
	srand(time(NULL));

	return 0;
}

//Generate a number between 1 and 10 for  point.
int getPoint() {
	return (rand() % 10 + 1);
}

//calculates Pi based on how many hits we have
double calculatePi(int hits, int darts) {
	//pi = 4 * hits / total_darts.	return ((4*hits)/darts);
}