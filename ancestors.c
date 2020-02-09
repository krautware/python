#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "qsort.h"

#define POPULATION 9000000
#define ANCESTORS 4001
#define NUMBERS 3001
#define MAX_PRIM_FACTORS 100

int numbers[NUMBERS] = { 0 };

typedef struct primes {
    int *prime;
    int count;
} PRIMES;
PRIMES prime;

typedef struct factorlist {
    int liste[4000];
    int index;
} FACTORLIST;
FACTORLIST v1, v2;

int sel1[ANCESTORS], sel2[ANCESTORS];
int *sel1_factors[ANCESTORS];
int *sel2_factors[ANCESTORS];
int factors1[20000] = { 0 };


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

char *get_factor_list(int *factor_list){
    static char s[1000] = { 0 };
    char *t = s + 2;

    strcpy(s, "[ ");
    if(factor_list == NULL){
        strcat(s, "- ]");
        return s;
    }
    
    while(*factor_list != 0){
        t += sprintf(t, "%d, ", *factor_list++);
    }

    strcpy(t - 2, " ]");
    
    return s;
}

int get_prime_factors(int number, int factors[]){
    int i, j;

    for(i = j = 0; (i < prime.count) && (number > 1); ++i){
        while((number % prime.prime[i]) == 0){
            number /= prime.prime[i];
            factors[j++] = prime.prime[i];
        }
    }
    if(number > 1){
        factors[j++] = number;
    }
    factors[j] = 0;
    return j;
}

int append(FACTORLIST *f, int *p){
    int *i = p;

    while(*p){
        f->liste[f->index++] = *p++;
        /* printf("Übertragung: index = %d\n", f->index); */
    }

    return (p - i);
}


int factorize(int sel[], int *factor_vec[], FACTORLIST *factor_list){
    int prime_factors[MAX_PRIM_FACTORS];
    int i, j, k, *p, count = 0;

    for(i = 0; i < ANCESTORS; ++i){
        if(sel[i] == 1){
            continue;
        }
        count += j = get_prime_factors(sel[i], prime_factors);
        if(( p = factor_vec[i] = calloc(j + 1, sizeof(int))) == NULL){
            fprintf(stderr, "No more memory in function factorize!\n");
            exit(1);
        }
        for(k = 0; k < j; ++k){
            *p++ = prime_factors[k];
        }
        *p = 0;
        append(factor_list, factor_vec[i]);
    }
    return count;
}


int how_many_primes(int start, int end){
    int i, j, k = 0;

    for(i = start; i <= end; ++i){
        for(j = 0; j < prime.count; ++j){
            if((i % prime.prime[j]) == 0){
                break;
            }
        }
        if(j >= prime.count){
            k++;
        }
    }
    return k;
}

int reduce(int sel[], int start){
    int i, j, k, d, p_factors;
    int prime_factors[MAX_PRIM_FACTORS];

    for(i = ANCESTORS; i > 1; --i){
            d = start % i;
            /* printf("i = %d -> sel[%d] = %d!\n", i, d, sel[d]); */
            if((sel[d] % i) == 0){
                sel[d] /= i;
            }else{
                while((d += i) < ANCESTORS){
                    if((sel[d] % i) == 0){
                        sel[d] /= i;
                        break;
                    }
                }
                if(d >= ANCESTORS){
                    p_factors = get_prime_factors(i, prime_factors);
                    
                    assert(p_factors > 0);
                    
                    d = p_factors;
                    for(k = 0; k < ANCESTORS; ++k){
                        if(d == 0){
                            break;
                        }
                        for(j = 0; j < p_factors; ++j){
                            if(prime_factors[j] == 0){
                                continue;
                            }
                            if((sel[k] % prime_factors[j]) == 0){
                                sel[k] /= prime_factors[j];
                                prime_factors[j] = 0;
                                d--;
                            }
                        }
                    }
                    assert(d == 0);
                }
            }
    }

    return 0;
}

int shorten(FACTORLIST *factor_list){
    int i, j, k, *p, *r = NULL, count = 0;

    p = factor_list->liste;
    j = factor_list->index;

    for(i = k = 0; i < j; ++i){
        while(*p == 1){
            ++p, ++k;
        }
        if(k){
            if(r == NULL){
                r = p - k;
            }                
            count += k;
            while(*p > 1){
                *r++ = *p++;
            }
            j -= k, k = 0;
        }
    }
    assert(j + count == factor_list->index);
    for(i = j; i < factor_list->index; ++i){
        factor_list->liste[i] = 0;
    }
    factor_list->index -= count;
    return count;
}

int truncate(void){
    int i, j, k, p, count = 0;
    int *r, *s;

    if(v1.liste[v1.index - 1] < v2.liste[v2.index - 1]){
        i = v1.index, r = v1.liste;
        j = v2.index, s = v2.liste;
    } else{
        i = v2.index, r = v1.liste;
        j = v1.index, s = v2.liste;

    }

    for(k = 0; k < i; ++k){
        p = *r;
        while(*s < p){
            s++;
        }
        if(*s == p){
            *r++ = 1;
            *s++ = 1;
            count++;
        }else{
            r++;
        }
    }

    /* Now shorten the list */
    if(((i = shorten(&v1)) == count) && ((j = shorten(&v2)) == count)){
        return count;
    }else{
        return -1;
    }
    
}



int main(int argc, char **argv){
    int i, j, k, primes, *p;
    int primesOfSel1, primesOfSel2;
    uint64_t res1 = 0;
    uint64_t res2 = 0;
    FACTORLIST *fp;

    v1.index = v2.index = 0;

    prime.count = primes = build_primes();
    if((p = prime.prime = calloc(primes, sizeof(int))) == NULL){
        fprintf(stderr, "No more memory!\n");
        exit(1);
    }

    for(i = 2; i < NUMBERS; ++i){
        if(numbers[i]){
            *p++ = i;
        }
    }
    
    i = POPULATION - ANCESTORS + 1, j = POPULATION;

    printf("It exists %d primes between %d and %d!\n",
            how_many_primes(i, j), i, j);
    
    i = POPULATION - 2 * ANCESTORS + 1, j = POPULATION - ANCESTORS;
    printf("It exists %d primes between %d and %d!\n",
            how_many_primes(i, j), i, j);

    for(j = POPULATION, k = POPULATION - ANCESTORS, i = 0; 
        i < ANCESTORS; ++i, --j, --k){
        sel1[i] = j, sel2[i] = k;
    }
    
    reduce(sel1, POPULATION);
    reduce(sel2, POPULATION - ANCESTORS);

    /*How many 1-items? */

    for(i = j = k = 0; i < ANCESTORS; ++i){
        if(sel1[i] == 1){
            j++;
        }else{
            res1 += sel1[i];
        }             
        
        if(sel2[i] == 1){
            k++;
        }else{
            res2 += sel2[i];
        }            
    }

    fprintf(stderr, "sel1 hat %4d 1ner, sel2 hat %4d 1ner!\n", j, k);
    fprintf(stderr, "sel1 hat Summe %llu, sel2 hat Summe %llu!\n", res1, res2);

    primesOfSel1 = factorize(sel1, sel1_factors, &v1);
    primesOfSel2 = factorize(sel2, sel2_factors, &v2);

    printf("In sel1 bleiben %d Primzahlfaktoren!\n", primesOfSel1);
    printf("In v1 sind %d Faktoren enthalten!\n", v1.index);
    printf("In sel2 bleiben %d Primzahlfaktoren!\n", primesOfSel2);
    printf("In v2 sind %d Faktoren enthalten!\n", v2.index);

    printf("Letzter Eintrag in v1 = %d\n", v1.liste[v1.index]);
    printf("Letzter Eintrag in v2 = %d\n", v2.liste[v2.index]);
    q_sort(v1.liste, 0, v1.index - 1);
    q_sort(v2.liste, 0, v2.index - 1);

    for(i = 0; i < 100; ++i)
        printf("%d, ", v1.liste[i]);

    printf("\n");

    for(i = 0; i < 100; ++i)
        printf("%d, ", v2.liste[i]);
        
    printf("\n");

    j = truncate(); 

    if(j != -1)
        printf("Es gab %d Kürzungen\n", j);
    else
    {
        printf("something has gone terribly wrong: j = %d\n", j);
    }

    printf("v1 enthält %d, v2 %d Primzahlfaktoren\n", v1.index, v2.index);  
    
    /* for(i = 0; i < ANCESTORS; ++i){
        fprintf(stderr, "sel1[%4d] = %9d --> %s\n",
                i, sel1[i], get_factor_list(sel1_factors[i]));
        fprintf(stderr, "\t\t\t\t\t\t\tsel2[%4d] = %9d --> %s\n",
                i, sel2[i], get_factor_list(sel2_factors[i]));
    } */  
    j = 2;
    fp = &v1;
    while(j){
        for(i = 0; i < fp->index; ++i){
            fprintf(stderr, "%7d, ", fp->liste[i]);
            if((i + 1) % 12 == 0)
                fprintf(stderr, "\n");
        }
        fprintf(stderr, "\n--------++++++++++++------------\n");
        fp = &v2;
        --j;
    }   
    exit(0);
}
