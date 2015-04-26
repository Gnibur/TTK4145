#include <stdio.h>
#include <stdlib.h>

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
