#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main (int argc, char *argv[]) {
    int i, n, l, j, res = 0;
    time_t t;
    int augen[100] = {0};
   

    n = (argc > 1 ? atoi(argv[1]) : 5);
    l = (argc > 2 ? atoi(argv[2]) : 5);
 
    int loops[n];
   
    /* Intializes random number generator */
    srand((unsigned) time(&t));

  
    for( i = 0 ; i < n ; i++ ) {
        int k = 1;
        for(j = 0; j < l; ++j){
            int m;
            while(augen[m = (rand() % l)] != 0){
                k++;
            }
            augen[m] = 1;
        }
        res += k;
        loops[i] = k;
        for(j = 0; j < l; ++j)
            augen[j] = 0;
        
    }
    
    for(j = i = 0; i < n; ++i){
        if(j < loops[i])
            j = loops[i];
    }
     
    printf("Gesamtläufe: %d -- Durchschnitt: %f -- maximal: %d\n\n", res, (float)res/n, j);
    return(0);
}