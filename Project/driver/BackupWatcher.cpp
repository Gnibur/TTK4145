#include "BackupWatcher.h"

#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include <iostream>


void *runBackupWatcher(void *);

void startBackupProcess()
{
	pthread_t backupWatcher;
	pthread_create(&backupWatcher, NULL, runBackupWatcher, NULL);
}


void *runBackupWatcher(void *args)
{
	while (true){
		pid_t pid = fork();

		if (pid < 0){
			std::cout << "Was not able to create backup process\n";
			exit(1);
		}
		else if (pid == 0){
			// if backup doesn't exist, run, start another one
			if (system("pidof -x backupProcess > /dev/null"))
				execlp("./BackupProcess", NULL);
			else {
				std::cout << "Was not able to create backup process\n";
				exit(1);
			}
			break;
		}

		// wait until backup stops
		int statusFromBackup;
		waitpid(pid, &statusFromBackup, 0);
	}
}



