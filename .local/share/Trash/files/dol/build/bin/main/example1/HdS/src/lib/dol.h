#ifndef DOL_H
#define DOL_H

/************************************************************************
 * do not add code to this header
 ************************************************************************/

/**
 *  Define the DOL process handler scheme.
 *  - Local variables are defined in structure LocalState. Local
 *    variables may vary from different processes.
 *  - The ProcessInit function pointer points to a function which
 *    initializes a process.
 *  - The ProcessFire function pointer points to a function which
 *    performs the actual computation. The communication between
 *    processes is inside the ProcessFire function.
 *  - The WPTR is a placeholder for callback. One can just
 *    leave it blank.
 */

//structure for local memory of process
typedef struct _local_states *LocalState;

//additional behavioral functions could be declared here
typedef void (*ProcessInit)(struct _process*);
typedef int (*ProcessFire)(struct _process*);
typedef void *WPTR;

//process handler
struct _process;

typedef struct _process {
    LocalState     local;
    ProcessInit    init;
    ProcessFire    fire;
    WPTR           wptr; //placeholder for wrapper instance
} DOLProcess;

#endif
