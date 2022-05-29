#include <unistd.h>
#include <stdio.h>

int main()
{
    char *str = getlogin();
    printf("%s\n", str);
    return 0;
}