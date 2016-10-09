#ifndef square_WRAPPER_H
#define square_WRAPPER_H

#include "ProcessWrapper.h"
#include "dolSupport.h"

class square_wrapper : public ProcessWrapper {
    public:
        square_wrapper(char* name);
        virtual ~square_wrapper();
        Fifo *INPORT_1;
        Fifo *OUTPORT_2;
    protected:
        LocalState _state;
};

#endif
