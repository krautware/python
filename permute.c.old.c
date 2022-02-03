#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <inttypes.h>
#include <string.h>

#define MAX_SIZE 12
#define SUCCESS 0
#define FAILURE -1

int tsp[MAX_SIZE + 10] = {0};

uint64_t permute(int n, int *vec, uint64_t (*c)(uint64_t, int)){
#define MAX_STACK 30
    uint64_t counter = 0;
    int stack[MAX_STACK][2];
    int d, l, i;

    for(i = 1; i < MAX_STACK; ++i){
        stack[i][0] = 0, stack[i][1] = i;
    }

    d = 1;

    while(d > 0){
        if(stack[d][0] <= n - d){
            stack[d][0]++;
            if(vec[l = stack[d][1]] == d){
                vec[l++] = 0;
            }
            if(l > n){
                l = 1;
            }
            while(vec[l] != 0){
                l++;
                if(l > n){
                    l = 1;
                }
            }
            vec[l] = d;
            /*
            if((vec[i = stack[d][1]] == d) && (i != l)){
                vec[i] = 0;
            } */
            stack[d][1] = l;
            if(d == n){
                counter++;
                c(counter, n);
                vec[l] = 0;
                stack[d][0] = 0;
                --d;
            }
            else{
                d++;
            }
        }
        else{
            stack[d][0] = 0;
            vec[l = stack[d][1]] = 0;
            stack[d][1] = ++l;
            --d;
        }

    }
    return counter;
}

uint64_t call_path(uint64_t nr, int ende){
    static uint64_t counter = 0;
    int i, j;
    int control[ende + 1];
    int res = SUCCESS;

    if(nr == 0){
        return counter;
    }
    for(i = 1; i <= ende; ++i){
        control[i] = 0;
    }
    for(i = 1; i <= ende; ++i){
        control[tsp[i]]++;
    }
    for(i = 1; i <= ende; ++i){
        if(control[i] != 1){
            /* Failure! */
            counter++;
            printf("\nFehler in %" PRIu64 ". Permutation: %d fehlt!", nr, i);
            for(j = 1; j <= ende; ++j){
                printf("%3d ", tsp[j]);
            }
            res = FAILURE;
            break;
        }
    }
    return (uint64_t) res;
}

int main(int argc, char *argv[]){
    uint64_t (*calc) (uint64_t, int);
    uint64_t perm, failure;
    int max_size = MAX_SIZE;
    int h = 0;
    calc = call_path;

    if(argc > 1){
        if((h = atoi(argv[argc -1])) != 0){
            max_size = h;
        }
    }
    for(int i = 0; i < argc; ++i){
        printf("Command line arg %2d: %s\n", i, argv[i]);
        if((h = atoi(argv[i])) != 0){
            printf("Command line arg %2d: %s\n", h, argv[i]);
        }
    }
    printf("\n%" PRIu64 " Permutationen!\n", perm = permute(max_size - 1, tsp, calc));
    printf("\n%" PRIu64 " Fehler in permute()\n", failure = calc(0, 0));
    printf("\n%2.2f%% Fehlerrate", ((double)failure/perm)*100);
}