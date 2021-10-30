#include<stdio.h>
#include<pthread.h>

#define MAX_PROCESS = 4

int *add(void* data){
	int* act_data = data;
	return data[0] + data[1];
}

void main(){

	pthread_t pthread[MAX_PROCESS];
	FILE* text = fopen("temp.txt", "a+");
	
	int tmp;
	int data[2];
	char line[3];
	
	parameter.data = data; 
	for (float i = MAX_PROCESS; i >= 1; i /= 2){
		for (int j = 0; j < i; j++){
			for(int k = 0; k < 2; k++){
				fgets(line, 2, text);
				data[k] = line[0];
			}
			pthread_create(&pthread[0], NULL, add, (void *)parameter);
		}
		for (int j = 0; j < i; j++){
			pthread_join(pthread[j], );
		}
	}
	fclose(text);
	
}
