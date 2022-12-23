//
// Created by talro on 21/12/2022.
//
#ifndef WET2_PLAYER_H
#define WET2_PLAYER_H

#include "wet2util.h"
#include "Team.h"

class Player {
private:
    int playerId;
    int gamesPlayed;
    int ability;
    int cards;
    bool goalKeeper;
    Team* originalTeam;
    const permutation_t spirit;

public:
    Player(int playerId, int gamesPlayed, int ability, int cards,  bool goalKeeper, Team* team, const permutation_t& spirit) :
            playerId(playerId), gamesPlayed(gamesPlayed), ability(ability), cards(cards), goalKeeper(goalKeeper),
            originalTeam(team), spirit(spirit) {}
    ~Player(){
        if (originalTeam != nullptr)
            delete originalTeam;
    }
};


#endif //WET2_PLAYER_H
