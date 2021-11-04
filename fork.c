#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<pthread.h>
#include<signal.h>

#pragma warning(disable:4996)

#define MAX_PROCESS 4


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int offset;
void* add(void* data) {
	int ret;
	char line[10000];
	
	pthread_mutex_lock(&lock);
	FILE* text = fopen("temp.txt", "r");
	fseek(text, offset, SEEK_SET);
	
	ret = 0;
	
	for (int k = 0; k < 2; k++) {
		fgets(line, 10000, text);
		ret += atoi(line);
	}
	offset = ftell(text);
	fclose(text);
	pthread_mutex_unlock(&lock);
	
	return ret;
}

void main() {

	pthread_t pthread[MAX_PROCESS];

	void* data;
	int error;
	
	for (float i = MAX_PROCESS; i >= 1; i /= 2) {
		for (int j = 0; j < i; j++) {
			FILE* text = fopen("temp.txt", "a");
			error = pthread_create(&pthread[j], NULL, add, NULL);
			
			if (error < 0) printf("error");
			else {
				pthread_join(pthread[j], &data);
				fprintf(text, "%d\n", (int)data);
				fclose(text);
			}
		}
	}
}