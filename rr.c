#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
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
    //printf("Enqueueing %d at %d\n",pid,queue->back);
    queue->back=(queue->back+1)%queue->totalsize;
    queue->len++;
    //printf("Back after enqueue: %d len= %d\n",queue->back,queue->len);
}

//returns NULL if queue is empty
Node * dequeue(Queue * queue)
{
    Node * ret; 
    //printf("\ndeq\n");
    //printf("Front before dequeue: %d len= %d PID=%d\n",queue->front,queue->len,queue->array[queue->front].pid);
    ret = queue->array+(queue->front);
    queue->front=(queue->front+1)%queue->totalsize;
    queue->len--;

    //printf("Front after dequeue: %d len= %d PID=%d\n",queue->front,queue->len,queue->array[queue->front].pid);

    
    return ret;//return the actual item pointer
}

void term_child(int sig)
{
    child_term=1;//say that the child finished

    //wait(NULL);
}

int main( int argc , char* argv[] )
{
    int qt;
    //struct sigaction sig_happen;
    //sig_happen.sa_handler = term_child;
    Queue q;
    
    q.totalsize=argc-2;
    q.front=0;
    q.back=0;
    q.len=0;
    child_term=0;
    //sigset_t signals;

    //sigaddset(&signals,SIGCHLD);

    q.array = malloc(sizeof(Node)*(q.totalsize));//allocate our queue

    if(q.array==NULL)
    {
        printf("Error with memory allocation");
        exit(-1);
    }

    //sigaction(SIGCHLD,&sig_happen,NULL);//safe init handler, does not matter for this application.


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

    signal(SIGCHLD,term_child);//initialize handler for child complete

    for (int i=2;i<argc;i++)
    {
        int pid=0;
        printf( "Message from father : Creating program %s \n" , argv[i]);

        if((pid=fork())!=-1)
        {
            if(pid==0)
            {
                execl(argv[i],argv[i],(char*)NULL);
                printf("no beans");
            }else{
                enqueue(&q,pid);//put process into queue
                usleep(10);
                //kill(pid,SIGUSR1);//stop the process from running...
            }
        }
    }

    //printf("we have: %d progs",q.len);
    sleep(1); // short delay for synchronization

    printf ("\n I am the Scheduler and I will now begin scheduling my programs : \n" );
    // PART 3 : Scheduling loop
    while (q.len!=0) // Scheduling loop
    {

        //printf("sending CONT to process %d\n",(q.array+q.front)->pid);
        kill((q.array+q.front)->pid,SIGCONT);//begin the first process in the queue
        
    	usleep(qt);
	    //if (sigpending(signals,SIGCHLD))
        if(!child_term)
    	{
            Node * temp;

            temp = dequeue(&q);
		    kill(temp->pid,SIGUSR1);
		    usleep (1000);
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
