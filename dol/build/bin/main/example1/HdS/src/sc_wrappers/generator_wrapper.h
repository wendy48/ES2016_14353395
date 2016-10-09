#ifndef generator_WRAPPER_H
#define generator_WRAPPER_H

#include "ProcessWrapper.h"
#include "dolSupport.h"

class generator_wrapper : public ProcessWrapper {
    public:
        generator_wrapper(char* name);
        virtual ~generator_wrapper();
        Fifo *OUTPORT_1;
    protected:
        LocalState _state;
};

#endif
