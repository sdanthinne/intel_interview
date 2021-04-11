#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

struct Node {
    // struct describing the node of the queue 
    int pid;
    
};

void enqueue() {
}

int dequeue(){
}

void term_child(){
  // signal handler, the termination of a process

    
}

int main( int argc , char* argv[] )
{
    // variable initialization
    // add anything you need here

    int qt,queue_size=argc-2;
    Node * queue = malloc(sizeof(Node)*(queue_size));//allocate our queue
    if(queue==NULL)
    {
        printf("Error with memory allocation");
        exit(-1);
    }
    Node * qp = queue; //our queue pointer

    signal(SIGCHLD,term_child);//initialize handler for child complete

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

    for (int i=0;i<argc-2;i++)
    {
        printf( "Message from father : Creating program %s \n" , argv[i]) ;
        // fork new process
        // execute the processes: execl [execl( argv [i] , argv[i] ,NULL)] and enqueue the process
    }
    sleep(1); // short delay for synchronization

    printf ("\ n I am the Scheduler and I will now begin scheduling my programs : \n" );
    // PART 3 : Scheduling loop
    while (...) // Scheduling loop
    {
	send the signal SIGCONT to the first element in the queue
	usleep ( qt ) ;
	if (child has not finished )
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
