#include<stdio.h>
struct process{
	int pid;
	int bt;
	int pri;
	int tat;
	int wt;
};
int main(){
int n;
printf("\nPriority\n");
printf("Enter the no of processes");
scanf("%d",&n);
struct process p[10];
float twt=0,ttat=0;
for(int i=0;i<n;i++){
	p[i].pid=i+1;
	printf("Enter the BT and Priority of p%d:",i+1);
	scanf("%d%d",&p[i].bt,&p[i].pri);
}
for(int i=0;i<n-1;i++){
for(int j=i+1;j<n;j++){
if(p[j].pri<p[i].pri){
	struct process temp=p[i];
	p[i]=p[j];
	p[j]=temp;
	}
}
}

p[0].wt=0;
p[0].tat=p[0].bt;
for(int i=1;i<n;i++){
	p[i].wt=p[i-1].wt+p[i-1].bt;
	p[i].tat=p[i].wt+p[i].bt;
}
printf("%8s%5s%5s%5s%5s\n","Pno","BT","WT","TAT","Priority");
for(int i=0;i<n;i++){
	printf("%8d%5d%5d%5d%5d\n",p[i].pid,p[i].bt,p[i].wt,p[i].tat,p[i].pri);
	twt+=p[i].wt;
	ttat+=p[i].tat;
}
printf("Avg WT:%.2f\n",twt/n);
printf("Avg TAT:%.2f\n",ttat/n);
}

	
