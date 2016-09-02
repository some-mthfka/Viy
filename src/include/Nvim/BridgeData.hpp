#ifndef NVIM_BRIDGE_DATA_HPP
#define NVIM_BRIDGE_DATA_HPP

#include <SFML/System/String.hpp>

#include <mutex>
#include <atomic>

/* BridgeData struct keeps data used both by the bindings
 * and by nvim::Neovim to support communication between
 * GUI and nvim.
 */

namespace nvim
{
    class Neovim;
    class DataHolder;

    //Contains stuff somehow useful for managing
    //data flows between the Python side and the
    //C++ side.  
    struct BridgeData
    { 
        BridgeData()
        {
            newDimensionsRows = -1;
            newDimensionsCols = -1; 
            quitFlag = false;
            switchToNavigatorFlag = false;
        }

        nvim::Neovim     
            *neovim = nullptr;
        nvim::DataHolder 
            *dataHolder = nullptr;

        std::mutex updateMutex,
             resizeMutex,
             nvimInputMutex; //UI -> Nvim input

        std::atomic<int32_t>          
             newDimensionsRows, 
             newDimensionsCols;

        std::atomic<bool>             
             quitFlag,
             switchToNavigatorFlag;

        sf::String
             userInput;

        std::string      
             commandLineArgument;
    };
}

#endif
