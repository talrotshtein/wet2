//
// Created by talro on 21/12/2022.
//

#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H
#include "list.h"
#include "stdexcept"

static const int INITIAL_SIZE = 100;

template<typename T>
class HashTable{
private:
    list<T>** table;
    int size;
    int count;
public:
    explicit HashTable() : table(new list<T>*[INITIAL_SIZE]), size(INITIAL_SIZE), count(0) {}
    void makeEmpty();
    ~HashTable(){
        delete []table;
    }
    list<T>* find(int key);
    void insert(T obj, int key);
    void remove(int key);
    void rehash(int newSize);
    int hash(int key);
};

template<typename T>
void HashTable<T>::insert(T obj, int key) {
    int index = hash(key);
    list<T>* node = this->table[index];
    while (node->GetNext() != nullptr){
        node = node->GetNext();
    }
    node->SetNext(new list<T>(obj, key));
    count++;
    if (count >= size / 2){
        rehash(size*2);
    }
}

template<typename T>
void HashTable<T>::remove(int key) {
    int index = hash(key);
    list<T>* node = this->table[index];
    if (find(key) == nullptr)
        return;
    if (node->GetKey() == key){
        list<T>* temp = node->GetNext();
        node->SetNext(nullptr);
        delete node;
        this->table[index] = temp;
    }else{
        while (node->GetNext()->GetKey() != key){
            node = node->GetNext();
        }
        list<T>* temp = node->GetNext();
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
    list<T>** newTable = new list<T>*[size*2]();
    try{
        for (int i = 0; i < size; ++i) {
            list<T>* node = this->table[i];
            while (node != nullptr){
                int index = hash(node->GetKey());
                list<T>* newNode = newTable[index];
                while (newNode->GetNext() != nullptr){
                    newNode = newNode->GetNext();
                }
                newNode->SetNext(new list<T>(node->GetData(), node->GetKey()));
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
list<T> *HashTable<T>::find(int key) {
    int index = hash(key);
    list<T>* node = this->table[index];
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
        list<T>* node = this->table[i];
        while (node != nullptr){
            node->GetData()->eraseData();
            node->eraseData();
            node = node->GetNext();
        }
    }
}

#endif //WET2_HASHTABLE_H
