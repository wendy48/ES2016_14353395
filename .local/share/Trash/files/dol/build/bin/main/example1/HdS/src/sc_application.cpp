#include <systemc>
#include <list>
#ifdef INCLUDE_PROFILER
#include <stdio.h>
#endif
#include "dol_sched_if.h"

#include "generator_wrapper.h"
#include "consumer_wrapper.h"
#include "square_wrapper.h"

using namespace std;

#ifdef INCLUDE_PROFILER
#define PROFILER_OUTPUT_FILENAME "profile.txt"
FILE *profiler_output_file;
unsigned int profiler_event_counter;
#endif

class sc_application : public sc_module 
{
    public:
    SC_HAS_PROCESS(sc_application);

    generator_wrapper generator_ins;
    sc_event generator_event;
    consumer_wrapper consumer_ins;
    sc_event consumer_event;
    square_wrapper square_ins;
    sc_event square_event;

    sc_event sched_event;
    list<sc_event* > eventList;
    list<sc_event* >::iterator iter;


    Fifo C1_ins;
    Fifo C2_ins;

    sc_application(sc_module_name name)
    :       sc_module(name),
    generator_ins("generator"),
    consumer_ins("consumer"),
    square_ins("square"),
    C1_ins("C1", 10),
    C2_ins("C2", 10)
    {
        generator_ins.OUTPORT_1 = &C1_ins;
        square_ins.INPORT_1 = &C1_ins;
        square_ins.OUTPORT_2 = &C2_ins;
        consumer_ins.INPORT_1 = &C2_ins;


        SC_THREAD(thread_init);
        SC_THREAD(thread_sched);
        SC_THREAD(thread_generator);
        SC_THREAD(thread_consumer);
        SC_THREAD(thread_square);
    }

    void thread_init()
    {
        generator_ins.initialize();
        consumer_ins.initialize();
        square_ins.initialize();
    }

    void thread_sched()
    {
        while (1)
        {
            for (iter=eventList.begin(); iter != eventList.end(); ++iter)
            {
                sc_event* e = (*iter);
                e->notify();
            }
            eventList.clear();
            wait(sched_event);
        }
    }

    void thread_generator()
    {
        while (!generator_ins.isDetached())
        {
            #ifdef INCLUDE_PROFILER
            if (profiler_output_file != NULL) fprintf(profiler_output_file, "%u generator started.\n", profiler_event_counter++);
            #endif
            generator_ins.fire();
            #ifdef INCLUDE_PROFILER
            if (profiler_output_file != NULL) fprintf(profiler_output_file, "%u generator stopped.\n", profiler_event_counter++);
            #endif
            eventList.push_back(&generator_event);
            sched_event.notify();
            wait(generator_event);
        }
    }
    void thread_consumer()
    {
        while (!consumer_ins.isDetached())
        {
            #ifdef INCLUDE_PROFILER
            if (profiler_output_file != NULL) fprintf(profiler_output_file, "%u consumer started.\n", profiler_event_counter++);
            #endif
            consumer_ins.fire();
            #ifdef INCLUDE_PROFILER
            if (profiler_output_file != NULL) fprintf(profiler_output_file, "%u consumer stopped.\n", profiler_event_counter++);
            #endif
            eventList.push_back(&consumer_event);
            sched_event.notify();
            wait(consumer_event);
        }
    }
    void thread_square()
    {
        while (!square_ins.isDetached())
        {
            #ifdef INCLUDE_PROFILER
            if (profiler_output_file != NULL) fprintf(profiler_output_file, "%u square started.\n", profiler_event_counter++);
            #endif
            square_ins.fire();
            #ifdef INCLUDE_PROFILER
            if (profiler_output_file != NULL) fprintf(profiler_output_file, "%u square stopped.\n", profiler_event_counter++);
            #endif
            eventList.push_back(&square_event);
            sched_event.notify();
            wait(square_event);
        }
    }
    #ifdef INCLUDE_PROFILER
    void initialize_profiler()
    {
        if ((profiler_output_file = fopen(PROFILER_OUTPUT_FILENAME,"w"))==NULL)
        {
            printf("Unable to open profiler output file. No profiling output is written.\n");
            return;
        }

        fprintf(profiler_output_file, "c C1 10 o generator %pO i square %pI\n",(generator_ins.OUTPORT_1),(square_ins.INPORT_1));
        fprintf(profiler_output_file, "c C2 10 o square %pO i consumer %pI\n",(square_ins.OUTPORT_2),(consumer_ins.INPORT_1));
    }
    #endif

}
;
int sc_main (int argc, char *argv[])
{
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
    sc_report::register_id(5000, "parameter problem" );
    sc_application my_app_mdl("example1_flattened");
    #ifdef INCLUDE_PROFILER
    my_app_mdl.initialize_profiler();
    #endif
    sc_start(-1,SC_NS);
    #ifdef INCLUDE_PROFILER
    if (profiler_output_file != NULL) fclose(profiler_output_file);
    #endif
    return 0;
}
