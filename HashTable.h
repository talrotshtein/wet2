//
// Created by talro on 21/12/2022.
//

#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H

#include <iostream>
#include "Node.h"

template <typename K, typename V>
class HashTable {
public:
    // Constructs a hash table with the given size.
    explicit HashTable(int size) : size(size), count(0) {
        buckets = new Node<K,V>*[size];
        for (int i = 0; i < size; ++i) {
            buckets[i] = NULL;
        }
    }

    // Destructor.
    ~HashTable() {
        // Free the memory allocated for each bucket.
        for (int i = 0; i < size; ++i) {
            Node<K, V>* curr =  buckets[i];
            while (curr != nullptr)
            {
                Node<K, V>* temp = curr->next;
                delete curr;
                curr = temp;
            }
        }
        delete[] buckets;
    }

    // Inserts the given key-value pair into the hash table.
    void insert(const K& key, const V& value) {
        // Check if the hash table needs to be resized.
        if (count >= size / 2) {
            resize(size*2);
        }

        // Calculate the bucket for the key.
        int bucket = key % size;

        // Check if the bucket is empty.
        if (buckets[bucket] == NULL) {
            // If the bucket is empty, create a new entry.
            buckets[bucket] = new Node<K,V>{key, value, NULL};
            count++;
            return;
        }

        // The bucket is not empty, so find the end of the chain.
        Node<K,V>* current = buckets[bucket];
        while (current->next != NULL) {
            current = current->next;
        }

        // Add a new node to the end of the chain.
        current->next = new Node<K,V>{key, value, NULL};
        count++;
    }

    // Returns the value associated with the given key, or null if the key is not
    // present in the hash table.
    const V* get(const K& key) const {
        // Calculate the bucket for the key.
        int bucket = key % size;

        // Search the bucket for the key.
        Node<K,V>* current = buckets[bucket];
        while (current != NULL) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }

        // Key not found.
        return NULL;
    }

    // Remove the element associated with the given key (if it exists in the structure).
    void remove(const K& key){
        //Calculate the bucket for the key.
        int bucket = key % size;

        //Search the bucket for the key.
        Node<K,V>* current = buckets[bucket];

        // Current.key == key - special remove case (first Node in bucket).
        if (current != NULL && current->key == key){
            Node<K,V>* temp = current;
            current = current->next;
            delete temp;
            buckets[bucket] = current;
            return;
        }

        // Else, element is in the middle of bucket.
        while (current->next != NULL){
            if (current->next->key == key){
                Node<K,V>* temp = current->next;
                current->next = current->next->next;
                delete temp;
                return;
            }
            current = current->next;
        }
    }

    void MakeEmpty(void(*deleteFunc)(V)){
        for (int i = 0; i < size; ++i) {
            Node<K,V>* current = buckets[i];
            while (current != NULL) {
                deleteFunc(current->value);
                current = current->next;
            }
        }

        // Update the size and buckets of the hash table.
        delete[] buckets;
    }

private:

    // Resizes the hash table and rehashes all the keys.
    void resize(int new_size) {
        Node<K,V>** new_buckets = new Node<K,V>*[new_size];
        for (int i = 0; i < new_size; ++i) {
            new_buckets[i] = NULL;
        }
        // Rehash all the keys and insert them into the new array of buckets.
        for (int i = 0; i < size; ++i) {
            Node<K,V>* current = buckets[i];
            while (current != NULL) {
                int new_bucket = current->key % new_size;
                new_buckets[new_bucket] = new Node<K,V>{current->key, current->value, new_buckets[new_bucket]};
                Node<K,V>* next = current->next;
                delete current;
                current = next;
            }
        }

        // Update the size and buckets of the hash table.
        delete[] buckets;
        buckets = new_buckets;
        size = new_size;
    }

    // The array of buckets, where each bucket is a linked list of nodes.
    Node<K,V>** buckets;

    // The size of the hash table.
    int size;

    // The number of key-value pairs in the hash table.
    int count;
};

#endif //WET2_HASHTABLE_H
