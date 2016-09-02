#ifndef PYTHON_BINDINGS_HPP
#define PYTHON_BINDINGS_HPP

#include <Nvim/BridgeData.hpp>

namespace nvim
{
    //Starts the Python interpreter.
    //Does not return, so better be launched
    //in a separate thread.
    void runBridge(BridgeData &data);
}

#endif
