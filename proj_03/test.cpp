#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <time.h>

using namespace std;

int main() {
    int i;
    double p;

    int j = 0;
    int k = 0;

    srand(time(NULL));

    for (i = 0; i< 10;i++){
        p = (rand() % 201 - 100) / 100.0;
        printf("%f\n", p);
        cout << p << endl;
        if(p == 100){
            k++;
        } else if(p == -100){
            j++;
        }
        
    }

    cout << "j: " << j << endl;
    cout << "k: " << k << endl;

    return 0;
}
