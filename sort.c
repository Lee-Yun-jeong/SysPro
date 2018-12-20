
void be_sort(int togrep[2],int tosort[2],int fromsort[2])
{
    FILE *fpout,*fpin;

    close(togrep[0]);
    close(togrep[1]);
    close(tosort[1]);
    close(fromsort[0]);
	 dup2(tosort[0],0);
    fpout = fdopen(fromsort[1],"w");
    fpin = fdopen(tosort[0],"r");

	execlp("sort","sort","-nk2r", "-t\":\"", NULL);





}

