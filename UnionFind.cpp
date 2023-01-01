//
// Created by yonat on 25/12/2022.
//

#include "UnionFind.h"

void minimizePath(Node<int, Player*>* node){
    if (node->next == NULL || node->next->next == NULL){
        return;
    }
    minimizePath(node->next);
    Player* current = node->value;
    Player* next = node->next->value;
    current->SetGamesPlayed(current->GetGamesPlayed() + next->GetGamesPlayed());
    current->multiplySpirit(next->getPartialSpirit()*next->getSpirit().inv());
    node->next = node->next->next;
}

Node<int, Player*>* UnionFind::makeset(Player &player, int teamId) {
    Node<int, Player*>* newNode = new Node<int, Player*>{player.GetPlayerId(), &player, NULL};
    players->insert(player.GetPlayerId(), newNode);
    teams->insert(teamId, newNode);
    Node<int, Player*>* team = *teams->get(teamId);
    Team* teamPtr = team->value->GetTeam();
    teamPtr->SetSpirit(new permutation_t(player.getSpirit()));
    teamPtr->SetGamesPlayed(player.GetGamesPlayed());
    teamPtr->SetNumOfPlayers(teamPtr->GetNumOfPlayers()+1);
    if (player.isGoalKeeper())
        teamPtr->SetNumGoalKeepers(teamPtr->GetNumGoalkeepers()+1);
    teamPtr->SetAbility(teamPtr->GetAbility() + player.GetAbility());
    teamPtr->UpdateStrength();
    return newNode;
}


Node<int, Player*>* UnionFind::find(int key) {
    Node<int, Player*>* playerNode = *players->get(key);
    if (playerNode == NULL){
        return NULL;
    }

    while (playerNode->next != NULL){
        playerNode = playerNode->next;
    }

    minimizePath(playerNode);

    return playerNode;
}

void UnionFind::unite(Node<int, Player*>* buyer, Node<int, Player*>* bought) {
    if (buyer == NULL || bought == NULL){
        return;
    }
    Team* buyerPtr = buyer->value->GetTeam();
    Team* boughtPtr = bought->value->GetTeam();
    int buyerSize = buyerPtr->GetNumOfPlayers();
    int boughtSize = boughtPtr->GetNumOfPlayers();
    int buyerId = buyerPtr->GetId();
    int boughtId = boughtPtr->GetId();
    if (buyerSize < boughtSize){
        buyer->next = bought;
        buyer->value->multiplySpirit(*boughtPtr->GetTeamSpirit());
        boughtPtr->multiplyTeamSpirit(buyer->value->getSpirit());
        buyerPtr->SetGamesPlayed(buyerPtr->GetGamesPlayed() - boughtPtr->GetGamesPlayed());
        boughtPtr->SetNumOfPlayers(buyerPtr->GetNumOfPlayers() + boughtPtr->GetNumOfPlayers());
        boughtPtr->SetAbility(buyerPtr->GetAbility() + boughtPtr->GetAbility());
        boughtPtr->SetNumGoalKeepers(buyerPtr->GetNumGoalkeepers() + boughtPtr->GetNumGoalkeepers());
        boughtPtr->SetPoints(buyerPtr->GetPoints() + boughtPtr->GetPoints());
        boughtPtr->UpdateStrength();
        teams->remove(buyerId);
    }else
    {
        bought->next = buyer;
        bought->value->multiplySpirit(*buyerPtr->GetTeamSpirit());
        buyerPtr->multiplyTeamSpirit(bought->value->getSpirit());
        boughtPtr->SetGamesPlayed(boughtPtr->GetGamesPlayed() - buyerPtr->GetGamesPlayed());
        buyerPtr->SetNumOfPlayers(buyerPtr->GetNumOfPlayers() + boughtPtr->GetNumOfPlayers());
        buyerPtr->SetAbility(buyerPtr->GetAbility() + boughtPtr->GetAbility());
        buyerPtr->SetNumGoalKeepers(buyerPtr->GetNumGoalkeepers() + boughtPtr->GetNumGoalkeepers());
        buyerPtr->SetPoints(buyerPtr->GetPoints() + boughtPtr->GetPoints());
        buyerPtr->UpdateStrength();
        teams->remove(boughtId);
    }

}

void UnionFind::addPlayerToTeam(Player &player, int teamId) {
    Node<int, Player*>* newNode = new Node<int, Player*>{player.GetPlayerId(), &player, NULL};
    players->insert(player.GetPlayerId(), newNode);
    Node<int, Player*>* team = *teams->get(teamId);
    Team* teamPtr = team->value->GetTeam();
    teamPtr->multiplyTeamSpirit(player.getSpirit());
    teamPtr->SetNumOfPlayers(teamPtr->GetNumOfPlayers()+1);
    if (player.isGoalKeeper())
        teamPtr->SetNumGoalKeepers(teamPtr->GetNumGoalkeepers()+1);
    teamPtr->SetAbility(teamPtr->GetAbility() + player.GetAbility());
    teamPtr->UpdateStrength();
    player.SetGamesPlayed(player.GetGamesPlayed() - teamPtr->GetGamesPlayed());
    player.multiplySpirit(*teamPtr->GetTeamSpirit());
    newNode->next = team;
}

UnionFind::~UnionFind() {
    delete players;
    delete teams;
}

void UnionFind::MakeEmpty(void (*deleteFunc)(Node<int, Player *> *)) {
    this->players->MakeEmpty(deleteFunc);
}

bool UnionFind::DoesPlayerExist(int id) {
    return this->players->get(id) != nullptr;
}

int UnionFind::NumPlayedGamesForPlayer(int playerId) {
    Node<int, Player*>* current = *players->get(playerId);
    minimizePath(current);
    return current->value->GetGamesPlayed();
}

Node<int, Player *> *UnionFind::get(int key) {
    return *players->get(key);
}

const permutation_t &UnionFind::getPartialSpirit(int playerId) {
    Node<int, Player*>* player = *players->get(playerId);
    minimizePath(player);
    return player->value->getPartialSpirit();
}




