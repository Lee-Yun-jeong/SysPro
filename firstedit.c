#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>

//계란 소금 계란물
//grep -c "[]\|[]\|[]공백없이" * | sort -nk2r -t":"
void do_fork();
char ryo[20][20];
int  ryonum=0;
char *arglist[5];


int main()
{
    int  i=0 ;
    char temp[20];

       puts("재료로 요리 찾기를 선택\n>> 가지고 있는 재료를 space bar 단위 입력 후 엔터 후 ctrl+d\n>>");
            while(scanf("%s", temp )==1){
                strcpy(ryo[i],temp);
                i++;
            }//while
            //i개의 재료=num 

            ryonum=i;
            printf("재료를 %d개 입력했음.\n", ryonum);
		
            do_fork();



    wait(NULL);
    printf("sorting fin\n");
}//switch()



void do_fork(  )
    /*
     *	use fork and execvp and wait to do it
     */
{
	int thepipe[2];
	int thepipe2[2];
    int	pid, pid2, exitstatus, i,j,k;				/* of child	*/
    char cm0[6]="grep";
    char cm1[4]="-c";
    char cm2[300]="\""; // "[]\|[]"
    char cm3[3] ="*";
    char *cm4=NULL;
    arglist[0]=cm0;	arglist[1]=cm1;	
    arglist[3]=cm3;
    arglist[4]=cm4;	
 //   char cm4[6]="sort";
 //   char cm5[7]= "-nk2r";
 //   char cm6[8]= "-t\":\"";
 //   char *cm7 = NULL; 
 //  arglist[6]=cm6; arglist[7]=cm7;

    strcat(cm2, "[");
    strcat(cm2, ryo[0]);
    strcat(cm2, "]");
    for(i = 1 ; i < ryonum; i++){
        strcat(cm2, "\\|[");
        strcat(cm2, ryo[i]);
        strcat(cm2, "]");
    }
    strcat(cm2, "\"");
    arglist[2]=cm2;
	if(pipe(thepipe)==-1)exit(3);	
    pid = fork();					/* make new process */
    switch( pid ){
        case -1:	
            perror("fork failed");
            exit(1);
            break;
        case 0:
//		dup2(thepipe[1],1);
            execvp(arglist[0], arglist);		/* do it */
            perror("execvp failed");
            exit(1);
            break;//case0
  	default:
		wait(NULL);
		dup2(thepipe[0],0);
		if(pipe(thepipe2)==-1)exit(3);
		pid2=fork();
		if(pid2==-1)exit(4);
		else if(pid2==0){
		
	//	dup2(thepipe2[1],1);
		execlp("sort", "sort","-nk2r", "-t\":\"" ,  NULL);  /* do it */
		}
         	else {
		wait(NULL);
	//	dup2(thepipe2[0],0);
		
		}
	   perror("execvp failed");
            exit(2);
	
    }//switch()
}
