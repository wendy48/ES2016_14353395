#include "ProcessWrapper.h"
#include "dolSupport.h"

/**
 *
 */
ProcessWrapper::ProcessWrapper(
        sc_module_name name = sc_gen_unique_name("process"))
        : sc_module(name) {

    _name = new char[strlen(name) + 1];
    strcpy(_name, name);

    _isDetached = false;
    for (int i = 0; i < 4; i++) {
        _iteratorIndex[i] = getIndex(_name, "_", i);
    }
}

/**
 *
 */
ProcessWrapper::~ProcessWrapper() {
    if (_name) {
        delete _name;
    }
}

/**
 *
 */
void ProcessWrapper::initialize() {
    _process.init(&_process);
}

/**
 *
 */
int ProcessWrapper::fire()
{
    int returnValue;

    #ifdef INCLUDE_TRACE
    start_line = -1;
    #endif

    #ifdef INCLUDE_PERFORMANCE
    start_line = -1;
    get_current_time(&start_time);
    #endif

    returnValue = _process.fire(&_process);

    #ifdef INCLUDE_TRACE
    end_line = -1;
    dol_functional_trace.create_computation_event(basename(),
            start_line, end_line);
    #endif

    #ifdef INCLUDE_PERFORMANCE
    get_current_time(&end_time);
    end_line = -1;
    performance_extraction.add_computation_performance(basename(),
            start_line, end_line, &start_time, &end_time);
    #endif

    return returnValue;
}


/**
 *
 */
void ProcessWrapper::detach() {
    _isDetached = true;
}


/**
 * Gets an index of a string, where the index must be separated by
 * a character specified in tokens.
 * Returns -1, when an error occurs.
 *
 * Example:
 * getIndex("name_1_2", "_", 0) will return 1.
 * getIndex("name_1_2", "_", 1) will return 2.
 *
 * @param string string to parse
 * @param tokens delimiter of indices
 * @param indexNumber position of index (starting at 0)
 */
int ProcessWrapper::getIndex(const char* string, char* tokens,
        int indexNumber) const {
    char* string_copy;
    char* token_pointer;
    int index = 0;

    string_copy = (char*) malloc(sizeof(char) * (strlen(string) + 1));
    if (!string_copy) {
        fprintf(stderr, "getIndex(): could not allocate memory.\n");
        return -1;
    }

    strcpy(string_copy, string);

    token_pointer = strtok(string_copy, tokens);
    do {
        token_pointer = strtok(NULL, tokens);
        index++;
    } while (index <= indexNumber && token_pointer != 0);

    if (token_pointer) {
        index = atoi(token_pointer);
        free(string_copy);
        return index;
    }

    return -1;
}


/**
 * Get the index of this process.
 * @param indexNumber position of index (starting at 0)
 */
int ProcessWrapper::getIndex(unsigned indexNumber) const {
    if (indexNumber < 4) {
        return _iteratorIndex[indexNumber];
    }
    return -1;
}


/**
 * Get the name of this process.
 */
char* ProcessWrapper::getName() const {
    return _name;
}


/**
 *
 */
#ifdef INCLUDE_PROFILER
void ProcessWrapper::addToProfile(const char *event, void *port,
    int length) {
    if (profiler_output_file != NULL) {
        fprintf(profiler_output_file, "%u %s %s %p%s %d\n",
                profiler_event_counter++, _name, event, port, (event[0] == 'r') ? "I" : "O",
                length);

    } else {
        printf("profiler_output_file does not exist");
    }
}
#endif
