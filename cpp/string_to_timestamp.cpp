#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    const char *src = "2016-06-01 00:00:01";
    struct tm mytm; 
    memset(&mytm, 0, sizeof(struct tm));
    strptime(src, "%Y-%m-%d %H:%M:%S", &mytm);
    printf("[%ld]\n", mktime(&mytm));
    return 0;
}
