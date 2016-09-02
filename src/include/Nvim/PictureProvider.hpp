#ifndef NVIM_PICTURE_PROVIDER_HPP
#define NVIM_PICTURE_PROVIDER_HPP

#include <Nvim/GUIOptions.hpp>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>

#include <unordered_map>
#include <vector>
#include <string>
#include <atomic>

class PictureProvider
{
    public:
        enum PictureLoadStatus {READY, LOADING, FAILED};
    public:
             PictureProvider(const nvim::GUIOptions &GUIOptions);

        const std::pair<PictureProvider::PictureLoadStatus, const sf::Texture *> 
             getPicture(sf::String name);

    private:
        //From cplusplus.com: "Map containers are generally slower than unordered_map containers
        //to access individual elements by their key, 
        //but they allow the direct iteration on subsets based on their order."
        //Since no iteration is needed, unordered map is used.
        std::unordered_map<std::string, sf::Texture>
             mLoadedPictures;

        std::atomic<bool>
             mAutoloadingInProgress;
};

#endif
