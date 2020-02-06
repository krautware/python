#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10000
#define RANGE 20000

int middle_item(int b[], int j){
    int mitte, l;

    mitte = ((b[j - 1] < b[j]) ? (j - 1) : j);
    mitte = ((b[mitte] > b[j + 1]) ? mitte : ((b[j + 1] < b[l = ((j == mitte) ? (j - 1) : j)]) ? (j + 1) : l));

    return mitte;
}

int check_order(int vec[], int start, int end){

    int i, count = 0;

    for(i = start; i < end - 1; ++i){
        if(vec[i] > vec[i + 1]){
            count++;
        }
    }

    return count;
}

int b_sort(int vec[], int start, int end){
    int i, j, count = 0, r = end;

    while((end - start) > 0){
        for(i = start; i < end; ++i){
            if(vec[i] > vec[i + 1]){
                j = vec[i], vec[i] = vec[i + 1], vec[i + 1] = j;
                count++;
            }
        }
        --end;
    }  
    if((i = check_order(vec, start, r)) != 0){
        printf("Fehler in b_sort()\n");
    }
    
    return count;
}

int q_sort(int vec[], int start, int end){
    int l = start, r = end;
    int pivot, t, count = 0;
    int m = (end - start) >> 1;

    if(m > 4){ 
        m = middle_item(vec, m + start);
     /*   pivot = vec[m];
    }else{
        return b_sort(vec, l, r); */
    } else{m = m + start; }
    pivot = vec[m];
    
    printf("in q_sort(): start = %d, end = %d, pivot = %d m = %d\n", start, end, pivot, m);
    while(l < r){
        while(vec[l] < pivot){
            ++l;
        }
        while(vec[r] > pivot){
            --r;
        }
        if(l < r){
            count++;
            t = vec[l], vec[l] = vec[r], vec[r] = t;
            ++l, --r;
        }
    }

    if(l == r){
        if(vec[l] < pivot){
            ++l;
        }else{
            --r;
        }
    }

    if(l < end){
        count += q_sort(vec, l, end);
    }
    if(r > start){
        count += q_sort(vec, start, r);
    }
    return count;

}


int main(int argc, char **argv){

    int b[SIZE] = { 0 };
    int i, j, l, count = 0, ll = 0;
    time_t t = time(NULL);

    srandom(t);
    
    for(j = 0; j < SIZE; ++j){
        for(i = 0; i < SIZE; ++i){
            b[i] = random() % RANGE; 
        }

        l = q_sort(b, 0, SIZE - 1);

        if((l = check_order(b, 0, SIZE)) != 0){
            printf("Es gibt in Lauf %d %d Anordnungsfehler!\n", j, l);
            count++;
            ll += l;
        }
    }    

    printf("------------++++++++++++++------------\n");
    printf("Es gab in %d von %d Läufen insgesamt %d Anordnungsfehler\n", count, SIZE, ll);
}