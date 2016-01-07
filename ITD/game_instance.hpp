#ifndef H_GAME_INSTANCE
#define H_GAME_INSTANCE

#include "basics.hpp"
#include "reference.hpp"
#include "tile.hpp"
#include "unit.hpp"

class game_instance_t
{

  public:

    int size_;
    int pixSize_;

    int captured_unit;

    int current_team;

    tile_t * regularTile_; //main tile

    tile_t * mark_;
    int markX_;
    int markY_;

    tile_t * cursor_; //cursor
    int cursX_;
    int cursY_;

    unit_t * teamA[6];
    unit_t * teamB[6];

    reference_t * mainReference_;
 
    game_instance_t ( reference_t * reference, int size, int pixSize );
    bool loadImages ( SDL_Renderer * gRenderer );
    bool loadUnits ( SDL_Renderer * gRenderer );
    void drawContent ( TTF_Font * fnt, SDL_Renderer * gRenderer );
    void checkState ( );
    void handlePress ( int x, int y );
    void handleMotion ( int x, int y );
    ~game_instance_t ( );

};

SDL_Texture * loadTexture ( SDL_Renderer * gRenderer, std::string path );
bool check_Range ( int xa, int ya, int xd, int yd , int wp );

#endif
