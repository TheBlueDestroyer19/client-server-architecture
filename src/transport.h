#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <semaphore.h>

#include "../include/ipc.h"

STATUS_CODE ipc_transport_send(sem_t *empty,
						   sem_t *full,
						   const char *filename,
						   const char *msg);

STATUS_CODE ipc_transport_receive(sem_t *empty,
							  sem_t *full,
							  const char *filename,
							  char *msg);

#endif
