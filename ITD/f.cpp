#include <stdio.h>
#include <stdlib.h>

const int Size = 6;

class unit_t
{
  public:
    int HP_;
    int AP_;
    int weapon_;
    int item_;
    unit_t ()
    {
      HP_ = 3;
      AP_ = 6;
      weapon_ = 0;
      item_ = 0;
    }
    void giveWeapon ( int x )
    {
      weapon_ = x;
    }
    void giveItem ( int x )
    {
      item_ = x;
    }
    void print ( )
    {
      printf ( "HP: %d\n", HP_ );
      printf ( "AP: %d\n", AP_ );
      printf ( "Weapon: " );
      switch (weapon_)
      {
        case 0 :
          printf ( "None\n" );
          break;
        case 1 :
          printf ( "Sword\n" );
          break;
        case 2 :
          printf ( "Shotgun\n" );
          break;
        case 3 :
          printf ( "Rifle\n" );
          break;
        case 4 :
          printf ( "Sniper rifle\n" );
          break;
        default :
          printf ( "Unknown\n" );
          break;
     }
     printf ( "Item: " );
     switch (item_)
     {
       case 0 :
         printf ( "None\n" );
         break;
       case 1 :
         printf ( "Armor\n" );
         break;
       case 3 :
         printf ( "Energy boost\n" );
         break;
       case 4 :
         printf ( "Lamp\n" );
         break;
       default :
         printf ( "Unknown\n" );
         break;
     }
  }
};

int main ()
{
  unit_t * teamA[6];
  for ( int i = 0; i < Size; i++ )
  {
    teamA[i] = new unit_t ( );
  }
  unit_t * teamB[6];
  for ( int i = 0; i < Size; i++ )
  {
    teamB[i] = new unit_t ( );
  }

  int mNumber = 0;
  int sNumber = 0;
  int wNumber = 0;

  bool done = false;

  while (!(done))
  {
    printf ( "A : Choose your team:\n" );
    for ( int i = 0; i < Size; i++ )
    {
      teamA[i]->print();
      printf ( "\n" );
    }
    scanf ( "%d %d %d", &mNumber, &sNumber, &wNumber );
    if ( mNumber == 0 )
    {
      done = true;
    }
    else
    {
      if ( sNumber == 1 )
        teamA[mNumber-1]->giveWeapon ( wNumber );
      else
        teamA[mNumber-1]->giveItem ( wNumber );
    }
  }

  done = false;

  while (!(done))
  {
    printf ( "A : Choose your team:\n" );
    for ( int i = 0; i < Size; i++ )
    {
      teamB[i]->print();
      printf ( "\n" );
    }
    scanf ( "%d %d %d", &mNumber, &sNumber, &wNumber );
    if ( mNumber == 0 )
    {
      done = true;
    }
    else
    {
      if ( sNumber == 1 )
        teamB[mNumber-1]->giveWeapon ( wNumber );
      else
        teamB[mNumber-1]->giveItem ( wNumber );
    }
  }

  for ( int i = 0; i < Size; i++ )
  {
    if ( teamA->item_ == 1 )
      teamA->HP_++;
    if ( teamA->item_ == 2 )
      teamA->AP_ += 2;
  }

  for ( int i = 0; i < Size; i++ )
  {
    if ( teamB->item_ == 1 )
      teamB->HP_++;
    if ( teamB->item_ == 2 )
      teamB->AP_ += 2;
  }
}
