#ifndef AVL_TREE_IMPL1_H_
#define AVL_TREE_IMPL1_H_

#include <exception>
#include <iostream>
#include <type_traits>
#include <utility>
#include <cassert>

namespace tree {

struct EmptyTree : public std::exception {
    const char *what() const noexcept override {
        return "EmptyTree";
    }
};

template <typename T>
using enable_if_t = typename std::enable_if<T::value>::type;

template <typename Comparable, char ALLOWED_IMBALANCE = 1>
class AvlTree {
  private:
    struct AvlNode {
        Comparable element_;
        AvlNode *left_;
        AvlNode *right_;
        char balance_;

        template <typename T, typename = enable_if_t<std::is_convertible<T, Comparable>>>
        AvlNode(T &&e, AvlNode *l = nullptr, AvlNode *r = nullptr)
        : element_(std::forward<T>(e))
        , left_(l)
        , right_(r)
        , balance_(0)
        {}
    };

    AvlNode *root_;

  public:
    AvlTree() : root_(nullptr) {}

    template <typename T, typename = enable_if_t<std::is_convertible<T, Comparable>>>
    void insert(T &&e) {
        std::pair<AvlNode **, int> parents[128];
        parents[0].first = &root_;

        int index = 0;
        auto node = root_;
        while (node) {
            if (node->element_ < e) {
                parents[index].second = 1;
                parents[++index].first = &node->right_;
                node = node->right_;
            } else if (e < node->element_) {
                // parents[index++] = std::make_pair(&node->left_, -1);
                parents[index].second = -1;
                parents[++index].first = &node->left_;
                node = node->left_;
            } else {
                return;
            }
        };

        *(parents[index].first) = new AvlNode(std::forward<T>(e));

        // find insert place
        for (int i = index - 1; i >= 0; --i) {
            const auto &p_ref = parents[i];
            auto temp = *p_ref.first;

            if (0 == temp->balance_) {
                temp->balance_ += p_ref.second;
            } else if ((temp->balance_ ^ p_ref.second)) {
                temp->balance_ += p_ref.second;
                break;
            } else {
                temp->balance_ += p_ref.second;
                if (temp->balance_ < -ALLOWED_IMBALANCE) {
                    assert(temp->left_->balance_ != 0 && "111111111111111111111111111");
                    if (temp->left_->balance_ < 0) {
                        single_rorate_left_child(p_ref.first);
                    } else {
                        double_rorate_left_child(p_ref.first);
                    }
                    break;
                } else if (temp->balance_ > ALLOWED_IMBALANCE) {
                    assert(temp->right_->balance_ != 0 && "222222222222222222222222222");
                    if (temp->right_->balance_ > 0) {
                        single_rorate_right_child(p_ref.first);
                    } else {
                        double_rorate_right_child(p_ref.first);
                    }
                    break;
                }
            }
        }
    }

    void remove(const Comparable &e) {
        std::pair<AvlNode **, int> parents[128];
        parents[0].first = &root_;

        int index = 0;
        auto node = root_;
        while (node) {
            if (node->element_ < e) {
                parents[index].second = 1;
                parents[++index].first = &node->right_;
                node = node->right_;
            } else if (e < node->element_) {
                // parents[index++] = std::make_pair(&node->left_, -1);
                parents[index].second = -1;
                parents[++index].first = &node->left_;
                node = node->left_;
            } else {
                // return;
                // find element
            }
        };

        return;
    }

  private:
    void single_rorate_left_child(AvlNode **node) {
        auto parent = *node;
        auto child = parent->left_;
        *node = child;
        parent->left_ = child->right_;
        child->right_ = parent;

        parent->balance_ += 1 - child->balance_;
        ++child->balance_;
    }

    void single_rorate_right_child(AvlNode **node) {
        auto parent = *node;
        auto child = parent->right_;
        *node = child;
        parent->right_ = child->left_;
        child->left_ = parent;

        parent->balance_ += -1 - child->balance_;
        --child->balance_;
    }

    void double_rorate_left_child(AvlNode **node) {
        auto result_right = *node;
        auto result_left = result_right->left_;
        *node = result_left->right_;
        auto result_parent = *node;

        result_left->right_ = result_parent->left_;
        result_right->left_ = result_parent->right_;
        result_parent->left_ = result_left;
        result_parent->right_ = result_right;

        if (result_parent->balance_ >= 0) {
            result_right->balance_ += 2;
            auto temp = result_left->balance_;
            result_left->balance_ += -1 - result_parent->balance_;
            if (result_left->balance_ < 0) {
                result_parent->balance_ = temp - 1;
            }
        } else {
            --result_left->balance_;
            auto temp = result_right->balance_;
            result_right->balance_ += 2 - result_parent->balance_;
            if (result_right->balance_ > 0) {
                result_parent->balance_ = 2 + temp;
            }
        }
    }

    void double_rorate_right_child(AvlNode **node) {
        auto result_left = *node;
        auto result_right = result_left->right_;
        *node = result_right->left_;
        auto result_parent = *node;

        result_left->right_ = result_parent->left_;
        result_right->left_ = result_parent->right_;
        result_parent->left_ = result_left;
        result_parent->right_ = result_right;

        if (result_parent->balance_ >= 0) {
            ++result_right->balance_;
            auto temp = result_left->balance_;
            result_left->balance_ += -2 - result_parent->balance_;
            if (result_left->balance_ < 0) {
                result_parent->balance_ = temp - 2;
            }
        } else {
            result_left->balance_ -= 2;
            auto temp = result_right->balance_;
            result_right->balance_ += 1 - result_parent->balance_;
            if (result_right->balance_ > 0) {
                result_parent->balance_ = 1 + temp;
            }
        }
    }
};

// template <typename Comparable>
// template <>
// template <typename T>
// void AvlTree<T, 1>::single_rorate_left_child(typename AvlTree<T, 1>::AvlNode **node) {}

}

#endif