#include <stdio.h>
#include <time.h>       /* time_t, struct tm, time, localtime, strftime */

int main ()
{
  time_t rawtime;
  time (&rawtime);
  printf("[%lu]\n", rawtime);

  struct tm * timeinfo;
  timeinfo = localtime (&rawtime);

  char buffer [80];
  strftime (buffer,80,"Now it's %I:%M%p.",timeinfo);
  puts (buffer);

  return 0;
}
