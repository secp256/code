#include<stdio.h>

int main()
{
    char *line = NULL;
    size_t len;
    int n = getline(&line, &len, stdin);
    printf("size: [%d], line: [%s], len: [%d]\n", n, line, len);
    return 0;
}
