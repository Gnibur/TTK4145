// gcc 4.7.2 +
// gcc -std=gnu99 -Wall -g -o helloworld_c helloworld_c.c -lpthread
#include <pthread.h>
#include <stdio.h>

int i = 0;

// Note the return type: void*
void* thread_1(){
	int j;
	for (j = 0; j < 1000000; j++)
	{
		i++;
	}
}

void* thread_2(){
	int j;
	for (j=0; j<1000000; j++)
	{
		i--;
	}
}

int main(){
	pthread_t sthread_1;
	pthread_t sthread_2;
	pthread_create(&sthread_1, NULL, thread_1, NULL);
	pthread_create(&sthread_2, NULL, thread_2, NULL);

	// Arguments to a thread would be passed here ---------^

	pthread_join(sthread_1, NULL);
	pthread_join(sthread_2, NULL);
	printf("This is i: %d", i);
	return 0;
}
/*
main:
    global shared int i = 0
    spawn thread_1
    spawn thread_2
    join all threads
    print i

thread_1:
    do 1_000_000 times:
        i++
thread_2:
    do 1_000_000 times:
        i--
*/
