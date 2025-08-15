#include<stdio.h>
#include<sys/shm.h>
#include<string.h>
#include<unistd.h>
int main(){
    shmid=shmget(1111,512,0666);
    printf("Key of shared memory:%s,shmid");
    void *shared_memory =shmat(shmid,NULL,0);
    printf("Process attached at%p",shared_memory);
    printf("Data read from mmry is %s\n,(char *)shared_memory")
}