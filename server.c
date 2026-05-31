#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>

#include "locks.h"


STATUS_CODE getmsg(char *msg, char *req, sem_t *empty1, sem_t *full1) {
	if(sem_wait(full1) == -1)
		return FAILURE;

	FILE *fptr = fopen(req, "r");

	if(fptr == NULL) {
		sem_post(empty1);
		return FAILURE;
	}

	fgets(msg,1024,fptr);

	fclose(fptr);

	sem_post(empty1);

	return SUCCESS;
}

STATUS_CODE server_init(char *req, char *res, sem_t *empty1, sem_t *full1, sem_t *empty2, sem_t *full2) {
	char c;

	if(sem_wait(full1) == -1)
		return FAILURE;

	FILE *fptr = fopen(req, "r");

	if(fptr == NULL) {
		sem_post(empty1);
		return FAILURE;
	}

	fscanf(fptr, "%c", &c);

	fclose(fptr);

	sem_post(empty1);

	if(sem_wait(empty2) == -1)
		return FAILURE;

	FILE *fptr2 = fopen(res, "w");

	if(fptr2 == NULL) {
		sem_post(empty2);
		return FAILURE;
	}

	if(c == SYN) {

		fprintf(fptr2,"%c",SYN);

		fclose(fptr2);

		sem_post(full2);

		if(sem_wait(full1) == -1)
			return FAILURE;

		fptr = fopen(req, "r");

		if(fptr == NULL) {
			sem_post(empty1);
			return FAILURE;
		}

		fscanf(fptr, "%c", &c);

		fclose(fptr);

		sem_post(empty1);

		if(c == ACK)
			return SUCCESS;

		return FAILURE;
	}

	fprintf(fptr2,"%c", ACK);

	fclose(fptr2);

	sem_post(full2);

	return FAILURE;
}

int main() {
	sem_t *empty1 = sem_open("/empty1", 0);
	sem_t *full1 = sem_open("/full1",0);

	sem_t *empty2 = sem_open("/empty2",0);
	sem_t *full2 = sem_open("/full2", 0);

	char msg[1024];

	while(1) {

		printf("Waiting for client...\n");

		STATUS_CODE start =server_init("request","response",empty1,full1,empty2,full2);

		if(start != SUCCESS) {
			printf("Connection unsuccessful.\n");
			continue;
		}

		printf("Connected to client\n");
		printf("Waiting for messages...\n");

		while(1) {

			STATUS_CODE sc =
				getmsg(msg,
				       "request",
				       empty1,
				       full1);

			if(sc != SUCCESS)
				continue;

      if(!strcmp(msg,EOF_MSG)) {
        printf("Client Disconnected!\n\n");
        break;
      }

			printf("Message received: %s\n", msg);
		}
	}

	sem_close(empty1);
	sem_close(full1);
	sem_close(empty2);
	sem_close(full2);

	printf("Server shutdown.\n");

	return 0;
}
