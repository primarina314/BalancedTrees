#include <iostream>
#include <algorithm>
#include <optional>
#include <unistd.h>

template <typename T, typename U>
class AVLNode{

    public :
        T key;
        U value;
        AVLNode<T,U> * left;
        AVLNode<T,U> * right;
        int height;

        AVLNode<T,U>(const T& k, const U& v)
        {
            key = k;
            value = v;
            left = nullptr;
            right = nullptr;
            height = 1;
        }

};

template <typename T, typename U>
class AVLTree {
    public :
        AVLNode<T,U> * root = nullptr;
        ~AVLTree() {
            removeall(root);
        }

        void insert(const T& key, const U& value);
        U search(const T& key);
        bool remove(const T& key);

        //for checking
        void preorder(AVLNode<T,U>*& node){
            if(!node) return;

            std::cout<<node->key<<": "<<node->value<<std::endl;
            preorder(node->left);
            preorder(node->right);
        }
        void inorder(AVLNode<T,U>*& node){
            if(!node) return;

            inorder(node->left);
            std::cout<<node->key<<": "<<node->value<<std::endl;
            inorder(node->right);
        }

    private :
        int getHeight(AVLNode<T,U>*& node);
        int getBalance(AVLNode<T,U>*& node);
        AVLNode<T,U>* rotate_left(AVLNode<T,U>*& node);
        AVLNode<T,U>* rotate_right(AVLNode<T,U>*& node);

        AVLNode<T,U>* insert(AVLNode<T,U>*& node, const T& key, const U& value);
        U search(AVLNode<T,U>*& node, const T& key);
        AVLNode<T,U>* remove(AVLNode<T,U>*& node, const T& key);
        void removeall(AVLNode<T,U>*& node);

        //additional function
        void make_balance(AVLNode<T,U>*& node);
        void get_min(AVLNode<T,U>* node, T& key, U& value);

};

template <typename T, typename U>
int AVLTree<T,U>::getHeight(AVLNode<T,U>*& node){
    if(!node) return 0;
    return node->height;
}

template <typename T, typename U>
int AVLTree<T,U>::getBalance(AVLNode<T,U>*& node){
    if(!node) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

template<typename T, typename U>
void AVLTree<T,U>::insert(const T& key, const U& value) {
    root = insert(root, key, value);
}

template<typename T, typename U>
U AVLTree<T,U>::search(const T& key) {
    return search(root, key);
}

template<typename T, typename U>
bool AVLTree<T,U>::remove(const T& key) {
    if(!search(root,key).length()) return false;
    root = remove(root, key);
    return true;
}

template<typename T, typename U>
AVLNode<T,U>* AVLTree<T,U>::rotate_left(AVLNode<T,U>*& node){
    //TODO
    AVLNode<T,U>* tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    node->height = 1+std::max(getHeight(node->left), getHeight(node->right));
    tmp->height = 1+std::max(getHeight(tmp->right), node->height);
    node = tmp;
    return node;
}

template<typename T, typename U>
AVLNode<T,U>* AVLTree<T,U>::rotate_right(AVLNode<T,U>*& node){
    //TODO
    AVLNode<T,U>* tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    node->height = 1+std::max(getHeight(node->left), getHeight(node->right));
    tmp->height = 1+std::max(getHeight(tmp->left), node->height);
    node = tmp;
    return node;
}

template<typename T, typename U>
AVLNode<T,U>* AVLTree<T,U>::insert(AVLNode<T,U>*& node, const T& key, const U& value) {
    //TODO
    if(node==nullptr)
    {
        root = new AVLNode<T,U>(key, value);
        return root;
    }
    else if(node->right==nullptr && node->key < key)
    {
        AVLNode<T,U>* newNode = new AVLNode<T,U>(key, value);
        node->right = newNode;
        node->height = 1+std::max(getHeight(node->left), getHeight(node->right));
        //height
        return node;
    }
    else if(node->left==nullptr && key < node->key)
    {
        AVLNode<T,U>* newNode = new AVLNode<T,U>(key, value);
        node->left = newNode;
        node->height = 1+std::max(getHeight(node->left), getHeight(node->right));
        return node;
    }
    else if(node->key < key)
    {
        node->right = insert(node->right, key, value);//have to check
        node->right->height = 1+std::max(getHeight(node->right->left), getHeight(node->right->right));
    }
    else if(key < node->key)
    {
        node->left = insert(node->left, key, value);
        node->left->height = 1+std::max(getHeight(node->left->left), getHeight(node->left->right));
    }
    else//replace(same key)
    {
        node->value = value;
        return node;
    }
    node->height = 1+std::max(getHeight(node->left), getHeight(node->right));
    //balancing
    make_balance(node);

    return node;
}

template<typename T, typename U>
U AVLTree<T,U>::search(AVLNode<T,U>*& node, const T& key) {
    //TODO
    //return NULL if there are no such key, return value if there is
    if(node==nullptr) return "";
    if(node->key == key) return node->value;
    if(node->key < key) return search(node->right, key);
    return search(node->left, key);
}

template<typename T, typename U>
void AVLTree<T,U>::get_min(AVLNode<T,U>* node, T& key, U& value)
{
    while(node->left != nullptr) node = node->left;
    key = node->key;
    value = node->value;
}

template<typename T, typename U>
AVLNode<T,U>* AVLTree<T,U>::remove(AVLNode<T,U>*& node, const T& key) {
    //TODO
    if(node==nullptr) return nullptr;
    if(node->key == key)
    {
        AVLNode<T,U>* tmp = node;
        if(node->left==nullptr)//one child(right) or leaf node
        {
            node = node->right;
            delete tmp;
        }
        else if(node->right==nullptr)//one child(left)
        {
            node = node->left;
            delete tmp;
        }
        else//full node
        {
            T _key;
            U _value;
            get_min(node->right, _key, _value);//min of right subtree
            node->key = _key;
            node->value = _value;
            node->right = remove(node->right, _key);
        }
    }
    else if(node->key < key)
    {
        node->right = remove(node->right, key);
        //if(node->right!=nullptr) node->right->height = 1+std::max(getHeight(node->right->left), getHeight(node->right->right));
    }
    else
    {
        node->left = remove(node->left, key);
        //if(node->left!=nullptr) node->left->height = 1+std::max(getHeight(node->left->left), getHeight(node->left->right));
    }
    //balancing
    if(node!=nullptr)
    {
        node->height = 1+std::max(getHeight(node->left), getHeight(node->right));
        make_balance(node);
    }
    
    return node;
}

template<typename T, typename U>
void AVLTree<T,U>::removeall(AVLNode<T,U>*& node) {
    //TODO
    //for destructor
    while(root!=nullptr)
    {
        root = remove(root, root->key);
    }
    //O(n) because accessing to root takes O(1) time;
    delete root;
    root = nullptr;
}

template<typename T, typename U>
void AVLTree<T,U>::make_balance(AVLNode<T,U>*& node) {
    if(std::abs(getHeight(node->left)-getHeight(node->right))>1)
    {
        //right-left
        if(getHeight(node->left)<getHeight(node->right) && getHeight(node->right->left)>getHeight(node->right->right))
        {
            node->right = rotate_right(node->right);
            node = rotate_left(node);
            return;
        }

        //right-right
        else if(getHeight(node->left)<getHeight(node->right) && getHeight(node->right->left)<=getHeight(node->right->right))
        {
            node = rotate_left(node);
            return;
        }

        //left-right
        else if(getHeight(node->left)>getHeight(node->right) && getHeight(node->left->left)<getHeight(node->left->right))
        {
            node->left = rotate_left(node->left);
            node = rotate_right(node);
            return;
        }

        //left-left
        else if(getHeight(node->left)>getHeight(node->right) && getHeight(node->left->left)>=getHeight(node->left->right))
        {
            node = rotate_right(node);
            return;
        }
    }
    return;
}

//valgrind --leak-check=yes ./test
//valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all ./test