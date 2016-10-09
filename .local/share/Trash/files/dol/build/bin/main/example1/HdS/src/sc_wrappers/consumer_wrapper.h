#ifndef consumer_WRAPPER_H
#define consumer_WRAPPER_H

#include "ProcessWrapper.h"
#include "dolSupport.h"

class consumer_wrapper : public ProcessWrapper {
    public:
        consumer_wrapper(char* name);
        virtual ~consumer_wrapper();
        Fifo *INPORT_1;
    protected:
        LocalState _state;
};

#endif
