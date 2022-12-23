//
// Created by yonat on 21/12/2022.
//

#ifndef WET2_NODE_H
#define WET2_NODE_H

template<typename T>
class list {
private:
    int key;
    T* data;
    list* next;
public:
    list() = default;
    list(const T& data, int key) : key(key), data(data), next(nullptr) {}
    ~list(){
        delete this->next;
    }
    list* GetNext() {return this->next;}
    list* GetData() {return this->data;}
    void eraseData(){
        delete data;
    }
    int GetKey() {return this->key;}
    void SetNext(list* other) {this->next = other;}
};


#endif //WET2_NODE_H
