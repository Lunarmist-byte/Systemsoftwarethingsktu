#include<stdio.h>
int main(){
	int rs[25],m[10],count[10],n,f,pf=0,next=1,min,i,j,flag;
	printf("Enter length of ref string:");
	scanf("%d",&n);
	printf("Enter reference string:");
	for(int i=0;i<n;i++){
		scanf("%d",&rs[i]); 
		flag=0;
	}
	printf("Enter the number of frames:");
	scanf("%d",&f);
	for(int i=0;i<f;i++){
	count[i]=0;
	m[i]=-1;
	}
	printf("\nLRU\n");
	for(int i=0;i<n;i++){
		flag=0;
		for(int j=0;j<f;j++){
			if(m[j]==rs[i]){
				count[j]=next++;
				flag=1;
				break;
			}
		}
	if(flag==0){	
		for(int j=0;j<f;j++){
			if(m[j]==-1){
				m[j]=rs[i];
				count[j]=next++;
				pf++;
				flag=2;
				break;
			}
		}
	if(flag==0){
		min=0;
		for(j=1;j<f;j++){
			if(count[j]<count[min]){
				min=j;
				}
			}
			m[min]=rs[i];
			count[min]=next++;
			pf++;
			flag=0;
		}
	}
	for(int j=0;j<f;j++){
		if(m[j]!=-1)
		printf("%d\t",m[j]);
		else
		    printf("-\t");
	}
	if(flag!=1){
		printf("PF No --%d",pf);
	}
	printf("\n");
	}
printf("No of page faults using LRU:%d",pf);
return 0;
}
