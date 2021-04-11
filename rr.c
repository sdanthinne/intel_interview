#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>


void term_child(int,siginfo_t *,void *);

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


volatile sig_atomic_t child_term=0;//flag for termination

/**
 * Enqueues the value pid into a Node object at index back
 */
void enqueue(Queue * queue, int pid) 
{
    (queue->array+queue->back)->pid=pid;//set the actual PID
    queue->back=(queue->back+1)%queue->totalsize;
    queue->len++;
}

/**
 * Dequeues item at index front and returns pointer to object
 * Safe use in this use-case
 */
Node * dequeue(Queue * queue)
{
    Node * ret; 

    ret = queue->array+(queue->front);
    queue->front=(queue->front+1)%queue->totalsize;
    queue->len--;
    
    return ret;//return the actual item pointer
}

/**
 * SIGCHLD handler
 *  sets global done flag, and waits so no children is left
 *  behind
 */
void term_child(int sig,siginfo_t * info,void * point)
{
    struct sigaction sig_happen;
    sig_happen.sa_sigaction = term_child;
    sig_happen.sa_flags|=SA_SIGINFO;

    printf("Child %d finished its workload\n",info->si_pid);
    sigaction(SIGCHLD,&sig_happen,NULL);//safe init handler
    child_term=1;//say that the child finished
    waitpid(info->si_pid,NULL,0);
    
}

/**
 * Round Robin Scheduler example.
 */
int main( int argc , char* argv[] )
{
    int qt;
    Queue q;

    struct sigaction sig_happen;
    sig_happen.sa_sigaction = term_child;
    sig_happen.sa_flags|=SA_SIGINFO;
    sigaction(SIGCHLD,&sig_happen,NULL);//safe SIGCHLD handler
    
    q.totalsize=argc-2+1;
    q.front=0;
    q.back=0;
    q.len=0;
    child_term=0;


    if((q.array = malloc(sizeof(Node)*(q.totalsize)))==NULL)//check malloc for error
    {
        printf("Error with memory allocation\n");
        exit(-1);
    }

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
                //child: exec the child function specified
                execl(argv[i],argv[i],(char*)NULL);
                fprintf(stderr,"no beans");//if there is an error execing
            }else{
                enqueue(&q,pid);//put process into queue
            }
        }
    }

    sleep(1); // short delay for synchronization

    printf ("\n I am the Scheduler and I will now begin scheduling my programs : \n" );
    // PART 3 : Scheduling loop
    while (q.len!=0) // Scheduling loop
    {
        kill((q.array+q.front)->pid,SIGCONT);//begin the first process in the queue
        
    	usleep(qt);//wait the specified process time
        
        printf("Child %d was running end=%d\n",(q.array+q.front)->pid,child_term);
        if(!child_term)
    	{
            //if the process is not yet done
            Node temp;
            //save off the Node somewhere else to prevent overwriting.
            memcpy(&temp,dequeue(&q),sizeof(Node));
		    kill(temp.pid,SIGUSR1);//send the stop signal to the child
		    usleep (1000);
            enqueue(&q,temp.pid);
	    }
	    else
	    {
		    printf ("A child is dead\n");
            dequeue(&q);
	    }
        child_term=0;//reset flag
    }
    printf("Queue empty\nAll of my children died so bye...\n");
    free(q.array);//cleanup, should have waited for all children already
}
