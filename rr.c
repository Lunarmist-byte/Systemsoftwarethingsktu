#include<stdio.h>
struct process{
	int pid;
	int at;
	int bt;
	int tat;
	int wt;
	int ct;
	int btl;
	int pri;
}p[10];

void main(){
int n,tq;
printf("Enter the no of process:");
scanf("%d",&n);
for(int i=0;i<n;i++){
	printf("Enter AT,BT of process %d:",i+1);
	scanf("%d%d",&p[i].at,&p[i].bt);
	p[i].pid=i+1;
	p[i].btl=p[i].bt;
	p[i].wt=p[i].tat=0;
}
printf("Enter the time quantum:");
scanf("%d",&tq);
int time=0,done=0;
while(done<n){
	int flag=0;
	for(int i=0;i<n;i++){
		if(p[i].btl>0&&p[i].at<=time){
			int exec=p[i].btl<tq?p[i].btl:tq;
			p[i].btl-=exec;
			time+=exec;
			flag=1;
			if(p[i].btl==0){
				p[i].ct=time;
				p[i].tat=p[i].ct-p[i].at;
				p[i].wt=p[i].tat-p[i].bt;
				done++;
			}
		}
	}
	if(!flag) time++;
}
float ttat=0,twt=0,atat,awt;
printf("%5s%4s%4s%5s%4s\n","Pid","BT","CT","TAT","WT");
for(int i=0;i<n;i++){
	printf("%5d%4d%4d%5d%4d\n",p[i].pid,p[i].bt,p[i].ct,p[i].tat,p[i].wt);
	ttat+=p[i].tat;
	twt+=p[i].wt;
}
printf("Average TAT:%.2f\n",ttat/n);
printf("Average WT:%.2f\n",twt/n);
}
