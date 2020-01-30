#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv){

int number = 4001;
int temp;

if(argc > 1)
    number = atoi(argv[1]);

if(number < 3){
    printf("%d ist prim\n", number);
    exit(0);
}

temp = number;
if(((temp >> 1) << 1) == number){
    printf("%d ist NICHT prim\n", number);
    exit(0);
}
for (int i = 3; i*i < number; i += 2){
    if((number % i) == 0){
        printf("%d ist NICHT prim\n", number);
        exit(0);
    }
}

printf("%d ist prim\n", number);
}