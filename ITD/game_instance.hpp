#ifndef H_GAME_INSTANCE
#define H_GAME_INSTANCE

#include "basics.hpp"
#include "reference.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include "team.hpp"
#include "game_features.hpp"
#include "command.hpp"


class game_instance_t
{

  public:

    int size_;
    int pixSize_;

    game_features_t * features_;

    command_t * currentCommand_;

    tile_t * tileSight_; //main tiles
    tile_t * fog_;
    tile_t * tileMove_;
    tile_t * tileAttack_;

    tile_t * cursor_; //cursor
    int cursX_;
    int cursY_;

 
    game_instance_t ( reference_t * reference, int size, int pixSize );
    void init ( );
    bool loadImages ( SDL_Renderer * gRenderer );
    bool loadUnits ( SDL_Renderer * gRenderer );
    void drawContent ( TTF_Font * fnt, SDL_Renderer * gRenderer );
    void checkState ( );
    bool handlePress ( int x, int y );
    void handleMotion ( int x, int y );
    ~game_instance_t ( );

};

SDL_Texture * loadTexture ( SDL_Renderer * gRenderer, std::string path );

#endif
