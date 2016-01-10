#ifndef H_BASICS
#define H_BASICS

//====Headers==========================
#include "SDL2/SDL.h"
#include "/usr/local/include/SDL2/SDL_image.h"
#include "/usr/local/include/SDL2/SDL_ttf.h"
#include <stdio.h>
#include <string>
#include <vector>
//=====================================

SDL_Texture * loadTexture ( SDL_Renderer * gRenderer, std::string path );
bool inSquare ( int x, int y, int xVer, int yVer, int w );

#endif
