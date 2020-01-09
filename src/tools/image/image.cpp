#include "image.h"
#include "component.h"
#include "window.h"

Kit::map<Kit::string, SDL_Texture*> Kit::image::_cache = {};

Kit::image::image(Kit::Component* parent_)
{
    this->_parent = parent_;

    this->_renderer = nullptr;
    this->_texture = nullptr;
    this->_textureSize = { 0, 0, 0, 0 };

    this->_needUpdate = false;

    this->setup();
}

void Kit::image::setup()
{
    IMG_Init(IMG_INIT_PNG);

    this->_renderer = _parent->renderer();
}

void Kit::image::load()
{
    if (_path.empty() || !_needUpdate)
        return;


    // check availability in cache
    const string& needle_image = std::to_string(_parent->parentWindow()->id()) + "@" + _path;
    const auto& it = _cache.find(needle_image);
    if (it != _cache.end())
    {
        _texture = _cache.at(needle_image);

        if (_texture == nullptr)
        {
            throw std::runtime_error("ERROR: cache was corrupted!");
        }

        // get size
        SDL_QueryTexture(_texture, nullptr, nullptr, &_textureSize.w, &_textureSize.h);

        _needUpdate = false;
        return;
    }


    _texture = IMG_LoadTexture(_renderer, _path.c_str());

    if (_texture == nullptr)
    {
        throw std::runtime_error(string("ERROR: ") + IMG_GetError());
    }

    // get size
    SDL_QueryTexture(_texture, nullptr, nullptr, &_textureSize.w, &_textureSize.h);

    _needUpdate = false;


    // add to cache
    const string& cache_image = std::to_string(_parent->parentWindow()->id()) + "@" + _path;
    _cache.insert(std::make_pair(cache_image, _texture));
}

Kit::image* Kit::image::path(const string& path_)
{
    if (path_.empty() || path_ == _path)
        return this;

    _path = path_;
    _needUpdate = true;

    load();

    return this;
}

Kit::image* Kit::image::position(const Point& position_)
{
    _size.start = position_;
    _size.calc(_parent->innerSize());

    return this;
}

Kit::image* Kit::image::size(const Kit::Size& size_, bool saveProportion_)
{
    _size.size = size_;
    _size.calc(_parent->innerSize());


    if (saveProportion_)
    {
        const int width = _size.size.w();
        const double originalProportion = _textureSize.h / (double)_textureSize.w;

        const int newHeight = (int)(width * originalProportion);

        _size.size.h(newHeight);
    }

    return this;
}

void Kit::image::render() const
{
    SDL_SetRenderTarget(_renderer, _parent->innerTexture());

    const SDL_Rect dstRect = _size.toSdlRect();

    SDL_RenderCopy(_renderer, _texture, nullptr, &dstRect);

    SDL_SetRenderTarget(_renderer, nullptr);
}

void Kit::image::delete_cache()
{
    _cache.clear();
}