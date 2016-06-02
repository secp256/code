#include <syslog.h>

int main(int argc, char * argv[])
{
    openlog("testsyslog", LOG_CONS | LOG_PID, 0);
    syslog(LOG_USER | LOG_DEBUG, "syslog test generated in program [%s]\n", argv[0]);
    closelog();
    return 0;
}
