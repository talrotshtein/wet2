//
// Created by talro on 21/12/2022.
//

#ifndef WET2_TEAM_H
#define WET2_TEAM_H

#include "wet2util.h"

class Team {
private:
    int teamId;
    int teamStrength;
    int numOfPlayers;
    int gamesPlayed;
    int points;
    int teamAbility;
    int numGoalKeepers;

public:
    Team(int id) : teamId(id), teamStrength(0), numOfPlayers(0), gamesPlayed(0), points(0), teamAbility(0), numGoalKeepers(0){}

};


#endif //WET2_TEAM_H
