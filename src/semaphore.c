#include <semaphore.h>
#include <fcntl.h>
#include <stddef.h>

#include "../include/ipc.h"
#include "protocol.h"
#include "semaphore.h"

STATUS_CODE ipc_semaphore_open(IPCConnection *conn) {

	conn->msg.empty = sem_open("/msg_empty", 0);
	conn->msg.full = sem_open("/msg_full", 0);
	conn->msg.filename = REQUEST_FILE;

	conn->hand_syn.empty = sem_open("/syn_empty", 0);
	conn->hand_syn.full = sem_open("/syn_full", 0);
	conn->hand_syn.filename = HAND_REQ_SYN_FILE;

	conn->hand_ack.empty = sem_open("/ack_empty", 0);
	conn->hand_ack.full = sem_open("/ack_full", 0);
	conn->hand_ack.filename = HAND_REQ_ACK_FILE;

	conn->hand_res.empty = sem_open("/res_empty", 0);
	conn->hand_res.full = sem_open("/res_full", 0);
	conn->hand_res.filename = HAND_RES_FILE;

	conn->connection_slot = sem_open("/connection_slot", 0);

	if(conn->msg.empty == SEM_FAILED ||
	   conn->msg.full == SEM_FAILED ||
	   conn->hand_syn.empty == SEM_FAILED ||
	   conn->hand_syn.full == SEM_FAILED ||
	   conn->hand_ack.empty == SEM_FAILED ||
	   conn->hand_ack.full == SEM_FAILED ||
	   conn->hand_res.empty == SEM_FAILED ||
	   conn->hand_res.full == SEM_FAILED ||
	   conn->connection_slot == SEM_FAILED)
		return FAILURE;

	conn->connected = 0;

	return SUCCESS;
}

void ipc_semaphore_close(IPCConnection *conn) {

	if(conn == NULL)
		return;

	if(conn->msg.empty != SEM_FAILED && conn->msg.empty != NULL)
		sem_close(conn->msg.empty);

	if(conn->msg.full != SEM_FAILED && conn->msg.full != NULL)
		sem_close(conn->msg.full);

	if(conn->hand_syn.empty != SEM_FAILED && conn->hand_syn.empty != NULL)
		sem_close(conn->hand_syn.empty);

	if(conn->hand_syn.full != SEM_FAILED && conn->hand_syn.full != NULL)
		sem_close(conn->hand_syn.full);

	if(conn->hand_ack.empty != SEM_FAILED && conn->hand_ack.empty != NULL)
		sem_close(conn->hand_ack.empty);

	if(conn->hand_ack.full != SEM_FAILED && conn->hand_ack.full != NULL)
		sem_close(conn->hand_ack.full);

	if(conn->hand_res.empty != SEM_FAILED && conn->hand_res.empty != NULL)
		sem_close(conn->hand_res.empty);

	if(conn->hand_res.full != SEM_FAILED && conn->hand_res.full != NULL)
		sem_close(conn->hand_res.full);

	if(conn->connection_slot != SEM_FAILED && conn->connection_slot != NULL)
		sem_close(conn->connection_slot);
}