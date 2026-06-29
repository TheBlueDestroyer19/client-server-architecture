#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>

#include "../include/ipc.h"

STATUS_CODE ipc_init() {

	sem_unlink("/msg_empty");
	sem_unlink("/msg_full");

	sem_unlink("/syn_empty");
	sem_unlink("/syn_full");

	sem_unlink("/ack_empty");
	sem_unlink("/ack_full");

	sem_unlink("/res_empty");
	sem_unlink("/res_full");

	sem_unlink("/connection_slot");

	sem_t *msg_empty = sem_open("/msg_empty", O_CREAT, 0666, 1);
	sem_t *msg_full = sem_open("/msg_full", O_CREAT, 0666, 0);

	sem_t *syn_empty = sem_open("/syn_empty", O_CREAT, 0666, 1);
	sem_t *syn_full = sem_open("/syn_full", O_CREAT, 0666, 0);

	sem_t *ack_empty = sem_open("/ack_empty", O_CREAT, 0666, 1);
	sem_t *ack_full = sem_open("/ack_full", O_CREAT, 0666, 0);

	sem_t *res_empty = sem_open("/res_empty", O_CREAT, 0666, 1);
	sem_t *res_full = sem_open("/res_full", O_CREAT, 0666, 0);

	sem_t *connection_slot = sem_open("/connection_slot", O_CREAT, 0666, 1);

	if(msg_empty == SEM_FAILED || msg_full == SEM_FAILED ||
	   syn_empty == SEM_FAILED || syn_full == SEM_FAILED ||
	   ack_empty == SEM_FAILED || ack_full == SEM_FAILED ||
	   res_empty == SEM_FAILED || res_full == SEM_FAILED ||
	   connection_slot == SEM_FAILED) {

		perror("sem_open");
		return FAILURE;
	}

	sem_close(msg_empty);
	sem_close(msg_full);

	sem_close(syn_empty);
	sem_close(syn_full);

	sem_close(ack_empty);
	sem_close(ack_full);

	sem_close(res_empty);
	sem_close(res_full);

	sem_close(connection_slot);

	printf("IPC initialized.\n");

	return SUCCESS;
}

STATUS_CODE ipc_cleanup() {

	sem_unlink("/msg_empty");
	sem_unlink("/msg_full");

	sem_unlink("/syn_empty");
	sem_unlink("/syn_full");

	sem_unlink("/ack_empty");
	sem_unlink("/ack_full");

	sem_unlink("/res_empty");
	sem_unlink("/res_full");

	sem_unlink("/connection_slot");

	remove("request");
	remove("hand_request_s");
	remove("hand_request_a");
	remove("hand_response");

	printf("IPC cleaned up.\n");

	return SUCCESS;
}

