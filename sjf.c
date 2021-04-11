#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

struct Node {
	// struct describing the node of the queue 
};

void enqueue(...) {
}

int dequeue(...){
}

void term_child(...){
  // signal handler, the termination of a process
    
}

int main( int argc , char* argv[] )
{
    // variable initialization
    // add anything you need here

    // Part 1: parse arguments from the user
    if(argc>2)
    {
    	qt=1000*atoi(argv[1]);  // Here you need the qt
    }
    else
    {
    	printf("%s qt prog1 [prog2] ... [prog[N] \n",argv[0]);exit(-1);
	exit(-1);
    }

    // PART 2 : Forking the processes

    for (... )
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
