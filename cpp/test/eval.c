#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXLINE 1000

int main()
{
    const char *cmd = "awk 'BEGIN {printf \"%.4f\", (1*3 + 4*3) / 2}'";

    char line[MAXLINE];
    FILE *fd = popen(cmd, "r");
    if (fd == NULL) {
        printf("popen error\n");
        return -1;
    }
    if (fgets(line, MAXLINE, fd) == NULL) {
        printf("get output error\n");
        return -1;
    }
    double score = atof(line);
    printf("score: [%lf]\n", score);

    if (pclose(fd) != 0) {
        printf("pclose error");
        return -1;
    }
    return 0;
}
