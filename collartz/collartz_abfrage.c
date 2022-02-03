#include <stdio.h>
#include <stdlib.h>

char vec[1000] = {0};

char *dtob(int j){
    int l, r;
    char a;

    l = r = 0;

    while(j > 0){
        if((j&1)^1){
            vec[r] = '0';
        }else{
            vec[r] = '1';
        }
        j >>= 1;
        ++r;
    }
    vec[r--] = 0;
    while(r > l){
        a = vec[r];
        vec[r] = vec[l];
        vec[l] = a;
        --r, ++l;
    }
    return vec;
}

int main(int argc, char *argv[]){
    int zahl, i, j, up, down;

    if(argc != 2){
        printf("Parameter Zahl fehlt!\n");
        printf("Usage: collartz_abfrage zahl!\n");
        exit(1);
    }

    zahl = atoi(argv[argc - 1]);
    i = zahl; 
    up = down = 0;
    dtob(i);
    printf("Step: %3d Zahl = %9d Binär = %30s\n", up + down, i, vec);

    while(i > 1){
        if((i&1)^1){
            i >>= 1;
            ++down;
        }else{
            i *= 3, ++i;
            ++up;
        }

        dtob(i);
        
        printf("Step: %3d Zahl = %9d Binär = %30s\n", up + down, i, vec);
    }


}