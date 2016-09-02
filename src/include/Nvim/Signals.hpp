#ifndef NVIM_SIGNALS_HPP
#define NVIM_SIGNALS_HPP

/* This struct is used to communicate UI-specific events
 * from VIM::Neovim to Application such as setting title of a window.
 * That could be done inside VIM::Neovim itself, but it would require
 * keeping a pointer to a sf::renderWindow. Not good.
 * Instead, a shared copy of Signals is kept inside both Application and
 * VIM::Neovim. When, say, a title needs to be changed,
 * Signals::SetTitle::status is set along with the title when
 * nvim calls VIM::Neovim::SetTitle(). Then Application scans
 * the flag and does what's appropriate.
 */

namespace nvim
{
    struct Signals
    {
        struct
        {
            bool status = false;
            sf::String title;
        } setTitle;

        struct
        {
            //atomic because can be accessed both by neovim bindings
            //and navigator's manager at the same time
            bool status = false;
        } switchToNavigator;
    };
}

#endif
