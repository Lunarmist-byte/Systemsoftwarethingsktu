#include<stdio.h>
void main(){
	int rs[25],m[10],flag[25],n,f,pf=0,i,j,k=0;
	printf("Enter length of ref string:");
	scanf("%d",&n);
	printf("Enter reference string:");
	for(int i=0;i<n;i++){
		scanf("%d",&rs[i]);
		flag[i]=0;
	}
	printf("Enter the number of frames:");
	scanf("%d",&f);
	for(i=0;i<f;i++){
	m[i]=-1;
	}
	printf("\n FIFO \n");
	for(int i=0;i<n;i++){
	int found=0;
	for(int j=0;j<f;j++){
		if(m[j]==rs[i]){
			found=1;
			break;
			}
		}
	if(found==0){
		m[k]=rs[i];
		k=(k+1)%f;
		pf++;
	}
	for(int j=0;j<f;j++){
		if(m[j]!=-1){
			printf("%d\t",m[j]);
			}
		else
			printf("-\t");
		}
		
		if(found==0)
		printf("PF--%d",pf);
		
	printf("\n");
	}
printf("\n Total page faults:%d\n", pf);
}
