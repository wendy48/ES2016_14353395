#include "consumer_wrapper.h"
#include "consumer.c"

consumer_wrapper::consumer_wrapper(char* name)
        : ProcessWrapper(name) {
    _state = (LocalState)new Consumer_State;
    _process.local = _state;
    _process.init = consumer_init;
    _process.fire = consumer_fire;
    _process.wptr = this;
}

consumer_wrapper::~consumer_wrapper() {
    if (_state)
        delete (Consumer_State*)_state;
}

