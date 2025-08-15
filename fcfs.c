#include<stdio.h>

struct process {
    int pid;
    int bt;  // Burst time
    int wt;  // Waiting time
    int tat; // Turnaround time
};

// Function to calculate waiting time and turnaround time
void findwt(struct process p[], int n) {
    p[0].wt = 0;  // First process has 0 waiting time
    for (int i = 1; i < n; i++) {
        p[i].wt = p[i - 1].wt + p[i - 1].bt;  // Waiting time for current process
    }
    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].wt + p[i].bt;  // Turnaround time = Waiting time + Burst time
    }
}

void main() {
    int n, ttat = 0, twt = 0;

    // Taking number of processes as input
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Dynamically creating array of processes
    struct process p[n];  // Array of n processes

    // Taking burst time as input for each process
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;  // Assigning process id (starting from 1)
        printf("Enter BT of Process %d: ", p[i].pid);
        scanf("%d", &p[i].bt);
    }

    // Calculate waiting time and turnaround time
    findwt(p, n);

    // Calculate total waiting time and total turnaround time
    for (int i = 0; i < n; i++) {
        ttat += p[i].tat;
        twt += p[i].wt;
    }

    // Output results
    printf("\nFCFS without AT\n");
    printf("\tPID\tBT\tWT\tTAT\n");

    // Display details of each process
    for (int i = 0; i < n; i++) {
        printf("\t%3d\t%2d\t%2d\t%3d\n", p[i].pid, p[i].bt, p[i].wt, p[i].tat);
    }

    // Calculate average waiting time and turnaround time
    float avgttat = (float)ttat / n;  // Average Turnaround Time
    float avgwt = (float)twt / n;     // Average Waiting Time

    // Display average times
    printf("Average Waiting Time: %.2f\n", avgwt);
    printf("Average Turnaround Time: %.2f\n", avgttat);
}
