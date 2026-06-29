#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "../include/ipc.h"

STATUS_CODE ipc_semaphore_open(IPCConnection *conn);

void ipc_semaphore_close(IPCConnection *conn);

#endif
