#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<pthread.h>

#define MAX_PROCESS 4
typedef struct {
	int data[2];
	FILE* fd;
}calc;

void *add(void* data){
	calc* act_data = data;
	int ret = (act_data->data[0] + act_data->data[1]);
	printf("%d\n", ret);
	fprintf(act_data->fd, "%d\n", ret);
}

void main(){

	pthread_t pthread[MAX_PROCESS];
	void* ret;
	char* one;
	char line[3];
	
	calc* data = malloc(sizeof(calc));
	for (float i = MAX_PROCESS; i >= 1; i /= 2){
		FILE* text = fopen("temp.txt", "a+");
		data->fd = text;
		for (int j = 0; j < i; j++){
			for(int k = 0; k < 2; k++){
				one = fgets(line, 3, text);
				data->data[k] = one[0];
			}
			pthread_create(&pthread[j], NULL, add, (void *)data);
			pthread_detach(pthread[j]);
		}
		fclose(text);
	}	
}
