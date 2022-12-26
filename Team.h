//
// Created by talro on 21/12/2022.
//

#ifndef WET2_TEAM_H
#define WET2_TEAM_H

#include "wet2util.h"
#include "Node.h"

class Player;

class Team {
private:
    int teamId;
    int teamStrength;
    int numOfPlayers;
    int gamesPlayed;
    int points;
    int teamAbility;
    int numGoalKeepers;
    bool isActive;
    permutation_t* teamSpirit;
    Node<int, Player*>* leader;

public:
    Team(int id) : teamId(id), teamStrength(0), numOfPlayers(0), gamesPlayed(0), points(0), teamAbility(0), numGoalKeepers(0),
    isActive(true), teamSpirit(NULL), leader(nullptr){}
    int GetId() const {return this->teamId;}
    int GetAbility() const {return this->teamAbility;}
    int GetNumOfPlayers() const {return this->numOfPlayers;}
    int GetGamesPlayed() const {return this->gamesPlayed;}
     permutation_t* GetTeamSpirit() const {return this->teamSpirit;}
     void multiplyTeamSpirit(const permutation_t& other) {*teamSpirit = *teamSpirit*other;}
     void SetAbilityi(int val) {this->teamAbility = val;}
    void SetLeader(Node<int, Player*>* other) {leader = other;}
    void SetActive(bool val) {this->isActive = val;}
    void SetGamesPlayed(int val) {this->gamesPlayed = val;}
    void SetNumOfPlayers(int val) {this->numOfPlayers = val;}
    void SetNumGoalKeepers(int val) {this->numGoalKeepers = val;}
};


#endif //WET2_TEAM_H
