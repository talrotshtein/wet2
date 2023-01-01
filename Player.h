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
    permutation_t spirit;
    permutation_t partialSpirit;

public:
    Player(int playerId, int gamesPlayed, int ability, int cards,  bool goalKeeper, Team* team, const permutation_t& spirit) :
            playerId(playerId), gamesPlayed(gamesPlayed), ability(ability), cards(cards), goalKeeper(goalKeeper),
            originalTeam(team), spirit(spirit), partialSpirit(spirit){}
    ~Player(){
        if (originalTeam != nullptr)
            delete originalTeam;

    }
    bool isGoalKeeper() const{return goalKeeper;}
    int GetAbility() const{return ability;}
    int GetPlayerId() {return playerId;}
    int GetGamesPlayed() {return gamesPlayed;}
    int GetCards() {return cards;}
    void SetGamesPlayed(int val) {this->gamesPlayed = val;}
    void SetCards(int val) {this->cards = val;}
    Team* GetTeam() {return originalTeam;}
    const permutation_t& getSpirit() {return spirit;}
    const permutation_t& getPartialSpirit() {return partialSpirit;}
    void multiplySpirit(const permutation_t& permutation){
        partialSpirit = permutation*(partialSpirit);
    }
};


#endif //WET2_PLAYER_H
