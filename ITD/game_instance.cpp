#include "game_instance.hpp"

SDL_Texture * loadTexture ( SDL_Renderer * gRenderer, std::string path )
{
  SDL_Texture* newTexture = NULL;
  SDL_Surface* loadedSurface = IMG_Load ( path.c_str() );
  if ( loadedSurface == NULL )
  {
    printf ( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
  }
  else
  {
    SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB ( loadedSurface->format, 0, 0, 0 ) );
    newTexture = SDL_CreateTextureFromSurface ( gRenderer, loadedSurface );
    if ( newTexture == NULL )
    {
      printf ( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }
    SDL_FreeSurface ( loadedSurface );
  }
  return newTexture;
}

game_instance_t::game_instance_t ( reference_t * reference, int size, int pixSize ) // TODO: create load with tiles and reference
{
  current_team = 0;
  captured_unit = -1;
  cursX_ = 0;
  cursY_ = 0;
  markX_ = -1;
  markY_ = -1;

  mainReference_ = reference;

  size_ = size;
  pixSize_ = pixSize;

  mainReference_->setDestPos ( size_*pixSize_, 0 );
}

bool game_instance_t::loadImages ( SDL_Renderer * gRenderer )
{
  bool success = true;
  cursor_ = new tile_t ( loadTexture ( gRenderer, "images/cursor.bmp" ) );
  if ( cursor_ == NULL ) 
  {
    printf("Failed loading cursor texture!\n");
    success = false;
  }
  mark_ = new tile_t ( loadTexture ( gRenderer, "images/mark.bmp" ) );
  if ( mark_ == NULL ) 
  {
    printf("Failed loading mark texture!\n");
    success = false;
  }
  regularTile_ = new tile_t ( loadTexture ( gRenderer, "images/tile.bmp" ) );
  if ( regularTile_ == NULL ) 
  {
    printf("Failed loading tile texture!\n");
    success = false;
  }
  return success;
}

bool game_instance_t::loadUnits ( SDL_Renderer * gRenderer )
{
  for ( int i = 0; i < 6; i++ )
  {
    teamA[i] = new unit_t ( i+6, i+6 );
    teamA[i]->tile_ = new tile_t ( loadTexture ( gRenderer, "images/unitA.bmp" ) );
    if ( teamA[i]->tile_ == NULL )
    {
      printf ( "Unable to load units.\n" );
      return false;
    }
  }
  for ( int i = 0; i < 6; i++ )
  {
    teamB[i] = new unit_t ( size_-i-6, size_-i-6 );
    teamB[i]->tile_ = new tile_t ( loadTexture ( gRenderer, "images/unitB.bmp" ) );
    if ( teamB[i]->tile_ == NULL )
    {
      printf ( "Unable to load units.\n" );
      return false;
    }
  }
  return true;
}

void game_instance_t::drawContent ( TTF_Font * fnt, SDL_Renderer * gRenderer )
{
  mainReference_->cleanText ( gRenderer );
  for ( int i = 0; i < size_; i++ )
  {
    for ( int j = 0; j < size_; j ++ )
    {
      if ( ( i == cursX_ ) && ( j == cursY_ ) )
        cursor_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
      else
      {
        if ( ( i == markX_ ) && ( j == markY_ ) )
          mark_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
        else
          regularTile_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
      }
      for ( int k = 0; k < 6; k++ )
      {
        if ( ( i == teamA[k]->posX_ ) && ( j == teamA[k]->posY_ ) )
          if ( teamA[k]->alive_ )
            teamA[k]->tile_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
      }
      for ( int k = 0; k < 6; k++ )
      {
        if ( ( i == teamB[k]->posX_ ) && ( j == teamB[k]->posY_ ) )
          if ( teamB[k]->alive_ )
            teamB[k]->tile_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
      }
    }
  }
  mainReference_->write ( fnt, gRenderer );
}

void game_instance_t::checkState ( )
{
  int sumAP = 0;
  if ( current_team == 0 )
  {
    for ( int i = 0; i < 6; i++ )
      sumAP += teamA[i]->AP_;
    if ( sumAP == 0 )
    {
      current_team = 1;
      for ( int i = 0; i < 6; i++ )
        teamB[i]->AP_ = teamB[i]->maxAP_;
      mainReference_->setText ( "Next move!" );
    }
  }
  if ( current_team == 1 )
  {
    for ( int i = 0; i < 6; i++ )
      sumAP += teamB[i]->AP_;
    if ( sumAP == 0 )
    {
      current_team = 0;
      for ( int i = 0; i < 6; i++ )
        teamA[i]->AP_ = teamA[i]->maxAP_;
      mainReference_->setText ( "Next move!" );
    }
  }
}

bool check_Range ( int xa, int ya, int xd, int yd , int wp )
{
  bool check = true;
  int rng = 0;
  switch ( wp )
  {
    case 0: rng = 1; break;
    case 1: rng = 3; break;
    default: break;
  }
  if ( ( (xa-rng == xd)||(xa+rng==xd) )&&( (ya-rng==yd)||(ya+rng==yd) )  )
    check = false;
  return check;
}

void game_instance_t::handlePress ( int x, int y )
{
  int tileNumberX = x / pixSize_;
  int tileNumberY = y / pixSize_;
  if ( ( tileNumberX <= size_ ) && ( tileNumberY <= size_ ) )
  {
    markX_ = tileNumberX;
    markY_ = tileNumberY;
    if ( captured_unit != -1 )
    {
      bool reserved = false; //collision detect
      int current_unit = -1;
      for ( int i = 0; i < 6; i++ )
      {
        if ( ( teamA[i]->posX_ == tileNumberX ) && ( teamA[i]->posY_ == tileNumberY ) )
        {
          reserved = true;
          current_unit = i;
        }
        if ( ( teamB[i]->posX_ == tileNumberX ) && ( teamB[i]->posY_ == tileNumberY ) )
        {
          reserved = true;
          current_unit = 6 + i;
        }
      }

      if ( ! ( reserved ) )
      {
        if ( captured_unit < 6 )
        {
          int possibleMovement = abs ( tileNumberX - teamA[captured_unit]->posX_ ) + abs ( tileNumberY - teamA[captured_unit]->posY_ );
          if ( ! ( possibleMovement > teamA[captured_unit]->AP_ ) )
          {
            teamA[captured_unit]->posX_ = tileNumberX;
            teamA[captured_unit]->posY_ = tileNumberY;
            teamA[captured_unit]->AP_-=possibleMovement;
          }
          else
            mainReference_->setText ( "Not enough action points!" );
        }
        else
        {
          int possibleMovement = abs ( tileNumberX - teamB[captured_unit%6]->posX_ ) + abs ( tileNumberY - teamB[captured_unit%6]->posY_ );
          if ( ! ( possibleMovement > teamB[captured_unit%6]->AP_ ) )
          {
            teamB[captured_unit%6]->posX_ = tileNumberX;
            teamB[captured_unit%6]->posY_ = tileNumberY;
            teamB[captured_unit%6]->AP_-=possibleMovement;
          }
          else
            mainReference_->setText ( "Not enough action points!" );
        }
      }
      else
      {
        if ( current_unit < 6 )
        {
          if ( teamB[captured_unit%6]->AP_ >= 3 )
          {
            bool out_of_range = checkRange ( teamB[captured_unit%6]->posX_, teamB[captured_unit%6]->posY_, teamA[current_unit]->posX_, teamA[current_unit]->posY_, teamB[captured_unit%6]->weapon_ );
            if ( out_of_range )
              mainReference_->setText ( "Out of range!" );
            else
            {
              teamB[captured_unit%6]->AP_-=3;
              teamA[current_unit]->HP_--;
              if ( teamA[current_unit]->HP_ <= 0 )
                teamA[current_unit]->kill ( );
            }
          }
          else
            mainReference_->setText ( "Not enough action points!" );
        }
        else
        {
          if ( teamA[captured_unit]->AP_ >= 3 )
          {
            bool out_of_range = checkRange ( teamA[captured_unit]->posX_, teamA[captured_unit]->posY_, teamB[current_unit%6]->posX_, teamB[current_unit%6]->posY_, teamA[captured_unit]->weapon_ );
            if ( out_of_range )
              mainReference_->setText ( "Out of range!" );
            else
            {
              teamA[captured_unit]->AP_-=3;
              teamB[current_unit%6]->HP_--;
              if ( teamB[current_unit%6]->HP_ <= 0 )
                teamB[current_unit%6]->kill ( );
            }
          }
          else
            mainReference_->setText ( "Not enough action points!" );
        }
      }
      markX_ = -1;
      markY_ = -1;
      captured_unit = -1;
    }
    else
    {
      for ( int i = 0; i < 6; i++ )
      {
        if ( ( tileNumberX == teamA[i]->posX_ ) && ( tileNumberY == teamA[i]->posY_ ) )
        {
          if ( current_team == 0 )
            captured_unit = i;
          else
            mainReference_->setText ( "Enemy" );
        }
        else
          if ( ( tileNumberX == teamB[i]->posX_ ) && ( tileNumberY == teamB[i]->posY_ ) )
          {
            if ( current_team == 1 )
              captured_unit = i + 6;
            else
              mainReference_->setText ( "Enemy" );
          }
      }
    }

  }
}

void game_instance_t::handleMotion ( int x, int y )
{
  int tileNumberX = x / pixSize_;
  int tileNumberY = y / pixSize_;
  if ( ( tileNumberX <= size_ ) && ( tileNumberY <= size_ ) )
  {
    cursX_ = tileNumberX;
    cursY_ = tileNumberY;
    if ( captured_unit != -1 )
    {
      mainReference_->setText ( "Move" );
/*
      for ( int i = 0; i < 6; i++ )
      {
        if ( ( tileNumberX == teamA[i]->posX_ ) && ( tileNumberY == teamA[i]->posY_ ) )
          mainReference_->setText ( "Cannot move here" );
        else
        {
          if ( ( tileNumberX == teamB[i]->posX_ ) && ( tileNumberY == teamB[i]->posY_ ) )
            mainReference_->setText ( "Attack" );
          else
            mainReference_->setText ( "Move" );
        }
      }
*/
    }
  }
}

game_instance_t::~game_instance_t()
{
  delete regularTile_;
  delete cursor_;
  delete mainReference_;
  for (int i = 0; i<6; i++)
    delete teamA[i];
  for (int i = 0; i<6; i++)
    delete teamB[i];
}
