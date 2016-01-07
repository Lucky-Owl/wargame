#include "tile.hpp"

tile_t::tile_t ( SDL_Texture * tileTexture )
{
  tileTexture_ = tileTexture;
}

void tile_t::draw ( SDL_Renderer * gRenderer, int x, int y, int size )
{
  SDL_Rect * dest = new SDL_Rect();
  dest->x = x;
  dest->y = y;
  dest->w = size;
  dest->h = size;
  SDL_RenderCopy ( gRenderer, tileTexture_ , NULL , dest);
  free ( dest );
}

tile_t::~tile_t()
{
  SDL_DestroyTexture( tileTexture_ );
  tileTexture_ = NULL;
}
