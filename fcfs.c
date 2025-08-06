#include<stdio.h>
int main(){
int n,ttat,twt;
printf("FCFS");
printf("Enter the no of processes");
scanf("%d",&n);
int bt[n],wt[n],tat[n],p[n];
for(int i=0;i<n;i++){
	printf("Enter the BT for process%d:",i+1);
	scanf("%d",&bt[i]);
	p[i]=i;
}
wt[0]=0;
tat[0]=bt[0];
ttat+=tat[0];
for(int i=1;i<n;i++){
	wt[i]=wt[i-1]+bt[i-1];
	tat[i]=wt[i]+bt[i];
	twt+=wt[i];
	ttat+=tat[i];
}
float awt=twt/n,atat=ttat/n;
printf("%8s%5s%5s%5s\n","Pno","BT","WT","TAT");
for(int i=0;i<n;i++){
	printf("%8d%5d%5d%5d\n",p[i]+1,bt[i],wt[i],tat[i]);
}
printf("Avg WT:%.2f\n",awt);
printf("Avg TAT:%.2f\n",atat);
}

	
