#ifndef NAVIGATOR_MODE_RENDERER_HPP
#define NAVIGATOR_MODE_RENDERER_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>

#include <RecordSystem.hpp>

#include <ResourceHolder.hpp>
#include <ResourceIdentifiers.hpp>

#include <utility> //pair

class NavigatorMode;
//struct ItemKind;

class NavigatorModeRenderer
{
    public:
             NavigatorModeRenderer(const NavigatorMode &navigatorMode,
                                   sf::RenderTexture &renderTexture);

        void updateRenderTexture();

        void render(sf::RenderTarget &target, const sf::RenderStates &states) const;

        void resize(int width, int height);

        sf::Vector2u
             getSize() const;

        //int  getItemHeight() const { return options.ItemHeight; }

        int  howManyElementsFit() const
        {
            return (mRenderTexture.getSize().y) * (1.0 - options.fractions.TopIndent) / getItemHeight(); 
        }
            
        
    private:
        void drawLabel(int position, int height, sf::Color color);

        void handleFirstOfTheKind(int position, sf::Color color);

        void drawParentFolderLabel(int index, int position, bool selected, bool isFirstOfTheKind);
        void drawNestedFolderLabel(int index, int position, bool selected, bool isFirstOfTheKind);
        void drawNestedRecordLabel(int index, int position, bool selected, bool isFirstOfTheKind);
        void drawCurrentFolderLabel(int position);

    public:
        struct Options
        {
            struct Fractions
            {
                const float 
                    ItemHeight = 0.1,
                    SideIndent = 0.1,
                    TopIndent = 0.2,
                    CaptionSideIndent = 0.02,
                    CaptionTopIndent = 0.0175,
                    FontSize = 0.045;
            } fractions;

            const sf::Color
                ParentFolderColor = sf::Color(0, 64, 26),
                CurrentFolderColor = sf::Color(50, 64, 59),
                NestedFolderColor = sf::Color(0, 64, 26),
                NestedRecordColor = sf::Color(0, 32, 64),
                CaptionColor = /*sf::Color::Transparent;*/sf::Color(99, 158, 99);//(66, 158, 91);
        };

        static Options options;

        float
             getItemHeight() const { return mRenderTexture.getSize().y * options.fractions.ItemHeight; }

    private:
        struct FontLoader
        {
            FontLoader() { font.loadFromFile("media/navigator.ttf"); }
            sf::Font font;
        } mFont;


    private:
        const NavigatorMode &
            mNavigatorMode;

        sf::RenderTexture
            &mRenderTexture;

        sf::Sprite
             mRenderTextureSprite;

		TextureHolder 
             mTextures;

        sf::Sprite
             mRecord,
             mTerminal;

        sf::Text
             mItemCaption;
};

#endif
