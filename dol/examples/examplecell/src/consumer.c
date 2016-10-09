#include <stdio.h>

#include "consumer.h"

void consumer_init(DOLProcess *p) {
    sprintf(p->local->name, "consumer");
    p->local->index = 0;
    p->local->len = LENGTH;
}

int consumer_fire(DOLProcess *p) {
    if (p->local->index < p->local->len) {
        DOL_read((void*)PORT_IN, &p->local->c, sizeof(float), p);
        printf("%s: %f\n", p->local->name, p->local->c);
        p->local->index++;
    } else {
        DOL_detach(p);
        return -1;
    }

    return 0;
}

