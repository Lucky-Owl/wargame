#include "basics.hpp"

SDL_Texture * loadTexture ( SDL_Renderer * gRenderer, std::string path )
{
  SDL_Texture* newTexture = NULL;
  SDL_Surface* loadedSurface = IMG_Load ( path.c_str() );
  if ( loadedSurface == NULL )
  {
    printf ( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
  }
  else
  {
    SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB ( loadedSurface->format, 0, 0, 0 ) );
    newTexture = SDL_CreateTextureFromSurface ( gRenderer, loadedSurface );
    if ( newTexture == NULL )
    {
      printf ( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }
    SDL_FreeSurface ( loadedSurface );
  }
  return newTexture;
}
