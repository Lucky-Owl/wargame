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
  fog_ = new tile_t ( loadTexture ( gRenderer, "images/fog.bmp" ) );
  if ( fog_ == NULL ) 
  {
    printf("Failed loading fog texture!\n");
    success = false;
  }
  return success;
}

bool game_instance_t::loadUnits ( SDL_Renderer * gRenderer )
{
  for ( int i = 0; i < 6; i++ )
  {
    teamA[i] = new unit_t ( i, i );
    teamA[i]->tile_ = new tile_t ( loadTexture ( gRenderer, "images/unitA.bmp" ) );
    if ( teamA[i]->tile_ == NULL )
    {
      printf ( "Unable to load units A.\n" );
      return false;
    }
  }
  for ( int i = 0; i < 6; i++ )
  {
    teamB[i] = new unit_t ( size_-i-1, size_-i-1 );
    teamB[i]->tile_ = new tile_t ( loadTexture ( gRenderer, "images/unitB.bmp" ) );
    if ( teamB[i]->tile_ == NULL )
    {
      printf ( "Unable to load units B.\n" );
      return false;
    }
  }
  return true;
}

void game_instance_t::drawContent ( TTF_Font * fnt, SDL_Renderer * gRenderer ) 
{
  mainReference_->cleanText ( gRenderer );
  bool visible = false;
  for ( int i = 0; i < size_; i++ )
  {
    for ( int j = 0; j < size_; j++ )
    {
      visible = false;
      //----Sight check
      for ( int h = 0; h < 6; h++ )
      {
        switch ( current_team )
        {
          case 0 :
            if ( ( abs ( teamA[h]->posX_ - i ) + abs ( teamA[h]->posY_ - j ) ) <= 2 )
              visible = true;
            break;
          case 1 :
            if ( ( abs ( teamB[h]->posX_ - i ) + abs ( teamB[h]->posY_ - j ) ) <= 2 )
              visible = true;
            break;
          default :
            break;
        }
      }
      if ( visible )
      {
       //----Mouse and map
        if ( ( i == cursX_ ) && ( j == cursY_ ) )
          cursor_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
        else
        {
          if ( ( i == markX_ ) && ( j == markY_ ) )
            mark_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
          else
            regularTile_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
        }
        //-----Units
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
      else
        fog_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
    }
  }
  mainReference_->write ( fnt, gRenderer );
}

void game_instance_t::checkState ( ) // would be nice to have "turn reference"
{
  int sumAP = 0;
  if ( current_team == 0 )
  {
    for ( int i = 0; i < 6; i++ )
      sumAP += teamA[i]->AP_;
    if ( sumAP == 0 )
    {
      current_team = 1;
      captured_unit = -1;
      markX_ = -1;
      markY_ = -1;
      for ( int i = 0; i < 6; i++ )
        teamB[i]->AP_ = teamB[i]->maxAP_;
      mainReference_->setText ( "Next move B!" );
    }
  }
  if ( current_team == 1 )
  {
    for ( int i = 0; i < 6; i++ )
      sumAP += teamB[i]->AP_;
    if ( sumAP == 0 )
    {
      current_team = 0;
      captured_unit = -1;
      markX_ = -1;
      markY_ = -1;
      for ( int i = 0; i < 6; i++ )
        teamA[i]->AP_ = teamA[i]->maxAP_;
      mainReference_->setText ( "Next move A!" );
    }
  }
}

bool checkRange ( int xa, int ya, int xd, int yd , int wp )
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

int game_instance_t::lookWho( int  tileNumberX, int tileNumberY ) // returns number of current unit, or -1 if none;
{
  int res = -1; 
  for ( int i = 0; i < 6; i++ )
  {
    if ( ( teamA[i]->posX_ == tileNumberX ) && ( teamA[i]->posY_ == tileNumberY ) )
    {
      res = i;
      return res;
    }
    if ( ( teamB[i]->posX_ == tileNumberX ) && ( teamB[i]->posY_ == tileNumberY ) )
    {
      res = i+6;
      return res;
    }
  }
  return res;
}

bool game_instance_t::attackUnit ( int current_unit )
{
  if ( current_unit < 6 )
  {
    if ( teamB[captured_unit%6]->AP_ >= 3 )
    {
       bool out_of_range = checkRange ( teamB[captured_unit%6]->posX_, teamB[captured_unit%6]->posY_, teamA[current_unit]->posX_, teamA[current_unit]->posY_, teamB[captured_unit%6]->weapon_ );
        if ( out_of_range )
        { 
          mainReference_->setText ( "Out of range!" );
          return false;
        }
        else
        {
          teamB[captured_unit%6]->AP_-=3;
          teamA[current_unit]->HP_--;
          if ( teamA[current_unit]->HP_ <= 0 )
                teamA[current_unit]->kill ( );
          return true;
        }
     }
     else
     {
       mainReference_->setText ( "Not enough action points!" );
       return false;
     }
   }
   else
   {
     if ( teamA[captured_unit]->AP_ >= 3 )
     {
       bool out_of_range = checkRange ( teamA[captured_unit]->posX_, teamA[captured_unit]->posY_, teamB[current_unit%6]->posX_, teamB[current_unit%6]->posY_, teamA[captured_unit]->weapon_ );
       if ( out_of_range )
       { 
         mainReference_->setText ( "Out of range!" );
         return false;
       }
       else
       {
         teamA[captured_unit]->AP_-=3;
         teamB[current_unit%6]->HP_--;
         if ( teamB[current_unit%6]->HP_ <= 0 )
                teamB[current_unit%6]->kill ( );
         return true;
       }
     }  
     else
     {
       mainReference_->setText ( "Not enough action points!" );
       return false;
     }
  }
}

bool game_instance_t::moveUnit ( int tileNumberX, int tileNumberY )
{
  if ( captured_unit < 6 ) //team 0
  {
    int possibleMovement = abs ( tileNumberX - teamA[captured_unit]->posX_ ) + abs ( tileNumberY - teamA[captured_unit]->posY_ );
    if ( possibleMovement <= teamA[captured_unit]->AP_ )
    {
      teamA[captured_unit]->posX_ = tileNumberX;
      teamA[captured_unit]->posY_ = tileNumberY;
      teamA[captured_unit]->AP_-=possibleMovement;
      return true;
    }
    else
    {
      mainReference_->setText ( "Not enough action points!" );
      return false;
    }
  }
  else //team 1
  {
    int possibleMovement = abs ( tileNumberX - teamB[captured_unit%6]->posX_ ) + abs ( tileNumberY - teamB[captured_unit%6]->posY_ );
    if  ( possibleMovement <=teamB[captured_unit%6]->AP_ ) 
    {
      teamB[captured_unit%6]->posX_ = tileNumberX;
      teamB[captured_unit%6]->posY_ = tileNumberY;
      teamB[captured_unit%6]->AP_-=possibleMovement;
      return true;
    }
    else
    {
      mainReference_->setText ( "Not enough action points!" );
      return false;
    }
  }
}

bool game_instance_t::handlePress ( int x, int y )
{
  int tileNumberX = x / pixSize_;
  int tileNumberY = y / pixSize_;
  int current_unit = -1;
  if ( ( tileNumberX > size_ ) && ( tileNumberY > size_ ) )
  {
    return false;
  }
  current_unit = lookWho( tileNumberX, tileNumberY );
  if ( ( current_unit >= current_team*6 ) && ( current_unit < ( current_team + 1 )*6 ) ) 
  {
    if ( current_unit == captured_unit )
    {
      markX_ = -1;
      markY_ = -1;
      captured_unit = -1; 
      return true;
    }
    else
    {
      markX_ = tileNumberX;
      markY_ = tileNumberY;
      captured_unit = current_unit; 
      return true;
    }
  }
  if ( captured_unit != -1 ) 
  {
    if ( ( ( current_unit < current_team*6 ) || ( current_unit >= ( current_team + 1 )*6 ) )&&( current_unit!=-1 ) )
    {
      if ( attackUnit ( current_unit ) )
      {
        markX_ = -1;
        markY_ = -1;
        captured_unit = -1; 
        return true;
      }
      else
      {
        return false;
      }
    } 
    else 
    {
      if ( moveUnit ( tileNumberX, tileNumberY ) )
      {
        markX_ = -1;
        markY_ = -1;
        captured_unit = -1; 
        return true;
      }
      else
      {
        return false;
      } 
    }
  }
  return false;
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
      int current_unit = lookWho( tileNumberX, tileNumberY );
      if ( ( current_unit >= current_team*6 ) && ( current_unit < ( current_team + 1 )*6 ) ) 
      {
        if ( current_unit == captured_unit )         
          mainReference_->setText ( "You are here" );
        else
         mainReference_->setText ( "Cannot move here" );
      }
      else
      {
        if ( current_unit == -1 )
          mainReference_->setText ( "Move" );
        else 
          mainReference_->setText ( "Attack" );
      }
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
