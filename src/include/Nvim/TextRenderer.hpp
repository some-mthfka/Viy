#ifndef NVIM_TEXT_RENDERER_HPP
#define NVIM_TEXT_RENDERER_HPP

#include <Nvim/DataHolder.hpp>
#include <Nvim/GUIOptions.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

/* Text renderer is responsible for drawing text.
 * For efficiency reasons, it keeps a transparent texture
 * where it renders it's text so that only this texture is
 * drawn every other frame, without having to loop through the
 * whole text information. But a bigger reason for having the
 * texture is not having to redraw the whole grid when, say,
 * only one character is updated. It detects the update, erases
 * the old character from the texture and draws the new one.
 */

namespace nvim
{
    class TextRenderer
    {
        public:
            TextRenderer(nvim::DataHolder &dataHolder, nvim::GUIOptions &GUIOptions);

            void render(sf::RenderTarget &renderTarget, const sf::RenderStates &states) const;

            void update();

            void resize();

            void setTextOffset(int x, int y);

        private:
            const nvim::DataHolder                   
                &mDataHolder;

            const nvim::GUIOptions                   
                &mGUIOptions;

            std::vector<std::vector<sf::Text>>      
                 mTextCache;

            sf::RenderTexture
                 mRenderedTexture;

            sf::Sprite 
                 mRenderedTextureSprite;

            std::vector<std::vector<sf::Color>>
                 mBackgroundColorCache;

            int  mOffsetX, mOffsetY;
    };
}

#endif
