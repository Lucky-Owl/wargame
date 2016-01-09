#ifndef H_TEAM
#define H_TEAM

#include "basics.hpp"
#include "unit.hpp"

class team_t
{
  public:
    int size_;
    unit_t * * crew_;
    int teamMarker_;

    team_t ( int size );
    void setMarker ( int x );
    ~team_t ( );
};

#endif
