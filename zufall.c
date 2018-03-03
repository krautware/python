#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_PIPS  100
#define MAX_FREQUENCE 1000

int main (int argc, char *argv[]) {
    int i, j, m, max_cycles, max_pips, cast, res = 0;
    time_t t;
    int pips[100] = {0};
    int frequence[MAX_FREQUENCE] = {0};

    max_pips = (argc > 1 ? atoi(argv[1]) : 6);
    max_cycles = (argc > 2 ? atoi(argv[2]) : 10);
    
    
    if(max_pips > MAX_PIPS)
        max_pips = MAX_PIPS;

    int cycles[max_cycles];
   
    /* Intializes random number generator */
    srand((unsigned) time(&t));

  
    for( i = 0 ; i < max_cycles ; i++ ) {
        for(cast = j = 0; j < max_pips; ++j, ++cast){
            while(pips[m = (rand() % max_pips)] != 0){
                cast++;
            }
            pips[m] = 1;
        }
        res += cast;
        cycles[i] = cast;
        for(j = 0; j < max_pips; ++j)
            pips[j] = 0;
        
    }
    
    for(m = j = i = 0; i < max_cycles; ++i){
        if(j < cycles[i])
            j = cycles[i];
        if(cycles[i] == max_pips)
            m++;
        frequence[cycles[i]]++;
    }
     
    printf("Gesamtläufe: %d -- Durchschnitt: %f -- maximal: %d -- Minimum: %d\n", 
            res, (float)res/max_cycles, j, m);
    for(i = 0; i < MAX_FREQUENCE; ++i){
        if(frequence[i])
            printf("%d: %d\n", i, frequence[i]);
    }
    
    return(0);
}