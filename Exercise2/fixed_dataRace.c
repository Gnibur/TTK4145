// gcc 4.7.2 +
// gcc -std=gnu99 -Wall -g -o helloworld_c helloworld_c.c -lpthread
#include <pthread.h>
#include <stdio.h>

int i = 0;
pthread_mutex_t lock;

// Note the return type: void*
void* thread_1(){
	int j;
	pthread_mutex_lock(&lock);
	for (j = 0; j < 1000000; j++)
	{
		i++;
	}
	pthread_mutex_unlock(&lock);
}

void* thread_2(){
	int j;
	pthread_mutex_lock(&lock);
	for (j=0; j<1000001; j++)
	{
		i--;
	}
	pthread_mutex_unlock(&lock);
}

int main(){
	if (pthread_mutex_init(&lock, NULL) != 0)
	{
		printf("\n mutex init failed\n");
		return 1;
    	}

	pthread_t sthread_1;
	pthread_t sthread_2;
	pthread_create(&sthread_1, NULL, thread_1, NULL);
	pthread_create(&sthread_2, NULL, thread_2, NULL);

	// Arguments to a thread would be passed here ---------^

	pthread_join(sthread_1, NULL);
	pthread_join(sthread_2, NULL);
	printf("This is i: %d", i);
	pthread_mutex_destroy(&lock);
	return 0;
}
