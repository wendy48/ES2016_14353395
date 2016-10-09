#include "dolSupport.h"
#include "ProcessWrapper.h"

/**
 *
 */
unsigned write(void *port, void *buf, unsigned len, DOLProcess *process)
{
    Fifo *fifo = static_cast<Fifo *>(port);
    char *str = static_cast<char*>(buf);
    #ifdef INCLUDE_PROFILER
    (static_cast<ProcessWrapper *>(process->wptr))->addToProfile(
            "w", port, len);
    #endif
    fifo->write((void*)str, len);
    #ifdef INCLUDE_TRACE
    strcpy((static_cast<ProcessWrapper *>(process->wptr))->channel_name,
            fifo->getName());
    #endif
    return len;
}


/**
 *
 */
unsigned read(void *port, void *buf, unsigned len, DOLProcess *process) {
    Fifo *fifo = static_cast<Fifo *>(port);
    char *str = static_cast<char*>(buf);
    #ifdef INCLUDE_PROFILER
    (static_cast<ProcessWrapper *>(process->wptr))->addToProfile(
            "r", port, len);
    #endif
    fifo->read((void*)str, len);
    #ifdef INCLUDE_TRACE
    strcpy((static_cast<ProcessWrapper *>(process->wptr))->channel_name,
            fifo->getName());
    #endif
    return len;
}


/**
 *
 */
int wtest(void *port, unsigned len, DOLProcess *process) {
    Fifo *fifo = static_cast<Fifo *>(port);
    return (fifo->unused() >= len) ? 1 : 0;
}


/**
 *
 */
int rtest(void *port, unsigned len, DOLProcess *process) {
    Fifo *fifo = static_cast<Fifo *>(port);
    return (fifo->used() >= len) ? 1 : 0;
}


/**
 *
 */
unsigned reserve(void* fifo, void** destination, unsigned len, DOLProcess* p) {
    return ((WindowedFifo*)fifo)->reserve(destination, len);
}

/**
 *
 */
void release(void* fifo, DOLProcess* p) {
    ((WindowedFifo*)fifo)->release();
}

/**
 *
 */
unsigned capture(void* fifo, void** destination, unsigned len, DOLProcess* p) {
    return ((WindowedFifo*)fifo)->capture(destination, len);
}

/**
 *
 */
void consume(void* fifo, DOLProcess* p) {
    ((WindowedFifo*)fifo)->consume();
}


/**
 *
 */
void DOL_detach(DOLProcess* p) {
    static_cast<ProcessWrapper *>(p->wptr)->detach();
}


/**
 *
 */
void createPort(void** port,
                void* base,
                int number_of_indices,
                int index0, int range0) {
    *port = (void**)((void**)base)[index0];
}


/**
 *
 */
void createPort(void** port,
                void* base,
                int number_of_indices,
                int index0, int range0,
                int index1, int range1) {
    *port = (void**)((void**)base)[index0 * range1 + index1];
}


/**
 *
 */
void createPort(void** port,
                void* base,
                int number_of_indices,
                int index0, int range0,
                int index1, int range1,
                int index2, int range2) {
    *port = (void**)((void**)base)[index0 * range1 * range2
                       + index1 * range2 + index2];
}


/**
 *
 */
void createPort(void** port,
                void* base,
                int number_of_indices,
                int index0, int range0,
                int index1, int range1,
                int index2, int range2,
                int index3, int range3) {
    *port = (void**)((void**)base)[index0 * range1 * range2 * range3
                       + index1 * range2 * range3
                       + index2 * range3
                       + index3];
}
