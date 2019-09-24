#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool fields_build = false;

unsigned long fields[64] = {0};

int build_fields(void){
    return 0;
}

bool check_board(int a, int b, int c, int d, int e){
    bool res = false;

    if(fields_build == false)
        build_fields();
    if((fields[a] | fields[b] | fields[c] | fields[d] | fields[e]) == UINT64_MAX)
        res = true;
    return res;
}

int print_board(int from, int til){
    return 0;
}

int main(int argc, char *argv[])
{
    int count = 0; 
    int hit = 0;
    int res;
    int hit_array[10000];

    for(int d1 = 0;d1 < 60; d1++){
        for(int d2 = d1 + 1; d2 < 61; d2++){
            for(int d3 = d2 + 1; d3 < 62; d3++){
                for(int d4 = d3 +1; d4 < 63; d4++){
                    for(int d5 = d4 + 1;d5 < 64; d5++){
                        count++;
                        if(check_board(d1, d2, d3, d4, d5)){
                            hit_array[hit] = count;
                            hit++;
                        }
                    }
                }
            }
        }
    }
    res += print_board(0, 63);
    printf("Zahl der Durchläufe: %d\n", count);
    exit(0);
}