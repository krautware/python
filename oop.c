#include <stdio.h>
#include <stdlib.h>


typedef struct oop{
    int a;
    char * (*fnc) (struct oop *, int );
} oop;



int main(int argc, char *argv[]){
    printf("Größe von int = %ld\n", sizeof(int));
}