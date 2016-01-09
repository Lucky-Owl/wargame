#include "command.hpp"

void command_t::setGame ( game_features_t * game )
{
  game_ = game;
}

informationCommand_t::informationCommand_t ( int x, int y ) : x_ ( x ), y_ ( y ) {}

void informationCommand_t::execute ( )
{
  if ( game_->currentTeam_->visible ( x_, y_ ) )
  {
    if ( (game_->lookUpUnit ( x_, y_ )) == NULL )
      game_->mainReference_->setText ( "It's just a ground." );
    else
      game_->mainReference_->setText ( game_->lookUpUnit ( x_, y_ ) -> information ( ) );
  }
  else
    game_->mainReference_->setText ( "Hidden in the dark." );
}

chooseCommand_t::chooseCommand_t ( int x, int y ) : x_ ( x ), y_ ( y ) {}

void chooseCommand_t::execute ( )
{
  if ( game_->currentTeam_->visible ( x_, y_ ) )
  {
    game_->markX_ = x_;
    game_->markY_ = y_;
    if ( game_->lookUpUnit ( x_, y_ ) != NULL )
    {
      if ( game_->lookUpUnit ( x_, y_ )->teamMarker_ == game_->currentTeam_->teamMarker_ )
      {
        game_->currentUnit_ = game_->lookUpUnit ( x_, y_ );
      }
    }
  }
}

moveCommand_t::moveCommand_t ( int x, int y ) : x_ ( x ), y_ ( y ) {}
void moveCommand_t::execute ( )
{
  if ( !( game_->currentTeam_->connected ( x_, y_, game_->currentUnit_ ) ) || !( game_->currentTeam_->visible ( x_, y_ ) ) )
    game_->mainReference_->setText ( "Do you want to die in this dark world?" );
  else
  {
    if ( ( abs ( game_->currentUnit_->posX_ - x_ ) + abs ( game_->currentUnit_->posY_ - y_ ) ) > game_->currentUnit_->AP_ )
      game_->mainReference_->setText ( "Not enough action points." );
    else
    {
      game_->currentUnit_->AP_ = game_->currentUnit_->AP_ - abs ( game_->currentUnit_->posX_ - x_ ) - abs ( game_->currentUnit_->posY_ - y_ );
      game_->currentUnit_->posX_ = x_;
      game_->currentUnit_->posY_ = y_;
    }
  }
  printf ( "AP: %d\n", game_->currentUnit_->AP_ );
  game_->markX_ = -1;
  game_->markY_ = -1;
  game_->currentUnit_ = NULL;
  printf ( "moved\n" );
}

attackCommand_t::attackCommand_t ( unit_t * unit ) : unit_ ( unit ) {}

void attackCommand_t::execute ( )
{
  if ( game_->currentTeam_->visible ( unit_->posX_, unit_->posY_ ) )
  {
    if ( unit_->teamMarker_ == game_->currentUnit_->teamMarker_ )
      game_->mainReference_->setText ( "Cannot attack your own unit." );
    else
    {
      if ( game_->currentUnit_->AP_ < 3 )
        game_->mainReference_->setText ( "Not enough action points." );
      else
      {
        switch ( game_->currentUnit_->weapon_ )
        {
          case 0:
            if ( inSquare ( unit_->posX_, unit_->posY_, game_->currentUnit_->posX_-1, game_->currentUnit_->posY_-1, 3 ) )
            {
              unit_->HP_-=1;
              game_->currentUnit_->AP_-=3;
            }
            else
              game_->mainReference_->setText ( "Not in range." );
            break;
          case 1:
            if ( ( inSquare ( unit_->posX_, unit_->posY_, game_->currentUnit_->posX_-3, game_->currentUnit_->posY_-3, 5 ) ) &&
               !( inSquare ( unit_->posX_, unit_->posY_, game_->currentUnit_->posX_-1, game_->currentUnit_->posY_-1, 3 ) ) )
            {
              unit_->HP_-=1;
              game_->currentUnit_->AP_-=3;
            }
            else
              game_->mainReference_->setText ( "Not in range." );
            break;
          case 2:
            if ( ( inSquare ( unit_->posX_, unit_->posY_, game_->currentUnit_->posX_-5, game_->currentUnit_->posY_-5, 7 ) ) &&
               !( inSquare ( unit_->posX_, unit_->posY_, game_->currentUnit_->posX_-3, game_->currentUnit_->posY_-3, 5 ) ) )
            {
              unit_->HP_-=1;
              game_->currentUnit_->AP_-=3;
            }
            else
              game_->mainReference_->setText ( "Not in range." );
            break;
          case 3:
            if ( !( inSquare ( unit_->posX_, unit_->posY_, game_->currentUnit_->posX_-5, game_->currentUnit_->posY_-5, 7 ) ) )
            {
              unit_->HP_-=1;
              game_->currentUnit_->AP_-=3;
            }
            else
              game_->mainReference_->setText ( "Not in range." );
            break;
          default:
            printf ( "Unknown type of weapon.\n" );
            break;
        }
      }
    }
  }
  else
    game_->mainReference_->setText ( "Do you want to die in this dark world?" );
  game_->markX_ = -1;
  game_->markY_ = -1;
  game_->currentUnit_ = NULL;
}
