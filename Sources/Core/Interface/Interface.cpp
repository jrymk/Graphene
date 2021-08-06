#include "Interface.h"

namespace gfn {
    Interface::Interface() :
            graph(TRIPLEBUFFER_COPY),
            commands(TRIPLEBUFFER_SWAP),
            responses(TRIPLEBUFFER_SWAP) {

    }
}