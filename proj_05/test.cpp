#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

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
}
