#ifndef CONSUMER_H
#define CONSUMER_H

#include <dol.h>
#include "global.h"

#define PORT_IN  ((static_cast<consumer_wrapper *>(p->wptr))->INPORT_1)

typedef struct _local_states {
    char name[10];
    int index;
    int len;
} Consumer_State;

void consumer_init(DOLProcess *);
int consumer_fire(DOLProcess *);

#endif
