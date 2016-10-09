#ifndef _FIFO_H_
#define _FIFO_H_

#include "systemc.h"

class Fifo {
    public:
        Fifo(char* name, unsigned size);
        virtual ~Fifo();

        virtual unsigned read(void *destination, unsigned len);
        virtual unsigned write(const void *source, unsigned len);
        virtual unsigned used() const;
        virtual unsigned unused() const;
        virtual unsigned size() const;
        virtual char* getName() const;

    protected:
        char *_buffer;
        //unsigned _head;
        unsigned _tail;
        unsigned _size;
		unsigned _use; 
        char *_name;
        sc_event _readEvent;
        sc_event _writeEvent;
};

#endif
