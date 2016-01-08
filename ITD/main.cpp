#include "basics.hpp"
#include "game_instance.hpp"
#include "tile.hpp"
#include "reference.hpp"
#include "basics.hpp"

//-------Screen Constants--------------
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 660;
//-------------------------------------

int fnt_sz = 20;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

//---Basic functions-------------------
bool init()
{
  bool success = true;
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
  {
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    success = false;
  }
  else
  {
    if ( TTF_Init()==-1)
    {
      printf ( "Can't initialize ttf lib.\n" );
      success = false; 
    }
    else
    {
      //Set texture filtering to linear
      if ( !SDL_SetHint ( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
      {
        printf ( "Warning: Linear texture filtering not enabled!\n" );
      }
      gWindow = SDL_CreateWindow ( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
      if( gWindow == NULL )
      {
        printf ( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        success = false;
      }
      else
      {
        gRenderer = SDL_CreateRenderer ( gWindow, -1, SDL_RENDERER_ACCELERATED );
        if( gRenderer == NULL )
        {
          printf ( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
          success = false;
        }
        else
        {
          SDL_SetRenderDrawColor ( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
          int imgFlags = IMG_INIT_PNG;
          if ( ! ( IMG_Init ( imgFlags ) & imgFlags ) )
          {
            printf ( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
            success = false;
          }
        }
      }
    }
  }
  return success;
}

void close()
{  
  SDL_DestroyTexture ( gTexture );
  gTexture = NULL;
  SDL_DestroyRenderer ( gRenderer );
  SDL_DestroyWindow ( gWindow );
  gWindow = NULL;
  gRenderer = NULL;
  IMG_Quit();
  SDL_Quit();
}

int main ( int argc, char* args[] )
{

  if ( !init() )
  {
    printf( "Failed to initialize!\n" );
  }
  else
  {
    TTF_Font * fnt;
    fnt = TTF_OpenFont ( "fonts/GOST-type-B-Standard.ttf", fnt_sz );
    if ( fnt == NULL ) 
    {
      printf ("Failed to open font!\n");
    }

    reference_t * myReference = new reference_t ( );

    game_instance_t * Game = new game_instance_t ( myReference, 30, 22 );
    bool success = Game->loadImages ( gRenderer );
    if ( ! ( success ) )
    {
      printf ( "Unable to load images.\n" );
      return 1;
    }
    success = Game->loadUnits ( gRenderer );
    if ( ! ( success ) )
    {
      printf ( "Unable to load units.\n" );
      return 1;
    }

    bool quit = false;

    SDL_Event event1;
    SDL_Event event2;

    int x = 0,
        y = 0;

    Uint32 gID = SDL_GetWindowID ( gWindow );

   while ( !quit )
    {
      while ( SDL_PollEvent ( &event1 ) != 0 )
      {
        if ( event1.type == SDL_QUIT )
        {
          quit = true;
        }
        else
        {
          if ( ( event1.type == SDL_MOUSEBUTTONUP ) && ( event2.type == SDL_MOUSEBUTTONDOWN ) )
          {
            x = event1.button.x;
            y = event1.button.y;
            if ( event1.button.windowID == gID )
              Game->handlePress ( x, y );
          }
          else
          {
            if ( event1.type == SDL_MOUSEMOTION )
            {
              x = event1.motion.x;
              y = event1.motion.y;
              if ( event1.motion.windowID == gID )
                Game->handleMotion ( x, y );
            }
          }
          event2 = event1;
        }
      }
      SDL_RenderClear ( gRenderer );
      Game->drawContent ( fnt, gRenderer );
      SDL_RenderPresent ( gRenderer );
      Game->checkState ( );
    }
    TTF_CloseFont( fnt );
  }
  close();
  return 0;
}
