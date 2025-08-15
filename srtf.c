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
};

// Function to calculate waiting time and turnaround time
void findwt(struct process p[], int n) {
    int completed = 0, t = 0, shortest = -1, min_rt;

    // Initialize burst time as btl
    for (int i = 0; i < n; i++) {
        p[i].bt = p[i].btl;  // Setting the burst time to the original burst time
    }

    // Process all jobs until completion
    while (completed != n) {
        shortest = -1;
        min_rt = INT_MAX;

        // Find the process with the shortest remaining burst time
        for (int i = 0; i < n; i++) {
            if (p[i].at <= t && p[i].btl > 0 && p[i].btl < min_rt) {
                shortest = i;
                min_rt = p[i].btl;
            }
        }

        // If no process is found, increment time (i.e., skip time to the next process arrival)
        if (shortest == -1) {
            t++;
            continue;
        }

        // Process the selected job
        p[shortest].btl--;  // Decrease burst time
        t++;  // Increment time

        // If process is completed
        if (p[shortest].btl == 0) {
            p[shortest].ct = t;
            p[shortest].tat = p[shortest].ct - p[shortest].at;
            p[shortest].wt = p[shortest].tat - p[shortest].bt;
            completed++;
        }
    }
}

int main() {
    int n, ttat = 0, twt = 0;

    // Taking number of processes as input
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Dynamically creating array of processes
    struct process p[n];  // Array of n processes

    // Taking burst time and arrival time as input for each process
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;  // Assigning process id (starting from 1)
        printf("Enter Arrival Time (AT) and Burst Time (BT) of Process %d: ", p[i].pid);
        scanf("%d %d", &p[i].at, &p[i].bt);
    }

    // Calculate waiting time and turnaround time
    findwt(p, n);

    // Calculate total waiting time and total turnaround time
    for (int i = 0; i < n; i++) {
        ttat += p[i].tat;
        twt += p[i].wt;
    }

    // Output results
    printf("\nSJF Scheduling\n");
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
