#include<stdio.h>
#include<limits.h>

struct process {
    int pid;
    int at;  // Arrival time
    int bt;  // Burst time
    int btl; // Original Burst time for reference
    int ct;  // Completion time
    int wt;  // Waiting time
    int tat; // Turnaround time
    int pri;
};

// Function to calculate waiting time and turnaround time
void findwt(struct process p[], int n) {
    int tq=0,cmp,adv,time=0,pos=0,t=0, i;
    printf("Enter the time quantum;");
    scanf("%d",&tq);
    while(1){;
        if(p[i].btl!=0){
            cmp=0;
        }
    if(cmp){
        break;
    }
    }
    for(int i=0;i<n;i++){
        if(p[i].btl>0){
            adv=p[i].btl<tq?p[i].btl:tq;
            p[i].btl-=adv;
            time+=adv;
        }
        if(p[i].btl!=0){
            p[i].ct=t;
            p[i].tat=p[i].ct-p[i].at;
            p[i].wt=p[i].tat-p[i].bt;
        }
    }
}int main() {
    int n, ttat = 0, twt = 0;

    // Taking number of processes as input
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Dynamically creating array of processes
    struct process p[n];  // Array of n processes

    // Taking burst time and arrival time as input for each process
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;  // Assigning process id (starting from 1)
        printf("Enter Arrival Time (AT) and Burst Time (BT) of Process and Priority %d: ", p[i].pid);
        scanf("%d %d %d", &p[i].at, &p[i].bt,&p[i].pri);
        p[i].btl=p[i].bt;
    }

    // Calculate waiting time and turnaround time
    findwt(p, n);

    // Calculate total waiting time and total turnaround time
    for (int i = 0; i < n; i++) {
        ttat += p[i].tat;
        twt += p[i].wt;
    }

    // Output results
    printf("\tPriority Scheduling\n");
    printf("\tPID\tAT\tBT\tWT\tTAT\n");

    // Display details of each process
    for (int i = 0; i < n; i++) {
        printf("\t%3d\t%2d\t%2d\t%2d\t%3d\n", p[i].pid, p[i].at, p[i].bt, p[i].wt, p[i].tat);
    }

    // Calculate average waiting time and turnaround time
    float avgttat = (float)ttat / n;  // Average Turnaround Time
    float avgwt = (float)twt / n;     // Average Waiting Time

    // Display average times
    printf("Average Waiting Time: %.2f\n", avgwt);
    printf("Average Turnaround Time: %.2f\n", avgttat);

    return 0;
}
