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
    current->multiplySpirit(next->GetPartialSpirit());
    node->next = node->next->next;
}

Node<int, Player*>* UnionFind::makeset(Player &player, int teamId) {
    Node<int, Player*>* newNode = new Node<int, Player*>{player.GetPlayerId(), &player, NULL};
    players->insert(player.GetPlayerId(), newNode);
    teams->insert(teamId, newNode);
    Team* teamPtr = newNode->value->GetTeam();
    teamPtr->SetTeamSpirit(new permutation_t(player.GetSpirit()));
    teamPtr->SetGamesPlayed(player.GetGamesPlayed());
    teamPtr->SetNumOfPlayers(1);
    if (player.isGoalKeeper())
        teamPtr->SetNumGoalKeepers(1);
    teamPtr->SetAbility(player.GetAbility());
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

    // Get pointers to teams & id's.
    Team* buyerPtr = buyer->value->GetTeam();
    Team* boughtPtr = bought->value->GetTeam();
    int buyerId = buyerPtr->GetId();
    int boughtId = boughtPtr->GetId();
    Team* temp;
    // Simple case ('bought' < 'buyer').
    if (buyerPtr->GetNumOfPlayers() > boughtPtr->GetNumOfPlayers()){
        // Fix the nodes (using the "Crates method" from tutorial).
        bought->next = buyer;
        bought->value->SetGamesPlayed(bought->value->GetGamesPlayed() - buyer->value->GetGamesPlayed());
        bought->value->multiplySpirit(*buyerPtr->GetTeamSpirit()); // Does: buyer*bought (buyer was before).

        // Remove 'bought' root from the array that holds pointers to team roots (it was united).
        teams->remove(boughtId);
        // Delete team 'bought' (it isn't necessary anymore).
        temp = boughtPtr;
        bought->value->SetTeam(NULL);
    }else{ // Hard case ('buyer' < 'bought').
        // Fix the nodes (using the "Crates method" from tutorial).
        buyer->next = bought;
        bought->value->multiplySpirit(*buyerPtr->GetTeamSpirit()); //Does: buyer*bought (buyer was before).
        buyer->value->multiplySpirit(bought->value->GetPartialSpirit().inv()); //Abort the effect of 'bought' root.
        buyer->value->SetGamesPlayed(buyer->value->GetGamesPlayed() - bought->value->GetGamesPlayed());

        // Remove 'buyer' root from the array that holds pointers to team roots (it was united).
        teams->remove(buyerId);

        // Make sure that the root of 'bought' holds a pointer to team 'buyer' (he is the new root).
        temp = boughtPtr;
        bought->value->SetTeam(buyerPtr);
        buyer->value->SetTeam(NULL);
        buyerPtr->SetLeader(bought);
    }
    // Update the values of 'buyer'.
    buyerPtr->multiplyTeamSpirit(*boughtPtr->GetTeamSpirit()); // Does: buyer*bought (buyer was before).
    buyerPtr->SetNumOfPlayers(buyerPtr->GetNumOfPlayers() + boughtPtr->GetNumOfPlayers());
    buyerPtr->SetAbility(buyerPtr->GetAbility() + boughtPtr->GetAbility());
    buyerPtr->SetNumGoalKeepers(buyerPtr->GetNumGoalkeepers() + boughtPtr->GetNumGoalkeepers());
    buyerPtr->SetPoints(buyerPtr->GetPoints() + boughtPtr->GetPoints());
    buyerPtr->UpdateStrength();
    delete temp;
}

void UnionFind::addPlayerToTeam(Player &player, int teamId) {
    Node<int, Player*>* newNode = new Node<int, Player*>{player.GetPlayerId(), &player, NULL};
    players->insert(player.GetPlayerId(), newNode);
    Node<int, Player*>* team = *teams->get(teamId);
    Team* teamPtr = team->value->GetTeam();
    teamPtr->multiplyTeamSpirit(player.GetSpirit());
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
    int gamesPlayed = current->value->GetGamesPlayed();
    if (current->next != NULL)
        gamesPlayed += current->next->value->GetGamesPlayed();
    return gamesPlayed;
}

Node<int, Player *> *UnionFind::get(int key) {
    return *players->get(key);
}

permutation_t UnionFind::getPartialSpirit(int playerId) {
    Node<int, Player*>* current = *players->get(playerId);
    minimizePath(current);
    permutation_t p = current->value->GetPartialSpirit()*current->value->GetSpirit();
    if (current->next != NULL)
        return p = current->next->value->GetPartialSpirit()*p;
    return p;
}




