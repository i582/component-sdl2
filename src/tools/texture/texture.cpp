#include "texture.h"

SDL_Texture* Kit::Texture::create(SDL_Renderer* renderer, const Rect& size)
{
    return create(renderer, SimpleSize(size.w(), size.h()));
}

SDL_Texture* Kit::Texture::create(SDL_Renderer* renderer, const Kit::Size& size)
{
    return create(renderer, SimpleSize(size.w(), size.h()));
}

SDL_Texture* Kit::Texture::create(SDL_Renderer* renderer, const SimpleRect& size)
{
    return create(renderer, SimpleSize(size.w, size.h));
}

SDL_Texture* Kit::Texture::create(SDL_Renderer* renderer, const Kit::SimpleSize& size)
{
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.w,
                                             size.h);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    return texture;
}

void Kit::Texture::destroy(SDL_Texture* texture)
{
    SDL_DestroyTexture(texture);
}