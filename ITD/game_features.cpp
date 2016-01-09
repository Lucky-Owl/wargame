#include "game_features.hpp"

game_features_t::game_features_t ( reference_t * ref, unit_t * unit, team_t * team )
{
  mainReference_ = ref;
  currentUnit_ = unit;
  currentTeam_ = team;
}

bool game_features_t::loadFeatures ( SDL_Renderer * gRenderer, int mapSize )
{
  teamA_ = new team_t ( 6 );
  teamA_->setMarker ( 0 );
  for ( int i = 0; i < teamA_->size_; i++ )
  {
    teamA_->crew_[i] = new unit_t ( i, i );
    teamA_->crew_[i]->setMarker ( teamA_->teamMarker_ );
    teamA_->crew_[i]->tile_ = new tile_t ( loadTexture ( gRenderer, "images/unitA.bmp" ) );
    if ( teamA_->crew_[i]->tile_ == NULL )
    {
      printf ( "Unable to load units A.\n" );
      return false;
    }
  }
  teamB_ = new team_t ( 6 );
  teamB_->setMarker ( 1 );
  for ( int i = 0; i < teamB_->size_; i++ )
  {
    teamB_->crew_[i] = new unit_t ( mapSize-i-1, mapSize-i-1 );
    teamB_->crew_[i]->setMarker ( teamB_->teamMarker_ );
    teamB_->crew_[i]->tile_ = new tile_t ( loadTexture ( gRenderer, "images/unitB.bmp" ) );
    if ( teamB_->crew_[i]->tile_ == NULL )
    {
      printf ( "Unable to load units B.\n" );
      return false;
    }
  }
  return true;
}

unit_t * game_features_t::lookUpUnit ( int tileNumberX, int tileNumberY ) // returns number of current unit, or -1 if none;
{
  for ( int i = 0; i < teamA_->size_; i++ )
  {
    if ( ( teamA_->crew_[i]->posX_ == tileNumberX ) && ( teamA_->crew_[i]->posY_ == tileNumberY ) )
    {
      return teamA_->crew_[i];
    }
  }
  for ( int i = 0; i < teamA_->size_; i++ )
  {
    if ( ( teamB_->crew_[i]->posX_ == tileNumberX ) && ( teamB_->crew_[i]->posY_ == tileNumberY ) )
    {
      return teamB_->crew_[i];
    }
  }
  return NULL;
}

game_features_t::~game_features_t ( )
{
  delete currentUnit_;
  delete currentTeam_;
  delete teamA_;
  delete teamB_;
  delete mainReference_;
}
