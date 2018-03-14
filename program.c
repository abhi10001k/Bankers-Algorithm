#include<stdio.h>
#include<pthread.h>
int main()
{
    pthread_t t1;
    int x = pthread_create(&t1,NULL,fun,NULL);
    
}