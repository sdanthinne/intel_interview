#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <signal.h>

Queue q;

struct Node 
{
    // struct describing the node of the queue 
    int pid;
    
};

struct Queue 
{
    int front, back, len,totalsize;
    Node * array;
};

void enqueue(Queue * queue, int pid) 
{
    
    (queue->array+back)->pid=pid;//set the actual PID
    queue->back=(queue->back+1)%queue->totalsize;
    queue->len++;
}

//returns NULL if queue is empty
Node * dequeue(Queue * queue)
{

    queue->front=(queue->front+1)%queue->totalsize;
    queue->len--;
    if(queue->len==0)
    {
        return NULL;//if the queue is empty
    }
    return queue->array+(queue->front-1);//return the actual item pointer
}

void term_child(int sig)
{
    // signal handler, the termination of a process
    dequeue(q);
    
}

int main( int argc , char* argv[] )
{
    int qt;
    char ** progs;
    q.totalsize=argc-2;
    q.front=0;
    q.back=0;
    q.len=0;
    sigset_t signals;

    sigaddset(&signals,SIGCHLD);

    q.array = malloc(sizeof(Node)*(q.totalsize));//allocate our queue

    if(q.array==NULL)
    {
        printf("Error with memory allocation");
        exit(-1);
    }
    signal(SIGCHLD,term_child);//initialize handler for child complete

    // Part 1: parse arguments from the user
    if(argc>2)
    {
    	qt=1000*atoi(argv[1]);  // Here you need the qt
        progs = argv+2;
    }
    else
    {
    	printf("%s qt prog1 [prog2] ... [prog[N] \n",argv[0]);
	exit(-1);
    }

    // PART 2 : Forking the processes

    for (int i=0;i<argc-2;i++)
    {
        int pid=0;
        printf( "Message from father : Creating program %s \n" , argv[i]);

        if((pid=fork())!=-1)
        {
            if(pid==0)
            {
                execl(argv[i],argv[i]);
            }else{
                enqueue(q,pid);//put process into queue
                kill(pid,SIGUSR1);//stop the process from running...
            }
        }
    }
    sleep(1); // short delay for synchronization

    printf ("\ n I am the Scheduler and I will now begin scheduling my programs : \n" );
    // PART 3 : Scheduling loop
    while (q.len!=0) // Scheduling loop
    {

        kill((q->array+q->front)->pid,SIGCONT);//begin the first process in the queue
	usleep(qt);
	if (sigpending(signals,SIGCHLD))
	{
		send the signal SIGUSR1 to the first element in the queue
		usleep (1000) ;
		dequeue and re−enqueue
	}
	else
	{
		printf ( ”A child is dead\n” ) ;
		remove from list & mark child as dead
	}
    }
}
