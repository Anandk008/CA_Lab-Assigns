#include<stdio.h>
#include<stdlib.h>


int main(){
	FILE *fptr;
	fptr = fopen("./inputfile.txt","w");
	int i;
	for(i = 10000; i > 0; i--){
		fprintf(fptr, "%d ", i);		
	}
	fclose(fptr);
	return 0;
}
