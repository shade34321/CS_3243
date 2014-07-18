#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <iomanip>
using namespace std;

struct t{
    int a;
    int b;
    int c;
};

void testMem(int start, int *p){

    if(p){
        cout << "P has a value " << *p << endl;
    } else {
        cout << "p has no value " << endl;
    }
}

int main(){
    int a[5];
    int b[10];

    t alpha[4];

    t *x;
    x = (t *)malloc(sizeof(t));
    x->a = 7;
    x->b = 8;
    x->c = 9;

    alpha[3] = *x;

    cout << alpha[3].a << endl;
    cout << alpha[3].b << endl;
    cout << alpha[3].c << endl;

    fill_n(a, 5, 1);
    fill_n(b, 10, 5);
  
    for(int i = 0; i < 10; i++){
        cout << b[i] << endl;
    } 
    
    memcpy(&b[4],a, sizeof(int)*5);

    for(int i = 0; i < 10; i++){
        cout << b[i] << endl;
    }

    testMem(a[0], NULL);
    testMem(a[0], 0);
    testMem(a[0], b); 

	char output[80];
	memset(output, 0, 80);
	sprintf(output, "QUANTA ELAPSED: %d", a[0]);
	printf("%-25s\n", output);
	sprintf(output, "MEMORY: %d b", a[0]);
	printf("%-25s", output);
	sprintf(output, "USED: %d b", a[0]);
	printf("%-25s", output);
	sprintf(output, "FREE: %d b (%.2f%%)", a[0], a[0]);
	printf("%-25s\n", output);
	sprintf(output, "PROCESS: %d b (%.2f%%)", a[0], a[0]);
	printf("%-25s", output);
	sprintf(output, "LOADED: %d b (%.2f%%)", a[0], a[0]);
	printf("%-25s", output);
	sprintf(output, "UNLOADED: %d b (%.2f%%)", a[0], a[0]);
	printf("%-25s\n", output);

	/*
	cout << "QUANTA ELAPSED: " << a[0] << endl;
	
	cout << "MEMORY: " << a[0] << setw(5) << "b ";
	cout << "USED: " << a[0] << setw(5) << "b (" << setprecision(2) << a[0] << "%) ";
	cout << "FREE: " << a[0] << "b (" << setprecision(2) << a[0] << "%) " << endl;

	cout << "PROCESSES: " << setw(5) << a[0] << "LOADED: " << a[0] << "(" << fixed << setprecision(2) << a[0] << setw(15) << "%) ";
	cout << "UNLOADED: " << setw(5) << a[0] << " (" << fixed << setprecision(2) << a[0] << "%) " << endl;
	*/
}
