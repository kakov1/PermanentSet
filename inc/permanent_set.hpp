#pragma once

#include "AVL_Tree.hpp"
#include <list>
#include <utility>

namespace hwt {
template <typename T, typename Comp = std::less<T>>
class PermanentSet final : public SearchTree<T, Comp> {
public:
  using typename SearchTree<T>::Node;
  using typename SearchTree<T>::NodePtr;
  using SearchTree<T>::root_;
  using SearchTree<T>::nodes_;

private:
  std::list<T> ans{};
  Node *new_root_ = nullptr;
  std::unordered_map<T, NodePtr> new_nodes_{};
  Comp comp_;

  Node *copy_node(Node *node) {
    if (!node)
      return nullptr;

    NodePtr node_copy = std::make_unique<Node>(node->key_);
    Node *copy_ptr = node_copy.get();

    *copy_ptr = *node;

    new_nodes_[node_copy->key_] = std::move(node_copy);

    return copy_ptr;
  }

  Node *balance(Node *node) {
    assert(node);

    set_height(node);
    set_size(node);
    if (calculate_balance_factor(node) > 1) {
      if (node->left_ && calculate_balance_factor(node->left_) < 0) {
        rotate_left(node->left_);
      }
      return rotate_right(node);
    } else if (calculate_balance_factor(node) < -1) {
      if (node->right_ && calculate_balance_factor(node->right_) > 0) {
        rotate_right(node->right_);
      }
      return rotate_left(node);
    }
    return node;
  }

  Node *insert_node(Node *insertable) {
    ans.clear();
    Node *node = copy_node(root_);

    Node *copy;

    while (node) {
      ans.push_back(node->key_);
      if (comp_(insertable->key_, node->key_)) {
        if (!node->left_) {
          node->left_ = insertable;
          insertable->parent_ = node;
          break;
        }
        copy = node;
        node = copy_node(node->left_);
        copy->left_ = node;
      }

      else {
        if (!node->right_) {
          node->right_ = insertable;
          insertable->parent_ = node;
          break;
        }
        copy = node;
        node = copy_node(node->right_);
        copy->right_ = node;
      }
    }

    set_new_parents();

    while (node->parent_) {
      //  SearchTree<T>::balance(node);
      node = node->parent_;
    }

    // return SearchTree<T>::balance(node);
    return node;
  }

  void forget_old() {
    for (auto &node : new_nodes_) {
      auto node_it = nodes_.find(node.first);

      if (node_it != nodes_.end()) {
        std::swap(nodes_[node.first], node.second);
      }

      else {
        nodes_[node.first] = std::move(node.second);
      }
    }

    forget_new();
  }

  void forget_new() {
    new_nodes_.clear();
    new_root_ = nullptr;
  }

  Node *create_new(const T &key) {
    NodePtr new_node = std::make_unique<Node>(key);
    Node *new_node_ptr = new_node.get();

    new_node_ptr->size_++;
    new_node_ptr->height_++;

    new_nodes_[new_node->key_] = std::move(new_node);

    return new_node_ptr;
  }

  void set_new_parents() {
    for (auto &node_pair : new_nodes_) {
      Node *node = node_pair.second.get();

      if (node->right_) {
        node->right_->parent_ = node;
      }

      if (node->left_) {
        node->left_->parent_ = node;
      }
    }
  }

  void set_old_parents() {
    for (auto &node_pair : new_nodes_) {
      if (nodes_.find(node_pair.first) == nodes_.end()) {
        break;
      }

      Node *node = node_pair.second.get();

      if (node->right_) {
        node->right_->parent_ = nodes_[node_pair.first].get();
      }

      if (node->left_) {
        node->left_->parent_ = nodes_[node_pair.first].get();
      }
    }
  }

public:
  PermanentSet() : SearchTree<T>{} {}
  ~PermanentSet() override = default;

  void reset() {
    set_old_parents();
    forget_new();
  }

  std::list<T> get_versioned() const {
    return ans;
  };

  void insert(const T &key) {
    if (SearchTree<T>::search(root_, key)) {
      return;
    }

    if (!root_) {
      SearchTree<T>::insert(key);
      return;
    }

    if (!new_root_) {
      new_root_ = insert_node(create_new(key));
      return;
    }

    root_ = new_root_;

    forget_old();
    new_root_ = insert_node(create_new(key));
  }

  void print_new_nodes() const {
    std::cout << "---NEW_NODES---" << std::endl;

    for (auto &node_pair : new_nodes_) {
      std::cout << node_pair.first << std::endl;
    }

    std::cout << "---------------" << std::endl;
  }

  void print_old_nodes() const {
    std::cout << "---OLD_NODES---" << std::endl;

    for (auto &node_pair : nodes_) {
      std::cout << node_pair.first << std::endl;
    }

    std::cout << "---------------" << std::endl;
  }

  void print_new_nodes_ptr() const {
    std::cout << "---NEW_NODES---" << std::endl;

    for (auto &node_pair : new_nodes_) {
      std::cout << node_pair.second.get() << std::endl;
    }

    std::cout << "---------------" << std::endl;
  }

  void print_old_nodes_ptr() const {
    std::cout << "---OLD_NODES---" << std::endl;

    for (auto &node_pair : nodes_) {
      std::cout << node_pair.second.get() << std::endl;
    }

    std::cout << "---------------" << std::endl;
  }
};
} // namespace hwt