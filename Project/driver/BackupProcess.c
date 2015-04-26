#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include <stdio.h>


int main() 
{
	printf("Backup has started!\n");
	
	while (1){
		if (system("pidof -x heis > /dev/null") == 0){
			sleep(5);
		} else { 
			printf("Detected that program crashed, restarting\n");
			system("./heis");
			break;
		}
	}

}
