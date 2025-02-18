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
    if (!node)
      return nullptr;

    NodePtr node_copy = std::make_unique<Node>(node->key_);
    Node *copy_ptr = node_copy.get();

    *copy_ptr = *node;

    new_nodes_[node_copy->key_] = std::move(node_copy);

    return copy_ptr;
  }

  Node *copy_child(Node *&child) {
    child = copy_node(child);

    return child;
  }

  std::vector<T> insert_node(Node *&insertable) {
    if (!new_root_ && !root_) {
      new_root_ = insertable;
      return {};
    }

    std::vector<T> versioned{};
    Node *node = copy_node(root_);
    Node *parent{};

    while (node) {
      versioned.push_back(node->key_);
      node->parent_ = parent;
      parent = node;
      if (comp_(insertable->key_, node->key_)) {
        if (!node->left_) {
          node->left_ = insertable;
          insertable->parent_ = node;
          break;
        }
        node = copy_child(node->left_);
      }

      else {
        if (!node->right_) {
          node->right_ = insertable;
          insertable->parent_ = node;
          break;
        }
        node = copy_child(node->right_);
      }
    }

    while (node->parent_) {
      SearchTree<T>::balance(node);
      node = node->parent_;
    }

    new_root_ = SearchTree<T>::balance(node);

    return versioned;
  }

  void forget_old() {
    for (auto &&node : new_nodes_) {
      if (nodes_.find(node.first) != nodes_.end()) {
        std::swap(nodes_[node.first], node.second);
      }

      else {
        nodes_[node.first] = std::move(node.second);
      }
    }

    root_ = new_root_;
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

  void set_old_parents() {
    for (auto &&node_pair : new_nodes_) {
      if (nodes_.find(node_pair.first) == nodes_.end()) {
        continue;
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

  void swap(PermanentSet &other) {
    std::swap(new_nodes_, other.new_nodes_);
    std::swap(new_root_, other.new_root_);
  }

public:
  PermanentSet() : SearchTree<T>{} {}

  PermanentSet(const PermanentSet &other)
      : SearchTree<T>{static_cast<const SearchTree<T> &>(other)} {
    for (auto &&node : other.new_nodes_) {
      copy_node(node.second.get());
    }

    new_root_ = new_nodes_[other.new_root_->key_].get();
  }

  PermanentSet(PermanentSet &&other) noexcept
      : SearchTree<T>{static_cast<SearchTree<T> &&>(std::move(other))} {
    swap(other);
  }

  PermanentSet &operator=(const PermanentSet &other) {
    if (this != &other) {
      PermanentSet tmp{other};

      SearchTree<T>::operator=(static_cast<const SearchTree<T> &>(tmp));
      swap(tmp);
    }

    return *this;
  }

  PermanentSet &operator=(PermanentSet &&other) noexcept {
    SearchTree<T>::operator=(static_cast<SearchTree<T> &&>(other));
    swap(other);

    return *this;
  }

  ~PermanentSet() override = default;

  void reset() {
    set_old_parents();
    forget_new();
  }

  std::vector<T> insert(const T &key) {
    if (SearchTree<T>::search(root_, key)) {
      return {};
    }

    if (new_root_) {
      forget_old();
    }

    //#define DEBUG

#ifdef DEBUG

    std::cout << "INSERT: Key = " << key << std::endl;

    print_old_nodes();
    print_new_nodes();

#endif

    Node *new_node = create_new(key);
    return insert_node(new_node);

#ifdef DEBUG

    print_old_nodes();
    print_new_nodes();

    std::cout << "EXIT INSERT" << std::endl;

#endif
  }

  bool operator==(const PermanentSet &rhs) const {
    return SearchTree<T>::is_equal(new_root_, rhs.new_root_);
  }

  void print_new_nodes() const {
    std::cout << "---NEW_NODES---" << std::endl;

    for (auto &node_pair : new_nodes_) {
      std::cout << "Key: " << node_pair.first
                << " Address: " << node_pair.second.get() << std::endl;
    }

    std::cout << "---------------" << std::endl;
  }

  void print_old_nodes() const {
    std::cout << "---OLD_NODES---" << std::endl;

    for (auto &node_pair : nodes_) {
      std::cout << "Key: " << node_pair.first
                << " Address: " << node_pair.second.get() << std::endl;
    }

    std::cout << "---------------" << std::endl;
  }
};
} // namespace hwt