#ifndef H_REFERENCE
#define H_REFERENCE

#include "basics.hpp"

class reference_t
{

  public:
    std::string text_;
    SDL_Rect * dest_;
  
    reference_t ( );
    void write ( TTF_Font * fnt, SDL_Renderer * gRenderer );
    void setText ( std::string s );
    void setDestPos ( int x, int y );
    void cleanText ( SDL_Renderer * gRenderer );
    ~reference_t ( );

};

#endif
