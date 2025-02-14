#pragma once

#include "AVL_Tree.hpp"

namespace hwt {
template <typename T, typename Comp = std::less<T>>
class PermanentSet final : public SearchTree<T, Comp> {
public:
  using typename SearchTree<T>::Node;
  using typename SearchTree<T>::NodePtr;
  using SearchTree<T>::root_;
  using SearchTree<T>::nodes_;

private:
  Node *new_root_ = nullptr;
  std::unordered_map<T, NodePtr> new_nodes_{};
  Comp comp_;

  Node *copy_node(Node *node) {
    NodePtr node_copy = std::make_unique<Node>(node->key_);
    Node *copy_ptr = node_copy.get();

    *copy_ptr = *node;

    new_nodes_[node_copy->key_] = std::move(node_copy);

    return copy_ptr;
  }

  Node *insert_node(Node *insertable) {
    if (!root_) {
      root_ = insertable;
      return root_;
    }

    Node *node = copy_node(root_);

    while (node) {
      if (comp_(insertable->key_, node->key_)) {
        if (!node->left_) {
          node->left_ = insertable;
          insertable->parent_ = node;
          break;
        }
        node = copy_node(node->left_);
      }

      else {
        if (!node->right_) {
          node->right_ = insertable;
          insertable->parent_ = node;
          break;
        }
        node = copy_node(node->right_);
      }
    }

    while (node->parent_) {
      SearchTree<T>::balance(node);
      node = node->parent_;
    }

    return SearchTree<T>::balance(node);
  }

  void forget_old() {
    for (auto &node : new_nodes_) {
      auto node_it = nodes_.find(node.first);

      if (node_it != nodes_.end()) {
        nodes_.erase(node_it);
      }

      nodes_[node.first] = std::move(node.second);
    }
  }

  void forget_new() {
    new_nodes_.clear();
    new_root_ = nullptr;
  }

  Node *create_new(const T& key) {
    NodePtr new_node = std::make_unique<Node>(key);
    Node *new_node_ptr = new_node.get();

    new_node_ptr->size_++;
    new_node_ptr->height_++;

    new_nodes_[new_node->key_] = std::move(new_node);

    return new_node_ptr;
  }

  // void reset();

public:
  PermanentSet() : SearchTree<T>{} {}
  ~PermanentSet() override = default;

  void reset() { forget_new(); }

  std::vector<T> get_versioned() const {
    std::vector<T> versioned_keys;

    for (const auto &pair : new_nodes_) {
      versioned_keys.push_back(pair.first);
    }
    return versioned_keys;
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

    for (auto& a : new_nodes_) {
      std::cout << a.first << std::endl;
    }
    std::cout << "--------"<<std::endl;

    forget_old();

    new_root_ = insert_node(create_new(key)); // SIGSEGV
  }
};
} // namespace hwt