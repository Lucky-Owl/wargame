#ifndef H_GAME_FEATURES
#define H_GAME_FEATURES

#include "basics.hpp"
#include "unit.hpp"
#include "team.hpp"
#include "reference.hpp"

class game_features_t
{
  public:
    unit_t * currentUnit_;
    team_t * currentTeam_;
    reference_t * mainReference_;
    team_t * teamA_;
    team_t * teamB_;

    tile_t * mark_;
    int markX_;
    int markY_;

    game_features_t ( reference_t * ref, unit_t * unit, team_t * team );
    bool loadFeatures ( SDL_Renderer * gRenderer, int mapSize );
    unit_t * lookUpUnit ( int tileNumberX, int tileNumberY );
    ~game_features_t ( );
};

#endif
