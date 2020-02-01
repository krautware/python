#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000000000
#define POPULATION 9000000
#define ANCESTORS 4001
#define DESCENDANTS 100000
#define FREQUENCY 10

typedef struct P {
    int ancestor;
    struct P *p;
} P;

int ancestors[ANCESTORS], counts[ANCESTORS];
P hash_vec[ANCESTORS];
int descendants[DESCENDANTS];
int ancestor_freq[20];

int my_random(void){
    static long max_member = RAND_MAX - (RAND_MAX % POPULATION);
    long member;

    while((member = random()) > max_member)
        ;
    return (member % POPULATION) + 1;
}

int isancestor(int ancestor){
    int hash = ancestor % ANCESTORS;
    P *p;

    if(hash_vec[hash].ancestor == 0)
        return 0;
    if(hash_vec[hash].ancestor == ancestor)
        return 1;
    p = hash_vec[hash].p;
    while(p){
        if(p->ancestor == ancestor){
            return 1;
        }
        else{
            p = p->p;
        }
    }

    return 0;
}

int check_descendants(void){
    int i, sum;

    for(sum = i = 0; i < ANCESTORS; ++i){
        sum += isancestor(my_random());
    }

    return sum;
}

void check_hash_vec(int frequency[]){

    int i, j;
    P *p;

    for(i = 0; i < ANCESTORS; ++i){
        if(hash_vec[i].ancestor == 0){
            frequency[0]++;
            continue;
        }
        
        if(hash_vec[i].p == NULL){
            frequency[1]++;
            continue;
        }
        
        p = hash_vec[i].p;
        j = 2;
        while((p = p->p)){
            j++;
        }
        frequency[j]++;

    }
}

int main(int argc, char** argv){

    int candidate;
    int i, sum, sum2, singular, hash;
    long max = -1;
    long min = RAND_MAX;
    int doublette = 0;

    P *p = NULL;
    int frequency[FREQUENCY] = {0};
    int frequency2[FREQUENCY] = {0};

    time_t t = time(NULL);

    srandom(t);

    for(i = 0; i < ANCESTORS; ++i){
        singular = 1;
    
        do{
            candidate = my_random();
            hash = candidate % ANCESTORS;
        
            if(hash_vec[hash].ancestor == 0){
                hash_vec[hash].ancestor = candidate;
                singular = 0;
            }   
            else{
                if(hash_vec[hash].ancestor == candidate){
                    doublette++;
                    continue;
                }else{
                    if((hash_vec[hash].ancestor != candidate) && (hash_vec[hash].p == NULL)){
                        if((hash_vec[hash].p = malloc(sizeof(P))) == NULL){
                            fprintf(stderr, "No more memory!\n");
                            exit(1);
                        }
                        hash_vec[hash].p->ancestor = candidate;
                        hash_vec[hash].p->p = NULL;
                        singular = 0;                        
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
        counts[hash]++;
    }

    for(i = 0; i < ANCESTORS; ++i){
        frequency[counts[i]]++;
    }

    check_hash_vec(frequency2);

    for(i = 0; i < ANCESTORS; ++i){
        if(max < ancestors[i])
            max = ancestors[i];
        if(min > ancestors[i])
            min = ancestors[i];
    }

    printf("Doubletten = %d\n", doublette);

/*
    printf("\nMIN Wert: %9.ld\nMAX Wert: %9.ld\n", min, max);
    
    
    for(sum = sum2 = i = 0; i < FREQUENCY; ++i){
        printf("%2d -> %4d --> %4d\t%2d -> %4d -->%4d ---> Differenz = %4d\n", 
                i, frequency[i], sum += frequency[i],
                i, frequency2[i], sum2 += frequency2[i],
                frequency[i] - frequency2[i]);
    }
    
    for(sum = i = 0; i < FREQUENCY; ++i){
        sum += frequency[i] * i;
    }

    printf("Gesamt: %d\n", sum);
    printf("Durchschnitt: %.2f\n", (float)sum / (ANCESTORS - frequency[0]));
*/
    t += my_random();
    srandom(t);

    for(i = 0; i < DESCENDANTS; ++i){
        descendants[i] = check_descendants();
    }

    for(i = 0; i < DESCENDANTS; ++i){
        ancestor_freq[descendants[i]]++;
    }

    for(sum = i = 0; i < 20; ++i){
        sum += ancestor_freq[i] * i;
    }

    printf("Gesamt: %d\n", sum);
    printf("Durchschnitt: %.2f\n", (float) sum / DESCENDANTS);

    for(sum = i = 0; i < 20; ++i){
        printf("%2d --> %5d --> %6d --> %6.2f%%\n", 
            i, ancestor_freq[i], sum += ancestor_freq[i],
            (float) ancestor_freq[i]/DESCENDANTS * 100);
    }
}
