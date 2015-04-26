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
		if (system("pidof -x heis") == 0){
			printf("Heis was found\n");
		} else { 
			printf("Heis was not found\n");
			system("./heis");
			break;
		}
	}

}
