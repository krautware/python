#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define MAX_SIZE 200000
#define FAILURE -1 

#define MY_RAND_MAX ((RAND_MAX << 16) + RAND_MAX)

/* GLOBAL */
int debug = 0;
int vector[MAX_SIZE];
int hits[MAX_SIZE];
   int failures[MAX_SIZE];

int my_random(int start, int end){
    int last = MY_RAND_MAX - (MY_RAND_MAX % end);
    int res;

    do{
        res = rand();
        res <<= 4;
        res += rand();
    }while(res > last);
    return ((res % (end - start + 1)) + start);
}

int build_vector(int *v, int len, int start, int width){
    int i;
    int value = start;
    int p;
    int span = start + width;

    p = 0;
    for(i = 0; i < len; ++i){
        v[i] = p;
        v[i] += my_random(value, span);
        p = v[i];
    }
    
    return i;
}

int linear_search(int *v, int item, int start, int end){
    int mid;
    int counter = 0;
    uint64_t temp;

    counter = 0;
    while((v[start] < item) && (v[end] > item)){
        counter++;
        temp = item - v[start];
        temp *= (end - start);
        temp /= (v[end] - v[start]);
        /* mid = start + ((item - v[start]) * ((end - start) / (v[end] - v[start]))); */
        mid = start + temp;
        if(v[mid] == item){
            return counter;
        }
        if(v[mid] > item){
            end = mid - 1;
        }
        else{
            start = mid + 1;
        }
    }

    counter++; 
    
    if(v[start] == item){
        return counter;
    }
    if(v[end] == item){
        return counter;
    }

    return -counter;
}

int seq_search(int *v, int item, int len){
    int i;

    for(i = 0; i < len; ++i){
        if(v[i] == item){
            return i;
        }
    }
    return FAILURE;
}

int createSearchItems(int *v, int *h, int *f, int len){
    int i, c, j;
    int t, d = 0;

    for(i = 0; i < len; ++i){
        t = 0;
        do{
            t++;
            c = my_random(0, len - 1);
            j = (i > 0) ? i - 1 : 0;
        }while(seq_search(h, c, i) >= 0);

        if(t > 1){
            d++;
        }
        h[i] = c;
        f[i] = v[i];
        f[i]++;
    }
    printf("In createSearchItems: %d mal musste mehrfach gezogen werden!\n", d);
    return i;
}

int find_double(int *h, int len){
    int i, j, c, max = 0;
    int d;
    int error = 0;

    for(i = 0; i < len - 1; ++i){
        c = 1;
        for(j = i + 1; j < len; ++j){
            if(h[i] == h[j]){
                c++;
            }
        }
        if(c > 1){
            /* printf("Die Zahl %d taucht %d mal auf!\n", h[i], c);*/
            error++;
            if(c > max){
                max = c;
                d = h[i];
            }
        }
    }
    if(max > 0)
        printf("Maximale Doublette %d tritt  %d mal auf\n", d, max);
    return error;
}

int main(int argc, char **argv){
    
    int start = 5;
    int width = 20;
    int i, v;
    int res = -1;
    int found = 0;
    int not_found = 0;
    int sum = 0;
    int min = MY_RAND_MAX, max = 0;
    time_t t;
    clock_t start_t, end_t, all_start_t, all_end_t;
    int frequency_hit[10] = { 0 };
    int frequency_not_found[10] = { 0 };

    all_start_t = clock();
    srand((unsigned) time(&t));

    build_vector(vector, MAX_SIZE, start, width);

    (void) createSearchItems(vector, hits, failures, MAX_SIZE);
    min = MY_RAND_MAX, max = -1;
    for(i = 0; i < MAX_SIZE; ++i){
        if(hits[i] < min){
            min = hits[i];
        }
        if(hits[i] > max){
            max = hits[i];
        }
    }
    printf("Nach createSearchItems()-Rückkehr! Min = %d, Max = %d\n", min, max);
    
    printf("Der hitsvektor enthält %d Doubletten!\n", find_double(hits, MAX_SIZE));
    for(i = 0; i < MAX_SIZE; ++i){
        hits[i] = vector[hits[i]];
    }

    start_t = clock();
    debug = 0;
    
    for(i = 0; i < MAX_SIZE; ++i){
        v = hits[i];
        if((res = linear_search(vector, v, 0, MAX_SIZE - 1)) > 0){
            found++;
            sum += res;
            frequency_hit[res]++;
        }
    }
    end_t = clock();

    printf("%d nicht gefunden! Dauer %f!\n", (i - found), (double)(end_t - start_t)/CLOCKS_PER_SEC);
    printf("Durchschnittliche Suchschritte bei Erfolg: %f\n", (double) sum / found);
    printf("Frequency of required steps!\n");
    for(i = 0; i < 10; ++i){
        printf("%d steps: %d occurencies\n", i, frequency_hit[i]);
    }
    
    sum = 0;
    printf("And now searching the failures!\n");
    start_t = clock();
    for(i = 0; i < MAX_SIZE; ++i){
        v = failures[i];
        if((res = linear_search(vector, v, 0, MAX_SIZE - 1)) < 0){
            not_found++;
            sum += res;
            res = -res;
            frequency_not_found[res]++;
        }
    }
    sum = -sum;
    end_t = clock();

    printf("%d gefunden! Dauer %f!\n", (i - not_found), (double)(end_t - start_t)/CLOCKS_PER_SEC);
    printf("Durchschnittliche Suchschritte bei Erfolg: %f\n", (double) sum / not_found);
    printf("Frequency of required steps!\n");
    for(i = 0; i < 10; ++i){
        printf("%d steps: %d occurencies\n", i, frequency_not_found[i]);
    }
    all_end_t = clock();

    printf("Gesamtdauer: %f!\n", (double)(all_end_t - all_start_t)/CLOCKS_PER_SEC);


}


