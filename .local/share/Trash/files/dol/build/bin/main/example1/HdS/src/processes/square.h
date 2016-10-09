#ifndef SQUARE_H
#define SQUARE_H

#include <dol.h>
#include "global.h"

#define PORT_IN   ((static_cast<square_wrapper *>(p->wptr))->INPORT_1)
#define PORT_OUT  ((static_cast<square_wrapper *>(p->wptr))->OUTPORT_2)

typedef struct _local_states {
    int index;
    int len;
} Square_State;

void square_init(DOLProcess *);
int square_fire(DOLProcess *);

#endif
