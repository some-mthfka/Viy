template <typename T>
std::string toString(const T& value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}

/*
template <typename T>
std::basic_string<sf::Uint32> toUtf32String(const T& value)
{
    std::basic_stringstream<sf::Uint32> stream;
    stream << value;
    return stream.str();
}
*/
