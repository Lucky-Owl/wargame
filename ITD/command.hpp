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

    void setGame ( game_features_t * game );
};

class informationCommand_t : public command_t
{
  private:
    int x_;
    int y_;
  public:
    informationCommand_t ( int x, int y );
   // ~informationCommand_t ( ) {}
    void execute ( );
};

class chooseCommand_t : public command_t
{
  private:
    int x_;
    int y_;
  public:
    chooseCommand_t ( int x, int y );
 //   ~chooseCommand_t ( ) {}
    void execute ( );
};

class moveCommand_t : public command_t
{
  private:
    int x_;
    int y_;
  public:
    moveCommand_t ( int x, int y );
  //  ~moveCommand_t ( ) {}
    void execute ( );
};

class  attackCommand_t : public command_t
{
  private:
    unit_t * unit_;
  public:
    attackCommand_t ( unit_t * unit );
    ~attackCommand_t ( ) {}
    void execute ( );
};

#endif
