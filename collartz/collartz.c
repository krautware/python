#include <stdio.h>
#include <stdlib.h>

#define MAX_COLLARTZ 1000000
#define MAX_SEQUENCE 2000
#define MAX_ITERATE 100000

typedef struct{
    int nr;
    int steps;
    int ups;
    int downs;
    int max;
    int max_odd;
} COLLARTZ;

typedef struct{
    int nr;
    int steps;
    int ups;
    int max;
} C_SEQUENCE;

COLLARTZ collartz[MAX_COLLARTZ] = {0};
C_SEQUENCE c_sequence[MAX_SEQUENCE] = {0};

int top_collartz[100] = {0};
int tp = -1;

int main(int argc, char *argv[]){

    int i, j , k, m, n, o, top;
    int l, max, max_odd, g_max, i_max, ups, downs, steps;
    COLLARTZ *p;
    C_SEQUENCE blank;

    blank.nr = blank.ups = blank.max = 0;

    collartz[0].nr = 1;
    collartz[0].max_odd = collartz[0].max = 1;

    g_max = 0;
    for(top = -1, i = 3; i < MAX_ITERATE; i += 2){
        l = i;
        max_odd = i;
        max = ups = downs = steps = 0;
        while(l >= i){
            if(l == i){
                if(steps == 0){
                    while(top >= 0){
                        c_sequence[top--] = blank;
                    }
                }
                else{
                    printf("Number %d occured second time!\n", i);
                    exit(1);
                }
            }
            else{ /* l != i */
                /* printf("in else-zweig: l != i\n"); */
                /* lookup whether l is already in chain */
                j = top;
                while(j >= 0){
                    if(l == c_sequence[j--].nr){
                        printf("Number %d occured second time!\n", l);
                        exit(1);
                    }
                }
                /* l not in chain, that means its new */
                ++top;
                c_sequence[top].nr = l, c_sequence[top].steps = steps, 
                c_sequence[top].ups = ups, c_sequence[top].max = max;
                if(l > max_odd){
                    max_odd = l;
                } 
            }

            l *= 3, ++l;
            ++steps, ++ups;

            if(l > max){
                max = l;
            }

            while((l&1)^1){
                l >>= 1;
                ++steps, ++downs;
            }
        } /* while(l >= i) */

        j = l >> 1, k = i >> 1;
        collartz[k].nr = i;
        collartz[k].steps = collartz[j].steps + steps;
        collartz[k].ups = collartz[j].ups + ups;
        collartz[k].downs = collartz[j].downs + downs;
        collartz[k].max = collartz[j].max > max ? collartz[j].max : max;
        collartz[k].max_odd = collartz[j].max_odd > max_odd ? collartz[j].max_odd : max_odd;
        
        /* process items in the c_sequence */
        m = top;
        while(top >= 0){
            j = c_sequence[top].nr >> 1;
            if(j >= MAX_COLLARTZ){
                --top;
                continue;
            } 
            collartz[j].nr = c_sequence[top].nr;
            collartz[j].steps = collartz[k].steps - c_sequence[top].steps;
            collartz[j].ups = collartz[k].ups - c_sequence[top].ups;
            collartz[j].downs = collartz[j].steps - collartz[j].ups;

            n = top, o = 0;
            while(n <= m){
                if(c_sequence[n].nr > o){
                    o = c_sequence[n].nr;
                }
                ++n;
            }
            
            collartz[j].max_odd = o > collartz[l >> 1].max_odd ? o : collartz[l >> 1].max_odd;
            collartz[j].max = 3 * collartz[j].max_odd + 1;
            --top;
        }
        top = m;
        if(g_max < collartz[k].max){
            g_max = collartz[k].max;
            i_max = i;
        }
        /* Insert number in top100 if possible */
        if (tp == -1){
            ++tp;
            top_collartz[tp] = k;
        }else{
            j = tp;
            while((j >= 0) && (collartz[k].steps > collartz[top_collartz[j]].steps)) {
                --j;
            }
            if(j == tp){
                if(tp < 99){
                    top_collartz[++tp] = k;
                }
            }else{
                m = tp < 99 ? tp : 98;
                while(m > j){
                    top_collartz[m + 1] = top_collartz[m];
                    --m;
                } 
                top_collartz[++m] = k;
            }
            if(tp < 99){
                ++tp;
            }
        }
    } /* i-loop */

    printf("Groesster erreichter Wert: %d bei Nummer %d\n", g_max, i_max);
    for(i = 0; i < 100; ++i){
        j = top_collartz[i];
        /* p = &collartz[j]; */
        p = collartz + j;
        printf("Number %6d - Steps %4d - Ups %3d - Downs %4d - Max %10d Max_Odd %9d\n",
                p->nr, p->steps, p->ups, p->downs, p->max, p->max_odd);
    }

    for(i = 0; i < MAX_ITERATE; ++i){
        p = collartz + i;
        if(p->nr == 0) continue;
        printf("Number %6d  ->  Steps %4d  ->  Ups %3d  ->  Downs %4d  ->  Max %10d  ->  Max_Odd %9d\n",
                p->nr, p->steps, p->ups, p->downs, p->max, p->max_odd);
    }

}