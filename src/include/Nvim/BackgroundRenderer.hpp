#ifndef NVIM_BACKGROUND_RENDERER_HPP
#define NVIM_BACKGROUND_RENDERER_HPP

#include <vector>

#include <Nvim/DataHolder.hpp>
#include <Nvim/GUIOptions.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

/* Background renderer keeps a render texture where it draws out
 * the background grid. On an update, only the changed cells are
 * redrawn. This is done for efficiency.
 */

namespace nvim 
{
    class BackgroundRenderer
    {
        public:
            BackgroundRenderer(nvim::DataHolder &dataHolder, nvim::GUIOptions &GUIOptions);

            void render(sf::RenderTarget &target, const sf::RenderStates &states) const;

            void update();

            void resize();

        private:
            const nvim::DataHolder
                &mDataHolder;

            const nvim::GUIOptions
                &mGUIOptions;

            sf::RenderTexture
                 mBackground;

            sf::Sprite 
                 mBackgroundSprite;

            std::vector<std::vector<sf::Color>>
                 mBackgroundColorCache;

    };
}

#endif
