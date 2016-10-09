#ifndef _PROCESSWRAPPER_H_
#define _PROCESSWRAPPER_H_

#include "systemc.h"
#include "dol.h"
#include "dol_sched_if.h"
#include "Fifo.h"
#include "WindowedFifo.h"
#include "functional_trace.h"
#include "Performance_Extraction.h"


#ifdef INCLUDE_PROFILER
extern FILE *profiler_output_file;
extern unsigned int profiler_event_counter;
#endif

class ProcessWrapper : virtual public dol_sched_if, public sc_module
{
    public:
        ProcessWrapper(sc_module_name name);
        virtual ~ProcessWrapper();
        virtual void initialize();
        virtual int fire();
        virtual bool isDetached() { return _isDetached; }
        virtual void detach();
        virtual int getIndex(unsigned indexNumber) const;
        virtual char* getName() const;

#ifdef INCLUDE_PROFILER
        virtual void addToProfile(const char *event, void *port,
                int length);
#endif

#ifdef INCLUDE_TRACE
        int start_line;
        int end_line;
        char channel_name[NAME_LENGTH];
#endif

#ifdef INCLUDE_PERFORMANCE
        int start_line;
        int end_line;
        CURRENT_TIME start_time;
        CURRENT_TIME end_time;
#endif

    protected:
        char* _name;
        DOLProcess _process;
        bool _isDetached;
        int _iteratorIndex[4];
        virtual int getIndex(const char* string, char* tokens,
                int indexNumber) const;
};

#endif

