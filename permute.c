#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <inttypes.h>
#include <string.h>

#define MAX_SIZE 13
#define MAX_STACK 30
#define SUCCESS 0
#define FAILURE -1

int tsp[MAX_SIZE + 10] = {0};

uint64_t permute(int n, int *vec, int (*c)(uint64_t, int)){
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
                vec[l] = 0;
            }
            do{
                ++l;
                if(l > n){
                    l = 1;
                }
            } while(vec[l] != 0);

            vec[l] = d;
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

uint64_t permute_neu(int n, int *vec, int (*c)(uint64_t, int)){
    uint64_t counter = 0;
    int cycle[MAX_STACK];
    int last_position[MAX_STACK];
    int d, l, i;

    for(i = 1; i < MAX_STACK; ++i){
        cycle[i] = 0, last_position[i] = i;
    }

    d = 1;

    while(d > 0){
        if(cycle[d] <= n - d){
            cycle[d]++;
            if(vec[l = last_position[d]] == d){
                vec[l] = 0;
            }
            do{
                ++l;
                if(l > n){
                    l = 1;
                }
            } while(vec[l] != 0);

            vec[l] = d;
            last_position[d] = l;
            if(d == n){
                counter++;
                c(counter, n);
                vec[l] = 0;
                cycle[d] = 0;
                --d;
            }
            else{
                d++;
            }
        }
        else{
            cycle[d] = 0;
            /* vec[last_position[d]] = 0; */
            vec[l = last_position[d]] = 0;
            last_position[d] = ++l;
            --d;
        }

    }
    return counter;
}

int call_path(uint64_t nr, int ende){
    static uint64_t counter = 0;
    int i, j;
    int control[MAX_STACK];
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
    return res;
}

int main(int argc, char *argv[]){
    time_t t;
    clock_t start_t, end_t;
    int (*calc) (uint64_t, int);
    uint64_t (*p)(int, int *, int (uint64_t, int));
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
    h = 0;
    p = permute;
    do{
        start_t = clock();
        printf("\n%" PRIu64 " Permutationen!\n", perm = p(max_size - 1, tsp, calc));
        end_t = clock();
        printf("\n%" PRIu64 " Fehler in %s()\n", failure = calc(0, 0), (h == 0 ? "permute" : "permute_neu"));
        printf("\n%2.2f%% Fehlerrate", ((double)failure/perm)*100);
        printf("Total CPU clocks: %ld\n", end_t - start_t);
        printf("Total CPU Time: %f\n", (double)(end_t - start_t) / CLOCKS_PER_SEC);
        p = permute_neu;
        ++h;
    }while(h < 2);
}
