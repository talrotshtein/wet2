//
// Created by yonat on 25/12/2022.
//

#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include "Player.h"
#include "HashTable.h"

static const int INITIAL_SIZE = 128;

class UnionFind {
private:
    HashTable<int, Node<int, Player*>*>* players;
    HashTable<int, Node<int, Player*>*>* teams;
public:
    UnionFind() : players(new HashTable<int ,Node<int, Player*>*>(INITIAL_SIZE)),
                  teams(new HashTable<int ,Node<int, Player*>*>(INITIAL_SIZE)){}
    void makeset(Player& player, int teamId);
    Node<int, Player*>* find(int key);
    void unite(int team1, int team2);
};


#endif //WET2_UNIONFIND_H
