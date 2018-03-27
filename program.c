#include<stdio.h>	
#include<pthread.h>
#include<stdlib.h>

int i,j;

int no_of_process;                             
int no_of_resources;
int isActive[20]={0};                      
int totalResource[20];
int availResource[20];
int allocResource[20][20];
int maxResource[20][20];
int needResource[20][20];

pthread_mutex_t mutex;

void printAvailResource();
void printAllocResource();
void printNeedResource();
int requestResource(int a,int b[]);
int isEnoughToAlloc(int a[]);
int isGreaterThanNeedResource(int a,int b[]);
int isNeedIsZero(int a);
int isSafeState();


void* Processes_fun(void *);

int main()
{

    printf("Enter the no of process/thread : ");
    scanf("%d",&no_of_process);
    
    printf("Enter the no of Resources present in the System : ");
    scanf("%d",&no_of_resources);
        
    printf("\n----------Enter the no of instance of each resources present in the system-----------\n");
    for(i=0;i<no_of_resources;i++)
    {
        printf("Resource %d : ",i);
        scanf("%d",&totalResource[i]);
        availResource[i]= totalResource[i];
    }

    int check[no_of_resources];
    s3:
        printf("\n----------Enter no of instance of Resources initially allocated to each Process-----------\n");
        //int check[no_of_resources] ={0};
        for(i=0;i<no_of_resources;i++)
            check[i]=0;
            
        for(i=0;i<no_of_process;i++)
        {
            printf("For Process %d : ",i);
            for(j=0;j<no_of_resources;j++)
                {
                    scanf("%d",&allocResource[i][j]);
                    check[j]+=allocResource[i][j];
                }
        }
        if(isEnoughToAlloc(check)==0)
            {
                printf("Sorry! Resource Allocated is bigger than available in system. Re-Enter\n");
                goto s3;
            }
        else
        {
            for(i = 0;i<no_of_resources;i++)
                availResource[i] = totalResource[i] - check[i];
        }


    printf("\n---------Enter no of maximum instance allocated to a process-----------------\n");
    for(i=0;i<no_of_process;i++)
    {
        printf("For Process %d : ",i);
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
    pthread_t *threadid = malloc(sizeof(pthread_t)*no_of_process);
    int *proid = malloc(sizeof(int)*no_of_process);

    //creating thread
    for(i=0;i<no_of_process;i++)
    {
        *(proid + i) = i;
        isActive[i]=1;
        pthread_create((threadid+i),NULL,Processes_fun,(void *)(proid+i));
    }

    //joining Threads
    for(i=0;i<no_of_process;i++)
    {
        pthread_join(*(threadid+i),NULL);
    }

    return 0;
}

void * Processes_fun(void *x)
{
    int processID = *(int*)x;
    while(isActive[processID]==1)
    {
        sleep(1);
        int reqResource[no_of_resources];
        
        pthread_mutex_lock(&mutex); 

        printf("Process %d trying to request some Resources \n",processID);
        printf("Enter the instance  of each resources requested by Process %d",processID);
        for(i=0;i<no_of_resources;i++)
            scanf("%d",&reqResource[i]);
        int x=requestResource(processID,reqResource);
        
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);
        if(isNeedIsZero(processID)==1)
        {
            printf("Process %d is releasing its all alocated resource and finnally terminated \n",processID);
            isActive[processID]=0;
            for(i=0;i<no_of_resources;i++)
            {
                availResource[i] += allocResource[processID][i];
                allocResource[processID][i]=0;
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    
}

void printAvailResource()
{
    for(i=0;i<no_of_resources;i++)
    {
        printf("%d\t",availResource[i]);
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

int requestResource(int processID,int reqResource[])
{
    if(isGreaterThanNeedResource(processID,reqResource)==1)
    {
        printf("Requested Resource is greater than needed.\n");
        printf("Can't be Allocated\n");
        return 0;
    }

    if(isEnoughToAlloc(reqResource)==0)
    {
        printf("Requested Resource is greater than the Available Resource\n");
        printf("Can't be Allocated\n");
        return 0;
    }

    for (i = 0; i < no_of_resources; i++)
	{
		needResource[processID][i] -= reqResource[i];
		allocResource[processID][i] += reqResource[i];
		availResource[i] -= reqResource[i];
	}
	printf("Checking if it is safe to allocate...\n");

    if(isSafeState()==1)
    {
        printf("System is in safe state. Successfully Allocated!...\n");
        printf("Available Resource\n");
        printAvailResource();
        printf("Allocated Resource\n");
        printAllocResource();
        printf("Need Resource\n");
        printNeedResource();
        return 1;
    }
    else
    {
        printf("Allocating Resource does not leave the system in Safe State\n");
        for (i = 0; i < no_of_resources; ++i)
		{
			needResource[processID][i] += reqResource[i];
		    allocResource[processID][i] -= reqResource[i];
		    availResource[i] += reqResource[i];
		}
        return 0;
    }
}

int isGreaterThanNeedResource(int processID,int reqResource[])
{
	for (i = 0; i < no_of_resources; i++)
	{
		if (reqResource[i] <= needResource[processID][i])
		{
			continue;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

int isEnoughToAlloc(int reqResource[])
{
	for (i = 0; i < no_of_resources; ++i)
	{
		if (reqResource[i] <= availResource[i])
		{
			continue;
		}
		else
		{
			return 0;
		}
	}
	return 1;
}

int isNeedIsZero(int processID)
{
    for(i=0;i<no_of_resources;i++)
    {
        if(needResource[processID]==0)
        {
            continue;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

int isSafeState()
{
    int isEnd[no_of_process];
    for(i=0;i<no_of_process;i++)
        {
            if(isActive[i]==1)
                isEnd[i]=0;
            else    
                isEnd[i]=1;
        }

    int avail[no_of_resources];
    for(i=0;i<no_of_resources;i++)
    {
        avail[i]=availResource[i];
    }
    int k;
    for(i=0;i<no_of_process;i++)
    {
        if(isEnd[i]==0)
        {
            for(j=0;j<no_of_resources;j++)
            {
                if(needResource[i][j] <= avail[j])
                {
                    if(j==no_of_resources-1)
                    {
                        isEnd[j]=1;
                        for(k=0;k<no_of_resources;k++)
                        {
                            avail[k]+=allocResource[i][k];
                        }

                        i = -1;
                        break;
                    }
                    else   
                    {
                        continue;
                    }

                }
                else
                {
                    break;
                }
            }

        }
        else
        {
            continue;
        }
    }

    for(i=0;i<no_of_process;i++)
    {
        if(isEnd[i]==1)
            continue;
        else
            return 0;
    }
    return 1;
    
}