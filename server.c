#include <stdio.h>
#include <string.h>

#include "include/ipc.h"

int main() {

	IPCConnection conn;

	if(ipc_open(&conn) == FAILURE) {
		printf("Failed to initialize IPC.\n");
		return 1;
	}

	while(1) {

		printf("Waiting for client...\n");

		if(ipc_accept(&conn) == FAILURE)
			continue;

		printf("Client connected.\n");

    while(1) {
      char msg[SIZE];

      STATUS_CODE sc = ipc_receive(&conn, msg);

      if(sc == DISCONNECTED) {
        printf("Client disconnected.\n");
        ipc_server_disconnect(&conn);
        break;
      }

      if(sc == FAILURE)
        break;

      printf("Message: %s\n", msg);
    }	
	}

	ipc_close(&conn);

	return 0;
}
