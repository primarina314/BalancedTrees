#include <iostream>
#include <algorithm>
#include <optional>
#include <unistd.h>

template <typename T, typename U>
class RBNode{
    
    public :
        T key;
        U value;
	    RBNode<T,U> * parent;
        RBNode<T,U> * left;
        RBNode<T,U> * right;
        int color; // 1 -> red, 0 -> black

        RBNode<T,U>(const T& k, const U& v)
        {
            key = k;
            value = v;
            left = nullptr;
            right = nullptr;
            parent = nullptr;
        }
        
};

template <typename T, typename U>
class RBTree {
    public : 
        RBNode<T,U> * root = nullptr;
        ~RBTree() {
            removeall(root);
        }

        void insert(const T& key, const U& value);
        U search(const T& key);
        bool remove(const T& key);

        //for checking
        void preorder(RBNode<T,U>*& node){
            if(!node) return;

            std::cout<<node->key<<": "<<node->value<<std::endl;
            preorder(node->left);
            preorder(node->right);
        }
        void inorder(RBNode<T,U>*& node){
            if(!node) return;

            inorder(node->left);
            std::cout<<node->key<<": "<<node->value<<std::endl;
            inorder(node->right);
        }

    private :
        RBNode<T,U>* rotate_left(RBNode<T,U>*& node);
        RBNode<T,U>* rotate_right(RBNode<T,U>*& node);

        RBNode<T,U>* insert(RBNode<T,U>*& node, const T& key, const U& value);
        U search(RBNode<T,U>*& node, const T& key);
        RBNode<T,U>* remove(RBNode<T,U>*& node, const T& key);
        void removeall(RBNode<T,U>*& node);

        //additinal function
        void fix_insertion(RBNode<T,U>*& node);
        void fix_removal(RBNode<T,U>* node);
        void get_min(RBNode<T,U>* node, T& key, U& value);//not lvalue ref, not const, not const
        void get_max(RBNode<T,U>* node, T& key, U& value);
        
        int get_color(RBNode<T,U>*& node);

};

template<typename T, typename U>
void RBTree<T,U>::insert(const T& key, const U& value) {
    root = insert(root, key, value);
}

template<typename T, typename U>
U RBTree<T,U>::search(const T& key) {
    return search(root, key);
}

template<typename T, typename U>
bool RBTree<T,U>::remove(const T& key) {
    if(!search(root,key).length()) return false;
    root = remove(root, key);
    return true;
}

template<typename T, typename U>
RBNode<T,U>* RBTree<T,U>::rotate_left(RBNode<T,U>*& node)
{
    RBNode<T,U>* tmp = node->right;
    node->right = tmp->left;
    if(node->right) node->right->parent = node;

    tmp->parent = node->parent;
    if(node->parent == nullptr) root = tmp;
    else if(node == node->parent->left) node->parent->left = tmp;
    else node->parent->right = tmp;

    tmp->left = node;
    node->parent = tmp;
    return node;
}

template<typename T, typename U>
RBNode<T,U>* RBTree<T,U>::rotate_right(RBNode<T,U>*& node)
{
    RBNode<T,U>* tmp = node->left;
    node->left = tmp->right;
    if(node->left) node->left->parent = node;

    tmp->parent = node->parent;
    if(node->parent == nullptr) root = tmp;
    else if(node == node->parent->left) node->parent->left = tmp;
    else node->parent->right = tmp;

    tmp->right = node;
    node->parent = tmp;
    return node;
}

template<typename T, typename U>
RBNode<T,U>* RBTree<T,U>::insert(RBNode<T,U>*& node, const T& key, const U& value) {
    //TODO
    if(node==nullptr)
    {
        node = new RBNode<T,U>(key, value);
        node->color = (root==node)?0:1;
        return node;
    }
    if(key < node->key)
    {
        node->left = insert(node->left, key, value);
        node->left->parent = node;
    }
    else if(node->key < key)
    {
        node->right = insert(node->right, key, value);
        node->right->parent = node;
    }
    else//replace
    {
        node->key = key;
        node->value = value;
    }
    if(node==root)
    {
        RBNode<T,U>* tmp = root;
        while(tmp->key != key)
        {
            if(tmp->key < key) tmp = tmp->right;
            else tmp = tmp->left;
        }
        fix_insertion(tmp);
    }
    return node;
}

template<typename T, typename U>
U RBTree<T,U>::search(RBNode<T,U>*& node, const T& key) {
    //TODO
    //return NULL if there are no such key, return value if there is
    if(node==nullptr) return "";
    if(node->key == key) return node->value;
    if(node->key < key) return search(node->right, key);
    return search(node->left, key);
}

template<typename T, typename U>
void RBTree<T,U>::get_min(RBNode<T,U>* node, T& key, U& value)
{
    while(node->left != nullptr) node = node->left;
    key = node->key;
    value = node->value;
}

template<typename T, typename U>
void RBTree<T,U>::get_max(RBNode<T,U>* node, T& key, U& value)
{
    while(node->right != nullptr) node = node->right;
    key = node->key;
    value = node->value;
}

template<typename T, typename U>
RBNode<T,U>* RBTree<T,U>::remove(RBNode<T,U>*& node, const T& key) {
    //TODO
    if(node == nullptr) return nullptr;
    
    if(node->key == key)
    {
        RBNode<T,U>* tmp = node;
        if(node->color==1 && node->left==nullptr && node->right==nullptr)//red leaf node
        {
            node = nullptr;
            delete tmp;
        }
        else if(node->color==0 && node->left==nullptr && node->right!=nullptr)//black node with one right child(red).
        {
            //replace : right child(red) -> extra black
            RBNode<T,U>* p = node->parent;
            node = node->right;
            node->parent = p;
            node->color = 0;//red and black -> black
            delete tmp;
        }
        else if(node->color==0 && node->left!=nullptr && node->right==nullptr)//black node with one left child(red). red cannot have one child(only leaf or full)
        {
            //replace : left child(red) -> extra black
            RBNode<T,U>* p = node->parent;
            node = node->left;
            node->parent = p;
            node->color = 0;//red and black -> black
            delete tmp;
        }
        else if(node->color==0 && node->left==nullptr && node->right==nullptr)//black leaf node
        {
            //replace : NIL -> extra black.
            fix_removal(node);

            node = nullptr;
            delete tmp;
            //doubly black
        }
        else//full node
        {
            T _key;
            U _value;
            get_min(node->right, _key, _value);//min of right subtree
            // get_max(node->left, _key, _value);//max of left subtree
            node->key = _key;
            node->value = _value;
            node->right = remove(node->right, _key);//min of right subtree
            // node->left = remove(node->left, _key);//max of left subtree
        }
    }
    else if(node->key < key) node->right = remove(node->right, key);
    else node->left = remove(node->left, key);
    
    return node;
}

template<typename T, typename U>
void RBTree<T,U>::removeall(RBNode<T,U>*& node) {
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
int RBTree<T,U>::get_color(RBNode<T,U>*& node)
{
    if(!node) return 0;
    return node->color;
}

template<typename T, typename U>
void RBTree<T,U>::fix_insertion(RBNode<T,U>*& node)
{
    //fix tree from node to root i.e. upward
    RBNode<T,U>* father = nullptr;
    RBNode<T,U>* grandfather = nullptr;
    
    while(node!=root && node->color==1 && node->parent->color==1)
    {
        father = node->parent;
        grandfather = node->parent->parent;

        if(father == grandfather->left)
        {
            RBNode<T,U>* uncle = grandfather->right;
            if(get_color(uncle)==1)//recoloring
            {
                grandfather->color = 1;
                father->color = 0;
                uncle->color = 0;
                node = grandfather;
            }
            else//restructing
            {
                if(node == father->right)
                {
                    rotate_left(father);
                    node = father;
                    father = node->parent;
                }
                rotate_right(grandfather);
                std::swap(father->color, grandfather->color);
                node = father;
            }
        }
        else
        {
            RBNode<T,U>* uncle = grandfather->left;
            if(get_color(uncle)==1)//recoloring
            {
                grandfather->color = 1;
                father->color = 0;
                uncle->color = 0;
                node = grandfather;
            }
            else//restructing
            {
                if(node == father->left)
                {
                    rotate_right(father);
                    node = father;
                    father = node->parent;
                }
                rotate_left(grandfather);
                std::swap(father->color, grandfather->color);
                node = father;
            }
        }
    }
    root->color = 0;
}
//fix_insertion : node(pointer) is not changed even after rotation;

template<typename T, typename U>
void RBTree<T,U>::fix_removal(RBNode<T,U>* doubly_black)
{
    RBNode<T,U>* sibling;
    while (doubly_black != root && doubly_black->color == 0)//doubly_black->color==0 means black-black, and doubly_black->color==1 means red-black;
    {
        if (doubly_black == doubly_black->parent->left)
        {
            sibling = doubly_black->parent->right;
            if (get_color(sibling) == 1)
            {
                //case4
                sibling->color = 0;
                doubly_black->parent->color = 1;
                rotate_left(doubly_black->parent);
                sibling = doubly_black->parent->right;
            }

            if (get_color(sibling->left) == 0 && get_color(sibling->right) == 0)
            {
                //case3
                sibling->color = 1;
                doubly_black = doubly_black->parent;//if doubly_black->parent is red-black -> while statement breaks, and it become black at the last line of this func;
            }
            else
            {
                if (get_color(sibling->right) == 0)
                {
                    //case2
                    sibling->left->color = 0;
                    sibling->color = 1;
                    rotate_right(sibling);
                    sibling = doubly_black->parent->right;
                } 

                //case1
                sibling->color = doubly_black->parent->color;
                doubly_black->parent->color = 0;
                sibling->right->color = 0;
                rotate_left(doubly_black->parent);
                doubly_black = root;
            }
        }
        else
        {
            sibling = doubly_black->parent->left;
            if(get_color(sibling) == 1)
            {
                //case4
                sibling->color = 0;
                doubly_black->parent->color = 1;
                rotate_right(doubly_black->parent);
                sibling = doubly_black->parent->left;
            }

            if(get_color(sibling->left) == 0 && get_color(sibling->right) == 0)
            {
                //case3
                sibling->color = 1;
                doubly_black = doubly_black->parent;
            }
            else
            {
                if (get_color(sibling->left)== 0)
                {
                    //case2
                    sibling->right->color = 0;
                    sibling->color = 1;
                    rotate_left(sibling);
                    sibling = doubly_black->parent->left;
                } 

                //case1
                sibling->color = doubly_black->parent->color;
                doubly_black->parent->color = 0;
                sibling->left->color = 0;
                rotate_right(doubly_black->parent);
                doubly_black = root;
            }
        }
    }
    doubly_black->color = 0;
}

//valgrind --leak-check=yes ./test
//valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all ./test