#include "square_wrapper.h"
#include "square.c"

square_wrapper::square_wrapper(char* name)
        : ProcessWrapper(name) {
    _state = (LocalState)new Square_State;
    _process.local = _state;
    _process.init = square_init;
    _process.fire = square_fire;
    _process.wptr = this;
}

square_wrapper::~square_wrapper() {
    if (_state)
        delete (Square_State*)_state;
}

