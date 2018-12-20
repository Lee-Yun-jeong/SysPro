/*

pipe3.c // ls sort head
*/

#include <sys/types.h>
#include <sys/wait.h>
#include	<stdio.h>
#include	<unistd.h>
#include	<stdlib.h>

#define 	oops(m,x)	{ perror(m); exit(x); }

int main(int ac, char **av)
{
	int	thepipe[2],		/* two file descriptors */
		mypipe[2],
		newfd,			/* useful for pipes	*/
		pid;			/* and the pid		*/

	if ( ac !=4 ) {
		fprintf( stderr, "usage : pipe cmd1 cmd2 cmd3 \n" );
		exit(1);
	}

	if ( pipe ( thepipe ) == -1)	/* get a pipe		*/          
		oops ("Cannot get a pipe", 1);

	/*------------------------------------------------------*/
	/*   now we have a pipe, now let's get two processes    */
	if (	(pid = fork()) == -1 )		/* get a proc	*/          
		oops("Cannot fork", 2);

	/*-------------------------------------------------------*/
	/*	Right here, there are two processes		*/
	/*	parent will read from pipe			*/
	if ( pid > 0 ){			 
        close( thepipe[1] ); 
		if ( dup2(thepipe[0], 0) == -1)  
								 
			oops("could not redirect stdin" ,3);		
		
		close( thepipe[0] );	 
		execlp ( av[3], av[3], NULL);  
	}
	/* first child execs av[2] and writes into pipe */                / 
	if ( pipe ( mypipe ) == -1)	/* get a pipe		*/
		oops ("Cannot get a pipe2 ", 1);
	if (	(pid = fork()) == -1 )		/* get a proc second fork	*/ 
		oops("Cannot fork2 ", 3);  / 
	if ( pid > 0 ){	//second parent
		wait(NULL);			 
		close( mypipe[1] );	close( thepipe[0] );	 
		if ( dup2(mypipe[0], 0) == -1 )				 
			oops("could not redirect stdout", 4);
		if ( dup2(thepipe[1], 1) == -1 )			 
			oops("could not redirect stdout", 4);
		close(mypipe[0]);		/* stdout is duped, close pipe	*/ //  
		close( thepipe[1] );		 
		execlp ( av[2], av[2], NULL);							 
		oops(av[2], 6);
	}
 	//second child execs av[1] and writes into pipe */
		close( thepipe[0] );	close( thepipe[1] );				 
		close( mypipe[0] );		/* child doesn't read from pipe	*/
		if ( dup2(mypipe[1], 1) ==-1 )												
			oops("could not redirect stdout", 4);					 
		close(mypipe[1]);		/* stdout is duped, close pipe	*/	 
		execlp ( av[1], av[1], NULL);	 
		oops(av[1], 5); //execlp 

}
