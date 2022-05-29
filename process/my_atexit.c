#include <stdlib.h>
#include <stdio.h>

void my_exit1(void) 
{
    printf("my_exit1\n");
}

void my_exit2(void)
{
    printf("my_exit2\n");
}

int main(void)
{
    if (atexit(my_exit1) != 0) {
        printf("can't register my_exit1\n");
    }   

    if (atexit(my_exit2) != 0) {
        printf("can't register my_exit2\n");
    } 
    
    printf("main is done\n");
    return 0;
}