#ifndef BINARY_SEARCH_TREE_H_
#define BINARY_SEARCH_TREE_H_

#include <iostream>
#include <exception>

namespace tree {

struct UnderflowException : public std::exception {
    const char*
    what() const noexcept override {
        return "UnderflowException";
    }
};

template <typename Comparable>
class BinarySearchTree {
  public:
    BinarySearchTree() : root_(nullptr)
    {}

    BinarySearchTree(const BinarySearchTree &other)
    {
        root_ = clone(other.root_);
    }

    BinarySearchTree(BinarySearchTree &&other) : root_(other.root_)
    {
        other.root_ = nullptr;
    }

    ~BinarySearchTree() {
        if (root_) {
            delete root_;
            root_ = nullptr;
        }
    }

    const Comparable &findMin() const;
    const Comparable &findMax() const;
    bool contains(const Comparable &) const;
    bool isEmpty() const;
    void printTree(std::ostream &out = std::cout) const {
        if (!root_) {
            return;
        }

        printTree(root_, out);
    }

    void makeEmpty() {
        if (root_) {
            makeEmpty(root_);
        }
    }

    void insert(const Comparable &);
    void insert(Comparable &&);
    void remove(const Comparable &);

    BinarySearchTree &operator=(const BinarySearchTree &other) {
        if (this == &other) {
            return *this;
        }

        if (root_) {
            delete root_;
        }

        root_ = clone(other.root_);

        return *this;
    }

    BinarySearchTree &operator=(BinarySearchTree &&other) {
        auto temp = root_;
        root_ = other.root_;
        other.root_ = temp;

        return *this;
    }

  private:
    struct BinaryNode {
        Comparable element_;
        BinaryNode *left_;
        BinaryNode *right_;

        BinaryNode(const Comparable &e, BinaryNode *l, BinaryNode *r)
            : element_(e), left_(l), right_(r) {}

        BinaryNode(Comparable &&e, BinaryNode *l, BinaryNode *r)
            : element_(std::move(e)), left_(l), right_(r) {}

        ~BinaryNode() {
            // if (left_) {
            //     delete left_;
            //     left_ = nullptr;
            // }

            // if (right_) {
            //     delete right_;
            //     right_ = nullptr;
            // }
        }
    };

    BinaryNode *root_;

    void insert(const Comparable &, BinaryNode* &);
    void insert(Comparable &&, BinaryNode* &);
    void remove(const Comparable &, BinaryNode* &);
    BinaryNode *findMin(BinaryNode *) const;
    BinaryNode *findMax(BinaryNode *) const;
    bool contains(const Comparable &, BinaryNode *) const;
    void makeEmpty(BinaryNode* &);
    void printTree(BinaryNode *, std::ostream &) const;
    BinaryNode *clone(BinaryNode *) const;
};

template <typename Comparable>
const Comparable &BinarySearchTree<Comparable>::findMin() const {
    if (!root_) {
        throw UnderflowException();
    }

    return findMin(root_)->element_;
}

template <typename Comparable>
typename BinarySearchTree<Comparable>::BinaryNode *BinarySearchTree<Comparable>::findMin(BinaryNode *node) const {
    while (node->left_) {
        node = node->left_;
    }

    return node;
}

template <typename Comparable>
const Comparable &BinarySearchTree<Comparable>::findMax() const {
    if (!root_) {
        throw UnderflowException();
    }

    return findMax(root_)->element_;
}

template <typename Comparable>
typename BinarySearchTree<Comparable>::BinaryNode *BinarySearchTree<Comparable>::findMax(BinaryNode *node) const {
    while (node->right_) {
        node = node->right_;
    }

    return node;
}

template <typename Comparable>
bool BinarySearchTree<Comparable>::contains(const Comparable &e) const {
    if (!root_) {
        return false;
    }

    return contains(e, root_);
}

template <typename Comparable>
bool BinarySearchTree<Comparable>::contains(const Comparable &e, BinaryNode *node) const {
    if (!node) {
        return false;
    }

    if (e < node->element_) {
        return contains(e, node->left_);
    } else if (node->element_ < e) {
        return contains(e, node->right_);
    } else {
        return true;
    }
}

template <typename Comparable>
bool BinarySearchTree<Comparable>::isEmpty() const {
    return !root_;
}

template <typename Comparable>
void BinarySearchTree<Comparable>::insert(const Comparable &e) {
    return insert(e, root_);
}

template <typename Comparable>
void BinarySearchTree<Comparable>::insert(const Comparable &e, BinaryNode* &node) {
    if (!node) {
        node = new BinaryNode(e, nullptr, nullptr);
        return;
    }

    if (e < node->element_) {
        insert(e, node->left_);
    } else if (node->element_ < e) {
        insert(e, node->right_);
    } else {
        std::cout << "already in tree" << std::endl;
    }
}

template <typename Comparable>
void BinarySearchTree<Comparable>::insert(Comparable &&e) {
    return insert(std::move(e), root_);
}

template <typename Comparable>
void BinarySearchTree<Comparable>::insert(Comparable &&e, BinaryNode* &node) {
    if (!node) {
        node = new BinaryNode(std::move(e), nullptr, nullptr);
        return;
    }

    if (e < node->element_) {
        insert(std::move(e), node->left_);
    } else if (node->element_ < e) {
        insert(std::move(e), node->right_);
    } else {
        std::cout << "already in tree" << std::endl;
    }
}

template <typename Comparable>
void BinarySearchTree<Comparable>::remove(const Comparable &e) {
    remove(e, root_);
}

template <typename Comparable>
void BinarySearchTree<Comparable>::remove(const Comparable &e, BinaryNode* &node) {
    if (!node) {
        return;
    }

    if (e < node->element_) {
        remove(e, node->left_);
    } else if (node->element_ < e) {
        remove(e, node->right_);
    } else {
        if (node->left_ && node->right_) {
            node->element_ = findMin(node->right_)->element_;
            remove(node->element_, node->right_);
        } else {
            auto old = node;
            node = node->left_ ? node->left_ : node->right_;
            delete old;
        }
    }
}

template <typename Comparable>
void BinarySearchTree<Comparable>::printTree(BinaryNode *node, std::ostream &out) const {
    if (node) {
        printTree(node->left_, out);
        out << node->element_ << std::endl;
        printTree(node->right_, out);
    }
}

template <typename Comparable>
typename BinarySearchTree<Comparable>::BinaryNode *BinarySearchTree<Comparable>::clone(BinaryNode *node) const {
    if (!node) {
        return nullptr;
    }

    auto left = clone(node->left_);
    auto right = clone(node->right_);
    return new BinaryNode(node->element_, left, right);
}

void BinarySearchTree<Comparable>::makeEmpty(BinaryNode* &node) {


    
    if (node->left_) {
        makeEmpty(node->left_);
    }

    if (node->right_) {
        makeEmpty(node->right_);
    }

    delete node;
    node = nullptr;
}

}

#endif // BINARY_SEARCH_TREE_H_