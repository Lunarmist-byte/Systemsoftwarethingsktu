#include<stdio.h>
#include<stdbool.h>

#define MAX 10
int process,resources;
int allocation[MAX][MAX],need[MAX][MAX],maximum[MAX][MAX],available[MAX];
void calculateNeed(){
	for(int i=0;i<process;i++){
		for(int j=0;j<resources;j++){
			need[i][j]=maximum[i][j]-allocation[i][j];
			}
		}
	}
bool isSafe(int safeSequence[]){
	int work[resources];
	bool finish[process];
	for(int i=0;i<resources;i++){
		work[i]=available[i];
	}
	for(int i=0;i<process;i++){
		finish[i]=false;
	}
	int count=0;
	int index=0;
	while(count<process){
		bool found=false;
		for(int i=0;i<process;i++){
			if(!finish[i]){
				int j;
				for(j=0;j<resources;j++){
				if(need[i][j]>work[j]){
				break;
				}
			}
		if(j==resources){
			for(int k=0;k<resources;k++){
				work[k]+=allocation[i][k];
				}
				finish[i]=true;
				safeSequence[index++]=i;
				found=true;
				count++;
			}
		}
	}
	if(!found){
		return false;
	}
}
return true;
}
void main(){
	printf("Enter the no of process:");
	scanf("%d",&process);
	printf("Enter the number of resources:");
	scanf("%d",&resources);
	printf("Enter the allocation matrix:\n");
	for(int i=0;i<process;i++){
		for(int j=0;j<resources;j++){
			printf("Allocation[%d][%d]:",i,j);
			scanf("%d",&allocation[i][j]);
		}
	}
	printf("Enter the maximum matrix:\n");
	for(int i=0;i<process;i++){
		for(int j=0;j<resources;j++){
			printf("Maximum[%d][%d]:",i,j);
			scanf("%d",&maximum[i][j]);
			}
	}
	printf("\nEnter the avaialble resources:\n");
	for(int i=0;i<resources;i++){
		printf("Available[%d]:",i);
		scanf("%d",&available[i]);
	}
	calculateNeed();
	int safeSequence[MAX];
	if(isSafe(safeSequence)){
		printf("\nSystem is in Safe State\n");
		printf("Safe Sequence:");
		for(int i=0;i<process;i++){
			printf("P%d - ",safeSequence[i]);
		}
		printf("\n");
		}else{
		printf("System is unsafe\n");
	}
}
					
