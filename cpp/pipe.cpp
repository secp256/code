#include <iostream>
#include <signal.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

using namespace std;

void * listen_thread(void *arg);
static void sig_usr(int signo);
static void sig_pipe(int signo);
int pipe_fd_work[2];

int setnonblock( int fd )
{
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

int main(int argc, char** argv)
{
    if (pipe(pipe_fd_work) < 0)
    {
        cerr<<"open pipe_fd_work error:"<<strerror(errno)<<endl;
        return -1;
    }
    else
    {
        //if i close this read of pipe ,thread can't read it
        //because they share the same fd
        //close(pipe_fd_work[0]);
    }

    signal(SIGINT, sig_usr);
    pthread_t listen_pid;
    pthread_create(&listen_pid, NULL, listen_thread, NULL);

    int nwrited;
    char *writebuf = new char[1024];
    setnonblock(pipe_fd_work[1]);
    while(1)
    {
        cout<<"before write\n"<<endl;

        nwrited = write(pipe_fd_work[1], writebuf, 1024);
        cout<<"after write\n"<<endl;
        if (nwrited == -1)
        {
            cerr<<"write pipe error:"<<strerror(errno)<<endl;
        }
        else
        {
            cout<<"writen:"<<nwrited<<endl;
        }
    }

    pthread_join(listen_pid, NULL);

    return 0;
}

void * listen_thread(void *arg)
{
    char *buf = new char[2048];
    int nreaded;
    while(1)
    {
        nreaded = read(pipe_fd_work[0], buf, 2048);
        if (nreaded == -1)
            break;
        else
        {
            cout<<"readed:"<<nreaded<<endl;
        }
        sleep(10);
    }
    delete [] buf;
    buf = NULL;
    signal(SIGPIPE, sig_pipe);
}

static void sig_pipe(int signo)
{
    if (signo == SIGPIPE)
    {
        cout<<"progrom exit"<<endl;
        exit(1);
    }
}

static void sig_usr(int signo)
{
    if (signo == SIGINT)
    {
        cout<<"progrom exit"<<endl;
        exit(1);
    }
}

