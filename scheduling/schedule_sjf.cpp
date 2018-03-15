//Simulation of Shortest Job First (SJF) Scheduling in C++
//Written by Shreyak Chakraborty

#include <iostream>
#include <algorithm>
#include <stdio.h>

#define N 5   //Number of Processes

using namespace std;

int T=0;  //total time to execute all processes

struct process
{
    int pid;  //PID of process
    int pt;   //processing time of process
    int art;  //arrival time of process
};

int execute(process p)  //executes a process return 1 if execution finished else 0
{
    for(int j=0;j<p.pt;j++)
        T+=1;

    return 1;
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void sort_processes(int proc_times[N], int pids[N])  //sorts an array of processes (PIDs) based on Processing times using basic sorting
{
    int i,j,min,tmp;
    for(i=0;i<N;i++)
    {
        min=i;
        for(j=i+1;j<N;j++)
            if(proc_times[j] < proc_times[min])
                min=j;
        swap(&pids[min],&pids[i]);  //swap the PIDs
    }
}


int main()
{
    int wt[N],  //waiting time of processes
        tat[N], //turnaround time of processes
        i,s_pr[N],pts[N];
    float avg_wt,avg_tat,tput;

    process pr_q[N];  //the ready-queue of processes



    cout<<"----------------SJF Schedule Simulation----------------------"<<endl;
    for(i=0;i<N;i++)
    {
        wt[i]=0;  //initialize waiting time of all processes to zero
        cout<<"Enter processing time of Process"<<i+1<<":";
        cin>>pr_q[i].pt;
        pr_q[i].pid=i+1;  //set PID of process
    }

    for(i=0;i<N;i++)
        pts[i]=pr_q[i].pt;  //processing times of processes

    for(i=0;i<N;i++)
        s_pr[i]=pr_q[i].pid;

    //sort the processes in increasing order of processing time
    sort_processes(pts,s_pr);


    cout<<endl;


    for(i=0;i<N;i++)
    {
        cout<<"Enter arrival time of Process"<<i+1<<":";
        cin>>pr_q[i].art;
    }


    cout<<"\nSorted PIDs:";
    for(i : s_pr)
    {
        cout<<i<<" ";
    }

    cout<<"\n\nProcess\t\tExec Time\tTurnaround Time\tWaiting Time";

    //execute processes
    process curr_proc;
    for(i=0;i<N;i++)
    {
        curr_proc=pr_q[s_pr[i]-1]; //get the current process
        if(execute(curr_proc)==1)
        {
            tat[i]=T-curr_proc.art;
            wt[i]=tat[i]-curr_proc.pt;
        }

        printf("\nP%d\t\t  %d\t\t    %d\t\t\t%d",pr_q[i].pid,pr_q[i].pt,tat[i],wt[i]);
    }

    //calculate values
    avg_wt=0.0;
    avg_tat=0.0;
    for(i=0;i<N;i++)
    {
        avg_wt+=wt[i];
        avg_tat+=tat[i];
    }

    avg_tat/=N;
    avg_wt/=N;
    tput=N/T; //the throughput

    cout<<"\n\nTotal Time:"<<T<<endl;
    cout<<"\nAverage Turnaround Time:"<<avg_tat<<endl;
    cout<<"\nAverage Waiting Time:"<<avg_wt<<endl;

    return 0;
}
