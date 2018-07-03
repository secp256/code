#include <stdio.h>

int main()
{
    short int x;
    char      x0, x1;
    x = 0x1122;

    x0 = ((char*) &x)[0];      //低地址单元
    x1 = ((char*) &x)[1];      //高地址单元

    if (x0 == 0x11)
        printf("little-endian \n");
    else
        printf("big-endian \n");
    return 0;
}
