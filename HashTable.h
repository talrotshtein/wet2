//
// Created by talro on 21/12/2022.
//

#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H
#include "node.h"
#include "stdexcept"
#include "math.h"

static const int INITIAL_SIZE = 100;
static const int DELETED_NODE_ID = -1;
static const double ALPHA = (sqrt(5)-1)/2;


template<typename T>
class HashTable {
private:
    struct HashNode{
        node<T>* data;
        int key;
    };

    HashNode** table;
    int size;
    int num_active;
    int num_inactive;
    void rehash(int newSize);

public:
    explicit HashTable() : table(new HashNode*[INITIAL_SIZE]), size(INITIAL_SIZE), num_active(0), num_inactive(0) {}
    void makeEmpty();
    ~HashTable(){delete []table;}
    T* find(int key);
    void insert(T* obj, int key);
    void remove(int key);
    int hash(int key);
};

template<typename T>
void HashTable<T>::makeEmpty() {
    for (int i = 0; i < size; ++i) {
        HashNode* temp = table[i];
        temp->data->eraseData();
        delete temp->data;
    }
}

template<typename T>
T *HashTable<T>::find(int key) {
    int index = hash(key);
    HashNode* temp = table[index];
    while (temp != NULL) {
        if (temp->key == key) {
            return temp->data;
        }
        temp = table[index++];
    }
    return NULL;
}

template<typename T>
int HashTable<T>::hash(int key) {
    return floor(size*(ALPHA*key - floor(ALPHA*key)));
}

template<typename T>
void HashTable<T>::insert(T *obj, int key) {
    int index = hash(key);
    HashNode* temp = table[index];
    while (temp != NULL && temp->key != DELETED_NODE_ID) {
        temp = table[index++];
    }
    if (temp->key == DELETED_NODE_ID){
        delete temp;
        num_inactive--;
    }
    table[index] = new HashNode{new node<T>(obj), key};
    num_active++;
    if (num_active + num_inactive >= (size / 2)){
        rehash(size*2);
    }
}

template<typename T>
void HashTable<T>::remove(int key) {
    int index = hash(key);
    HashNode* temp = table[index];
    while (temp != NULL && temp->key != key) {
        temp = table[index++];
    }
    if (temp != NULL){
        temp->key = DELETED_NODE_ID;
        num_active--;
        num_inactive++;
    }
    if (num_active + num_inactive <= size/4){
        rehash(size/2);
    }
}

template<typename T>
void HashTable<T>::rehash(int newSize) {
    int temp = size;
    size = newSize;
    HashNode** newTable;
    try{
        newTable = new HashNode*[newSize];
        for (int i = 0; i < size; ++i) {
            HashNode* temp = table[i];
            if (temp == NULL){
                continue;
            }
            else if (temp->key == DELETED_NODE_ID){
                delete temp;
            }else{
                int index = hash(temp->key);
                while (newTable[index] != NULL){
                    index++;
                }
                newTable[index] = new HashNode*{temp->data, temp->key};
                delete temp;
            }
        }
    }catch(const std::bad_alloc&){
        size = temp;
        delete[] newTable;
    }

    delete[] table;
    table = newTable;
}

/*
private:
    node<T>** table;
    int size;
    int count;
public:
    explicit HashTable() : table(new node<T>*[INITIAL_SIZE]), size(INITIAL_SIZE), count(0) {}
    void makeEmpty();
    ~HashTable(){
        delete []table;
    }
    node<T>* find(int key);
    void insert(T obj, int key);
    void remove(int key);
    void rehash(int newSize);
    int hash(int key);
};

template<typename T>
void HashTable<T>::insert(T obj, int key) {
    int index = hash(key);
    node<T>* node = this->table[index];
    while (node->GetNext() != nullptr){
        node = node->GetNext();
    }
    node->SetNext(new node<T>(obj, key));
    count++;
    if (count >= size / 2){
        rehash(size*2);
    }
}

template<typename T>
void HashTable<T>::remove(int key) {
    int index = hash(key);
    node<T>* node = this->table[index];
    if (find(key) == nullptr)
        return;
    if (node->GetKey() == key){
        node<T>* temp = node->GetNext();
        node->SetNext(nullptr);
        delete node;
        this->table[index] = temp;
    }else{
        while (node->GetNext()->GetKey() != key){
            node = node->GetNext();
        }
        node<T>* temp = node->GetNext();
        node->SetNext(temp->GetNext());
        temp->SetNext(nullptr);
        delete temp;
    }
    count--;
    if (count <= size / 4){
        rehash(size / 2);
    }

}

template<typename T>
void HashTable<T>::rehash(int newSize) {
    int temp = size;
    this->size = newSize;
    node<T>** newTable = new node<T>*[size*2]();
    try{
        for (int i = 0; i < size; ++i) {
            node<T>* node = this->table[i];
            while (node != nullptr){
                int index = hash(node->GetKey());
                node<T>* newNode = newTable[index];
                while (newNode->GetNext() != nullptr){
                    newNode = newNode->GetNext();
                }
                newNode->SetNext(new node<T>(node->GetData(), node->GetKey()));
                node = node->GetNext();
            }
        }
    }catch(const std::bad_alloc& ){
        delete[] newTable;
        this->size = temp;
    }
    delete[] this->table;
    table = newTable;
}


template<typename T>
int HashTable<T>::hash(int key) {
    return key % size;
}

template<typename T>
node<T> *HashTable<T>::find(int key) {
    int index = hash(key);
    node<T>* node = this->table[index];
    while (node != nullptr){
        if (node->GetKey() == key){
            return node;
        }
        node = node->GetNext();
    }
    return nullptr;
}

template<typename T>
void HashTable<T>::makeEmpty() {
    for (int i = 0; i < this->size; ++i) {
        node<T>* node = this->table[i];
        while (node != nullptr){
            node->GetData()->eraseData();
            node->eraseData();
            node = node->GetNext();
        }
    }
}
*/
#endif //WET2_HASHTABLE_H
