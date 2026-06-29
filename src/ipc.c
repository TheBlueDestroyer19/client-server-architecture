#include <string.h>

#include "../include/ipc.h"
#include "protocol.h"
#include "semaphore.h"
#include "transport.h"

STATUS_CODE ipc_open(IPCConnection *conn) {

	if(conn == NULL)
		return FAILURE;

	if(ipc_semaphore_open(conn) == FAILURE)
		return FAILURE;

	conn->connected = 0;

	return SUCCESS;
}

void ipc_close(IPCConnection *conn) {

	if(conn == NULL)
		return;

	ipc_semaphore_close(conn);
}

STATUS_CODE ipc_send(IPCConnection *conn, const char *msg) {

	if(conn == NULL || !conn->connected)
		return FAILURE;

	return ipc_transport_send(conn->msg.empty,
				  conn->msg.full,
				  conn->msg.filename,
				  msg);
}

STATUS_CODE ipc_receive(IPCConnection *conn, char *msg) {

	if(conn == NULL || !conn->connected)
		return FAILURE;

	if(ipc_transport_receive(conn->msg.empty,
				 conn->msg.full,
				 conn->msg.filename,
				 msg) == FAILURE)
		return FAILURE;

	if(strcmp(msg, EOF_MSG) == 0) {
		conn->connected = 0;
		return DISCONNECTED;
	}

	return SUCCESS;
}

STATUS_CODE ipc_connect(IPCConnection *conn) {

	char msg[SIZE];

	if(conn == NULL)
		return FAILURE;

	if(sem_wait(conn->connection_slot) == -1)
		return FAILURE;

	if(ipc_transport_send(conn->hand_syn.empty,
			      conn->hand_syn.full,
			      conn->hand_syn.filename,
			      SYN) == FAILURE) {

		sem_post(conn->connection_slot);
		return FAILURE;
	}

	if(ipc_transport_receive(conn->hand_res.empty,
				 conn->hand_res.full,
				 conn->hand_res.filename,
				 msg) == FAILURE) {

		sem_post(conn->connection_slot);
		return FAILURE;
	}

	if(strcmp(msg, SYN) != 0) {
		sem_post(conn->connection_slot);
		return FAILURE;
	}

	if(ipc_transport_send(conn->hand_ack.empty,
			      conn->hand_ack.full,
			      conn->hand_ack.filename,
			      ACK) == FAILURE) {

		sem_post(conn->connection_slot);
		return FAILURE;
	}

	conn->connected = 1;

	return SUCCESS;
}

STATUS_CODE ipc_accept(IPCConnection *conn) {

	char msg[SIZE];

	if(conn == NULL)
		return FAILURE;

	if(ipc_transport_receive(conn->hand_syn.empty,
				 conn->hand_syn.full,
				 conn->hand_syn.filename,
				 msg) == FAILURE)
		return FAILURE;

	if(strcmp(msg, SYN) != 0)
		return FAILURE;

	if(ipc_transport_send(conn->hand_res.empty,
			      conn->hand_res.full,
			      conn->hand_res.filename,
			      SYN) == FAILURE)
		return FAILURE;

	if(ipc_transport_receive(conn->hand_ack.empty,
				 conn->hand_ack.full,
				 conn->hand_ack.filename,
				 msg) == FAILURE)
		return FAILURE;

	if(strcmp(msg, ACK) != 0)
		return FAILURE;

	conn->connected = 1;

	return SUCCESS;
}

STATUS_CODE ipc_disconnect(IPCConnection *conn) {

	if(conn == NULL || !conn->connected)
		return FAILURE;

	if(ipc_send(conn, EOF_MSG) == FAILURE)
		return FAILURE;

	conn->connected = 0;

	sem_post(conn->connection_slot);

	return SUCCESS;
}

STATUS_CODE ipc_server_disconnect(IPCConnection *conn) {

	if(conn == NULL)
		return FAILURE;

	conn->connected = 0;

	return SUCCESS;
}
