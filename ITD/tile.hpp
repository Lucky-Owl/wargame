#ifndef H_TILE
#define H_TILE

#include "basics.hpp"

class tile_t
{

  public:
    SDL_Texture * tileTexture_;
 
    tile_t ( SDL_Texture * tileTexture ); 
    void draw ( SDL_Renderer * gRenderer, int x, int y, int size );
    ~tile_t ( );

};

#endif
