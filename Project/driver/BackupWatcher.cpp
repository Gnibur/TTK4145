#include "BackupWatcher.h"

#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include <assert.h>

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

		// must be able to create backup process
		assert(pid >= 0);

		if (pid == 0){
			// if backup doesn't exist, run another one
			assert(system("pidof -x backupProcess"));
			execlp("./BackupProcess", NULL);
			break;
		}

		// wait until backup stops
		int statusFromBackup;
		waitpid(pid, &statusFromBackup, 0);
	}
}



