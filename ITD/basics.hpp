#ifndef H_BASICS
#define H_BASICS

//====Headers==========================
#include "/usr/local/include/SDL2/SDL.h"
#include "/usr/local/include/SDL2/SDL_image.h"
#include "/usr/local/include/SDL2/SDL_ttf.h"
#include <stdio.h>
#include <string>
//=====================================

SDL_Texture * loadTexture ( SDL_Renderer * gRenderer, std::string path );

#endif
