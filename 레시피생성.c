#include <stdio.h>
#include <string.h>
void main(){
	
	FILE * fp ;
	char buf[30]="";
	char nbuf[30]="";
	char line[100]="";
	char nline[100]="";
while(1)
{	
	puts("each until stop");
	puts("what's the menu?");
	fscanf(stdin,"%s", buf );
	if(strcmp(buf,"stop")==0)return;
	sprintf(nbuf,"%s.txt", buf);
	fp = fopen( nbuf , "a+");
	fprintf(fp,"<%s>\n", buf);
	printf(" ingredient \n>>");
	
	while(1){
	fscanf(stdin,"%s", line);
	if(strcmp(line,"stop")==0)return;
	fprintf(fp,"#%s#\n", line);
	}

	fclose(fp);


}//while

}



