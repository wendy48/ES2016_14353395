#ifndef _WINDOWEDFIFO_H_
#define _WINDOWEDFIFO_H_

#include "systemc.h"

class WindowedFifo {
    public:
        WindowedFifo(char* name, unsigned size);
        virtual ~WindowedFifo();

        virtual unsigned reserve(void** destination, unsigned len);
        virtual void release();

        virtual unsigned capture(void** destination, unsigned len);
        virtual void consume();

        virtual unsigned used() const;
        virtual unsigned unused() const;
        virtual unsigned size() const;
        virtual char* getName() const;

    protected:
        char *_buffer;
        unsigned _head;
        unsigned _tail;
        unsigned _headRoom;
        unsigned _tailRoom;
        unsigned _size;
        unsigned _use;
        unsigned _writeReserve;  
        unsigned _readReserve; 
        //bool _isFull;
        bool _isHeadReserved;
        bool _isTailReserved;
        char *_name;
        sc_event _readEvent;
        sc_event _writeEvent;
};

#endif
