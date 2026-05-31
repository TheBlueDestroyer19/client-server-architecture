#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include "locks.h"

STATUS_CODE send(char *msg, char *req, sem_t *empty1, sem_t *full1) {
	if(sem_wait(empty1) == -1) {
		if(errno == EINTR)
			return FAILURE;
		return FAILURE;
	}

	FILE *fptr = fopen(req, "w");

	if(fptr == NULL) {
		sem_post(empty1);
		return FAILURE;
	}

	fprintf(fptr, "%s", msg);

	fclose(fptr);

	sem_post(full1);

	return SUCCESS;
}

STATUS_CODE client_init(char *req, char *res, sem_t *empty1, sem_t *full1, sem_t *empty2, sem_t *full2) {

	if(sem_wait(empty1) == -1)
		return FAILURE;

	FILE *fptr = fopen(req, "w");

	if(fptr == NULL) {
		sem_post(empty1);
		return FAILURE;
	}

	fprintf(fptr,"%c",SYN);

	fclose(fptr);

	sem_post(full1);

	if(sem_wait(full2) == -1)
		return FAILURE;

	FILE *fptr2 = fopen(res, "r");

	if(fptr2 == NULL) {
		sem_post(empty2);
		return FAILURE;
	}

	char c;

	fscanf(fptr2, "%c", &c);

	fclose(fptr2);

	sem_post(empty2);

	if(c == SYN) {

		if(sem_wait(empty1) == -1)
			return FAILURE;

		fptr = fopen(req, "w");

		if(fptr == NULL) {
			sem_post(empty1);
			return FAILURE;
		}

		fprintf(fptr,"%c",ACK);

		fclose(fptr);

		sem_post(full1);

		return SUCCESS;
	}

	return FAILURE;
}

int main() {

	sem_t *full1 = sem_open("/full1", 0);
	sem_t *full2 = sem_open("/full2", 0);

	sem_t *empty1 = sem_open("/empty1", 0);
	sem_t *empty2 = sem_open("/empty2", 0);

	if(full1 == SEM_FAILED || full2 == SEM_FAILED ||
	   empty1 == SEM_FAILED || empty2 == SEM_FAILED) {

		perror("sem_open");
		return 1;
	}

	if(client_init("request", "response", empty1, full1, empty2, full2) == SUCCESS)
		printf("Connected to the server\n");
	else {
		printf("Server connection failed!\n");
		goto End;
	}

	while(1) {

		char msg[1024];

		printf("Enter the message:\n");

		fgets(msg,1024,stdin);
    msg[strcspn(msg,"\n")]='\0';
		STATUS_CODE sc = send(msg, "request", empty1, full1);

    if(!strcmp(msg,EOF_MSG))
      break;

		if(sc == SUCCESS)
			printf("Message Sent\n");
		else
			printf("Failed to send the message!\n");
	}

	printf("\nDisconnecting from server...\n");

End:

	sem_close(empty1);
	sem_close(full1);
	sem_close(empty2);
	sem_close(full2);

	printf("Client shutdown complete.\n");

	return 0;
}
