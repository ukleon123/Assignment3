#include<stdio.h>

#define MAX_PROCESS 8
int main(){
	FILE* f_write = fopen("temp.txt", "w");
	for(int i = 0; i < MAX_PROCESS * 2; i ++){
		fprintf(f_write, "%d\n", i + 1);
	}
	fclose(f_write);
}
