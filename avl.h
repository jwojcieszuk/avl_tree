
#pragma once
#include <algorithm>
#include <utility>
#include <iostream>

/* ->difference between heights of left and right subtrees cannot be more than one for all nodes
 *
 * ->height of a node = length of a largest path from it down to a leaf
 *
 * ->height of BST = length of longest path from root down to leaf
 */


template<typename Key, typename Info>
class Dictionary {

private:
    struct node {
        Key _key = 0;
        Info _info = 0;
        int height = 1;
        node *left;
        node *right;

        node() = default;

        node(const Key &k, const Info &i, node *p = nullptr) : _key(k), _info(i),
                                                               left(p), right(p) {}
    };

    node *root = nullptr;

    bool insert(node *&, const Key &, const Info &);

    int height(node *n) { return n ? n->height : 0; };

    node * rotate_left(node *&);

    node * rotate_right(node *&);

    int get_balance(node *n) { return n ? height(n->left) - height(n->right) : 0; }

    int max(const Key &x, const Key &y) { return x > y ? x : y; }

    void destroy_node(node *);

    void print(node *&, int space);

    node *find_min(node *);

    node *find_max(node *);

    bool remove(node *&, const Key &);

    void copy(node *&, node *);
    
    node * search(const Key &) const;

public:

    Dictionary() = default;

    Dictionary(const Dictionary &);

    ~Dictionary();

    void insert(const Key &, const Info &);

    bool remove(const Key &);

    void print();

    Dictionary &operator=(const Dictionary &);

    bool find(const Key&);

    int max();

    int min();

    Info& getInfo(const Key&);

    Key& getKey();
};
template <typename Key, typename Info>
bool Dictionary<Key, Info>::find(const Key& k){
    auto x = search(k);
    if(x){
        return true;
    }
    else {
        return false;
    }
}

template<typename Key, typename Info>
Info& Dictionary<Key, Info>::getInfo(const Key& k) {
    node *tmp = search(k);
    if(tmp){
        return tmp->_info;
    }
    throw std::exception();
}

template <typename Key, typename Info>
auto Dictionary<Key,Info>::find_max(node *rt) -> node*{
    if (rt) {
        node *curr = rt;
        while (curr->right) {
            curr = curr->right;
        }
        return curr;
    }
    return nullptr;
}

template<typename Key, typename Info>
int Dictionary<Key, Info>::max() {
    return find_max(root)->_key;
}

template<typename Key, typename Info>
int Dictionary<Key, Info>::min() {
    return find_min(root)->_key;
}

template<typename Key, typename Info>
auto Dictionary<Key, Info>::search(const Key &k) const -> node * {
    bool found = false;
    if (root) {
        node *curr;
        curr = root;
        while (curr && !found) {
            if (curr->_key == k) {
                found = true;
            } else if (curr->_key > k) {
                curr = curr->left;
            } else {
                curr = curr->right;
        }
            }

    return curr;
    }
    return nullptr;
}

template<typename Key, typename Info>
auto Dictionary<Key, Info>::operator=(const Dictionary &source) -> Dictionary & {
    if (this != &source) {
        if (root) {
            destroy_node(root);
        }
        if (!source.root) {
            root = nullptr;
        } else
            copy(root, source.root);
    }
    return *this;
}

template<typename Key, typename Info>
void Dictionary<Key, Info>::copy(node *&copiedTreeRoot, node *otherTreeRoot) {
    if (otherTreeRoot == nullptr) {
        copiedTreeRoot = nullptr;
    } else {
        copiedTreeRoot = new node(otherTreeRoot->_key, otherTreeRoot->_info);
        copy(copiedTreeRoot->left, otherTreeRoot->left);
        copy(copiedTreeRoot->right, otherTreeRoot->right);
    }
}

template<typename Key, typename Info>
Dictionary<Key, Info>::Dictionary(const Dictionary &source) {
    if (!source.root) {
        root = nullptr;
    } else {
        copy(root, source.root);
    }
}

template<typename Key, typename Info>
void Dictionary<Key, Info>::print(node *&n, int space) {
    if (n) {
        space += 10;
        print(n->right, space);
        std::cout << std::endl;
        for (int i = 10; i < space; i++) {
            std::cout << " ";
        }
        std::cout << n->_key << std::endl;
        print(n->left, space);
    }
}

template<typename Key, typename Info>
auto Dictionary<Key, Info>::find_min(node *rt) -> node * {
    if (rt) {
        node *curr = rt;
        while (curr->left) {
            curr = curr->left;
        }
        return curr;
    }
    return nullptr;
}

template<typename Key, typename Info>
bool Dictionary<Key, Info>::remove(node *&rt, const Key &k) {
    if (rt) {
        if (k < rt->_key) {
            remove(rt->left, k);
        } else if (k > rt->_key) {
            remove(rt->right, k);
        } else {
            if (!rt->left || !rt->right) {
                node *tmp = rt->left ? rt->left : rt->right;
                if (!tmp) {
                    rt = nullptr;
                } else {
                    *rt = *tmp;
                    delete tmp;
                }
            } else {
                node *tmp = find_max(rt->left);
                rt->_key = tmp->_key;
                rt->_info = tmp->_info;
                remove(rt->left, tmp->_key);
            }
        }
        if (rt) {
            rt->height = 1 + max(height(rt->left), height(rt->right));

            if (get_balance(rt) > 1 && get_balance(rt->left) >= 0)
                rotate_right(rt);

            if (get_balance(rt) > 1 && get_balance(rt->left) < 0) {
                rotate_left(rt->left);
                rotate_right(rt);
            }

            if (get_balance(rt) < -1 && get_balance(rt->right) <= 0)
                rotate_left(rt);

            if (get_balance(rt) < -1 && get_balance(rt->right) > 0) {
                rotate_right(rt->right);
                rotate_left(rt);
            }
            return true;

        }
    }
    return false;
}


template<typename Key, typename Info>
bool Dictionary<Key, Info>::insert(node *&rt, const Key &k, const Info &i) {

    if (!rt) {
        node *new_node = new node(k, i);
        rt = new_node;
    } else {
        if (k < rt->_key)
            insert(rt->left, k, i);
        else if (k > rt->_key)
            insert(rt->right, k, i);
        else return false;
    }

    rt->height = 1 + max(height(rt->left),
                         height(rt->right));

    if (get_balance(rt) > 1 && k < rt->left->_key) {
        std::cout << "-----------------BEFORE ROTATION---------------------\n";
        print();
        std::cout << "-----------------AFTER ROTATION---------------------\n";
        rotate_right(rt);
        print();
    }

    if (get_balance(rt) < -1 && k > rt->right->_key) {
        std::cout << "-----------------BEFORE ROTATION---------------------\n";
        print();
        rotate_left(rt);
        std::cout << "-----------------AFTER ROTATION---------------------\n";
        print();
    }

    if (get_balance(rt) > 1 && k > rt->left->_key) {
        std::cout << "-----------------BEFORE ROTATION---------------------\n";
        print();
        rt->left = rotate_left(rt->left);
        rotate_right(rt);
        std::cout << "-----------------AFTER ROTATION---------------------\n";
        print();
    }

    if (get_balance(rt) < -1 && k < rt->right->_key) {
        std::cout << "-----------------BEFORE ROTATION---------------------\n";

        print();
        std::cout << "---------------------\n";
        rt->right = rotate_right(rt->right);
        rotate_left(rt);
        std::cout << "-----------------AFTER ROTATION---------------------\n";
        print();
    }

    return true;
}


template<typename Key, typename Info>
bool Dictionary<Key, Info>::remove(const Key &k) {
    return remove(root, k);
}

template<typename Key, typename Info>
void Dictionary<Key, Info>::print() {
    print(root, 0);
}

template<typename Key, typename Info>
void Dictionary<Key, Info>::destroy_node(node *n) {
    if (n) {
        destroy_node(n->left);
        destroy_node(n->right);
        delete n;
    }
}

template<typename Key, typename Info>
Dictionary<Key, Info>::~Dictionary() {
    destroy_node(root);
}


template<typename Key, typename Info>
auto Dictionary<Key, Info>::rotate_left(node *&pivot) -> node *{

    if (pivot && pivot->right) {
        node *tmp = pivot->right;
        pivot->right = tmp->left;

        tmp->left = pivot;
        pivot->height = max(height(pivot->left),
                            height(pivot->right)) + 1;
        pivot = tmp;
        return pivot;
    }
}


template<typename Key, typename Info>
auto Dictionary<Key, Info>::rotate_right(node *&pivot) -> node*{
    if (pivot && pivot->left) {
        node *tmp = pivot->left;
        pivot->left = tmp->right;

        tmp->right = pivot;
        pivot->height = max(height(pivot->left),
                            height(pivot->right)) + 1;
        pivot = tmp;
        return pivot;
    }
}


template<typename Key, typename Info>
void Dictionary<Key, Info>::insert(const Key &k, const Info &i) {
    insert(root, k, i);
}

