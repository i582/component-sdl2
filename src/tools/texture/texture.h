#include "SDL.h"

#include "extended-rect.h"

namespace Kit
{

    class Texture
    {
    public:
        static SDL_Texture* create(SDL_Renderer* renderer, const Rect& size);
        static SDL_Texture* create(SDL_Renderer* renderer, const Size& size);

        static void destroy(SDL_Texture* texture);
    };

}