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
    current->multiplySpirit(next->getSpirit());
    node->next = node->next->next;
}

Node<int, Player*>* UnionFind::makeset(Player &player, int teamId) {
    Node<int, Player*>* newNode = new Node<int, Player*>{player.GetPlayerId(), &player, NULL};
    players->insert(player.GetPlayerId(), newNode);
    teams->insert(teamId, newNode);
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

void UnionFind::unite(Node<int, Player*>* team1, Node<int, Player*>* team2) {
    if (team1 == NULL || team2 == NULL){
        return;
    }
    Team* team1ptr = team1->value->GetTeam();
    Team* team2ptr = team2->value->GetTeam();
    int team1Size = team1ptr->GetNumOfPlayers();
    int team2Size = team2ptr->GetNumOfPlayers();
    int team1Id = team1ptr->GetId();
    int team2Id = team2ptr->GetId();
    if (team1Size < team2Size){
        team1->next = team2;
        team1->value->multiplySpirit(*team2ptr->GetTeamSpirit());
        team2ptr->multiplyTeamSpirit(team1->value->getSpirit());
        team1ptr->SetGamesPlayed(team1ptr->GetGamesPlayed() - team2ptr->GetGamesPlayed());
        team2ptr->SetNumOfPlayers(team1ptr->GetNumOfPlayers() + team2ptr->GetNumOfPlayers());
        teams->remove(team1Id);
    }else
    {
        team2->next = team1;
        team2->value->multiplySpirit(*team1ptr->GetTeamSpirit());
        team2ptr->SetGamesPlayed(team2ptr->GetGamesPlayed() - team1ptr->GetGamesPlayed());
        team1ptr->SetNumOfPlayers(team1ptr->GetNumOfPlayers() + team2ptr->GetNumOfPlayers());
        teams->remove(team2Id);
    }

}

void UnionFind::addPlayerToTeam(Player &player, int teamId) {
    Node<int, Player*>* newNode = new Node<int, Player*>{player.GetPlayerId(), &player, NULL};
    players->insert(player.GetPlayerId(), newNode);
    Node<int, Player*>* team = *teams->get(teamId);
    Team* teamPtr = team->value->GetTeam();
    teamPtr->multiplyTeamSpirit(player.getSpirit());
    teamPtr->SetNumOfPlayers(teamPtr->GetNumOfPlayers()+1);
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




