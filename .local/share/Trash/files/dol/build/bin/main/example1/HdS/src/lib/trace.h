#ifndef _TRACE_H
#define _TRACE_H

#include <stdio.h>
#include <string.h>
#include "xmlParser.h"
#include <list>

using namespace std;

#define NAME_LENGTH 256
#ifdef DBG
#define DBGPRINT printf("file: %s--line: %d\n", __FILE__, __LINE__)
#else
#define DBGPRINT
#endif

typedef enum TRACE_EVENT_TYPE
{
    COMPUTATION_EVENT,
    READ_EVENT,
    WRITE_EVENT,
    UNKOWN
}TRACE_EVENT_TYPE;

#define MAX_EVENT_TYPE 3

typedef struct TRACE_EVENT
{
    char process_name[NAME_LENGTH];
    char event_type;
    int computation_start_line;
    int computation_end_line;
    int data_num;
    char channel_name[NAME_LENGTH];
}TRACE_EVENT;

#endif
