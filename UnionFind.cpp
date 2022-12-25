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
    current->multiplySpirit(*next->getSpirit());
    node->next = node->next->next;
}

void UnionFind::makeset(Player &player, int teamId) {
    Node<int, Player*>* newNode = new Node<int, Player*>{player.GetPlayerId(), &player, NULL};
    players->insert(player.GetPlayerId(), newNode);
    teams->insert(teamId, newNode);
}

Node<int, Player*>* UnionFind::find(int key) {
    Node<int, Player*>* playerNode = players->get(key);
    if (playerNode == NULL){
        return NULL;
    }

    while (playerNode->next != NULL){
        playerNode = playerNode->next;
    }

    minimizePath(playerNode);

    return playerNode;
}

void UnionFind::unite(int team1Id, int team2Id) {
    Node<int, Player*>* team1 = teams->get(team1Id);
    Node<int, Player*>* team2 = teams->get(team2Id);
    if (team1 == NULL || team2 == NULL){
        return;
    }
    Team* team1ptr = team1->value->GetTeam();
    Team* team2ptr = team2->value->GetTeam();
    int team1Size = team1ptr->GetNumOfPlayers();
    int team2Size = team2ptr->GetNumOfPlayers();
    if (team1Size < team2Size){
        team1->next = team2;
        team1->value->multiplySpirit(*team2ptr->GetTeamSpirit());
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
