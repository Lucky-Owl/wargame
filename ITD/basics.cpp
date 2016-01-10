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

bool inSquare ( int x, int y, int xVer, int yVer, int w )
{
  return ( ( x >= xVer ) && ( x <= xVer + w-1 ) && ( y >=yVer ) && ( y <= yVer + w-1 ) );
}

bool checkRange ( int xD, int yD, int xA, int yA, int w )
{
  switch ( w )
  {
    case 0:
      if ( inSquare ( xD, yD, xA-1, yA-1, 3 ) )
        return true;
      else
        return false;
      break;
    case 1:
      if ( ( inSquare ( xD, yD, xA-3, yA-3, 7 ) ) && !( inSquare ( xD, yD, xA-1, yA-1, 3 ) ) )
        return true;
      else
        return false;
      break;
    case 2:
      if ( ( inSquare ( xD, yD, xA-5, yA-5, 11 ) ) && !( inSquare ( xD, yD, xA-3, yA-3, 7 ) ) )
        return true;
      else
        return false;
      break;
    case 3:
      if ( !( inSquare ( xD, yD, xA-5, yA-5, 11 ) ) )
        return true;
      else
        return false;
      break;
    default:
      printf ( "Unknown type of weapon.\n" );
      return false;
      break;
  }
  return false;
}
