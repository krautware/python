#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define POPULATION 9000000
#define ANCESTORS 4001 
#define NUMBERS 3001

int numbers[NUMBERS] = { 0 };
int *prime;
int sel1[ANCESTORS], sel2[ANCESTORS];

int build_primes(void){
    int i, j, count = 0;

    for(i = 2;i < NUMBERS; ++i)
        numbers[i] = 1;

    for(i = 2; i < NUMBERS; ++i){
        if(numbers[i] == 0){
            continue;
        }
        count++;
        j = i;
        while((j += i) < NUMBERS){
            numbers[j] = 0;
        }
    }

    return count;
}

int prime_factors(int number, int factors[]){

    printf("In prime_factors!\n");
    return 0;
}



int main(int argc, char **argv){
    int i, primes, *p;
    
    primes = build_primes();
    if((p = prime = calloc(primes, sizeof(int))) == NULL){
        fprintf(stderr, "No more memory!\n");
        exit(1);
    }

    for(i = 2; i < NUMBERS; ++i){
        if(numbers[i]){
            *p++ = i;
        }
    }
    printf("Anzahl Primzahlen: %d\n", primes);
    printf("Letzte Primzahl = %d\n", prime[429]);

    exit(0);
}
