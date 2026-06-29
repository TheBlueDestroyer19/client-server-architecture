#include <stdio.h>

#include "../include/ipc.h"
#include "transport.h"

STATUS_CODE ipc_transport_send(sem_t *empty,
	sem_t *full,
	const char *filename,
	const char *msg) {

	if(sem_wait(empty) == -1)
		return FAILURE;

	FILE *fptr = fopen(filename, "w");

	if(fptr == NULL) {
		sem_post(empty);
		return FAILURE;
	}

	fprintf(fptr, "%s", msg);

	fclose(fptr);

	sem_post(full);

	return SUCCESS;
}

STATUS_CODE ipc_transport_receive(sem_t *empty,
	sem_t *full,
	const char *filename,
	char *msg) {

	if(sem_wait(full) == -1)
		return FAILURE;

	FILE *fptr = fopen(filename, "r");

	if(fptr == NULL) {
		sem_post(empty);
		return FAILURE;
	}

	if(fgets(msg, SIZE, fptr) == NULL) {
		fclose(fptr);
		sem_post(empty);
		return FAILURE;
	}

	fclose(fptr);

	sem_post(empty);

	return SUCCESS;
}