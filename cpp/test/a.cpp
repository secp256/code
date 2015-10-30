#include <cstdio>
#include <stdlib.h>
#include <pthread.h>

int counter = 0;
void* routine(void *ptr);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

class NonCopyable
{
public:
    NonCopyable(){};
private:
    NonCopyable (NonCopyable const &); // private copy constructor
    NonCopyable & operator = (NonCopyable const &); // private assignment operator
};

class ScopeMutex:NonCopyable
{
public:
    ScopeMutex(pthread_mutex_t* mutex):mutex_(mutex){
        pthread_mutex_lock( mutex_ );
    }

    ~ScopeMutex(){
        pthread_mutex_unlock( mutex_ );
    }
private:
    pthread_mutex_t *mutex_;
};

int main(int argc, char *argv[])
{
    int rc1, rc2;
    pthread_t thread1, thread2;
    if( (rc1=pthread_create( &thread1, NULL, routine, NULL)) )
    {
        printf("Thread creation failed: %d\n", rc1);
    }

    if( (rc2=pthread_create( &thread2, NULL, routine, NULL)) )
    {
        printf("Thread creation failed: %d\n", rc1);
    }
    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);
}

void* routine(void *ptr)
{
    ScopeMutex scopeMutex(&mutex);
    counter++;
    printf("%d\n",counter);
}
