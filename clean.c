#include <stdio.h>
#include <semaphore.h>

int main() {

	sem_unlink("/empty1");
	sem_unlink("/full1");
	sem_unlink("/empty2");
	sem_unlink("/full2");
  sem_unlink("/connection_slot");

	remove("hand_request_a");
  remove("hand_request_s");
	remove("hand_response");

	printf("IPC cleaned up.\n");

	return 0;
}
