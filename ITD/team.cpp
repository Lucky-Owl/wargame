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

team_t::~team_t ( )
{
  for ( int i = 0; i < size_; i++ )
    delete crew_[i];
}
