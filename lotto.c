#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>

#define MAX_COMB 13983816
#define SUCCESS 0
#define FAILURE -1
#define LOTTO 49

int zliste[50] = {0};
int zl[LOTTO] = {0};
int gz[6] = {0};
int gz_resolv[6] = {0};
int comb[48][5] = {0};
int all_comb[MAX_COMB + 1] = {0};

typedef struct{
    int nr;
    int counter;
} Combination;




int n_over_k(int n, int k){

    uint64_t res = 1;
    int fak = 1;

    if((n < 0) || (k < 0))
        return 0;

    for(int i = 1; i <= k; ++i){
        res *= n--;
        fak *= i;
    }

    return (res/fak);
}

int my_random(void){
    static int last = RAND_MAX - (RAND_MAX % LOTTO);
    int res;
    while((res = rand()) > last)
        ;
    return (res % LOTTO) + 1;
}

int ziehung(void){
    int t, e;

    for(int z = 0; z < 6; ++z){
        while(zliste[t = my_random()])
            ;
        zliste[t] = 1;
        zl[t]++;
        zliste[0]++;
    }
    /* assert(zliste[0] == 6); */

    t = 1, e = 0;
    while ((t < 50) && zliste[0]){
        if(zliste[t]){
            gz[e++] = t;
            zliste[t] = 0;
            zliste[0]--;
        }
        ++t;
    }

    assert((zliste[0] == 0) && (e == 6));

    return SUCCESS;
}
int init_comb_vec(void){

    for(int i = 0; i < 48; ++i){
        for(int j = 0; j < 5; ++j){
            if(i < j)
                break;
            if((48 - i + j) < 5)
                continue;
            if(i == 0){
                comb[i][j] = n_over_k(48 - i, 5 - j);
            }else{
                comb[i][j] = comb[i - 1][j] + n_over_k(48 - i, 5 - j);
            }
        }
    }

    return SUCCESS;
}

int check_combination(void){
    int n = 0;
    int lastz = 0;
    int end_z2 = 45, end_z3 = 46, end_z4 = 47, end_z5 = 48, end_z6 = 49;;
    for(int z1 = 1; z1 <= gz[0]; ++z1){
        if(z1 == gz[0]){
            lastz = 1;
            end_z2 = gz[1];
        }
        for(int z2 = z1 + 1; z2 <= end_z2; ++z2){
            if((lastz == 1) && (z2 == gz[1])){
                lastz = 2;
                end_z3 = gz[2];
            }
            for(int z3 = z2 + 1; z3 <= end_z3; ++z3){
                if((lastz == 2) && (z3 == gz[2])){
                    lastz = 3;
                    end_z4 = gz[3];
                }
                for(int z4 = z3 + 1; z4 <= end_z4; ++z4){
                    if((lastz == 3) && (z4 == gz[3])){
                        lastz = 4;
                        end_z5 = gz[4];
                    }
                    for(int z5 = z4 + 1; z5 <= end_z5; ++z5){
                        if((lastz == 4) && (z5 == gz[4])){
                            lastz = 5;
                            end_z6 = gz[5];
                        }
                        for(int z6 = z5 + 1; z6 <= end_z6; ++z6)
                            n++;
                    }
                }
            }                            
        }                            
    }
    return n;
}

int calc_comb(void){
    int sum = 0;
    int j, u;

    for(int i = 0; i < 5; ++i){
        u = ((i > 0) ? gz[i - 1] - 1: 0);
        j = gz[i] - 2;
        if(j < 0)
            continue;
        if(u != 0)
            u = comb[u][i];
        sum += (comb[j][i] - u);
    }
    sum += (gz[5] - gz[4]);
    return sum;
}

int resolv_comb(int number){
    int temp = 0, sum = 0, j = 0, i, nr = number;

    if((number < 1) || (number > MAX_COMB))
        return FAILURE;
    for(i = 0; i < 5; ++i){
        while((comb[j][i]) < nr){
            ++j;
        }
        sum += comb[j - 1][i];
        temp = ((i < 4) ? comb[j][i + 1] : 0);
        nr -= (comb[j - 1][i] - temp);
        gz_resolv[i] = ++j;
    }
    gz_resolv[i] = gz_resolv[i - 1] + nr;
    /* assert(gz_resolv[i] < 50); */
    return SUCCESS;
}

int compare_comb(void){
    int res = 0;

    for(int i = 0; i < 6; ++i){
        if(gz[i] != gz_resolv[i]){
            res = i + 1;
            break;
        }
    }

    return res;
}

int print_comb(int *vec){
    int i;
    for(i = 0; i < 5; ++i){
        printf("%d, ", vec[i]);
    }

    printf("%d\n", vec[i]);

    return i;
}

int print_all_not_chosen(void){
    int j = 0;

    for(int i = 1; i <= MAX_COMB; ++i){
        if(all_comb[i] == 0){
            j++;
            printf("Kombination %d -> ", i);
            resolv_comb(i);
            print_comb(gz_resolv);
        }
    }

    return j;
}

Combination *print_most_chosen(int top){
    int i, j, k, bottom;
    Combination *p, temp;

    if((p = (Combination *)malloc(top * sizeof(Combination))) == NULL){
        printf("Fehler by malloc() in Zeile %d!\n", __LINE__);
        exit(1);
    }

    for(i = 0; i < top; ++i){
        p[i].nr = i + 1;
        p[i].counter = all_comb[i + 1];
    }

    for(i = 0; i < top - 1; ++i){
        for(j = i + 1; j < top; ++j){
            if(p[i].counter < p[j].counter){
                temp = p[j];
                p[j] = p[i];
                p[i] = temp;
            }
        }
    }

    bottom = top - 1;

    for(i = top + 1; i <= MAX_COMB; ++i){
        j = bottom;
        while(all_comb[i] > p[j].counter){
            j--;
            if(j < 0)
                break;
        }
        if(j < bottom){
            j++;
            for(k = bottom; k > j; --k){
                p[k] = p[k - 1];
            }
            p[j].nr = i;
            p[j].counter = all_comb[i];
        }
    }
    for(i = 0; i < top; ++i){
        printf("Kombination %d wurde %d mal gezogen! ", p[i].nr, p[i].counter);
        if(resolv_comb(p[i].nr) == SUCCESS){
            print_comb(gz_resolv);
        }
    }
}

int main(int argc, char **argv){
    time_t t;
    clock_t start_t, end_t;
    int not_chosen = MAX_COMB;
    int comb_nr, n, z = 0, i = 0, j, res, sum = 0;

    start_t = clock();
    init_comb_vec();
    /* for(i = 0; i < 48; ++i){
        printf("%2d -> ", i);
        for(j = 0; j < 5; ++j){
            printf("%9d ", comb[i][j]);
        }
        printf("\n");
    } */
    srand((unsigned) time(&t));
    /* resolv_comb(11071959);
    print_comb(gz_resolv); exit(0); 
    gz[0] = 11, gz[1] = 19, gz[2] = 27, gz[3] = 28, gz[4] = 42, gz[5] = 45;
    printf("Kombination -> %d\n", calc_comb());exit(0); */
    i = 0;
    while(not_chosen > 0){
        i++; 
        ziehung();
        if(resolv_comb(comb_nr = calc_comb()) == FAILURE){
            printf("Fehler bei resolv_comb()\n");
        }
        /* if(res = compare_comb()){
            printf("Abweichung in Ziehung %d in Zahl %d\n", i + 1, res);
        } */

        all_comb[comb_nr]++;
        if(all_comb[comb_nr] < 2){
            not_chosen--; 
        }else{
            z++;
            if(z == 1){
                printf("Erste Dublette nach %d Ziehungen!\n", i);
                print_comb(gz);
            }
        }
        
        
        if(i % 10000000 == 0){
            printf("Ziehung: %d, bisher %d verschiedene Kombinationen gezogen!\n", i, MAX_COMB - not_chosen);
            fflush(stdout);
            srand((unsigned)time(&t) + my_random());
        }
        
        
    }
    printf("Ziehung: %d, bisher %d verschiedene Kombinationen gezogen!\n", i, MAX_COMB - not_chosen);
    printf("Letzte Ziehung: ");
    print_comb(gz);
    (void) print_most_chosen(10);
    if((j = print_all_not_chosen()) != not_chosen){
        printf("Fehler bei not_chosen! Zurückgeliefert wird %d\n", j);
    } 
    for(i = 1; i < 50; ++i){
        printf("%2d: %4d\n", i, all_comb[i]);
    }

    res = 0;
    for(i = 1; i <= MAX_COMB; ++i){
        if(all_comb[i] == 1){
            res++;
            resolv_comb(i);
            print_comb(gz_resolv);
        }
    }

    printf("Es gibt %d Kombinationen, die nur einmal gezogen wurden!\n", res); 

    end_t = clock();
    printf("Total CPU clocks: %d\n", end_t - start_t);
    printf("Total CPU Time: %f\n", (double)(end_t - start_t) / CLOCKS_PER_SEC);
}