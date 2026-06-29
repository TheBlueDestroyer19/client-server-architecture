#include <stdio.h>
#include <string.h>

#include "include/ipc.h"

int main() {

	IPCConnection conn;

	if(ipc_open(&conn) == FAILURE) {
		printf("Failed to initialize IPC.\n");
		return 1;
	}

	if(ipc_connect(&conn) == FAILURE) {
		printf("Failed to connect to server.\n");
		ipc_close(&conn);
		return 1;
	}

	printf("Connected to server.\n");

	while(1) {

    char msg[SIZE];

    printf("Enter message:\n");

    if(fgets(msg, SIZE, stdin) == NULL)
      break;

    msg[strcspn(msg, "\n")] = '\0';

    if(strcmp(msg, "exit") == 0) {
      ipc_disconnect(&conn);
      break;
    }

    if(ipc_send(&conn, msg) == FAILURE) {
      printf("Failed to send message.\n");
      break;
    }
  }

	ipc_close(&conn);

	return 0;
}
