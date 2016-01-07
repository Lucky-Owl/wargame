#include "reference.hpp"

reference_t::reference_t()
{
  text_ = " "; 
  dest_ = new SDL_Rect();
}

void reference_t::write ( TTF_Font * fnt, SDL_Renderer * gRenderer )
{
  SDL_Surface * textSurface;
  SDL_Texture * textTexture;
  SDL_Color txt_clr = { 0, 0, 0};
  textSurface = TTF_RenderText_Blended(fnt, text_.c_str(), txt_clr);
  if ( textSurface == NULL )
  { 
    printf ("Unable to render text surface!\n");
  }
  else
  { 
    dest_->w = textSurface->w;
    dest_->h = textSurface->h;
    textTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
    if ( textTexture == NULL )
    {
      printf("Unable to create texture from rendered text!\n");
    }
    else
    { 
      SDL_RenderCopy ( gRenderer, textTexture, NULL , dest_);
    }
  }
  SDL_FreeSurface ( textSurface );
  textSurface = NULL;
  SDL_DestroyTexture ( textTexture );
  textTexture = NULL;
}

void reference_t::setText ( std::string s )
{
  text_ = s;
}

void reference_t::setDestPos ( int x, int y )
{
  dest_->x = x;
  dest_->y = y;
}

void reference_t::cleanText ( SDL_Renderer * gRenderer )
{
  SDL_SetRenderDrawColor ( gRenderer, 255, 255, 255, 255 );
  SDL_RenderFillRect ( gRenderer, dest_ );
}

reference_t::~reference_t ( )
{
  free ( dest_ );
}
