#include<stdio.h>
void printallocation(const char *title,int allocation[],int processsize[],int n){
	printf("\n%s Allocation:\n",title);
	printf("ProcessNo.\tProcessSize\tBlock No.\n");
		for(int i=0;i<n;i++){
			if(allocation[i]!=-1)
				printf("%d\t\t%d\t\t%d\n",i+1,processsize[i],allocation[i]+1);
			else
				printf("%d\t\t%d\t\tNot Allocated\n",i+1,processsize[i]);
			}
}
void FirstFit(int blocksize[],int m, int processsize[], int n){
	int allocation[n];
	for(int i=0;i<n;i++){
		allocation[i]=-1;
	}
	for(int i=0;i<n;i++){
		for(int j=0;j<m;j++){
			if(blocksize[j]>=processsize[i]){
				allocation[i]=j;
				blocksize[j]-=processsize[i];
				break;
				}
			}
		}
	printallocation("First_Fit",allocation,processsize,n);
}
void BestFit(int blocksize[],int m, int processsize[], int n){
	int allocation[n];
	for(int i=0;i<n;i++){
		allocation[i]=-1;
	}
	for(int i=0;i<n;i++){
	int bestidx=-1;
		for(int j=0;j<m;j++){
			if(blocksize[j]>=processsize[i]){
			
			if(bestidx==-1 || blocksize[j]<blocksize[bestidx]){
			bestidx=j;
				}
			}
		}
		if(bestidx!=-1){
			allocation[i]=bestidx;
			blocksize[bestidx]-=processsize[i];
			}
		}
	printallocation("Best_Fit",allocation,processsize,n);
}
void WorstFit(int blocksize[],int m, int processsize[], int n){
	int allocation[n];
	for(int i=0;i<n;i++){
		allocation[i]=-1;
	}
	for(int i=0;i<n;i++){
	int worstidx=-1;
		for(int j=0;j<m;j++){
			if(blocksize[j]>=processsize[i]){
			
			if(worstidx==-1 || blocksize[j]>blocksize[worstidx]){
			worstidx=j;
				}
			}
		}
		if(worstidx!=-1){
			allocation[i]=worstidx;
			blocksize[worstidx]-=processsize[i];
			}
		}
		printallocation("Worst_Fit",allocation,processsize,n);
	}
int main(){
int n, m,choice;
printf("\nEnter the number of processes:");
scanf("%d",&n);
int processsize[n];
printf("Enter the process sizes:");
for(int i=0;i<n;i++){
	scanf("%d",&processsize[i]);
}
printf("\nEnter the number of memory blocks:");
scanf("%d",&m);
int blocksize[m];
int originalblocksize[m];
printf("\nEnter the block sizes:");
for(int i=0;i<m;i++){
	scanf("%d",&blocksize[i]);
	originalblocksize[i]=blocksize[i];
}
printf("\nMemory allocation Algorithms present\n1)First Fit\n2)Best Fit\n3)Worst Fit");
printf("\nEnter your choice:");
scanf("%d",&choice);
switch(choice){
	case 1:FirstFit(blocksize,m,processsize,n);
		break;
	case 2:BestFit(blocksize,m,processsize,n);
		break;
	case 3:WorstFit(blocksize,m,processsize,n);
		break;
	default:printf("Invalid choice");
	}
return 0;
}
	
	

