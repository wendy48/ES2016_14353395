#include "Fifo.h"

/**
 *
 */
Fifo::Fifo(char* name, unsigned size = 18) {
    //std::cout << "Create Fifo." << std::endl;
    //except at the beginning, _head and _tail must never overlap,
    //otherwise one does not know whether the buffer is full or
    //empty. to have nevertheless a buffer with the given capacity,
    //a buffer with one more element is allocated.
    _size = size;
    _buffer = new char[_size];
    //_head = 0;
	_use = 0;
    _tail = 0;
    _name = new char[strlen(name) + 1];
    strcpy(_name, name);
}

/**
 *
 */
Fifo::~Fifo() {
    //std::cout << "Delete Fifo." << std::endl;
    if (_buffer) {
        delete _buffer;
    }
    if (_name) {
        delete _name;
    }
    _buffer = 0;
    //_head = 0;
	_use = 0;
    _tail = 0;
    _name = 0;
    //std::cout << "Deleted Fifo." << std::endl;
}

/**
 *
 */
unsigned Fifo::read(void *destination, unsigned len) {
    char* buffer = (char*)destination;
    unsigned read = 0;
    //std::cout << "Try to read " << len << " bytes from Fifo " << _name << "." << std::endl;


    while (read < len) {
      if (_use == 0) {
        wait(_writeEvent);
      } else {
    	  unsigned tocopy = (len - read <= _use ? len - read : _use);

          if ((unsigned)_tail + tocopy < _size) {
              memcpy(buffer, _buffer + (unsigned)_tail, tocopy);
          }
          else {
              memcpy(buffer, _buffer + (unsigned)_tail, _size - (unsigned)_tail);
              memcpy(buffer + _size - (unsigned)_tail, _buffer, tocopy - _size + (unsigned)_tail);
          }

          read += tocopy;
          buffer += tocopy;
          _tail = (_tail + tocopy) % _size;
		  _use -= read;
          _readEvent.notify();
      }
    }

    /*while (read < len) {
	  if (used() == 0) {
		wait(_writeEvent);
	  } else if ((len - read) < used()) {
		while (read < len) {
		  unsigned tocopy = (len - read + _tail >= _size) ? _size - _tail : len - read;
		  memcpy(buffer, _buffer + _tail, tocopy);
		  _tail = (_tail + tocopy) % _size;
		  read += tocopy;
		  buffer += tocopy;
		}
		_readEvent.notify();
	  } else {
		*buffer++ = *(_buffer + _tail % _size);
		_tail = (_tail + 1) % _size;
		read++;
		_readEvent.notify();
	  }
	}*/

    //std::cout << "Read " << read << " bytes from Fifo " << _name << "." << std::endl;
    return read;
}

/**
 *
 */
unsigned Fifo::write(const void *source, unsigned len) {
    char* buffer = (char*)source;
    unsigned write = 0;
	unsigned head = (_tail + _use) % _size;
    //std::cout << "Try to write " << len << " bytes to Fifo " << _name << std::endl;

    while (write < len) {
      if (unused() == 0) {
        wait(_readEvent);
      } else {
    	  unsigned tocopy = (len - write <= unused() ? len - write : unused());

          if (head + tocopy < _size) {
              memcpy(_buffer + head, buffer, tocopy);
	      }
          else {
		      memcpy(_buffer + head, buffer, _size - head);
		      memcpy(_buffer, buffer + _size - head, tocopy - _size + head);
		  }

          write += tocopy;
          buffer += tocopy;
          head = (head + tocopy) % _size;
		  _use += write;
          _writeEvent.notify();
      }
    }

    /* while (write < len) {
      if (unused() == 0) {
        wait(_readEvent);
      } else if ((len - write) < unused()) {
        while (write < len) {
          unsigned tocopy = (len - write + _head >= _size) ? _size - _head : len - write;
          memcpy(_buffer + _head, buffer, tocopy);
          _head = (_head + tocopy) % _size;
          write += tocopy;
          buffer += tocopy;
        }
        _writeEvent.notify();
      } else {
        *(_buffer + (unsigned)(_head) % _size) = *buffer++;
        _head = (_head + 1) % _size;
        write++;
        _writeEvent.notify();
      }
    }*/

    //std::cout << "Wrote " << write << " bytes to Fifo " << _name << "." << std::endl;
    return write;
}

/**
 *
 */
unsigned Fifo::size() const {
    return (_size);
}

/**
 *
 */
unsigned Fifo::unused() const {
    return (_size) - _use;
}

/**
 *
 */
unsigned Fifo::used() const {
    return _use;
}

/**
 *
 */
char* Fifo::getName() const {
    return _name;
}

/**
 * Test the implementation
 */
/*
class producer : public sc_module
{
   public:
     Fifo *fifo;

     SC_HAS_PROCESS(producer);

     producer(sc_module_name name) : sc_module(name)
     {
       SC_THREAD(main);
     }

     void main()
     {
       const char *str =
         "Visit www.systemc.org and see what SystemC can do for you today!\n";

       while (*str) {
         fifo->write((void*)str++, 4);
       }
     }
};

class consumer : public sc_module
{
   public:
     Fifo *fifo;

     SC_HAS_PROCESS(consumer);

     consumer(sc_module_name name) : sc_module(name)
     {
       SC_THREAD(main);
     }

     void main()
     {
       char c;
       cout << endl << endl;

       while (true) {
         fifo->read(&c, 4);
         cout << c << flush;

         if (fifo->used() == 1)
             cout << "<1>" << flush;
         if (fifo->used() == 9)
             cout << "<9>" << flush;
       }
     }
};

class top : public sc_module
{
   public:
     Fifo *fifo_inst;
     producer *prod_inst;
     consumer *cons_inst;

     top(sc_module_name name) : sc_module(name)
     {
       fifo_inst = new Fifo("myfifo", 10);

       prod_inst = new producer("producer");
       prod_inst->fifo = fifo_inst;

       cons_inst = new consumer("consumer");
       cons_inst->fifo = fifo_inst;
     }
};

int sc_main (int argc , char *argv[]) {
   top top1("top");
   sc_start();
   return 0;
}
*/
