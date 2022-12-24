//
// Created by yonat on 21/12/2022.
//

#ifndef WET2_NODE_H
#define WET2_NODE_H

template<typename T>
class Node {
private:
    T* data;
    int key;
    Node* next;
public:
    Node(T* data, int key) : data(data), key(key), next(nullptr){}
    Node(const Node& other){
        *data = *other.data;
        key = other.key;
        next = other.next;
    }
    ~Node(){
        delete this->next;
    }
    Node* GetNext() {return this->next;}
    T* GetData() {return this->data;}
    int GetKey() {return this->key;}
    void eraseData(){
        delete data;
    }
    void SetNext(Node* other) { this->next = other;}
};


#endif //WET2_NODE_H
