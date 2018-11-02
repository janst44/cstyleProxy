#include <pthread.h>
Compile command:
c myprog.c -o myprof -lpthread

Thread creation:
int pthread_create(pthread_t *thread, pthread_attr_t *attr, void * (*start_routine)(void *), void * arg);

Thread Termination:
void pthread_exit(void* retval);

Watinting for treadds:
int pthread_join(pthread_t th, void **thread_return);


Hello World example:


#include <pthread.h>

void *helloworld(coid *vargp);

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, helloworld, NULL);//book does error handing in capital fuctions.
    //first arg is threadID , next Thread attributes, usually NULL, Next is thread routine, then thread arguments(must be void *p)
    pthread_join(tid, NULL);
    exit(0);
}

void *helloworld(void *argp)
{
    printf("Hello, world!\n");
    return NULL;
}

#p_thread_detach(pthread_self()); // the process doesn't need to clean up after this thread. It will be reaped automatically by the kernel



#for future lab:

struct ...
{
    int skt;
}

 Socket();
    Bind();
    Listen();

    while(1)
    {
        ns = accept()
        riorecv()
        riowrite()
        //struct ... *t = (struct ...*) malloc(sizeof(struct ...));
        //t->skt = ns;
        //pthread_create(&tid, NULL , HandleConnectionthread, (void*) t)//t is a struct pointer that was malloced
        //this assumes handleconnection cleaned up connection(free) and close socket
    }

forward declare handleconnection above then define here:
proxy.c: we do
copy code from tiny's handle connection call into my handle connection call. grab all headers and interpret that, then get the request stuff, instead of going and grabbing a stuff from a file like tiny we are going to go to a web server and get it then send the response.
