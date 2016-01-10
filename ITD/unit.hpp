#ifndef H_UNIT
#define H_UNIT

#include "basics.hpp"
#include "tile.hpp"

class unit_t
{
  public :
    tile_t * tile_;

    int teamMarker_;

    int HP_;
    int maxAP_;
    int AP_;
    int sight_;

    int weapon_; // 0 - sword, 1 - shotgun, 2 - rifle, 3 - sniper rifle

    int posX_;
    int posY_;
    bool alive_;

    unit_t ( int posX, int posY );
    void setMarker ( int x );
    bool visible ( int x, int y );
    bool reachable ( int x, int y );
    std::string information ( );
    void kill ( );
    ~unit_t ();
};

#endif
