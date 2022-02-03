#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <inttypes.h>

#define FAILURE -1
#define SUCCESS 0


uint64_t nCk(int n, int k){
    uint64_t res = 1;

    if(n < k || n < 0 || k < 0)
        return 0;
    if(2 * k > n)
        k = n - k;
    if(n > 75 || k > 20)
        return 0;
                
    for(int j = 1; j <= k; ++j){
        res *= n--;
        res /= j;
    }

    return res;
}

int resolv_number(int nr, int n, int k, int *p){
    uint64_t sum, temp;
    int j, z;

    if(nCk(n, k) < nr)
        return FAILURE; 
    
    for(sum = 0, z = 1, j = 1; j <= k; j++){
        while((sum + (temp = nCk(n - z, k - j))) < nr){
            sum += temp;
            z++;
        }
        if(sum + temp == nr){
           /* Finish! The remaining drawings are set to highest values */
            for(*p++ = z, j++; j <= k; j++){
                *p++ = n - (k - j);
            }
        }else{
            /* sum + temp > nr */
            *p++ = z++;
        }

    }

    return SUCCESS;
}

uint64_t resolv_comb(int n, int k, int *p){
    uint64_t temp, sum;
    int j, t, z;

    for(j = 0, sum = 1, z = 0; j < k; j++){
        z++;
        t = *(p + j);
        while(z < t){
            sum += nCk(n - z, k - j - 1);
            z++;
        }
        printf("%d. Zahl = %d, sum = %" PRIu64 " z = %d\n", j + 1, t, sum, z);
    }

    return sum;
}


int main(int argc, char **argv){
    int n = 49, k = 6, t;
    uint64_t nr, z, z1, z2;
    int d1_vec[5], d2_vec[1];
    int d1, d2, j;

    while(1){
        printf("Bitte Wert fuer Ziehung eingeben: ");
        scanf("%d", &nr);
        d1 = (nr - 1) / (j = nCk(10, 1)) + 1;
        d2 = nr % j;
        if(d2 == 0)
            d2 = j;
        d2--;
        t = d2 == 0 ? 0 : 1;
        printf("j = %d, d1 = %d, d2 = %d, t = %d\n", j, d1, d2, t);

        if(resolv_number(d1, n, k, d1_vec) == FAILURE){
            printf("Fehler in resolv_number!\n");
            exit(1);
        }
        /* if(resolv_number(d2, 10, 1, d2_vec) == FAILURE){
            printf("Fehler in resolv_number2!\n");
            exit(1);
        }  */ 

        printf("Die Zahlen der %" PRIu64 ". Kombination lauten: ", nr);
        for(int i = 0; i < k; i++){
            printf("%d, ", d1_vec[i]);
        }   
        printf("\b\b\t / %d\n", d2);

        z1 = resolv_comb(n, k, d1_vec);
        z2 = resolv_comb(10, 1, &d2);
        z = j * (z1 - 1) + z2 + t;
        if(z == nr){
            printf("Okay: z = %" PRIu64 ", nr = %" PRIu64 " !\n", z, nr);
        }else{
            printf("Something went wrong: z = %d, nr = %d!\n", z, nr);
            printf("z1 = %" PRIu64 ", z2 = %" PRIu64 ", j = %d, t = %d!\n", z1, z2, j, t);
            printf("Difference: %d!\n", z - nr);
        }
    }
/*    while(1){
        printf("Bitte Wert fuer n eingeben: ");
        scanf("%d", &n);
        printf("Bitte Wert fuer k eingeben: ");
        scanf("%d", &k);
        printf("%d ueber %d ist %" PRIu64 "!\n", n, k, nCk(n, k));
    }
*/
}