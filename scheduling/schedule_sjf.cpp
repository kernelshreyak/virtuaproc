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

int main()
{
    int wt[N],  //waiting time of processes
        tat[N], //turnaround time of processes
        i;
    float avg_wt,avg_tat,tput;

    process pr_q[N];  //the ready-queue of processes


    cout<<"----------------SJF Schedule Simulation----------------------"<<endl;
    for(i=0;i<N;i++)
    {
        wt[i]=0;  //initialize waiting time of all processes to zero
        cout<<"Enter processing time of Process"<<i+1<<":";
        cin>>pr_q[i].pt;
        pr_q.pid=i+1;  //set PID of process
    }

    cout<<endl;

    for(i=0;i<N;i++)
    {
        cout<<"Enter arrival time of Process"<<i+1<<":";
        cin>>pr_q[i].art;
    }



    cout<<"\nProcess\t\tExec Time\tTurnaround Time\tWaiting Time";

    //execute processes
    for(i=0;i<N;i++)
    {
        if(execute(pr_q[i])==1)
        {
            tat[i]=T-pr_q[i].art;
            wt[i]=tat[i]-pr_q[i].pt;
        }

        printf("\nP%d\t\t  %d\t\t    %d\t\t\t%d",i+1,pr_q[i].pt,tat[i],wt[i]);
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

    cout<<"\nAverage Turnaround Time:"<<avg_tat<<endl;
    cout<<"\nAverage Waiting Time:"<<avg_wt<<endl;
    cout<<"\nThroughput:"<<tput<<endl;
    return 0;
}
