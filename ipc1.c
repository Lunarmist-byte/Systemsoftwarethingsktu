#include<stdio.h>
#include<sys/shm.h>
#include<string.h>
#include<unistd.h>
int main(){
    shmid=shmget(1111,512,0666|IPC_CREAT);
    printf("Key of shared memory:%s,shmid");
    void *shared_memory =shmat(shmid,NULL,0);
    printf("Process attached at%p,Enter message",shared_memory);
    char buff[100];
    read(0,buff,100);
}