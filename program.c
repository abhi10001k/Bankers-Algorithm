#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
int i,j;
int no_of_process;
int no_of_resources;
int totalResource[20];
int availResouce[20];
int allocResource[20][20];
int maxResource[20][20];
int needResource[20][20];
pthread_mutex_t mutex;
void* fun(void *);
void printAvailResource()
{
    for(i=0;i<no_of_resources;i++)
    {
        printf("%d\t",availResouce[i]);
    }
    printf("\n");
}
void printAllocResource()
{
    for(i=0;i<no_of_process;i++)
    {
        printf("Process %d :\t",(i+1));
        for(j=0;j<no_of_resources;j++)
            printf("%d\t",allocResource[i][j]);
        printf("\n");
    }
}
void printNeedResource()
{
    for(i=0;i<no_of_process;i++)
    {
        printf("Process %d :\t",(i+1));
        for(j=0;j<no_of_resources;j++)
            printf("%d\t",needResource[i][j]);
        printf("\n");
    }
}
int less(int a[],int b[],int n)
{
    int i;
    for(i=0;i<n;i++)
        if(a[i]>b[i])
            return 1;
    return 0;
}

int main()
{

    s1:
        printf("Enter the no of process/thread : ");
        scanf("%d",&no_of_process);
        if(no_of_process > 20)
        {
            printf("Sorry! No of Process must no greater than 20.\n");
            goto s1;
        }


    s2:
        printf("Enter the no of Resources present in the System : ");
        scanf("%d",&no_of_resources);
        if(no_of_resources > 20)
        {
            printf("Sorry! No of Resources must no greater than 20.\n");
            goto s2;
        }
    printf("\n----------Enter the no of instance of each resources present in the system-----------\n");
    for(i=0;i<no_of_resources;i++)
    {
        printf("Resource %d : ",i+1);
        scanf("%d",&totalResource[i]);
    }


    int check[20];
    s3:
        printf("\n----------Enter no of instance of Resources initially allocated to each Process-----------\n");
        memset(check,0,sizeof(check));
        for(i=0;i<no_of_process;i++)
        {
            printf("For Process %d : ",(i+1));
            for(j=0;j<no_of_resources;j++)
                {
                    scanf("%d",&allocResource[i][j]);
                    check[j]+=allocResource[i][j];
                }
        }
        if(less(check,totalResource,no_of_resources))
            {
                printf("Sorry! Resource Allocated is bigger than available in system. Re-Enter\n");
                goto s3;
            }
        else
        {
            for(i = 0;i<no_of_resources;i++)
                availResouce[i] = totalResource[i] - check[i];
        }


     printf("\n---------Enter no of maximum instance allocated to a process-----------------\n");
     for(i=0;i<no_of_process;i++)
     {
         printf("For Process %d : ",(i+1));
         for(j=0;j<no_of_resources;j++)
            {
                scanf("%d",&maxResource[i][j]);
            }
     }


     for(i=0;i<no_of_process;i++)
        for(j=0;j<no_of_resources;j++)
            needResource[i][j] = maxResource[i][j] - allocResource[i][j];


     printf("Resource Currently Available :- \n");
     printAvailResource();

     printf("Initial Resource allocated :- \n");
     printAllocResource();

     printf("Initial Need of Resource :- \n");
     printNeedResource();

     //multiThreading Starts
     pthread_mutex_init(&mutex,NULL);
     no_of_process = 4;
     no_of_resources = 5;
     pthread_t *tid = malloc(sizeof(pthread_t)*no_of_process);
     int *pid = malloc(sizeof(int)*no_of_process);

     //creating thread
     for(i=0;i<no_of_process;i++)
     {
         *(pid + i) = i+1;
         pthread_create((tid+i),NULL,fun,(void *)(pid+i));
     }
     //joining Threads
     for(i=0;i<no_of_process;i++)
     {
         pthread_join(*(tid+i),NULL);
     }

}
void * fun(void *x)
{
    int processID = *(int*)x;
    printf("%d",processID);
}
