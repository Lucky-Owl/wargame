#include "unit.hpp"

unit_t::unit_t ( int posX, int posY )
{
  alive_ = true;
  HP_ = 3;
  maxAP_ = 6;
  AP_ = 6;
  weapon_ = 1;
  posX_ = posX;
  posY_ = posY;
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
