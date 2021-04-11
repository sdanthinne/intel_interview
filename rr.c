#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <signal.h>



typedef struct Node 
{
    // struct describing the node of the queue 
    int pid;
    
}Node;

typedef struct Queue 
{
    int front, back, len,totalsize;
    Node * array;
}Queue;


volatile char child_term=0;//flag for termination

void enqueue(Queue * queue, int pid) 
{
    
    (queue->array+queue->back)->pid=pid;//set the actual PID
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
    child_term=1;//say that the child finished
    
}

int main( int argc , char* argv[] )
{
    int qt;
    struct sigaction sig_happen;
    sig_happen.sa_handler = term_child;
    Queue q;
    
    q.totalsize=argc-2;
    q.front=0;
    q.back=0;
    q.len=0;
    //sigset_t signals;

    //sigaddset(&signals,SIGCHLD);

    q.array = malloc(sizeof(Node)*(q.totalsize));//allocate our queue

    if(q.array==NULL)
    {
        printf("Error with memory allocation");
        exit(-1);
    }

    sigaction(SIGCHLD,&sig_happen,NULL);//safe init handler, does not matter for this application.

    //signal(SIGCHLD,term_child);//initialize handler for child complete

    // Part 1: parse arguments from the user
    if(argc>2)
    {
    	qt=1000*atoi(argv[1]);  // Here you need the qt
    }
    else
    {
    	printf("%s qt prog1 [prog2] ... [prog[N] \n",argv[0]);
	exit(-1);
    }

    // PART 2 : Forking the processes

    for (int i=2;i<argc;i++)
    {
        int pid=0;
        printf( "Message from father : Creating program %s \n" , argv[i]);

        if((pid=fork())!=-1)
        {
            if(pid==0)
            {
                execl(argv[i],argv[i],(char*)NULL);
            }else{
                enqueue(&q,pid);//put process into queue
                kill(pid,SIGUSR1);//stop the process from running...
            }
        }
    }
    printf("we have: %d progs",q.len);
    sleep(1); // short delay for synchronization

    printf ("\n I am the Scheduler and I will now begin scheduling my programs : \n" );
    // PART 3 : Scheduling loop
    while (q.len!=0) // Scheduling loop
    {

        kill((q.array+q.front)->pid,SIGCONT);//begin the first process in the queue
    	usleep(qt);
	    //if (sigpending(signals,SIGCHLD))
        if(!child_term)
    	{
            Node * temp;

		    kill((q.array+q.front)->pid,SIGUSR1);
		    usleep (1000);
            temp = dequeue(&q);
            enqueue(&q,temp->pid);
	    }
	    else
	    {
		    printf ("A child is dead\n");
            dequeue(&q);
	    }
        child_term=0;
    }
}
