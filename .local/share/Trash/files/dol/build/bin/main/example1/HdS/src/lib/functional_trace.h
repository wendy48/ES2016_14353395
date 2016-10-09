#ifndef _FUNCTIONAL_H
#define _FUNCTIONAL_H

#include "trace.h"

typedef struct EVENT_ENTRY
{
    char event_type;
    int start_line;
    int end_line;
    int data_num;
    char *channel_name;

    EVENT_ENTRY *next;
}EVENT_ENTRY;

class Process_Trace
{
private:
    char _process_name[NAME_LENGTH];
    EVENT_ENTRY *_head;
    EVENT_ENTRY *_tail;

public:
    Process_Trace(const char *process_name);
    ~Process_Trace();

    const char *get_name();
    int add_entry(TRACE_EVENT *trace_event_ptr);
    EVENT_ENTRY *get_head_entry();
};

class functional_trace
{
public:
    char trace_file_name[NAME_LENGTH];
    unsigned long event_num;
    int file_index;

    list<Process_Trace *> _list_process_trace;
    list<Process_Trace *>::iterator _iter_process_trace;

    functional_trace();
    ~functional_trace();

    void add_event_node(TRACE_EVENT &trace_event);
    void create_computation_event(const char *process_name, int start_line, int end_line);
    void create_read_event(const char *process_name, int data_num, const char *channel_name);
    void create_write_event(const char *process_name, int data_num, const char *channel_name);
    int write_to_file(const char *trace_file_name);
    Process_Trace *get_process_trace(const char *process_name);
    void free_traces();
};

extern functional_trace dol_functional_trace;

#endif
