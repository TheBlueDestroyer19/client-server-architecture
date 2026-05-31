#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>

int main() {

  sem_unlink("/empty1");
  sem_unlink("/full1");
  sem_unlink("/empty2");
  sem_unlink("/full2");

	sem_t *empty1 = sem_open("/empty1", O_CREAT, 0666, 1);
	sem_t *full1 = sem_open("/full1", O_CREAT, 0666, 0);

	sem_t *empty2 = sem_open("/empty2", O_CREAT, 0666, 1);
	sem_t *full2 = sem_open("/full2", O_CREAT, 0666, 0);

	if(empty1 == SEM_FAILED ||
	   full1 == SEM_FAILED ||
	   empty2 == SEM_FAILED ||
	   full2 == SEM_FAILED) {

		perror("sem_open");
		return 1;
	}

	sem_close(empty1);
	sem_close(full1);
	sem_close(empty2);
	sem_close(full2);

	printf("IPC initialized.\n");

	return 0;
}
