#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>
#define oops(m,x) {perror(m); exit(x);}
#define TOTALFILE 70
void be_cat(int togrep[2],int tosort[2],int fromsort[2]);
void be_grep(int togrep[2],int tosort[2],int fromsort[2]);
void be_sort(int togrep[2],int tosort[2],int fromsort[2]);
char filename[TOTALFILE][20];
char filegrep[TOTALFILE][30];
char grep_argument[BUFSIZ]="",buff[BUFSIZ];
char argument_buff[BUFSIZ],*argument;
void main()
{
	
    int pid[2],togrep[2],tosort[2],fromsort[2];
//재료처리
 printf("Input Ingredients you have\n");
    while(1)
    {   
	
        scanf("%s",buff);
        if(buff[0]=='q' || strcmp(buff,"quit")==0)
        {
            grep_argument[strlen(grep_argument)-1]='\0';
            grep_argument[strlen(grep_argument)-1]='\0';
  
            break;
        }
        strcat(grep_argument,"#");
        strcat(grep_argument,buff);
        strcat(grep_argument,"#");
        strcat(grep_argument,"\\|");


    }

//재료처리
    if(pipe(togrep)==-1)
        oops("pipe to grep failed",1);
    if(pipe(tosort)==-1)
        oops("pipe to sort failed",1);
    if(pipe(fromsort)==-1)
        oops("pipe from sort failed",1);
    
   
    if((pid[0] = fork()) == -1  )
        oops("cannot fork",2);

    if(pid[0]>0)//grep process
    {
	//wait(NULL);
	//   be_cat (togrep, tosort, fromsort);
       
    }
    else if(pid[0]==0)
    {	
       
        pid[1]= fork();
        if( pid[1] == -1){
            oops("cannot fork",2);
        }
        else if(pid[1]>0)
        {//sort process
		//wait(NULL);
            be_sort(togrep,tosort,fromsort);
            perror("be_sort");
        }
        else if(pid[1]==0)
        {
        	 be_grep(togrep,tosort,fromsort);
        }
    }
    else//cat process
    {
        perror("pid[0]");
    }
}
void be_cat(int togrep[2],int tosort[2],int fromsort[2])
{
    char buff[BUFSIZ],file[100],food[100],chosen[100];
	 int num;
    FILE *fpout,*fpin;

    close(tosort[0]);
    close(tosort[1]);
    close(fromsort[1]);
    close(togrep[0]);

    fpout = fdopen(togrep[1],"w");
    fpin = fdopen(fromsort[0],"r");


    fprintf(fpout,"%s",grep_argument);
    fflush(fpout);

	 wait(NULL);

	 printf("choose one food recipe\n");

	 for(int i=0;i<3;i++)
	 {
		 fgets(buff,BUFSIZ,fpin);
		 sscanf(buff,"%s:%d",file,&num);
		 sscanf(file,"%s.txt",food);
		 printf("%s\n",food);
	 }
	scanf("%s",chosen);
	strcat(chosen,".txt");
	
	execlp("cat","cat",chosen, NULL);
}
void be_grep(int togrep[2],int tosort[2],int fromsort[2])
{
   //
    FILE *fpout,*fpin;
    DIR *dp = NULL;
    struct dirent *dr = NULL;
    struct stat info;
    int iter_pid;
    int i=0,j=0;
    close(fromsort[0]);
    close(fromsort[1]);
    close(togrep[1]);
    close(tosort[0]);

    dup2(tosort[1],1);
    fpout = fdopen(tosort[1],"w");
    fpin = fdopen(togrep[0],"r");

    fgets(argument_buff,BUFSIZ,fpin);
    argument = (char*)malloc(strlen(argument_buff)+1);
    strcpy(argument,argument_buff);
    //ref : lab4.find.

	execlp("grep","grep","-cr",argument,NULL);
    //ref : lab4.find.

}
void be_sort(int togrep[2],int tosort[2],int fromsort[2])
{
    FILE *fpout,*fpin;
	puts("sort");fflush(stdout);
    close(togrep[0]);
    close(togrep[1]);
    close(tosort[1]);
    close(fromsort[0]);
	// dup2(fromsort[1],1);
	
    fpout = fdopen(fromsort[1],"w");
    fpin = fdopen(tosort[0],"r");

	
	execlp("sort","sort","-nk2r", "-t", ":", NULL);





}

