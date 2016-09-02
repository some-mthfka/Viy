#include <NavigatorModeRenderer.hpp>

#include <cassert>

#include <Debug.hpp>

#include <NavigatorMode.hpp>

NavigatorModeRenderer::Options NavigatorModeRenderer::options;

NavigatorModeRenderer::NavigatorModeRenderer(const NavigatorMode &navigatorMode,
                                             sf::RenderTexture &renderTexture)
    : mNavigatorMode(navigatorMode)
    , mRenderTexture(renderTexture)
{
	mTextures.load(Textures::Terminal, "Media/Textures/terminal.png");
	mTextures.load(Textures::Record, "Media/Textures/Record.png");
	mTextures.load(Textures::Folder, "Media/Textures/Folder.png");

    mRecord.setTexture(mTextures.get(Textures::Record));
    mTerminal.setTexture(mTextures.get(Textures::Terminal));

    mRecord.setColor(sf::Color(200, 200, 200, 30));

    mItemCaption.setColor(options.CaptionColor);
}

void NavigatorModeRenderer::render(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    target.draw(mRenderTextureSprite, states);
}

void NavigatorModeRenderer::updateRenderTexture()
{
    const auto &mView = mNavigatorMode.mView;

    //mRenderTexture.clear(sf::Color::Black);

    mRenderTexture.draw(mTerminal);

    bool isFirstOfTheKind = false;

    int pos = 0;

    for (int i = mView.topItem; 
         i < mView.topItem + mView.maxItems && i < mView.totalItems; 
         i++)
    {
        bool selected = i == mView.currentItem;

        auto itemInfo = mNavigatorMode.getItemInfo(i);
        auto kind = itemInfo.first;
        auto index = itemInfo.second;

        //mItemFull.setPosition(0, pos * options.ItemHeight);
        
        //if (kind == NavigatorMode::ItemKind::ParentFolder)
            //pos = 0;

        int actualPosition = options.fractions.TopIndent * mRenderTexture.getSize().y
                             + pos * getItemHeight();

        mItemCaption.setPosition(
            mRenderTexture.getSize().x * (options.fractions.SideIndent + options.fractions.CaptionSideIndent), 
            actualPosition + mRenderTexture.getSize().y * options.fractions.CaptionTopIndent);

        switch (kind)
        {
            case NavigatorMode::ItemKind::ParentFolder: 
                drawParentFolderLabel(index, actualPosition, selected, isFirstOfTheKind);
                break;

            case NavigatorMode::ItemKind::CurrentFolder: 
                drawCurrentFolderLabel(actualPosition);
                break;

            case NavigatorMode::ItemKind::NestedFolder: 
                drawNestedFolderLabel(index, actualPosition, selected, isFirstOfTheKind);
                break;

            case NavigatorMode::ItemKind::NestedRecord: 
                drawNestedRecordLabel(index, actualPosition, selected, isFirstOfTheKind);
                break;
        }

        pos++;
    }

    mRenderTexture.display();
}

void NavigatorModeRenderer::drawLabel(int position, int height, sf::Color color)
{
    mRecord.setPosition(options.fractions.SideIndent * mRenderTexture.getSize().x, position);

    mRenderTexture.draw(mRecord);
}

void NavigatorModeRenderer::drawParentFolderLabel(int index, int position, bool selected, bool isFirstOfTheKind)
{
    if (selected)
        drawLabel(position, getItemHeight(), options.ParentFolderColor);

    sf::String name = sf::String("/ ") + mNavigatorMode.mPathToCurrentFolder[index] -> getName();

    mItemCaption.setString(name);

    mRenderTexture.draw(mItemCaption);
}

void NavigatorModeRenderer::drawCurrentFolderLabel(int position)
{
    //drawLabel(position, options.ItemHeight, options.CurrentFolderColor);

    sf::String name = sf::String("> ") + mNavigatorMode.mCurrentFolder -> getName();

    mItemCaption.setString(name);

    auto oldStyle = mItemCaption.getStyle();
    mItemCaption.setStyle(sf::Text::Style::Bold);

    mRenderTexture.draw(mItemCaption);

    mItemCaption.setStyle(oldStyle);
}

void NavigatorModeRenderer::drawNestedFolderLabel(int index, int position, bool selected, bool isFirstOfTheKind)
{
    if (selected)
        drawLabel(position, getItemHeight(), options.NestedFolderColor);

    sf::String name = sf::String("  ") + mNavigatorMode.mCurrentFolder -> getFolders()[index] -> getName();

    mItemCaption.setString(name);

    mRenderTexture.draw(mItemCaption);
}

void NavigatorModeRenderer::drawNestedRecordLabel(int index, int position, bool selected, bool isFirstOfTheKind)
{
    if (selected)
        drawLabel(position, getItemHeight(), options.NestedRecordColor);

    sf::String name = sf::String("  ") + mNavigatorMode.mCurrentFolder -> getRecords()[index] -> getName();

    mItemCaption.setString(name);

    mRenderTexture.draw(mItemCaption);
}

void NavigatorModeRenderer::resize(int width, int height)
{
    mRenderTexture.create(width, height);

    mRenderTextureSprite.setTexture(mRenderTexture.getTexture(), /*resetRect =*/ true);

    auto adjustSprite = [] (sf::Sprite &sprite, int width, int height)
    {
        float horizontalScale = float(width) / sprite.getTexture() -> getSize().x;
        float verticalScale = float(height) / sprite.getTexture() -> getSize().y;
        sprite.setScale(horizontalScale, verticalScale);
    };

    adjustSprite(mTerminal, width, height);
    adjustSprite(mRecord, 
                 width - options.fractions.SideIndent * 2 * mRenderTexture.getSize().x, 
                 getItemHeight());

    mItemCaption.setFont(mFont.font);
    mItemCaption.setCharacterSize(mRenderTexture.getSize().y * options.fractions.FontSize);
    
    updateRenderTexture();
}

sf::Vector2u NavigatorModeRenderer::getSize() const
{
    return mRenderTexture.getSize();
}
