#include <stdio.h>
#include <string.h>
#include <time.h>

int main()
{
    time_t sec = time(NULL);
    struct tm t_m;
    bzero(&t_m, sizeof(t_m));
    gmtime_r(&sec, &t_m);
    char buf_date[32] = {0};
    snprintf(buf_date, 32, "%d-%02d-%02d", t_m.tm_year+1900, t_m.tm_mon+1, t_m.tm_mday);
    printf("%s\n", buf_date);
    return 0;
}
