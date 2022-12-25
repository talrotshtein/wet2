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
    permutation_t* spirit;

public:
    Player(int playerId, int gamesPlayed, int ability, int cards,  bool goalKeeper, Team* team, permutation_t& spirit) :
            playerId(playerId), gamesPlayed(gamesPlayed), ability(ability), cards(cards), goalKeeper(goalKeeper),
            originalTeam(team), spirit(&spirit) {}
    Player(const Player& other){
        playerId = other.playerId;
        gamesPlayed = other.gamesPlayed;
        ability = other.ability;
        cards = other.cards;
        goalKeeper = other.goalKeeper;
        *originalTeam = *other.originalTeam;
        spirit = new permutation_t(*other.spirit);
    }
    ~Player(){
        if (originalTeam != nullptr)
            delete originalTeam;
    }
    int GetPlayerId() {return playerId;}
    int GetGamesPlayed() {return gamesPlayed;}
    void SetGamesPlayed(int val) {this->gamesPlayed = val;}
    Team* GetTeam() {return originalTeam;}
    permutation_t* getSpirit() {return spirit;}
    void multiplySpirit(const permutation_t& permutation){
        *spirit = permutation*(*spirit);
    }
};


#endif //WET2_PLAYER_H
