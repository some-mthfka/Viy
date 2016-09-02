#include <Nvim/ConversionUtilities.hpp>

#include <cstdint>
#include <cassert>

#include <boost/locale/encoding_utf.hpp>

sf::Color toColor(uint32_t value)
{
    //not sure about endianness though
    uint8_t blue =  (value & (255 << 0 )) >> 0;
    uint8_t green = (value & (255 << 8 )) >> 8;
    uint8_t red =   (value & (255 << 16)) >> 16;
    //uint8_t alpha = (value & (255 << 24)) >> 24;
    return sf::Color(red, green, blue, 255/*alpha*/);
}

std::basic_string<char32_t> UTF8ToUTF32(const char *str, size_t length)
{ 
    //could possibly be replaced with SFML sf::String::toUtf32
    return boost::locale::conv::utf_to_utf<char32_t>(str, str + length);   
}

nvim::CellAttribute toCellAttribute(int32_t id, uint32_t value)
{
    //2 args: attribute id + value
    //-1 for id means reset
    
    typedef nvim::CellAttribute::Text  textAttr;
    typedef nvim::CellAttribute::Color colorAttr;
    
    nvim::CellAttribute attr;
    
    if (id == -1)
    {
        attr.reset = true;
        return attr;
    }
    
    sf::Color c;

    switch(id)
    {
        case 0:
            attr.textAttr = textAttr::BOLD;
            attr.state = value;
            break;

        case 1:
            attr.textAttr = textAttr::UNDERLINED;
            attr.state = value;
            break;

        case 2:
            attr.textAttr = textAttr::UNDERCURL;
            attr.state = value;
            break;

        case 3:
            attr.textAttr = textAttr::ITALIC;
            attr.state = value;
            break;

        case 4:
            attr.colorAttr = colorAttr::REVERSE;
            attr.state = value;
            break;

        case 5:
            attr.colorAttr = colorAttr::FOREGROUND;
            attr.color = toColor(value);
            c = attr.color;
            //std::cout << "Foreground: " << int(c.r) << " " << int(c.g) << " " << int(c.b) << std::endl;
            break;

        case 6:
            attr.colorAttr = colorAttr::BACKGROUND;
            attr.color = toColor(value);
            c = attr.color;
            //std::cout << "Background: " << int(c.r) << " " << int(c.g) << " " << int(c.b) << std::endl;
            break;

        case 7:
            attr.colorAttr = colorAttr::SPECIAL;
            attr.color = toColor(value);
            c = attr.color;
            //std::cout << "Special: " << int(c.r) << " " << int(c.g) << " " << int(c.b) << std::endl;
            break;

        default:
            assert(false);
            break;
    }

    return attr;
}
