#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include <type_traits>
#include <exception>
#include <iostream>
#include <cassert>

namespace tree {

struct NullTree : public std::exception {
    const char *what() const noexcept override {
        return "NullTree";
    }
};

template <typename T>
using enable_if_t = typename std::enable_if<T::value>::type;

enum HelperInfo : unsigned char {
    LEFT_HIGHER,
    RIGHT_HIGHER,
    SAME_HEIGHT,

    HEIGHT_INCREASE,
    HEIGHT_DECREASE,
    HEIGHT_NO_CHANGE,
};

template <typename Comparable, int ALLOWED_IMBALANCE = 1>
class AvlTree {
  public:
    AvlTree() : root_(nullptr) {}

    AvlTree(const AvlTree &other)
    : root_(nullptr)
    {
        if (other.root_) {
            root_ = clone(other.root_);
        }
    }

    AvlTree(AvlTree &&other) : root_(other.root_) {
        other.root_ = nullptr;
    }

    ~AvlTree() {
        makeEmpty();
    }

    const Comparable &findMax() const {
        if (!root_) {
            throw NullTree();
        }

        return findMax(root_);
    }

    const Comparable &findMin() const {
        if (!root_) {
            throw NullTree();
        }

        return findMin(root_);
    }

    bool contains(const Comparable &e) const noexcept {
        auto node = root_;
        while (node) {
            if (node->element_ < e) {
                node = node->right_;
            } else if (e < node->element_) {
                node = node->left_;
            } else {
                return true;
            }
        }

        return false;
    }

    bool isEmpty() const noexcept {
        return root_ == nullptr;
    }

    void printTree(std::ostream &os = std::cout) const noexcept {
        if (root_) {
            printTree(os, root_);
        }
    }

    void makeEmpty() {
        if (!root_) {
            return;
        }

        makeEmpty(root_);
        root_ = nullptr;
    }

    template <typename T, typename = enable_if_t<std::is_convertible<T, Comparable>>>
    void insert(T &&e) {
        insert(root_, std::forward<T>(e));
    }

    void remove(const Comparable &e) {
        remove(root_, e);
    }

    AvlTree &operator=(const AvlTree &other) {
        if (this == &other) {
            return *this;
        }

        if (root_) {
            makeEmpty(root_);
        }

        if (other.root_) {
            root_ = clone(other.root_);
        } else {
            root_ = nullptr;
        }

        return *this;
    }

    AvlTree &operator==(AvlTree &&other) {
        auto temp = root_;
        root_ = other.root_;
        other.root_ = temp;

        return *this;
    }

  private:
    struct AvlNode {
        Comparable element_;
        AvlNode *left_;
        AvlNode *right_;
        int height_;
        HelperInfo balance_;

        template <typename T, typename = enable_if_t<std::is_convertible<T, Comparable>>>
        AvlNode(T &&e, AvlNode *l = nullptr, AvlNode *r = nullptr)
        : element_(std::forward<T>(e))
        , left_(l)
        , right_(r)
        , height_(0)
        , balance_(SAME_HEIGHT)
        {}
    };

    AvlNode *root_;

    const Comparable &findMax(const AvlNode *node) const noexcept {
        while (node->right_) {
            node = node->right_;
        }

        return node->element_;
    }

    const Comparable &findMin(const AvlNode *node) const noexcept {
        while (node->left_) {
            node = node->left_;
        }

        return node->element_;
    }

    void printTree(std::ostream &os, const AvlNode *node) const noexcept {
        if (node->left_) {
            printTree(os, node->left_);
        }

        os << node->element_ << std::endl;

        if (node->right_) {
            printTree(os, node->right_);
        }
    }

    void makeEmpty(AvlNode *node) {
        if (node->left_) {
            makeEmpty(node->left_);
        }

        if (node->right_) {
            makeEmpty(node->right_);
        }

        delete node;
    }

    template <typename T, typename = enable_if_t<std::is_convertible<T, Comparable>>>
    HelperInfo insert(AvlNode *&node, T &&e) {
        if (!node) {
            node = new AvlNode(std::forward<T>(e));
            return HEIGHT_INCREASE;
        }

        if (node->element_ < e) {
            if (HEIGHT_INCREASE == insert(node->right_, std::forward<T>(e))) {
                if (RIGHT_HIGHER == node->balance_) {
                    // maybe unbalance
                    // if (node->)
                    auto delta_height = node->right_->height_ - (node->left_ ? node->left_->height_ : -1);
                    if (delta_height > ALLOWED_IMBALANCE) {
                        balance_right(node);
                    } else {
                        ++node->height_;
                        return HEIGHT_INCREASE;
                    }
                } else if (SAME_HEIGHT == node->balance_) {
                    ++node->height_;
                    node->balance_ = RIGHT_HIGHER;
                    return HEIGHT_INCREASE;
                } else {
                    if (node->right_->height_ + 1 == node->left_->height_) {
                        node->balance_ = SAME_HEIGHT;
                    }
                }
            }
        } else if (e < node->element_) {
            if (HEIGHT_INCREASE == insert(node->left_, std::forward<T>(e))) {
                if (LEFT_HIGHER == node->balance_) {
                    // maybe unbalance
                    auto delta_height = node->left_->height_ - (node->right_ ? node->right_->height_ : -1);
                    if (delta_height > ALLOWED_IMBALANCE) {
                        balance_left(node);
                    } else {
                        ++node->height_;
                        return HEIGHT_INCREASE;
                    }
                } else if (SAME_HEIGHT == node->balance_) {
                    ++node->height_;
                    node->balance_ = LEFT_HIGHER;
                    return HEIGHT_INCREASE;
                } else {
                    if (node->left_->height_ + 1 == node->right_->height_) {
                        node->balance_ = SAME_HEIGHT;
                    }
                }
            }
        }

        return HEIGHT_NO_CHANGE;
    }

    HelperInfo remove(AvlNode *&node, const Comparable &e) {
        if (!node) {
            return HEIGHT_NO_CHANGE;
        }

        if (e < node->element_) {
            if (HEIGHT_DECREASE == remove(node->left_, e)) {
                if (RIGHT_HIGHER == node->balance_) {
                    auto delta_height = node->right_->height_ - (node->left_ ? node->left_->height_ : -1);
                    if (delta_height > ALLOWED_IMBALANCE) {
                        balance_right(node);
                        return HEIGHT_DECREASE;
                    }
                } else if (SAME_HEIGHT == node->balance_) {
                    node->balance_ = RIGHT_HIGHER;
                } else {
                    --node->height_;
                    if (!node->left_ || (node->right_ && (node->right_->height_ + 1 == node->left_->height_))) {
                        node->balance_ = SAME_HEIGHT;
                    }

                    return HEIGHT_DECREASE;
                }
            }
        } else if (node->element_ < e) {
            if (HEIGHT_DECREASE == remove(node->right_, e)) {
                if (LEFT_HIGHER == node->balance_) {
                    auto delta_height = node->left_->height_ - (node->right_ ? node->right_->height_ : -1);
                    if (delta_height > ALLOWED_IMBALANCE) {
                        balance_left(node);
                        return HEIGHT_DECREASE;
                    }
                } else if (SAME_HEIGHT == node->balance_) {
                    node->balance_ = LEFT_HIGHER;
                } else {
                    --node->height_;
                    if (!node->right_ || (node->left_ && (node->left_->height_ + 1 == node->right_->height_))) {
                        node->balance_ = SAME_HEIGHT;
                    }

                    return HEIGHT_DECREASE;
                }
            }
        } else {
            if (node->left_ && node->right_) {
                node->element_ = findMin(node->right_);
                if (HEIGHT_DECREASE == remove(node->right_, node->element_)) {
                    if (LEFT_HIGHER == node->balance_) {
                        auto delta_height = node->left_->height_ - (node->right_ ? node->right_->height_ : -1);
                        if (delta_height > ALLOWED_IMBALANCE) {
                            balance_left(node);
                            return HEIGHT_DECREASE;
                        }
                    } else if (SAME_HEIGHT == node->balance_) {
                        node->balance_ = LEFT_HIGHER;
                    } else {
                        --node->height_;
                        if (!node->right_ || (node->left_ && (node->left_->height_ + 1 == node->right_->height_))) {
                            node->balance_ = SAME_HEIGHT;
                        }

                        return HEIGHT_DECREASE;
                    }
                }
            } else {
                auto delete_node = node;
                node = node->left_ ? node->left_ : node->right_;
                delete delete_node;
                return HEIGHT_DECREASE;
            }
        }

        return HEIGHT_NO_CHANGE;
    }

    void balance_right(AvlNode *&node) {
        assert(node->right_->balance_ != SAME_HEIGHT && "Already unbalanced before this node!!!");
        if (RIGHT_HIGHER == node->right_->balance_) {
            single_rorate_right_child(node);
        } else {
            double_rorate_right_child(node);
        }
    }

    void balance_left(AvlNode *&node) {
        assert(node->left_->balance_ != SAME_HEIGHT && "Already unbalanced before this node!!!");
        if (LEFT_HIGHER == node->left_->balance_) {
            single_rorate_left_child(node);
        } else {
            double_rorate_left_child(node);
        }
    }

    void single_rorate_right_child(AvlNode *&node) {
        auto temp = node;
        node = node->right_;
        temp->right_ = node->left_;
        node->left_ = temp;

        // adjust height
        temp->height_ = temp->right_ ? temp->right_->height_ + 1 : 0;

        //adjust balance
        if (!temp->right_ || (temp->left_ && (temp->right_->height_ == temp->left_->height_))) {
            temp->balance_ = SAME_HEIGHT;
        }
        if (node->right_->height_ == node->left_->height_) {
            node->balance_ = SAME_HEIGHT;
        }
    }

    void single_rorate_left_child(AvlNode *&node) {
        auto temp = node;
        node = node->left_;
        temp->left_ = node->right_;
        node->right_ = temp;

        // adjust height
        temp->height_ = temp->left_ ? temp->left_->height_ + 1 : 0;

        //adjust balance
        if (!temp->left_ || (temp->right_ && (temp->right_->height_ == temp->left_->height_))) {
            temp->balance_ = SAME_HEIGHT;
        }
        if (node->right_->height_ == node->left_->height_) {
            node->balance_ = SAME_HEIGHT;
        }
    }

    void double_rorate_right_child(AvlNode *&node) {
        auto result_left = node;
        auto result_right = node->right_;
        node = result_right->left_;

        result_left->right_ = node->left_;
        result_right->left_ = node->right_;
        node->left_ = result_left;
        node->right_ = result_right;

        // adjust height
        change_height_and_balance(result_right);
        change_height_and_balance(result_left);
        change_height_and_balance(node);
    }

    void double_rorate_left_child(AvlNode *&node) {
        auto result_left = node->left_;
        auto result_right = node;
        node = result_left->right_;

        result_left->right_ = node->left_;
        result_right->left_ = node->right_;
        node->left_ = result_left;
        node->right_ = result_right;
        
        // adjust height
        change_height_and_balance(result_right);
        change_height_and_balance(result_left);
        change_height_and_balance(node);
    }

    void change_height_and_balance(AvlNode *node) {
        auto height_left = node->left_ ? node->left_->height_ : -1;
        auto height_right = node->right_ ? node->right_->height_ : -1;

        if (height_left == height_right) {
            node->height_ = height_left + 1;
            node->balance_ = SAME_HEIGHT;
        } else if (height_left > height_right) {
            node->height_ = height_left + 1;
            node->balance_ = LEFT_HIGHER;
        } else {
            node->height_ = height_right + 1;
            node->balance_ = RIGHT_HIGHER;
        }
    }

    AvlNode *clone(const AvlNode *node) const {
        // clone()
        AvlNode *left = nullptr, *right = nullptr;
        if (node->left_) {
            left = clone(node->left_);
        }

        if (node->right_) {
            right = clone(node->right_);
        }

        return new AvlNode(node->element_, left, right);
    }
};

}

#endif