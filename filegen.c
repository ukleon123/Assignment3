#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>

#define MAX_PROCESS 16384

int main(){
    char str[17];
    int chk = mkdir("./temp", 0777);
    if (chk == -1) return -1;

    srand((unsigned)time(NULL));

    for(int i = 0; i < MAX_PROCESS; i++){
        sprintf(str, "./temp/%d.txt", i);
        FILE* text = fopen(str, "w+");
        fprintf(text, "%d", 1 + rand() % 9);
        fclose(text);
    }
}