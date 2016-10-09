#include "WindowedFifo.h"

/**
 *
 */
WindowedFifo::WindowedFifo(char* name, unsigned size = 20) {
    //std::cout << "Create WindowedFifo." << std::endl;
    _size = size;
    _buffer = new char[_size];
    _head = 0;
    _tail = 0;
    _headRoom = 0;
    _tailRoom = 0;
    _use = 0;
    //indicates whether Fifo is empty or full if _head == _tail
    //_isFull = false;
    _isHeadReserved = false;
    _isTailReserved = false;
    _name = new char[strlen(name) + 1];
    strcpy(_name, name);
}

/**
 *
 */
WindowedFifo::~WindowedFifo() {
    //std::cout << "Delete WindowedFifo." << std::endl;
    if (_buffer) {
        delete _buffer;
    }
    if (_name) {
        delete _name;
    }
    _buffer = 0;
    _head = 0;
    _tail = 0;
    _name = 0;
    _use = 0;
    //std::cout << "Deleted WindowedFifo." << std::endl;
}

/**
 *
 */
unsigned WindowedFifo::reserve(void** dest, unsigned len) {
    char** destination = (char**)dest;
    //std::cout << "Attempt to reserve " << len << " bytes." << std::endl;

    //can only reserve once piece at a time
    if (_isHeadReserved) {
        *destination = 0;
        return 0;
    }

    while (unused() == 0) {
      wait(_readEvent);
    }

    //reserve at most as much memory as still available in the buffer
    unsigned write = (len <= _size - _use ? len : _size - _use);

    if (write > 0) {
        //if wrap-around in buffer: return only buffer for the
        //contiguous buffer space
        if (_head + write > _size) {
            write = _size - _head;
        }

        _headRoom = (_head + write) == _size? 0 : _head + write;
        *destination = &(_buffer[_head]);
        _isHeadReserved = true;

        //the following comparison is unsafe in a multi-threaded
        //environment and potentially leads to race-conditions
        /*if (_headRoom == _tail) {
            _isFull = true;
        } else {
            _isFull = false;
        }*/
    }

    //std::cout << "Reserved " << write << " bytes." << std::endl;
    _writeReserve = write; 
    return write;
}

/**
 *
 */
void WindowedFifo::release() {
    if (_isHeadReserved) {
        //std::cout << "Released " << _headRoom - _head << " bytes." << std::endl;
        _head = _headRoom;
        _use += _writeReserve;
        _isHeadReserved = false;
        _writeEvent.notify();
    }
}

/**
 *
 */
unsigned WindowedFifo::capture(void **dest, unsigned len) {
    char** destination = (char**)dest;
    //std::cout << "Attempt to capture " << len << " bytes." << std::endl;

    if (_isTailReserved) {
        //std::cout << "Only one attempt to capture allowed." << std::endl;
        *destination = 0;
        return 0;
    }

    while (used() == 0) {
      wait(_writeEvent);
    }

    //capture at most as much data as available in the buffer
    unsigned read = (len <= _use ? len : _use);

    if ( read > 0 ) {
        //if wrap-around in buffer: return only buffer for the
        //contiguous buffer space
        if (_tail + read> _size) {
            read = _size - _tail;
        }

        _tailRoom = (_tail + read) == _size ? 0 : _tailRoom = _tail + read;
        *destination = &(_buffer[_tail]);
        _isTailReserved = true;
    }

    //std::cout << "Captured " << read << " bytes." << std::endl;

    _readReserve = read; 
    return read;
}

/**
 *
 */
void WindowedFifo::consume() {
    if (_isTailReserved) {
        //std::cout << "Consumed " << _tailRoom - _tail << " bytes." << std::endl;
        _tail = _tailRoom;
        _use -= _readReserve;
        _isTailReserved = false;
        //_isFull = false;
        _readEvent.notify();
    }
}

/**
 *
 */
unsigned WindowedFifo::size() const {
    return _size;
}

/**
 *
 */
unsigned WindowedFifo::unused() const {
    return _size - _use;
}

/**
 *
 */
unsigned WindowedFifo::used() const {
	return _use;
    /*if (_headRoom > _tail) {
        return _headRoom - _tail;
    } else if (_headRoom == _tail) {
        if (_isFull == true) {
            return _size;
        } else {
            return 0;
        }
    }
    return _headRoom + _size - _tail;*/
}

/**
 *
 */
char* WindowedFifo::getName() const {
    return _name;
}

/**
 * Test the implementation
 */
/*
#include <iostream>
#include <iomanip>
#define LENGTH 10

class producer : public sc_module
{
    public:
    WindowedFifo *wfifo;

    SC_HAS_PROCESS(producer);

    producer(sc_module_name name) : sc_module(name)
    {
      SC_THREAD(main);
    }

    void main()
    {
        for (int j = 0; j < LENGTH; j++) {
            //std::cout << "write " << i << " to Fifo.    ";
            int *buf1;
            int write = wfifo->reserve((void**)&buf1, sizeof(int));

            if (write == sizeof(int)) {
                *buf1 = j;
                wfifo->release();
                //std::cout << "used: " << std::setw(2) << wfifo->used()
                //        << ", unused: " << std::setw(2) << wfifo->unused()
                //        << ", size: "  << std::setw(2) << wfifo->size()
                //        << std::endl;
            } else {
                std::cout << "Not successful: " << write << std::endl;
            }
        }
        printf("producer returns.\n");
    }
};

class consumer : public sc_module
{
    public:
    WindowedFifo *wfifo;

    SC_HAS_PROCESS(consumer);

    consumer(sc_module_name name) : sc_module(name)
    {
        SC_THREAD(main);
    }

    void main()
    {
        for (int j = 0; j < LENGTH; j++) {
            int* buf3;
            int read = wfifo->capture((void**)&buf3, sizeof(int));
            if (read == sizeof(int)) {
                std::cout << "read " << (unsigned)*buf3 << "  from WindowedFifo   ";
                std::cout << "used: " << std::setw(2) << wfifo->used()
                        << ", unused: " << std::setw(2) << wfifo->unused()
                        << ", size: "  << std::setw(2) << wfifo->size()
                        << std::endl;
                wfifo->consume();
            } else {
                std::cout << "Read nothing from WindowedFifo." << std::endl;
            }
        }
        printf("consumer returns.\n");
    }
};

class top : public sc_module
{
    public:
    WindowedFifo *wfifo_inst;
    producer *prod_inst;
    consumer *cons_inst;

    top(sc_module_name name) : sc_module(name)
    {
        wfifo_inst = new WindowedFifo("myfifo", 4);

        prod_inst = new producer("producer");
        prod_inst->wfifo = wfifo_inst;

        cons_inst = new consumer("consumer");
        cons_inst->wfifo = wfifo_inst;
     }
};

int sc_main (int argc , char *argv[]) {
    top top1("top");
    sc_start();
    return 0;
}
*/
