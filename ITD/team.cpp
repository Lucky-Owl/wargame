#include "team.hpp"

team_t::team_t ( int size )
{
  size_ = size;
  crew_ = new unit_t*[size_];
}

void team_t::setMarker ( int x )
{
  teamMarker_ = x;
}

bool team_t::connected ( int x, int y, unit_t * exp )
{
  int oldX = exp->posX_;
  int oldY = exp->posY_;
  exp->posX_ = x;
  exp->posY_ = y;
  int stack[size_];
  for ( int i = 0; i < size_; i++ )
    stack[i] = 0;

  for ( int i = 0; i < size_; i++ )
  {
    if ( stack[i] == 0 )
      DFS ( i, stack );
  }

  exp->posX_ = oldX;
  exp->posY_ = oldY;

  for ( int i = 0; i < size_; i++ )
  {
    if ( stack[i] == 0 )
    {
      return false;
    }
  }
  return true;
}

void team_t::DFS ( int x, int * stack )
{
  stack[x] = 1;
  for ( int i = 0; i < size_; i ++ )
  {
    if ( crew_[x]->visible ( crew_[i]->posX_, crew_[i]->posY_ ) && ( stack[i] == 0 ) )
      DFS ( i, stack );
  }
}

bool team_t::visible ( int x, int y )
{
  for ( int i = 0; i < size_; i++ )
  {
    if ( crew_[i]->visible ( x, y ) )
      return true;
  }
  return false;
}

team_t::~team_t ( )
{
  for ( int i = 0; i < size_; i++ )
    delete crew_[i];
}
