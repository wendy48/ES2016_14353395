#include "functional_trace.h"

functional_trace dol_functional_trace;

functional_trace::functional_trace()
{
    //DBGPRINT;
    strcpy(trace_file_name, "trace1.txt");
    event_num = 0;
    file_index = 1;
}

functional_trace::~functional_trace()
{
    //DBGPRINT;
    char temp[NAME_LENGTH];
#ifdef INCLUDE_TRACE
    strcpy(trace_file_name, "trace");
    sprintf(temp, "%d", file_index);
    strcat(trace_file_name, temp);
    strcat(trace_file_name, ".txt");
    write_to_file(trace_file_name);
    free_traces();
    //printf("The traces in the memory have been freed\n");
#endif
    //DBGPRINT;
}

void functional_trace::add_event_node(TRACE_EVENT &trace_event)
{
    Process_Trace *process_trace_ptr;
    char temp[NAME_LENGTH];

    process_trace_ptr = get_process_trace(trace_event.process_name);
    if (process_trace_ptr == NULL)
    {
        process_trace_ptr = new Process_Trace(trace_event.process_name);
        _list_process_trace.push_back(process_trace_ptr);
    }

    process_trace_ptr->add_entry(&trace_event);

    event_num++;
    if (event_num > 8000000)
    {
        strcpy(trace_file_name, "trace");
        sprintf(temp, "%d", file_index);
        strcat(trace_file_name, temp);
        strcat(trace_file_name, ".txt");
        write_to_file(trace_file_name);
        free_traces();
        file_index++;
        event_num = 0;
    }

}

/*
void functional_trace::add_event_node(TRACE_EVENT &trace_event)
{
    XMLNode process_node;
    char temp[NAME_LENGTH];
    process_node = xml_main_node.getChildNode(trace_event.process_name);

    if(process_node.isEmpty())
    {
        process_node = xml_main_node.addChild(trace_event.process_name);
    }

    XMLNode event_node = process_node.addChild("event");

    event_node.addAttribute("type", trace_event_type_string[trace_event.event_type]);

    if(trace_event.event_type == COMPUTATION_EVENT)
    {

        sprintf(temp, "%d", trace_event.computation_start_line);
        event_node.addAttribute("start", temp);
        sprintf(temp, "%d", trace_event.computation_end_line);
        event_node.addAttribute("end", temp);
    }
    else if((trace_event.event_type == READ_EVENT) || (trace_event.event_type == WRITE_EVENT))
    {
        sprintf(temp, "%d", trace_event.data_num);
        event_node.addAttribute("data_num", temp);
        event_node.addAttribute("channel_name", trace_event.channel_name);
    }
}
*/

void functional_trace::create_computation_event(const char *process_name, int start_line, int end_line)
{
    TRACE_EVENT trace_event;

    trace_event.event_type = COMPUTATION_EVENT;
    strcpy(trace_event.process_name, process_name);
    trace_event.computation_start_line = start_line;
    trace_event.computation_end_line = end_line;
    strcpy(trace_event.channel_name, "");
    trace_event.data_num = 0;

    add_event_node(trace_event);
}

void functional_trace::create_read_event(const char *process_name, int data_num, const char *channel_name)
{
    //DBGPRINT;
    TRACE_EVENT trace_event;
    trace_event.event_type = READ_EVENT;
    strcpy(trace_event.process_name, process_name);
    trace_event.data_num = data_num;
    strcpy(trace_event.channel_name, channel_name);

    trace_event.computation_start_line = 0;
    trace_event.computation_end_line = 0;

    add_event_node(trace_event);
}

void functional_trace::create_write_event(const char *process_name, int data_num, const char *channel_name)
{
    //DBGPRINT;
    TRACE_EVENT trace_event;
    trace_event.event_type = WRITE_EVENT;
    strcpy(trace_event.process_name, process_name);
    trace_event.data_num = data_num;
    strcpy(trace_event.channel_name, channel_name);

    trace_event.computation_start_line = 0;
    trace_event.computation_end_line = 0;

    add_event_node(trace_event);
}


Process_Trace *functional_trace::get_process_trace(const char *process_name)
{
    Process_Trace *process_trace_ptr;

    for (_iter_process_trace = _list_process_trace.begin();
         _iter_process_trace != _list_process_trace.end();
         _iter_process_trace++)
    {
        process_trace_ptr = *_iter_process_trace;

        if (strcmp(process_trace_ptr->get_name(), process_name) == 0)
        {
            break;
        }
    }

    if (_iter_process_trace == _list_process_trace.end())
    {
        process_trace_ptr = NULL;
    }

    return process_trace_ptr;
}

void functional_trace::free_traces()
{
    Process_Trace *process_trace_ptr;

    for (_iter_process_trace = _list_process_trace.begin();
         _iter_process_trace != _list_process_trace.end();
         _iter_process_trace++)
    {
        process_trace_ptr = *_iter_process_trace;
        delete process_trace_ptr;
    }
    _list_process_trace.clear();
}

int functional_trace::write_to_file(const char *trace_file_name) {
    int ret = -1;
    Process_Trace *process_trace_ptr;
    FILE *trace_file_handle;
    EVENT_ENTRY *event_entry_ptr;

    if ((trace_file_handle = fopen(trace_file_name, "w")) == NULL) {
        ret = -1;
        printf("Can not create file: %s\n", trace_file_name);
        goto end1;
    }

    for (_iter_process_trace = _list_process_trace.begin();
         _iter_process_trace != _list_process_trace.end();
         _iter_process_trace++) {
        process_trace_ptr = *_iter_process_trace;
        fprintf(trace_file_handle, "$ %s\n", process_trace_ptr->get_name());

        event_entry_ptr = process_trace_ptr->get_head_entry();
        while (event_entry_ptr != NULL) {
            if (event_entry_ptr->event_type == COMPUTATION_EVENT) {
                if (event_entry_ptr->end_line!=(event_entry_ptr->start_line+1))
                fprintf(trace_file_handle, "c %d %d\n",
                        event_entry_ptr->start_line,
                        event_entry_ptr->end_line);
            }
            else if (event_entry_ptr->event_type == WRITE_EVENT) {
                fprintf(trace_file_handle, "w %d %s\n",
                        event_entry_ptr->data_num,
                        event_entry_ptr->channel_name);
            }
            else if (event_entry_ptr->event_type == READ_EVENT) {
                fprintf(trace_file_handle, "r %d %s\n",
                        event_entry_ptr->data_num,
                        event_entry_ptr->channel_name);
            }

            event_entry_ptr = event_entry_ptr->next;
        }
    }

    fflush(trace_file_handle);
    fseek(trace_file_handle, 0, SEEK_SET);
    fclose(trace_file_handle);

end1:
    return ret;
}

Process_Trace::Process_Trace(const char *process_name)
{
    strcpy(_process_name, process_name);
    _head = NULL;
    _tail = NULL;
}

Process_Trace::~Process_Trace()
{
    EVENT_ENTRY *temp;

    while (_head != NULL)
    {
        temp = _head;
        _head = _head->next;
        if (temp->channel_name != NULL)
            delete [] temp->channel_name;
        delete temp;
    }
}

const char *Process_Trace::get_name()
{
    return _process_name;
}

EVENT_ENTRY *Process_Trace::get_head_entry()
{
    return _head;
}

int Process_Trace::add_entry(TRACE_EVENT *trace_event_ptr)
{
    int ret = 0;
    EVENT_ENTRY *temp;
    int channel_name_length;

    temp = new EVENT_ENTRY;
    memset(temp, 0, sizeof(EVENT_ENTRY));

    temp->event_type = trace_event_ptr->event_type;
    temp->start_line = trace_event_ptr->computation_start_line;
    temp->end_line = trace_event_ptr->computation_end_line;
    temp->data_num = trace_event_ptr->data_num;

    channel_name_length = strlen(trace_event_ptr->channel_name);
    if (channel_name_length != 0)
    {
        temp->channel_name = new char[channel_name_length + 1];
        strcpy(temp->channel_name, trace_event_ptr->channel_name);
    }
    else
    {
        temp->channel_name = NULL;
    }

    if (_head == NULL)
    {
        _head = temp;
        _tail = temp;
        temp->next = NULL;
    }
    else
    {
        _tail->next = temp;
        _tail = temp;
        temp->next = NULL;
    }

    return ret;
}


