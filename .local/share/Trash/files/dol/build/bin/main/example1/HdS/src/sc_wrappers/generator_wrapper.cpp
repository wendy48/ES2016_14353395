#include "generator_wrapper.h"
#include "generator.c"

generator_wrapper::generator_wrapper(char* name)
        : ProcessWrapper(name) {
    _state = (LocalState)new Generator_State;
    _process.local = _state;
    _process.init = generator_init;
    _process.fire = generator_fire;
    _process.wptr = this;
}

generator_wrapper::~generator_wrapper() {
    if (_state)
        delete (Generator_State*)_state;
}

