#ifndef H_COMMAND
#define H_COMMAND

#include "game_features.hpp"
#include "unit.hpp"
#include "team.hpp"

class command_t
{
  protected:
    game_features_t * game_;

  public:
    virtual void execute ( ) = 0;

    void setGame ( game_features_t * game )
    {
      game_ = game;
    }
};

class informationCommand_t : public command_t
{
  private:
    int x_;
    int y_;
  public:
    informationCommand_t ( int x, int y ) : x_ ( x ), y_ ( y ) {}
    void execute ( )
    {
      if ( (game_->lookUpUnit ( x_, y_ )) == NULL )
        game_->mainReference_->setText ( "It's just a ground." );
      else
        game_->mainReference_->setText ( game_->lookUpUnit ( x_, y_ ) -> information ( ) );
    }
};

class chooseCommand_t : public command_t
{
  private:
    int x_;
    int y_;
  public:
    chooseCommand_t ( int x, int y ) : x_ ( x ), y_ ( y ) {}
    void execute ( )
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
};

class moveCommand_t : public command_t
{
  private:
    int x_;
    int y_;
  public:
    moveCommand_t ( int x, int y ) : x_ ( x ), y_ ( y ) {}
    void execute ( )
    {

    }
};

class  attackCommand_t : public command_t
{
  private:
    unit_t * unit_;
  public:
    attackCommand_t ( unit_t * unit ) : unit_ ( unit ) {}
    void execute ( )
    {

    }
};

#endif
