#ifndef _PERFORMANCE_EXTRACTION_H
#define _PERFORMANCE_EXTRACTION_H

#include <sys/time.h>
#include <time.h>
#include "trace.h"

#define SYS_OVERHEAD 1400 //what should be exact vale
#define HOST_FREQUENCY 3 //Ghz

typedef struct COMP_ENTRY {
    int start_line;
    int end_line;
    double total_computation_time;
    int called_times;
    COMP_ENTRY *next;
} COMP_ENTRY;

typedef struct CURRENT_TIME {
    struct timespec ts;
}CURRENT_TIME;

extern  int get_current_time(CURRENT_TIME *current_time_ptr);
extern double sub_time(CURRENT_TIME *start_time_ptr, CURRENT_TIME *end_time_ptr);

class Process_Performance
{
private:
    char _process_name[NAME_LENGTH];
    COMP_ENTRY *_head;
    COMP_ENTRY *_tail;

public:
    Process_Performance(const char *process_name);
    ~Process_Performance();

    const char *get_name();
    COMP_ENTRY * add_entry(int start_line, int end_line);
    int set_entry(int start_line, int end_line, CURRENT_TIME *start_time_ptr,
                  CURRENT_TIME *end_time_ptr);
    COMP_ENTRY *get_entry(int start_line, int end_line);
    COMP_ENTRY *get_head_entry();
};

class Performance_Extraction
{
private:
    char _processor_type[NAME_LENGTH];
    char _chr_file_name[NAME_LENGTH];
    list<Process_Performance *> _list_process_performance;
    list<Process_Performance *>::iterator _iter_process_performance;

public:
    Performance_Extraction(const char *chr_file_name);
    ~Performance_Extraction();

    int add_computation_performance(const char *process_name, int start_line,
                                    int end_line, CURRENT_TIME *start_time_ptr,
                                    CURRENT_TIME *end_time_ptr);
    Process_Performance *get_process_performance(const char *process_name);
    int write_to_xml_file(const char *chr_file_name);
    int add_to_xml_file(const char *chr_file_name);
};

extern Performance_Extraction performance_extraction;

#endif
