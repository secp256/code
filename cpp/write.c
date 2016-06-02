#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    const char *data = "abcdefghijkabcdefghijkabcdefghijkabcdefghijkabcdefghijkabcdefghijkabcdefghijk\n";
    int fd = open("test.txt",  O_RDWR | O_CREAT | O_APPEND, 0666);
    // int fd = open("test.txt",  O_RDWR | O_CREAT | O_LARGEFILE | O_APPEND, 0666);
    if (fd < 0) {
        printf("open file failed");
        return -1;
    }

    while (1) {
        int rc = 0;
        rc = write(fd, data, strlen(data));
        if (rc < 0) {
            printf("write failed, rc: [%d]\n", rc);
            break;
        }
        printf("write [%d] bytes\n", rc);
    }
    return 0;
}
