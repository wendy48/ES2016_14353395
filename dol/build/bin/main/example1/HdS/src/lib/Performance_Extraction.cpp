#include "Performance_Extraction.h"
#include <string>
#include <fstream>


Performance_Extraction performance_extraction("static_characterization.xml");

/**
 *
 */
int get_current_time(CURRENT_TIME *current_time_ptr)
{
    return clock_gettime(CLOCK_REALTIME, &(current_time_ptr->ts));
}


/**
 *
 */
double sub_time(CURRENT_TIME *start_time_ptr, CURRENT_TIME *end_time_ptr)
{
    CURRENT_TIME diff_time;
    double time_ns;

    diff_time.ts.tv_sec = end_time_ptr->ts.tv_sec - start_time_ptr->ts.tv_sec;
    diff_time.ts.tv_nsec = end_time_ptr->ts.tv_nsec - start_time_ptr->ts.tv_nsec;
    time_ns = diff_time.ts.tv_sec * 1E+9 + diff_time.ts.tv_nsec;

    return time_ns;
}


/**
 *
 */
Process_Performance::Process_Performance(const char *name)
{
    strcpy(_process_name, name);
    _head = NULL;
    _tail = NULL;
}


/**
 *
 */
Process_Performance::~Process_Performance()
{
    COMP_ENTRY *temp;
    DBGPRINT;
    while (_head != NULL) {
        temp = _head;
        _head = _head->next;
        delete temp;
    }
}


/**
 *
 */
const char *Process_Performance::get_name()
{
    return _process_name;
}


/**
 *
 */
COMP_ENTRY *Process_Performance::get_head_entry()
{
    return _head;
}


/**
 *
 */
COMP_ENTRY *Process_Performance::get_entry(int start_line, int end_line)
{
    COMP_ENTRY *temp = NULL;
    temp = _head;
    while (temp != NULL) {
        if ((temp->start_line == start_line) && (temp->end_line == end_line)) {
            break;
        }
        temp = temp->next;
    }

    return temp;
}


/**
 *
 */
COMP_ENTRY *Process_Performance::add_entry(int start_line, int end_line)
{
    COMP_ENTRY *temp = NULL;
    temp = get_entry(start_line, end_line);
    if (temp == NULL) {
        temp = new COMP_ENTRY;

        if (_head == NULL) {
            _head = temp;
            _tail = temp;
            temp->next = NULL;
        } else {
            _tail->next = temp;
            _tail = temp;
            temp->next = NULL;
        }
        temp->start_line = start_line;
        temp->end_line = end_line;
        temp->total_computation_time = 0;
        temp->called_times = 0;
    }

    return temp;
}


/**
 *
 */
int Process_Performance::set_entry(int start_line, int end_line,
                                   CURRENT_TIME *start_time_ptr,
                                   CURRENT_TIME *end_time_ptr)
{
    int ret = 0;
    COMP_ENTRY *temp;
    double computation_time;
    computation_time = sub_time(start_time_ptr, end_time_ptr);
    temp = get_entry(start_line, end_line);
    if (temp == NULL) {
        temp = add_entry(start_line, end_line);
    }

    temp->total_computation_time += computation_time;
    temp->called_times++;

    return ret;
}


/**
 *
 */
Performance_Extraction::Performance_Extraction(const char *chr_file_name)
{
    strcpy(_chr_file_name, chr_file_name);
}


/**
 *
 */
Performance_Extraction::~Performance_Extraction()
{
    Process_Performance *process_performance_ptr;

#ifdef INCLUDE_PERFORMANCE
    //strcpy(_processor_type, "DSP");
    //add_to_xml_file(_chr_file_name);
    //strcpy(_processor_type, "RISC");
    //add_to_xml_file(_chr_file_name);

    write_to_xml_file(_chr_file_name);
#endif

    for (_iter_process_performance = _list_process_performance.begin();
         _iter_process_performance != _list_process_performance.end();
         _iter_process_performance++)
    {
        process_performance_ptr = *_iter_process_performance;
        delete process_performance_ptr;
    }
    _list_process_performance.clear();

}


/**
 *
 */
int Performance_Extraction::add_computation_performance(
    const char *process_name, int start_line, int end_line,
    CURRENT_TIME *start_time_ptr, CURRENT_TIME *end_time_ptr)
{
    int ret = 0;
    Process_Performance *process_performance_ptr;

    process_performance_ptr = get_process_performance(process_name);
    if (process_performance_ptr == NULL) {
        process_performance_ptr = new Process_Performance(process_name);
        _list_process_performance.push_back(process_performance_ptr);
    }

    process_performance_ptr->set_entry(start_line, end_line,
                                       start_time_ptr, end_time_ptr);

    return ret;
}


/**
 *
 */
Process_Performance *Performance_Extraction::get_process_performance(
    const char *process_name)
{
    Process_Performance *process_performance_ptr = NULL;

    for (_iter_process_performance = _list_process_performance.begin();
         _iter_process_performance != _list_process_performance.end();
         _iter_process_performance++) {
        process_performance_ptr = *_iter_process_performance;
        if (strcmp(process_performance_ptr->get_name(), process_name) == 0) {
            break;
        }
    }

    if (_iter_process_performance == _list_process_performance.end()) {
        process_performance_ptr = NULL;
    }

    return process_performance_ptr;
}


/**
 * write performance data into XML file. (identical to add_to_xml_file()
 * if file does not exist).
 */
int Performance_Extraction::write_to_xml_file(const char *chr_file_name)
{
    char start_line_str[NAME_LENGTH];
    char end_line_str[NAME_LENGTH];
    char computation_time_str[NAME_LENGTH];
    double computation_time;
    Process_Performance *process_performance_ptr;
    COMP_ENTRY *comp_entry;

    //create xml
    string text = "<?xml version=\"1.0\"?>\n<characterization>\n";
    for (_iter_process_performance = _list_process_performance.begin();
         _iter_process_performance != _list_process_performance.end();
         _iter_process_performance++) {
        process_performance_ptr = *_iter_process_performance;

        text += "  <process name=\"";
        text += process_performance_ptr->get_name();
        text += "\">\n";

        for (comp_entry = process_performance_ptr->get_head_entry();
             comp_entry != NULL;
             comp_entry = comp_entry->next) {
            sprintf(start_line_str, "%d", comp_entry->start_line);
            sprintf(end_line_str, "%d", comp_entry->end_line);

            //hack to eliminate the overhead of system call clock_gettime.
            //computation_time is nano sec. To get cycles, need to mutiply
            //by the frequency of cpu. result is in cycles.
            computation_time = (comp_entry->total_computation_time
                                / comp_entry->called_times - SYS_OVERHEAD)
                                * HOST_FREQUENCY;
            if (computation_time < 0) {
                computation_time = 0;
            }
            sprintf(computation_time_str, "%.0f", computation_time);

            text += "    <computation start=\"";
            text += start_line_str;
            text += "\" end=\"";
            text += end_line_str;
            text += "\">\n";
            text += "      <processor type=\"DSP\" time=\"";
            text += computation_time_str;
            text += "\"/>\n";
            text += "      <processor type=\"RISC\" time=\"";
            text += computation_time_str;
            text += "\"/>\n";
            text += "    </computation>\n";
        }
        text += "  </process>\n";
    }
    text += "  <communication name=\"read\">\n";
    text += "    <processor type=\"DSP\" time=\"2\"/>\n";
    text += "    <processor type=\"RISC\" time=\"2\"/>\n";
    text += "  </communication>\n";
    text += "  <communication name=\"write\">\n";
    text += "    <processor type=\"DSP\" time=\"2\"/>\n";
    text += "    <processor type=\"RISC\" time=\"2\"/>\n";
    text += "  </communication>\n";
    text += "</characterization>\n";

    //write xml to file
    std::ofstream out(chr_file_name, ios::out);
    if (!out) {
      printf("Cannot open file %s. Return.\n", chr_file_name);
      return -1;
    }
    out.write(text.c_str(), text.size());
    out.close();
    return 0;
}


/**
 * add performance data to existing XML file (or create new one if file
 * does not exist).
 */
int Performance_Extraction::add_to_xml_file(const char *chr_file_name)
{
    int ret = 0;
    XMLNode file_node;
    XMLNode root_node;
    XMLNode process_node;
    XMLNode comp_node;
    int computation_index = 0;
    XMLNode processor_node;
    XMLNode read_node;
    XMLNode write_node;

    Process_Performance *process_performance_ptr;
    COMP_ENTRY *comp_entry;

    char start_line_str[NAME_LENGTH];
    char end_line_str[NAME_LENGTH];
    char computation_time_str[NAME_LENGTH];
    double computation_time;

    file_node = XMLNode::parseFile(chr_file_name);
    if (file_node.isEmpty()) {
        file_node = XMLNode::createXMLTopNode("xml", TRUE);
        file_node.addAttribute("version", "1.0");
        root_node = file_node.addChild("characterization");
    }

    root_node = file_node.getChildNode();
    if (root_node.isEmpty()) {
        ret = -1;
        printf("Open characterization file error\n");
        return ret;
    }

    for (_iter_process_performance = _list_process_performance.begin();
         _iter_process_performance != _list_process_performance.end();
         _iter_process_performance++) {
        process_performance_ptr = *_iter_process_performance;

        printf("%s\n", process_performance_ptr->get_name());

        process_node = root_node.getChildNodeWithAttribute(
            "process", "name", process_performance_ptr->get_name());
        if (process_node.isEmpty())
        {
            process_node = root_node.addChild("process");
            process_node.addAttribute(
                "name",process_performance_ptr->get_name());
        }

        for (comp_entry = process_performance_ptr->get_head_entry();
             comp_entry != NULL;
             comp_entry = comp_entry->next) {
            sprintf(start_line_str, "%d", comp_entry->start_line);
            sprintf(end_line_str, "%d", comp_entry->end_line);

            printf("%d %d\n", comp_entry->start_line, comp_entry->end_line);

            /* Hack to eliminate the overhead of system call clock_gettime.
               computation_time is nano sec. To get cycles, need to mutiply
               by the frequency of cpu.
               result is cycles */
            computation_time = (comp_entry->total_computation_time
                                / comp_entry->called_times - SYS_OVERHEAD)
                                * HOST_FREQUENCY;

            if (computation_time < 0) {
                computation_time = 0;
            }

            sprintf(computation_time_str, "%.0f", computation_time);
            computation_index = 0;
            comp_node = process_node.getChildNode("computation",
                                                  computation_index++);
            while (!comp_node.isEmpty()) {
                if (strcmp(comp_node.getAttribute("start"),start_line_str)==0
                    && strcmp(comp_node.getAttribute("end"),end_line_str)==0) {
                    break;
                }

                comp_node = process_node.getChildNode("computation",
                                                      computation_index++);
            }

            if (comp_node.isEmpty()) {
                comp_node = process_node.addChild("computation");
                comp_node.addAttribute("start", start_line_str);
                comp_node.addAttribute("end", end_line_str);
            }

            processor_node = comp_node.getChildNodeWithAttribute("processor", "type", _processor_type);
            if (processor_node.isEmpty()) {
                processor_node = comp_node.addChild("processor");
                processor_node.addAttribute("type", _processor_type);
                processor_node.addAttribute("time", computation_time_str);
            }
        }
    }

    read_node = root_node.getChildNodeWithAttribute("communication",
                                                    "name", "read");
    if (read_node.isEmpty()) {
        read_node = root_node.addChild("communication");
        read_node.addAttribute("name", "read");
    }

    processor_node = read_node.getChildNodeWithAttribute("processor",
                                                         "type",
                                                         _processor_type);
    if (processor_node.isEmpty()) {
        processor_node = read_node.addChild("processor");
        processor_node.addAttribute("type", _processor_type);
        processor_node.addAttribute("time", "2");
    }

    write_node = root_node.getChildNodeWithAttribute("communication",
                                                     "name", "write");
    if (write_node.isEmpty()) {
        write_node = root_node.addChild("communication");
        write_node.addAttribute("name", "write");
    }

    processor_node = write_node.getChildNodeWithAttribute("processor",
                                                          "type",
                                                          _processor_type);
    if (processor_node.isEmpty()) {
        processor_node = write_node.addChild("processor");
        processor_node.addAttribute("type", _processor_type);
        processor_node.addAttribute("time", "2");
    }

    file_node.writeToFile(chr_file_name);
    return 0;
}
