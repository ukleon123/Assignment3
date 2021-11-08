#include<wait.h>
#include<time.h>
#include<stdio.h>
#include<sched.h>
#include<unistd.h>
#include<stdlib.h>

#include<sys/shm.h>
#include<sys/ipc.h>

#define MAX_PROCESS 16384

int main(){
	int pid = 0;
	int smid_1, smid_2;
	int root = 0;
	int status;
	int* filenum;
	int* lock;
	key_t key = 9999;
	key_t mukey = 7777;
	struct sched_param param;
	struct timespec start, end;
	void* sms;
	if((smid_1 = shmget(key, sizeof(int), 0666|IPC_CREAT)) == -1) printf("chk\n");
	if((sms = shmat(smid_1, (void*) 0, 0)) == (void*)-1) printf("chk\n");
	filenum = (int*) sms;
	if((smid_2 = shmget(mukey, sizeof(int), 0666|IPC_CREAT)) == -1) printf("chk\n");
	if((sms = shmat(smid_2, (void*) 0, 0)) == (void*)-1) printf("chk\n");
	lock = (int*) sms;
	*lock = 0;
	*filenum = 0;
	param.sched_priority = sched_get_priority_max(SCHED_OTHER);
	clock_gettime(CLOCK_MONOTONIC, &start);
	if(sms == (void*) -1) exit(0);
	for (int i = 0; i < MAX_PROCESS; i++){
		if (pid == 0){
			sched_setscheduler(pid, SCHED_OTHER, &param);
			pid = fork();
			if (pid != 0 && i == 0) root = 1;
		}
	}
	int tmp = 0;
	char str[17];
	char data[2];
	while(1){ 
		if (!*lock){ 
			*lock = 1;
			break;
		}
	}
	sprintf(str, "./temp/%d.txt", *filenum);
	(*filenum)++;
	*lock = 0;
	FILE* text = fopen(str, "r");
	fgets(data, 2, text);
	fclose(text);
	
	if(root){
		if(shmctl(smid_1, IPC_RMID, NULL)==-1)exit(0);
		if(shmctl(smid_2, IPC_RMID, NULL)==-1)exit(0);
		clock_gettime(CLOCK_MONOTONIC, &end);
		printf("%lf", (double)((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec))/1000000000);
	}
	return 0;
	
}
