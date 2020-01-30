#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000000000
#define POPULATION 9000000
#define ANCESTORS 4001
#define FREQUENCY 10

typedef struct P {
    int ancestor;
    struct P *p;
} P;

long my_random(void){
    static long max_member = RAND_MAX - (RAND_MAX % POPULATION);
    long member;

    while((member = random()) > max_member)
        ;
    return (member % POPULATION) + 1;
}

int main(int argc, char** argv){

int ancestors[ANCESTORS] = {0}, counts[ANCESTORS] = {0};
P hash_vec[ANCESTORS] = {0};

time_t t = time(NULL);

srandom(t);

long candidate;
int i, sum, singular, hash;
long max = -1;
long min = RAND_MAX;

P *p = NULL;
int frequency[FREQUENCY] = {0};

for(i = 0; i < ANCESTORS; ++i){
    singular = 1;
    
    do{
        candidate = my_random();
        hash = candidate % ANCESTORS;
        
        if(hash_vec[hash].ancestor == 0){
            hash_vec[hash].ancestor = candidate;
            counts[hash]++;
            singular = 0;
        }
        else{
            if(hash_vec[hash].ancestor == candidate){
                    continue;
            }else{
                if((hash_vec[hash].ancestor != candidate) && (hash_vec[hash].p == NULL)){
                    if((hash_vec[hash].p = malloc(sizeof(P))) == NULL){
                        fprintf(stderr, "No more memory!\n");
                        exit(1);
                    }
                    hash_vec[hash].p->ancestor = candidate;
                    hash_vec[hash].p->p = NULL;
                    counts[hash]++;
                }
                else{
                    p = hash_vec[hash].p;
                    while(p){
                        if(p->ancestor == candidate)
                            break;
                        if(p->p)
                            p = p->p;
                        else{
                            if((p->p = malloc(sizeof(P))) == NULL){
                                fprintf(stderr, "No more memory!\n");
                                exit(1);
                            }
                            p = p->p;
                            p->ancestor = candidate;
                            counts[hash]++;
                            p->p = NULL;
                            singular = 0;
                            p = p->p;
                        }
                    }        
                }
            }
        }
    }while(singular);
    
    ancestors[i] = candidate;
}

for(i = 0; i < ANCESTORS; ++i){
    frequency[counts[i]]++;
}

for(i = 0; i < ANCESTORS; ++i){
    max = ((max < ancestors[i]) ? ancestors[i] : max);
    min = ((min > ancestors[i]) ? ancestors[i] : min);
    printf("%d ", ancestors[i]);
}

printf("\nMIN Wert: %9.ld\nMAX Wert: %9.ld\n", min, max);

for(sum = i = 0; i < FREQUENCY; ++i){
    printf("%2d -> %4d --> %4d\n", i, frequency[i], sum += frequency[i]);
}
    
for(sum = i = 0; i < FREQUENCY; ++i){
    sum += frequency[i] * i;
}

printf("Gesamt: %d\n", sum);


}