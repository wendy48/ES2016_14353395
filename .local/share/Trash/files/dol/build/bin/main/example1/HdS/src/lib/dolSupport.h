#ifndef DOLSUPPORT_H
#define DOLSUPPORT_H

#include "dol.h"
#include "Fifo.h"
#include "functional_trace.h"
#include "Performance_Extraction.h"

#ifdef INCLUDE_PERFORMANCE
#define DOL_write(port, buf, len, process) {\
    (static_cast<ProcessWrapper *>(p->wptr))->end_line = __LINE__;\
    get_current_time(&((static_cast<ProcessWrapper *>(p->wptr))->\
    end_time));\
    performance_extraction.add_computation_performance(\
    (static_cast<ProcessWrapper *>(p->wptr))->basename(),\
    (static_cast<ProcessWrapper *>(p->wptr))->start_line,\
    (static_cast<ProcessWrapper *>(p->wptr))->end_line,\
    &((static_cast<ProcessWrapper *>(p->wptr))->start_time),\
    &((static_cast<ProcessWrapper *>(p->wptr))->end_time));\
    write(port, buf, len, process);\
    (static_cast<ProcessWrapper *>(p->wptr))->start_line = __LINE__;\
    get_current_time(&((static_cast<ProcessWrapper *>(p->wptr))->\
    start_time)); }
#define DOL_read(port, buf, len, process) {\
    (static_cast<ProcessWrapper *>(p->wptr))->end_line = __LINE__;\
    get_current_time(&((static_cast<ProcessWrapper *>(p->wptr))->\
    end_time));\
    performance_extraction.add_computation_performance(\
    (static_cast<ProcessWrapper *>(p->wptr))->basename(),\
    (static_cast<ProcessWrapper *>(p->wptr))->start_line,\
    (static_cast<ProcessWrapper *>(p->wptr))->end_line,\
    &((static_cast<ProcessWrapper *>(p->wptr))->start_time),\
    &((static_cast<ProcessWrapper *>(p->wptr))->end_time));\
    read(port, buf, len, process);\
    (static_cast<ProcessWrapper *>(p->wptr))->start_line = __LINE__;\
    get_current_time(&((static_cast<ProcessWrapper *>(p->wptr))->\
    start_time)); }
#elif INCLUDE_TRACE
#define DOL_write(port, buf, len, process) {\
    (static_cast<ProcessWrapper *>(p->wptr))->end_line = __LINE__;\
    dol_functional_trace.create_computation_event(\
    (static_cast<ProcessWrapper *>(p->wptr))->basename(),\
    (static_cast<ProcessWrapper *>(p->wptr))->start_line,\
    (static_cast<ProcessWrapper *>(p->wptr))->end_line);\
    write(port, buf, len, process);\
    dol_functional_trace.create_write_event(\
    (static_cast<ProcessWrapper *>(p->wptr))->basename(), len,\
    (static_cast<ProcessWrapper *>(p->wptr))->channel_name);\
    (static_cast<ProcessWrapper *>(p->wptr))->start_line = __LINE__; }
#define DOL_read(port, buf, len, process) {\
    (static_cast<ProcessWrapper *>(p->wptr))->end_line = __LINE__;\
    dol_functional_trace.create_computation_event(\
    (static_cast<ProcessWrapper *>(p->wptr))->basename(),\
    (static_cast<ProcessWrapper *>(p->wptr))->start_line,\
    (static_cast<ProcessWrapper *>(p->wptr))->end_line);\
    read(port, buf, len, process);\
    dol_functional_trace.create_read_event(\
    (static_cast<ProcessWrapper *>(p->wptr))->basename(), len,\
    (static_cast<ProcessWrapper *>(p->wptr))->channel_name);\
    (static_cast<ProcessWrapper *>(p->wptr))->start_line = __LINE__; }
#else
    #define DOL_write(port, buf, len, process) \
    write(port, buf, len, process)
    #define DOL_read(port, buf, len, process) \
    read(port, buf, len, process)
#endif

#define DOL_reserve(port, buf, size, process) \
    reserve(port, (void**)buf, size, process);

#define DOL_release(port, process) \
    release(port, process);

#define DOL_capture(port, buf, size, process) \
    capture(port, (void**)buf, size, process);

#define DOL_consume(port, process) \
    consume(port, process);

#define DOL_wtest(port, len, process) wtest(port, len, process)

#define DOL_rtest(port, len, process) rtest(port, len, process)

void DOL_detach(DOLProcess* p);

//fifo access functions
unsigned write(void* fifo, void* buf, unsigned len, DOLProcess* p);
unsigned read(void* fifo, void* buf, unsigned len, DOLProcess* p);
int wtest(void *port, unsigned len, DOLProcess *process);
int rtest(void *port, unsigned len, DOLProcess *process);

//windowed fifo access functions
unsigned reserve(void* fifo, void** destination, unsigned len, DOLProcess* p);
void release(void* fifo, DOLProcess* p);
unsigned capture(void* fifo, void** destination, unsigned len, DOLProcess* p);
void consume(void* fifo, DOLProcess* p);


void createPort(void **port,
                void *base,
                int number_of_indices,
                int index0, int range0);

void createPort(void **port,
                void *base,
                int number_of_indices,
                int index0, int range0,
                int index1, int range1);

void createPort(void **port,
                void *base,
                int number_of_indices,
                int index0, int range0,
                int index1, int range1,
                int index2, int range2);

void createPort(void **port,
                void *base,
                int number_of_indices,
                int index0, int range0,
                int index1, int range1,
                int index2, int range2,
                int index3, int range3);

#define GETINDEX(dimension) \
  static_cast<ProcessWrapper *>(p->wptr)->getIndex(dimension)

/**
 * macro to create a variable to store a port name
 *
 * @param name name of the variable
 */
#define CREATEPORTVAR(name) Fifo *name

/**
 * Create the port name of an iterated port based on its basename and the
 * given indices.
 *
 * @param port buffer where the result is stored (created using
 *             CREATEPORTVAR)
 * @param base basename of the port
 * @param number_of_indices number of dimensions of the port
 * @param index_range_pairs index and range values for each dimension
 */
#define CREATEPORT(port, base, number_of_indices, index_range_pairs...) \
  createPort((void**)(&port), base, number_of_indices, index_range_pairs)

#endif
