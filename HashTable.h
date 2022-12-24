//
// Created by talro on 21/12/2022.
//

#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H
#include "Node.h"
#include "stdexcept"
#include "math.h"

static const int INITIAL_SIZE = 10;

template<typename T>
class HashTable{
private:
    int size;
    int count;
    Node<T>** table;

public:
    HashTable() : size(INITIAL_SIZE), count(0), table(new Node<T>*[INITIAL_SIZE]){
        for (int i = 0; i < INITIAL_SIZE; ++i) {
            table[i] = NULL;
        }
    }
    ~HashTable(){
        delete []table;
    }
    void makeEmpty();
    void insert(T& x, int key);
    void remove(int key);
    Node<T>* find(int x);
    void rehash(int newSize);
    int hashFunc(int x, int m) {return x % m;};
};

template<typename T>
void HashTable<T>::insert(T& x, int key) {
    int index = hashFunc(key, size);
    Node<T>* newNode = new Node<T>(&x, key);
    newNode->SetNext(table[index]);
    table[index] = newNode;
    count++;
    if (count >= size/2){
        rehash(size*2);
    }
}

template<typename T>
void HashTable<T>::remove(int key) {
    int index = hashFunc(key, size);
    Node<T>* t = table[index];
    if (t->GetKey() == key){
        Node<T>* temp = t->GetNext();
        t->SetNext(NULL);
        delete t;
        table[index] = temp;
    }else{
        while (t->GetNext() != NULL && t->GetNext()->GetKey() != key){
            t = t->GetNext();
        }
        Node<T>* temp = t->GetNext();
        t->SetNext(temp->GetNext());
        temp->SetNext(nullptr);
        delete temp;
    }
    count--;
    if (count <= size/4){
        rehash(size/2);
    }

}

template<typename T>
void HashTable<T>::rehash(int newSize) {
    Node<T>** newTable;
    try{
        newTable = new Node<T>*[newSize];
        for (int i = 0; i < size; ++i) {
            Node<T>* temp = table[i];
            while (temp != NULL){
                int index = hashFunc(temp->GetKey(), newSize);
                Node<T>* newNode = new Node<T>(temp->GetData(), temp->GetKey());
                newNode->SetNext(newTable[index]);
                newTable[index] = newNode;
                temp = temp->GetNext();
            }
        }
    }catch(const std::bad_alloc&){
        delete[] newTable;
    }

    delete[] table;
    table = newTable;
    size = newSize;

}

template<typename T>
Node<T> *HashTable<T>::find(int x) {
    int index = hashFunc(x, size);
    Node<T>* temp = table[index];
    while (temp != NULL && temp->GetKey() != x){
        temp = temp->GetNext();
    }
    return temp;
}

template<typename T>
void HashTable<T>::makeEmpty() {
    for (int i = 0; i < size; ++i) {
        Node<T>* node = table[i];
        while (node != NULL){
            delete node->GetData();
            node = node->GetNext();
        }
    }
}




/*
template<typename T>
class HashTable {
private:
    Node<T>** table;
    int size;
    int count;
public:
    explicit HashTable() : table(new Node<T>*[INITIAL_SIZE]), size(INITIAL_SIZE), count(0) {}
    void makeEmpty();
    ~HashTable(){
        delete []table;
    }
    Node<T>* find(int key);
    void insert(T* obj, int key);
    void remove(int key);
    void rehash(int newSize);
    int hash(int key);
};

template<typename T>
void HashTable<T>::insert(T* obj, int key) {
    int index = hash(key);
    Node<T>* node = this->table[index];
    while (node->GetNext() != nullptr){
        node = node->GetNext();
    }
    node->SetNext(new ::Node<T>(obj, key));
    count++;
    if (count >= size / 2){
        rehash(size*2);
    }
}

template<typename T>
void HashTable<T>::remove(int key) {
    int index = hash(key);
    Node<T>* node = this->table[index];
    if (find(key) == nullptr)
        return;
    if (node->GetKey() == key){
        Node<T>* temp = node->GetNext();
        node->SetNext(nullptr);
        delete node;
        this->table[index] = temp;
    }else{
        while (node->GetNext()->GetKey() != key){
            node = node->GetNext();
        }
        Node<T>* temp = node->GetNext();
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
    Node<T>** newTable = new Node<T>*[size * 2]();
    try{
        for (int i = 0; i < size; ++i) {
            Node<T>* node = this->table[i];
            while (node != nullptr){
                int index = hash(node->GetKey());
                Node<T>* newNode = newTable[index];
                while (newNode->GetNext() != nullptr){
                    newNode = newNode->GetNext();
                }
                newNode->SetNext(new Node<T>(node->GetData(), node->GetKey()));
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
Node<T> *HashTable<T>::find(int key) {
    int index = hash(key);
    Node<T>* node = this->table[index];
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
        Node<T>* node = this->table[i];
        while (node != nullptr){
            node->GetData()->eraseData();
            node->eraseData();
            node = node->GetNext();
        }
    }
}
*/

#endif //WET2_HASHTABLE_H
