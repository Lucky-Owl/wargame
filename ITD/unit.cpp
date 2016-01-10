#include "unit.hpp"

unit_t::unit_t ( int posX, int posY )
{
  teamMarker_ = 0;
  alive_ = true;
  HP_ = 3;
  maxAP_ = 6;
  AP_ = 6;
  sight_ = 4;
  weapon_ = 1;
  posX_ = posX;
  posY_ = posY;
}

void unit_t::setMarker ( int x )
{
  teamMarker_ = x;
}

bool unit_t::visible ( int x, int y )
{
  if ( ( abs ( posX_ - x ) + abs ( posY_ - y ) ) > sight_ )
    return false;
  return true;
}

bool unit_t::reachable ( int x, int y )
{
  if ( ( abs ( posX_ - x ) + abs ( posY_ - y ) ) > AP_ )
    return false;
  return true;
}

std::string unit_t::information ( )
{
  return "It's a fucking unit, what do you want?";
}

void unit_t::kill ( )
{
  posX_ = -1;
  posY_ = -1;
  maxAP_ = 0;
  AP_ = 0;
  alive_ = false;
}

unit_t::~unit_t()
{
  delete tile_;
  posX_ = -1;
  posY_ = -1;
}
