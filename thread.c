#include<wait.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

#include<sys/shm.h>
#include<sys/ipc.h>

#define MAX_PROCESS 8

int main(){
	int pid = 0;
	int smid = 0;
	int root = 0;
	int status;
	int* offset;
	int* lock;
	key_t key = 9999;
	key_t mukey = 7777;
	FILE* text = fopen("temp.txt", "a+");

	void* sms;
	if((smid = shmget(key, sizeof(int), 0666|IPC_CREAT)) == -1) printf("chk\n");
	if((sms = shmat(smid, (void*) 0, 0)) == (void*)-1) printf("chk\n");
	offset = (int*) sms;
	if((smid = shmget(mukey, sizeof(int), 0666|IPC_CREAT)) == -1) printf("chk\n");
	if((sms = shmat(smid, (void*) 0, 0)) == (void*)-1) printf("chk\n");
	lock = (int*) sms;
	*lock = 0;
	*offset = 0;
	if(sms == (void*) -1) exit(0);
	for (int i = 0; i < 3; i++){
		if(pid == 0){
			pid = fork();
			if (pid != 0){
				pid = fork();
				if(pid != 0 && i == 0) root++;
			}
		}
	}

	int tmp = 0;
	char data[100000];
	while(1){ 
		if (!*lock){ 
			*lock = 1;
			break;
		}
	}
	fseek(text, *offset, 0);
	for(int i = 0; i < 2; i ++){
		fgets(data, 100000, text);
		tmp += atoi(data);
	}
	*offset = ftell(text);
	printf("%d\n", tmp);
	fprintf(text ,"%d\n", tmp);
	*lock = 0;
	if(root){ 
		fclose(text);
		if(shmctl(smid, IPC_RMID, NULL)==-1){
                	exit(0);
        	}
	}
	return 0;
	
}
