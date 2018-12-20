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
char filegrep[TOTALFILE];

void main()
{
	char grep_argument[BUFSIZ]="",buff[BUFSIZ];
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
	wait(NULL);
	   be_cat (togrep, tosort, fromsort);
       
    }
    else if(pid[0]==0)
    {	
       
        pid[1]= fork();
        if( pid[1] == -1){
            oops("cannot fork",2);
        }
        else if(pid[1]>0)
        {//sort process
		wait(NULL);
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
   
    FILE *fpout,*fpin;

    close(tosort[0]);
    close(tosort[1]);
    close(fromsort[1]);
    close(togrep[0]);

    fpout = fdopen(togrep[1],"w");
    fpin = fdopen(fromsort[0],"r");

   
    fprintf(fpout,"%s",grep_argument);
    fflush(fpout);
    wait(NULL);// 그램 주고 난 후에 

}
void be_grep(int togrep[2],int tosort[2],int fromsort[2])
{
    char argument_buff[BUFSIZ],*argument;
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

    //dup2(tosort[1],1);
    fpout = fdopen(tosort[1],"w");
    fpin = fdopen(togrep[0],"r");

    fgets(argument_buff,BUFSIZ,fpin);
    argument = (char*)malloc(strlen(argument_buff)+1);
    strcpy(argument,argument_buff);
    //ref : lab4.find.

    dp = opendir("."); //open the directory

    while ((dr = readdir(dp)) != NULL) //read directory
    {	
	printf("/////////////////%d", i++);
        if ( strcmp(dr->d_name, ".") == 0 || strcmp(dr->d_name, "..") == 0)//test is test.exe
            continue; //prevent infinite loop
        if (stat(dr->d_name, &info) == -1)
            continue; 
        if (S_ISDIR(info.st_mode) != 0)
            continue; //if the file is  dir
        if(strcmp(dr->d_name, "test") != 0 )
	{   
		//count i  doesn't increase
	//	  sprintf(filename[i],"%d~~%s:",i, dr->d_name ); fflush(stdout);
          //  	puts(filename[i++]);  fflush(stdout);
		//puts( dr->d_name);  fflush(stdout);
	//	puts( argument);  fflush(stdout);
		

            iter_pid=fork();
            if(iter_pid>0)
	    { 
		 wait(NULL);
                fprintf(stdout,"parent %s:", dr->d_name );
            }
            else if(iter_pid==0)
	    {

                execlp("grep","grep","-c",argument, dr->d_name ,NULL);
                perror("grep fail");
                exit(1);
            }
            else perror("fork");
        }
	
    }
    closedir(dp); //close directory


    //ref : lab4.find.

}
void be_sort(int togrep[2],int tosort[2],int fromsort[2])
{
    FILE *fpout,*fpin;

    close(togrep[0]);
    close(togrep[1]);
    close(tosort[1]);
    close(fromsort[0]);

    fpout = fdopen(fromsort[1],"w");
    fpin = fdopen(tosort[0],"r");
}

