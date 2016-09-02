#ifndef NVIM_PICTURE_RENDERER_HPP
#define NVIM_PICTURE_RENDERER_HPP

#include <Nvim/DataHolder.hpp>
#include <Nvim/GUIOptions.hpp>
#include <Nvim/Tag.hpp>

#include <PictureProvider.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace nvim
{
    class PictureRenderer
    {
        public:
            PictureRenderer(const DataHolder &dataHolder, 
                            const GUIOptions &GUIOptions,
                            PictureProvider &pictureProvider);

            void render(sf::RenderTarget &renderTarget, const sf::RenderStates &states) const;

            void update();

            void resize();

        private:
            void drawPictureFragment(const sf::Texture &texture, const Tag &pictureTag, int row);
            void drawPictureLoadMessage(sf::String message, int col, int row);

            void cleanRow(int row);

        private:
            const DataHolder                   
                &mDataHolder;

            const GUIOptions                   
                &mGUIOptions;

            PictureProvider
                &mPictureProvider;

            std::vector<sf::String>
                 mTextCache;

            std::vector<bool>
                 mIsRowClean;

            sf::RenderTexture
                 mRenderTexture;

            sf::Sprite 
                 mSprite;
    };
}

#endif
