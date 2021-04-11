#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <limits.h>


void term_child(int,siginfo_t *,void *);

typedef struct Node 
{
    // struct describing the node of the queue 
    int pid;
    long time;
    
}Node;

typedef struct Queue 
{
    int front, back, len,totalsize;
    Node * array;
}Queue;


volatile sig_atomic_t ready;//flag for termination

void enqueue(Queue * queue, int pid,int time) 
{
    
    (queue->array+queue->back)->pid=pid;//set the actual PID
    (queue->array+queue->back)->time=time;//set the actual PID
    queue->back=(queue->back+1)%queue->totalsize;
    queue->len++;
}

//returns NULL if queue is empty
Node * dequeue(Queue * queue)
{
    Node * ret; 
    ret = queue->array+(queue->front);
    queue->front=(queue->front+1)%queue->totalsize;
    queue->len--;
    
    return ret;//return the actual item pointer
}



void term_child(int sig,siginfo_t * info,void * point)
{
    struct sigaction sig_happen;
    sigset_t block_mask;//mask to block 
    sigfillset(&block_mask);//add all signals to be blocked
    sig_happen.sa_sigaction = term_child;
    sig_happen.sa_flags|=SA_SIGINFO;
    sig_happen.sa_mask = block_mask;

    sigaction(SIGCHLD,&sig_happen,NULL);//safe init handler to block signals

    printf("Child %d finished its workload\n",info->si_pid);
    waitpid(info->si_pid,NULL,0);//wait for the child
    //now we run the next process... 
    ready=1;//set our ready flag
    //unblock signals again
    sigemptyset(&block_mask);
    sigprocmask(SIG_SETMASK,&block_mask,NULL);
}

int comparator(const void * a, const void * b)
{
    return (((Node *)a)->time)> (((Node *)b)->time);
}

/**
 * ASSUME: arguments given follow the format detailed below.
 * ASSUME: The user knows the burst time of each program OR job 
 * weights are assigned by executable size (using option -f) OR 
 * weights are assigned by number within the program name(using option -n).
 */
int main( int argc , char* argv[] )
{
    Queue q;
    int force=0;
    struct sigaction sig_happen;
    sig_happen.sa_sigaction = term_child;
    sig_happen.sa_flags|=SA_SIGINFO;
    sigaction(SIGCHLD,&sig_happen,NULL);//safe init handler, does not matter for this application.
    
    q.totalsize=argc;
    q.front=0;
    q.back=0;
    q.len=0;
    ready=1;


    q.array = malloc(sizeof(Node)*(q.totalsize));//allocate our queue

    if(q.array==NULL)
    {
        printf("Error with memory allocation\n");
        exit(-1);
    }



    // Part 1: parse arguments from the user
    if( !((argc>1) && (((argc-1)%2==0)||(!strcmp(argv[argc-1],"-f"))||(!strcmp(argv[argc-1],"-n")))) )
    {
    	printf("%s prog1 time [prog2] [time] ... [prog[N] time[N] [-f | -n]\n",argv[0]);
	    exit(-1);
    }

    if(!strcmp("-f",argv[argc-1]))
    {
        force=1;
        argc--;
    }

    if(!strcmp("-n",argv[argc-1]))
    {
        force=-1;
        argc--;
    }

    // PART 2 : Forking the processes

    for (int i=1;i<argc;i+=2)
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
                if(force==1)
                {
                    //calculate the executable size (only works as intended
                    //if all loops have been unrolled, NEVER branches back up, purely sequential)
                    long size=INT_MAX;
                    FILE * executable = fopen(argv[i],"r");
                    if(executable==NULL)
                    {
                        fprintf(stderr,"there was a problem with opening file for size read\n");
                        exit(errno);
                    }

                    fseek(executable,0,SEEK_END);
                    size = ftell(executable);
                    printf("executable size: %ld pid:%d\n",size,pid);
                    enqueue(&q,pid,size);
                    fclose(executable);

                }else if(force == -1)
                {
                    enqueue(&q,pid,(long)atoi(argv[i]));
                }else
                {
                    enqueue(&q,pid,(long)atoi(argv[i+1]));//put process into queue
                }

            }
        }
    }

    sleep(1); // short delay for synchronization
    qsort(q.array,q.len,sizeof(Node),comparator);
    printf ("\n I am the Scheduler and I will now begin scheduling my programs : \n" );
    // PART 3 : Scheduling loop

    do
    {
        if(ready)//poll for ready signal
        {
            //run the next process
            Node * next = dequeue(&q);
            kill(next->pid,SIGCONT);
            ready = 0;
        }

    } while(q.len!=0); // Scheduling loop
    wait(NULL);//wait for the last child (no child left behind)
    printf("Queue empty\nAll of my children died so bye...\n");
    free(q.array);//cleanup, all children should already have been waited for
}
