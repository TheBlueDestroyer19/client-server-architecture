#ifndef IPC_H
#define IPC_H

#include <semaphore.h>

#define SIZE 1024

typedef enum {
	FAILURE,
	SUCCESS,
  DISCONNECTED
} STATUS_CODE;

typedef struct {
	sem_t *empty;
	sem_t *full;
	const char *filename;
} TransportChannel;

typedef struct {
	TransportChannel msg;
	TransportChannel hand_syn;
	TransportChannel hand_ack;
	TransportChannel hand_res;

	sem_t *connection_slot;

	int connected;
} IPCConnection;

STATUS_CODE ipc_open(IPCConnection *conn);

void ipc_close(IPCConnection *conn);

STATUS_CODE ipc_connect(IPCConnection *conn);

STATUS_CODE ipc_accept(IPCConnection *conn);

STATUS_CODE ipc_send(IPCConnection *conn, const char *msg);

STATUS_CODE ipc_receive(IPCConnection *conn, char *msg);

STATUS_CODE ipc_disconnect(IPCConnection *conn);

STATUS_CODE ipc_server_disconnect(IPCConnection *conn);

STATUS_CODE ipc_init(void);

STATUS_CODE ipc_cleanup(void);

#endif
