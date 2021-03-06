#include "game_instance.hpp"

game_instance_t::game_instance_t ( reference_t * reference, int size, int pixSize )
{
  cursX_ = 0;
  cursY_ = 0;

  features_ = new game_features_t ( reference, NULL, NULL );
  features_->markX_ = -1;
  features_->markY_ = -1;

  size_ = size;
  pixSize_ = pixSize;

  currentCommand_ = NULL;

  features_->mainReference_->setDestPos ( size_*pixSize_, 0 );
}

void game_instance_t::init ( )
{
  features_->currentTeam_ = features_->teamA_;
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
  features_->mark_ = new tile_t ( loadTexture ( gRenderer, "images/mark.bmp" ) );
  if ( features_->mark_ == NULL ) 
  {
    printf("Failed loading mark texture!\n");
    success = false;
  }
  tileMove_ = new tile_t ( loadTexture ( gRenderer, "images/tileMove.bmp" ) );
  if ( tileMove_ == NULL ) 
  {
    printf("Failed loading tile texture!\n");
    success = false;
  }
  tileSight_ = new tile_t ( loadTexture ( gRenderer, "images/tileSight.bmp" ) );
  if ( tileSight_ == NULL ) 
  {
    printf("Failed loading tile texture!\n");
    success = false;
  }
  tileAttack_ = new tile_t ( loadTexture ( gRenderer, "images/tileAttack.bmp" ) );
  if ( tileAttack_ == NULL ) 
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
  return features_->loadFeatures ( gRenderer, size_ );
}

void game_instance_t::drawContent ( TTF_Font * fnt, SDL_Renderer * gRenderer ) 
{
  features_->mainReference_->cleanText ( gRenderer );
  for ( int i = 0; i < size_; i++ )
  {
    for ( int j = 0; j < size_; j++ )
    {
//====Sight check======================
      bool visible = features_->currentTeam_->visible ( i, j );
//=====================================

      if ( visible )
      {

//====Mouse and map====================
        if ( ( i == cursX_ ) && ( j == cursY_ ) )
          cursor_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
        else
        {
          if ( ( i == features_->markX_ ) && ( j == features_->markY_ ) )
            features_->mark_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
          else
          {
            if ( features_->currentUnit_ != NULL )
            {
              if ( features_->lookUpUnit ( i, j ) == NULL )
              {
                if ( ( features_->currentTeam_->connected ( i, j, features_->currentUnit_ ) ) && ( features_->currentUnit_->reachable ( i, j ) ) )
                  tileMove_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
                else
                  tileSight_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
              }
              else
              {
                if ( features_->lookUpUnit ( i, j )->teamMarker_ == features_->currentUnit_->teamMarker_ )
                  tileSight_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
                else
                {
                  if ( checkRange ( features_->lookUpUnit ( i, j )->posX_, features_->lookUpUnit ( i, j )->posY_, features_->currentUnit_->posX_, features_->currentUnit_->posY_, features_->currentUnit_->weapon_ ) )
                    tileAttack_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
                }
              }
            }
            else
              tileSight_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
          }
        }
//=====================================

//====Units============================
        for ( int k = 0; k < features_->teamA_->size_; k++ )
        {
          if ( ( i == features_->teamA_->crew_[k]->posX_ ) && ( j == features_->teamA_->crew_[k]->posY_ ) )
            if ( features_->teamA_->crew_[k]->alive_ )
              features_->teamA_->crew_[k]->tile_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
        }
        for ( int k = 0; k < features_->teamB_->size_; k++ )
        {
          if ( ( i == features_->teamB_->crew_[k]->posX_ ) && ( j == features_->teamB_->crew_[k]->posY_ ) )
            if ( features_->teamB_->crew_[k]->alive_ )
              features_->teamB_->crew_[k]->tile_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
        }
//=====================================
      }
      else
        fog_->draw ( gRenderer, i*pixSize_, j*pixSize_, pixSize_ );
    }
  }
  features_->mainReference_->write ( fnt, gRenderer );
}

void game_instance_t::checkState ( ) // would be nice to have "turn reference"
{
  printf ( "checkState entered\n" );
  if ( currentCommand_ != NULL )
  {
    currentCommand_->setGame ( features_ );
    currentCommand_->execute ( );
    currentCommand_ = NULL;
  }

  for ( int i = 0; i < features_->teamA_->size_; i++ )
  {
    if ( features_->teamA_->crew_[i]->HP_ <= 0 )
      features_->teamA_->crew_[i] -> kill ( );
  }

  for ( int i = 0; i < features_->teamB_->size_; i++ )
  {
    if ( features_->teamB_->crew_[i]->HP_ <= 0 )
      features_->teamB_->crew_[i] -> kill ( );
  }

  int sumAP = 0;
  for ( int i = 0; i < features_->currentTeam_->size_; i++ )
    sumAP += features_->currentTeam_->crew_[i]->AP_;
  if ( sumAP == 0 )
  {
    if ( features_->currentTeam_ == features_->teamA_ )
      features_->currentTeam_ = features_->teamB_;
    else
      features_->currentTeam_ = features_->teamA_;
    features_->currentUnit_ = NULL;
    features_->markX_ = -1;
    features_->markY_ = -1;
    for ( int i = 0; i < 6; i++ )
      features_->currentTeam_->crew_[i]->AP_ = features_->currentTeam_->crew_[i]->maxAP_;
    features_->mainReference_->setText ( "Next move" );
  }
}

bool game_instance_t::handlePress ( int x, int y )
{
  int tileNumberX = x / pixSize_;
  int tileNumberY = y / pixSize_;
  if ( ( tileNumberX > size_ ) && ( tileNumberY > size_ ) )
  {
    return false;
  }
  if ( features_->currentUnit_ == NULL )
  {
    currentCommand_ = new chooseCommand_t ( tileNumberX, tileNumberY );
    return true;
  }
  else
  {
    unit_t * capturedUnit = features_->lookUpUnit ( tileNumberX, tileNumberY );
    if ( capturedUnit == NULL )
    {
      currentCommand_ = new moveCommand_t ( tileNumberX, tileNumberY );
      return true;
    }
    else
    {
      currentCommand_ = new attackCommand_t ( capturedUnit );
      return true;
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
    if ( ( tileNumberX != cursX_ ) || ( tileNumberY != cursY_ ) )
    {
      cursX_ = tileNumberX;
      cursY_ = tileNumberY;
      currentCommand_ = new informationCommand_t ( cursX_, cursY_ );
    }
  }
}

game_instance_t::~game_instance_t()
{
  delete currentCommand_;
  delete tileSight_;
  delete tileMove_;
  delete tileAttack_;
  delete cursor_;
  delete fog_;
  delete features_;
}
