#include <Nvim/PictureRenderer.hpp>

#include <SFML/Graphics/RectangleShape.hpp>

#include <iostream>

#include <Debug.hpp>

nvim::PictureRenderer::PictureRenderer(const DataHolder &dataHolder, 
                                       const GUIOptions &GUIOptions,
                                       PictureProvider &pictureProvider)
    : mDataHolder(dataHolder)
    , mGUIOptions(GUIOptions)
    , mPictureProvider(pictureProvider)
{
    resize(); //uses mGUIOptions for info
}

void nvim::PictureRenderer::drawPictureFragment(const sf::Texture &texture, 
                                                const Tag &pictureTag, 
                                                int row)
{
    //top, left, bottom, right
    sf::IntRect fragment(
        0,
        static_cast<float>(pictureTag.getFragmentNumber()) / pictureTag.getSize() * texture.getSize().y, 
        texture.getSize().x,
        texture.getSize().y / pictureTag.getSize());

    sf::Sprite picFragment(texture, fragment);

    picFragment.setScale(
        static_cast<float>(mGUIOptions.getLineSpacing()) / fragment.height,
        static_cast<float>(mGUIOptions.getLineSpacing()) / fragment.height);

    picFragment.setPosition(pictureTag.getHorizontalPosition() * mGUIOptions.getCharWidth(), 
                            mGUIOptions.getLineSpacing() * row);

    //clean with background color first
    sf::RectangleShape cellRow(
        sf::Vector2f(std::max
                        (static_cast<int>(fragment.width * picFragment.getScale().y), 
                         mGUIOptions.getCharWidth() * pictureTag.getLength()),
                     mGUIOptions.getLineSpacing()));

    cellRow.setPosition(picFragment.getPosition());
    cellRow.setFillColor(mDataHolder.getPrimaryBackgroundColor());

    mRenderTexture.draw(cellRow, sf::BlendNone);
    mRenderTexture.draw(picFragment);

    mIsRowClean[row] = false;
}

void nvim::PictureRenderer::drawPictureLoadMessage(sf::String message, int col, int row)
{
    sf::RectangleShape cellRow(sf::Vector2f(mGUIOptions.getCharWidth() * message.getSize(),
                                            mGUIOptions.getLineSpacing()));
    cellRow.setFillColor(sf::Color::Black);
    cellRow.setPosition(mGUIOptions.getCharWidth() * col,
                        mGUIOptions.getLineSpacing() * row);

    sf::Text text;
    text.setString(message);
    text.setFont(mGUIOptions.getFont());
    text.setCharacterSize(mGUIOptions.getFontSize());
    text.setPosition(cellRow.getPosition());

    mRenderTexture.draw(cellRow, sf::BlendNone);
    mRenderTexture.draw(text);

    mIsRowClean[row] = false;
}

void nvim::PictureRenderer::cleanRow(int row)
{
    if (!mIsRowClean[row])
    {
        sf::RectangleShape cellRow(sf::Vector2f(
                                       mGUIOptions.getCharWidth() * mGUIOptions.cols(),
                                       mGUIOptions.getLineSpacing()));

        cellRow.setPosition(0, mGUIOptions.getLineSpacing() * row);
        cellRow.setFillColor(sf::Color::Transparent);
        mRenderTexture.draw(cellRow, sf::BlendNone);
        mIsRowClean[row] = true;
    }
}

void nvim::PictureRenderer::update()
{
    for (int i = 0; i < mGUIOptions.rows(); i++)
    {
        const sf::String &textRow = mDataHolder.getTextAtRow(i);

        //We do not want to display a picture at the row
        //where the cursor is currently located.
        if (mDataHolder.getCursorPosition().x == i)
        {
            cleanRow(i);
            mTextCache[i] = "";
            continue;
        }

        if (mTextCache[i] != textRow)
        {
            cleanRow(i);

            //try constructing a tag
            Tag pictureTag(textRow);

            if (!pictureTag.isValid())
                mTextCache[i] = textRow;

            if (pictureTag.isValid())
            {
                auto requestedPicture = mPictureProvider.getPicture(
                                            sf::String(pictureTag.getName())); 

                switch (requestedPicture.first)
                {
                    case PictureProvider::PictureLoadStatus::READY:
                        drawPictureFragment(*requestedPicture.second, pictureTag, i);
                        mTextCache[i] = textRow;
                        break;

                    case PictureProvider::PictureLoadStatus::LOADING:
                        drawPictureLoadMessage("Loading", 
                                                pictureTag.getHorizontalPosition(), 
                                                i);
                        break;

                    case PictureProvider::PictureLoadStatus::FAILED:
                        drawPictureLoadMessage("Failed to load", 
                                                pictureTag.getHorizontalPosition(),
                                                i);
                        break;

                    default:
                        assert(false);
                        break;
                }
            }
        }
    }

    mRenderTexture.display();
}

void nvim::PictureRenderer::resize()
{ 
    const auto rows = mGUIOptions.rows();
    const auto cols = mGUIOptions.cols();

    mTextCache.resize(0);

    sf::String emptyRow;
    for (int i = 0; i < rows; i++)
        emptyRow += ' ';

    mTextCache.resize(rows, emptyRow);

    mRenderTexture.create(cols * mGUIOptions.getCharWidth(), 
                          rows * mGUIOptions.getLineSpacing());

    mRenderTexture.clear(sf::Color::Transparent);

    mSprite.setTexture(mRenderTexture.getTexture(), 
                       /*resetRect =*/ true);

    mIsRowClean.resize(rows, true);

    update();
}

void nvim::PictureRenderer::render(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    target.draw(mSprite, states);
}
