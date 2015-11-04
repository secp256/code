#include <stdio.h>
#include <ctype.h>

#define MAXLINE 1000

int main()
{
    char line[MAXLINE];
    FILE *fpin;

    if ((fpin = popen("./myuclc", "r")) == NULL) {
        printf("popen error");
        return -1;
    }

    for (; ;) {
        fputs("prompt> ", stdout);
        fflush(stdout);
        if (fgets(line, MAXLINE, fpin) == NULL) {
            break;
        }
        if (fputs(line, stdout) == EOF) {
            printf("fputs error to pipe");
            return -1;
        }
    }

    if (pclose(fpin) == -1) {
        printf("pclose error");
        return -1;
    }
    putchar('\n');
    return 0;
}
