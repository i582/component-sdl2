#include "texture.h"

SDL_Texture* Kit::Texture::create(SDL_Renderer* renderer, const Rect& size)
{
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.w(), size.h());
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    return texture;
}

SDL_Texture* Kit::Texture::create(SDL_Renderer* renderer, const Kit::Size& size)
{
    return create(renderer, { 0, 0, size.w(), size.h() });
}

void Kit::Texture::destroy(SDL_Texture* texture)
{
    SDL_DestroyTexture(texture);
}
