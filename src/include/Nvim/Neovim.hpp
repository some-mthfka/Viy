#ifndef NVIM_NEOVIM_HPP
#define NVIM_NEOVIM_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <Nvim/GUIOptions.hpp>
#include <Nvim/DataHolder.hpp>
#include <Nvim/BackgroundRenderer.hpp>
#include <Nvim/TextRenderer.hpp>
#include <Nvim/PictureRenderer.hpp>
#include <Nvim/BridgeData.hpp>
#include <Nvim/Blinky.hpp>

#include <PictureProvider.hpp>

#include <InitData.hpp>
#include <Nvim/Signals.hpp>

#include <thread>

/* Here is an overview of the workings of this program.
 * The way this C++ program communicates with a neovim instance is by 
 * running an embedded intepreter, which does pretty much all the dirty work.
 * The embedded interpreter has to communicate with the C++ program using
 * python bindings defined in PythonBindings.cpp.
 * Those bindings in turn call appropriate Neovim and DataHolder functions.
 * Neovim class manages renderers.
 * Renderers are: Background renderer, Foreground renderer, Blinky.
 * Renderers take the information about what to render from dataHolder kept
 * by Neovim class. Input and events also go through the Neovim class.
 */

/* The following class, on it's instantiation, fires up a Python interpreter.
 * It does so by calling runBridge provided by PythonBindings.hpp.
 * A separate thread for this is used because runBridge does not return.
 * Public bridgeData instance is used for communications between the
 * interpeter and screen with dataHolder.
 *
 * Mostly, though, this class is concerned with managing the work
 * of renderers: back/foreground renderer and blinky. 
 *
 * For the reason above, it also manages the access to the resources safely.
 *
 * First of all, when calling update(), if redraw was requested by nvim,
 * updateMutex is locked, which means that access to dataHolder is aquired 
 * (it can also be aquired by the python bindings). 
 * Now, the renderers can safely update themselves with new data.
 * 
 * The case of a resize initiated by nvim is handled specially.
 * Resize may not be called directly by nvim, because a drawing
 * operation might be taking place. Therefore, resizing is scheduled.
 * Also, after calling resize, nvim may not call any update functions
 * till the resize takes place. To ensure this, resizeMutex is used
 * (in addition to updateMutex).
 *
 * As for the communication in the opposite direction (UI -> nvim),
 * it is also managed here (by modifying bridgeData, later polled by nvim).
 */

namespace nvim
{
    class Neovim 
    {
        public:
                 Neovim(const InitData &initData, Signals &signals);

		    void render(sf::RenderTarget &target, const sf::RenderStates &states) const;
            void update(sf::Time dt);
            void handleEvent(const sf::Event &event);

            //API for the Python Bindings.
            void scheduleRedraw(bool f);
            void scheduleResize(int rows, int cols);
            void setTitle(const char *title);
            void changeMode(Blinky::Mode mode);
            void quit();

            void openPath(std::string path);
            void input(std::string input);

        private:
            void checkForResize(const sf::Event &event);
            void resize(int rows, int cols);

        private:
            GUIOptions
                 mGUIOptions;
            
            const InitData         
                &mInitData;

            Signals 
                &mSignals;

            DataHolder        
                 mDataHolder;

            PictureProvider
                 mPictureProvider;

            TextRenderer       
                 mTextRenderer;

            BackgroundRenderer 
                 mBackgroundRenderer;

            PictureRenderer
                 mPictureRenderer;

            bool mNeedRedraw, mNeedResize;

            Blinky             
                 mBlinky;

            int  mNewRowSize, mNewColSize;

            std::thread 
                 mPythonBridge;

        public:
            BridgeData         
                 bridgeData;
    };
}

#endif
