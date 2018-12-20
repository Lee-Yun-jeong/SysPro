#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define oops(m,x) {perror(m); exit(x);}

void be_cat(int togrep[2],int tosort[2],int fromsort[2]);
void be_grep(int togrep[2],int tosort[2],int fromsort[2]);
void be_sort(int togrep[2],int tosort[2],int fromsort[2]);

void main()
{

//execlp("grep","grep","-c", "#계란#\\|#계란물#" , "aaa.txt" ,NULL);

execlp("grep","grep","-c", "#계란#" , "\*" ,NULL);


}
