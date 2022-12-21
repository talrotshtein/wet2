//
// Created by talro on 21/12/2022.
//
#ifndef WET2_PLAYER_H
#define WET2_PLAYER_H

#include "wet2util.h"

class Player {
private:
    int playerId;
    int gamesPlayed;
    int ability;
    int cards;
    bool goalKeeper;
    const permutation_t spirit;

public:
    Player(int playerId, int gamesPlayed, int ability, int cards,  bool goalKeeper, const permutation_t& spirit) :
            playerId(playerId), gamesPlayed(gamesPlayed), ability(ability), cards(cards), goalKeeper(goalKeeper), spirit(spirit) {}

};


#endif //WET2_PLAYER_H
