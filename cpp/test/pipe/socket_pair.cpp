#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>

#define BUF_SIZE    100

int main(void) {
    int fd[2];
    pid_t pid;
    char ch;
    char read_buf[BUF_SIZE];
    int count = 0;
    socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
    if ((pid = fork()) == 0) { // child process
        close(fd[0]);	
        dup2(fd[1], STDOUT_FILENO);	
        dup2(fd[1], STDIN_FILENO);	
        close(fd[1]);
        count = 0;
        for (; count < 10;) {
            ch = '0' + (0xff & count++);
            write(STDIN_FILENO, &ch, 1);

            memset(read_buf, '0', BUF_SIZE);
            if (read(STDOUT_FILENO, read_buf, BUF_SIZE) != -1) { /* 也可以用STDOUT_FILENO */
            //if (read(STDIN_FILENO, read_buf, BUF_SIZE) != -1) { /* 也可以用STDOUT_FILENO */
                fprintf(stderr, "child  rev \"%s\"\n", read_buf);
            }
        }
    } else { // parent process
        close(fd[1]);
        for (;count < 10; count++) {
            if (read(fd[0], &ch, 1) != -1) {
                fprintf(stderr, "parent rev \"%c\"\n", ch);
                write(fd[0], "OK!", 4);
            }
        }

        shutdown(fd[0], SHUT_WR);	
        close(fd[0]);	
    }
}
