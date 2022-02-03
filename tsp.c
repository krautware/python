#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>
#include <inttypes.h>

#define MAX_STAEDTE 12
#define MAX_PATH_COUNTER 10
#define MIN_X 5
#define MAX_X 200 - MIN_X
#define MIN_Y 5
#define MAX_Y 200 - MIN_Y

typedef struct{
    /* x and y are the coordinates of the town,
       distance the distance of town to origin */
    int x, y;
    int distance; 
} Coordinate;

typedef struct{
    int max, min;
} Minmax;

int distance_matrix[MAX_STAEDTE][MAX_STAEDTE] = {0};
Coordinate towns[MAX_STAEDTE] = {0};
Minmax abs_max;
int tsp_path[MAX_STAEDTE + 1] = {0};
int min_path = INT_MAX;
int max_path = 0;
int min_path_list[MAX_PATH_COUNTER][MAX_STAEDTE + 1] = {0};
int min_path_counter = 0;


int my_random(int high, int low){
    int last = RAND_MAX - (RAND_MAX % high);
    int res;
    while((res = rand()) > last)
        ;
    return (res % high) + low;
}

int build_town_list(void){
    int i, j, r, lock;
    int x, y, d;

    for(i = 0; i < MAX_STAEDTE; ++i){
        lock = 1;
        while(lock){
            x = my_random(MAX_X, MIN_X);
            y = my_random(MAX_Y, MIN_Y);
            d = (int)(sqrt(x*x + y*y)) + 1;
            r = 0;
            for(j = 0; j < i; ++j){
                if(d == towns[j].distance){
                    if((x == towns[j].x) && (y == towns[j].y)){
                        r = 1;
                        break;
                    }
                }
            }
            if(r == 0){
                lock = 0;
            }
        }
        towns[i].x = x, towns[i].y = y, towns[i].distance = d;
    }

    return i;
}

int calc_distance(void){
    int d, i, j, x, y;

    for(i = 0; i < MAX_STAEDTE - 1; ++i){
        for(j = i + 1; j < MAX_STAEDTE; ++j){
            x = towns[i].x - towns[j].x;
            y = towns[i].y - towns[j].y;
            d = (int)(sqrt(y * y + x * x)) + 1;
            distance_matrix[i][j] = distance_matrix[j][i] = d;
        }
    }
    return j;
}

void print_dst_matrix(void){
    int i, j;
    char hr[86] = "----";

    for(i = 0; i < MAX_STAEDTE; ++i){
        strcat(hr, "----");
    }
    printf("     ");
    for(i = 0; i < MAX_STAEDTE; ++i){
        printf("%3d ", i);
    }
    printf("\n%s\n", hr);

    for(i = 0; i < MAX_STAEDTE; ++i){
        printf("\n");
        printf("%2d | ", i);
        for(j = 0; j < MAX_STAEDTE; ++j){
            printf("%3d ", distance_matrix[i][j]);
        }
        printf("  x = %3d, y = %3d, Distance = %3d", towns[i].x, towns[i].y, towns[i].distance);
    }
}

int calc_path(void){
    int i, sum = 0;

    for(i = 0; i < MAX_STAEDTE; ++i){
        sum += distance_matrix[tsp_path[i]][tsp_path[i + 1]];
    }
    if(sum < min_path){
        min_path = sum;
        min_path_counter = 0;
        for(i = 0; i <= MAX_STAEDTE; ++i){
            min_path_list[min_path_counter][i] = tsp_path[i];
        }    
    }
    if(sum == min_path){
        if(min_path_counter < MAX_PATH_COUNTER){
            for(i = 0; i <= MAX_STAEDTE; ++i){
                min_path_list[min_path_counter][i] = tsp_path[i];
            }
            min_path_counter++;
        }else{
            min_path_counter++;
        }     
    }
    if(sum > max_path){
        max_path = sum;
    }
    return sum;
}

uint64_t permute(int n, int *vec, int (*c)(void)){
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
                vec[l] = 0;
            }
           
            do{
                ++l;
                if(l > n){
                    l = 1;
                }
            }while(vec[l] != 0);
                    
            vec[l] = d;
            stack[d][1] = l;
            if(d == n){
                counter++;
                c();
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


uint64_t permute_rek(int n){
    static uint64_t counter = 0;
    int i;
    int d = n;
    int l = 0;

    
    ++l;
    while(d > 0){
        while((l < MAX_STAEDTE) && (tsp_path[l] != 0)){
            ++l;
        }
        if(tsp_path[l] == 0){
            tsp_path[l] = MAX_STAEDTE - n;
        }else{
            printf("Something has gone wrong in line %d! l = %d, d = %d\n", __LINE__, l, d);
            for(i = 0; i <= MAX_STAEDTE; ++i){
                printf("%d ", tsp_path[i]);
            }
            printf("\n");
            exit(1);
        }
        if(n > 1){
            permute_rek(n - 1);
        }else{
            counter++;
            calc_path();
        }    
        tsp_path[l++] = 0;
        --d;
    }
    return counter;
}


Minmax calc_abs_max(void){
    int i, j, d ,max, min;
    Minmax sum = {0, 0};

    for(i = 0; i < MAX_STAEDTE; ++i){
        max = 0, min = INT_MAX;
        for(j = 0; j < MAX_STAEDTE; ++j){
            if((d = distance_matrix[i][j]) > max){
                max = d;
            }
            if(d && (d < min)){
                min = d;
            }
        }
        sum.max += max;
        sum.min += min;
    }
    return sum;
}

uint64_t fak(int n){
    uint64_t p;

    for(p = 1; n > 1; --n){
        p *= n;
    }
    return p;
}

int main(int argc, char **argv){
    time_t t;
    clock_t start_t, end_t;
    uint64_t res, f;
    int i, j, h;

    srand((unsigned)time(&t));
    fflush(stdout);
    build_town_list();
    calc_distance();
    print_dst_matrix();
    abs_max = calc_abs_max();
    printf("\nAbsolute Max Distance: %d, absolute Min Distance: %d\n", abs_max.max, abs_max.min);
    for(h = 0; h < 2; ++h){
        start_t = clock();
        if(h < 1){
            printf("in permute_rek!\n");
            res = permute_rek(MAX_STAEDTE - 1);
        }else{
            res = permute(MAX_STAEDTE - 1, tsp_path, calc_path);
            
        }
        if(res != (f = fak(MAX_STAEDTE - 1))){
            printf("Something has gone wrong in permute! res = %" PRIu64 " f = %"  PRIu64 "\n", res, f);
        }
        printf("Es existieren %d kuerzeste Pfad mit Distanz %d\n", min_path_counter, min_path);
        for(int i = 0; i < min_path_counter; ++i){
            for(int j = 0; j <= MAX_STAEDTE; ++j){
                printf("%d ", min_path_list[i][j]);
            }
            printf("\n");
        }   
        printf("Laengste Distanz: %d\n", max_path);
        end_t = clock();
        printf("Total CPU clocks: %ld\n", end_t - start_t);
        printf("Total CPU Time: %f\n", (double)(end_t - start_t) / CLOCKS_PER_SEC);
        for(i = 1; i < MAX_STAEDTE; ++i){
            tsp_path[i] = 0;
        }
        for(i = 0; i < min_path_counter; ++i){
            for(j = 0; j <= MAX_STAEDTE; ++j){
                min_path_list[i][j] = 0;
            }
        }
        min_path_counter = 0;
        min_path = INT_MAX; 
        max_path = 0;
    }
}
