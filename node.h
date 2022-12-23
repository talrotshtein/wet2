//
// Created by yonat on 21/12/2022.
//

#ifndef WET2_NODE_H
#define WET2_NODE_H

template<typename T>
class node {
private:
    T* data;
    node* next;
public:
    node() = default;
    node(T* data, int key) : data(data), next(nullptr) {}
    ~node(){
        delete this->next;
    }
    node* GetNext() {return this->next;}
    node* GetData() {return this->data;}
    void eraseData(){
        delete data;
    }
    void SetNext(node* other) { this->next = other;}
};


#endif //WET2_NODE_H
