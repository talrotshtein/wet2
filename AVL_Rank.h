//
// Created by yonat on 23/12/2022.
//

#ifndef WET2_AVL_RANK_H
#define WET2_AVL_RANK_H

int max(int a, int b){
    return a > b ? a:b;
}

template <typename T>
class AVL_Rank {
private:
    class Node{
    public:
        int size;
        int height;
        T* data;
        Node* left;
        Node* right;

        Node(T* data) : size(0), height(0), data(data), left(nullptr), right(nullptr){}
    };

    Node* root;

    void makeEmpty(Node* t){
        if (t == nullptr)
            return;
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }

    int Height(Node* t){
        if (t == nullptr){
            return -1;
        }
        return t->height;
    }

    int BalanceFactor(Node* t){
        if (t == nullptr){
            return 0;
        }
        return t->left->height - t->right->height;
    }

    int Size(Node* t){
        if (t == nullptr){
            return 0;
        }
        return t->size;
    }

    Node* findMin(Node* t){
        Node* curr = t;
        while (curr != nullptr){
            curr = curr->left;
        }

        return curr;
    }

    T* find (Node* t, const T& x, bool(*smaller)(const T& x, const T& y)){
        if (t == nullptr){
            return t;
        }

        else if (smaller(x, *t->data)){
            t->left = insert(t->left, x, smaller);
        }else if (smaller(*t->data, x)){
            t->Right = insert(t->right, x, smaller);
        }else{
            return t->data;
        }
    }

    Node* rightRotate(Node* t){
        Node* u = t->left;
        Node* u_right = t->left->right;
        t->left = u_right;
        u->right = t;
        t->height = 1 + max(Height(t->left), Height(t->right));
        t->size = 1 + t->left->size + t->right->size;
        u->height = 1 + max(Height(u->left), Height(u->right));
        u->size = 1 + u->left->size + u->right->size;
        return t;
    }

    Node* leftRotate(Node* t){
        Node* u = t->right;
        Node* u_left = t->right->left;
        t->right = u_left;
        u->left = t;
        t->height = 1 + max(Height(t->right), Height(t->left));
        t->size = 1 + Size(t->left) + Size(t->right);
        u->height = 1 + max(Height(u->right), Height(u->left));
        u->size = 1 + Size(u->left) + Size(u->right);
        return t;
    }

    Node* insert(Node* t, T* data, bool(*smaller)(const T& x, const T& y)){
        if (t == nullptr){
            return new Node{data};
        }

        if (smaller(*data, *t->data)){
            t->left = insert(t->left, data, smaller);
        }else if (!smaller(*data, *t->data)){
            t->Right = insert(t->right, data, smaller);
        }

        t->height = 1 + max(Height(t->left), Height(t->right));
        int balance = BalanceFactor(t);

        //RR
        if (balance < -1 && smaller(*t->right->data, *data)){
            return leftRotate(t);
        }//RL
        else if (balance < -1 && !smaller(*t->right->data, *data)){
            t->right = rightRotate(t->right);
            return leftRotate(t);
        }//LL
        else if (balance > 1 && !smaller(*t->left->data, *data)){
            return rightRotate(t);
        }//LR
        else if (balance > 1 && maller(*t->right->data, *data)){
            t->left = leftRotate(t->left);
            return rightRotate(t);
        }

        return t;
    }

    Node* remove(Node* t, const T& val, bool(*smaller)(const T& x, const T& y)){
        if (t == nullptr){
            return t;
        }

        if (smaller(val, *t->data)){
            t->left = remove(t->left, val, smaller);
        }else if (smaller(*t->data, val)){
            t->Right = remove(t->right, val, smaller);
        }

        else{
            //Left & right child case:
            Node* temp;
            if (t->left && t->right){
                temp = findMin(t->right);
                t->data = temp->data;
                t->right = remove(t->right, *temp->data, smaller);
            }//Left & not right case:
            else if(t->left && !t->right){
                temp = t->left;
                t->data = temp->data;
            }//Right & not left case:
            else if (!t->left && t->right){
                temp = t->right;
                t->data = temp->data;
            }//Not right & not left case:
            else if (!t->left && !t->right){
                temp = t;
                t = nullptr;
            }
            delete temp;
        }
        if (t == nullptr){
            return t;
        }

        t->height = 1 + max(Height(t->left), Height(t->right));
        int balance = BalanceFactor(t);

        //RR
        if (balance < -1 && BalanceFactor(t->right) <= 0){
            return leftRotate(t);
        }//RL
        else if (balance < -1 && BalanceFactor(t->right) > 0){
            t->right = rightRotate(t->right);
            return leftRotate(t);
        }//LL
        else if (balance > 1 && BalanceFactor(t->left) >= 0){
            return rightRotate(t);
        }//LR
        else if (balance > 1 && BalanceFactor(t->left) < 0){
            t->left = leftRotate(t->left);
            return rightRotate(t);
        }

        return t;

    }
public:
    AVL_Rank(){
        root = nullptr;
    }

    ~AVL_Rank(){
        makeEmpty(root);
    }

    void insert(T* x, bool(*smaller)(const T& x, const T& y)){
        root = insert(root, x, smaller);

    }

    void remove(const T& x, bool(*smaller)(const T&, const T&))
    {
        root = remove(root, x, smaller);
    }

    T* find(const T& x, bool(*smaller)(const T&, const T&)){
        return find(root, x, smaller);
    }
};


#endif //WET2_AVL_RANK_H
