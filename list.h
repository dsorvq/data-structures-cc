#include <type_traits>

namespace dsc {
template <class T>
class List final {
 private:
  struct NodeBase {
    NodeBase() {}
    NodeBase(NodeBase* prev, NodeBase* next) : prev_(prev), next_(next) {}
    NodeBase* prev_{nullptr};
    NodeBase* next_{nullptr};
  };

  struct Node : public NodeBase {
    using NodeBase::prev_;
    using NodeBase::next_;
    Node(const T& value) : value_(value) {}
    T value_;
  };

  template<bool IsConst>
  class IteratorBase {
   public:
    using reference_type = std::conditional_t<IsConst, const T&, T&>;
    using pointer_type = std::conditional_t<IsConst, const T*, T*>;

    IteratorBase() {}
    IteratorBase(NodeBase* node) : node_(node) {}

    IteratorBase& operator++() {
      node_ = node_->next_;
      return *this;
    }

    IteratorBase& operator++(int) {
      IteratorBase tmp = *this;
      node_ = node_->next_;
      return tmp;
    }

    IteratorBase& operator--() {
      node_ = node_->prev_;
      return *this;
    }

    IteratorBase& operator--(int) {
      IteratorBase tmp = *this;
      node_ = node_->prev_;
      return tmp;
    }

    bool operator==(const IteratorBase& other) {
      return node_ == other.node_;
    }

    bool operator!=(const IteratorBase& other) {
      return !(*this == other);
    }

    reference_type operator*() {
      return static_cast<Node*>(node_)->value_;
    }

    pointer_type operator->() {
      return &static_cast<Node*>(node_)->value_;
    }

   private:
    NodeBase* node_{nullptr};
    friend List;
  };

 public:
  using iterator = IteratorBase<false>;
  using const_iterator = IteratorBase<true>;
  using size_type = std::size_t;

 public:
  List() {}

  ~List() {
    Clear();
  }

  iterator Insert(const iterator pos, const T& value) {
    Node* new_node = new Node(value);

    new_node->next_ = pos.node_;
    new_node->prev_ = pos.node_->prev_;
    pos.node_->prev_->next_ = new_node;
    pos.node_->prev_ = new_node;

    ++size_;
    return iterator(new_node);
  }

  iterator Erase(iterator pos) {
    Node* node = pos.node_;
    iterator return_iterator(pos.node_->next_);
    node->prev_->next_ = node->next_;
    node->next_->prev_ = node->prev_;

    delete node;
    --size_;
    return return_iterator;
  }

  void Clear() {
    for (NodeBase* current = sentinel_.prev_; current != &sentinel_; ) {
      NodeBase* next = current->next_;
      delete static_cast<Node*>(current);
      current = next;
    }
    sentinel_.next_ = &sentinel_;
    sentinel_.prev_ = &sentinel_;
    size_ = 0;
  }

  iterator begin() {
    return iterator(sentinel_.prev_);
  }

  iterator end() {
    return iterator(&sentinel_);
  }

  const_iterator cbegin() {
    return iterator(sentinel_.prev_);
  }

  const_iterator cend() {
    return iterator(&sentinel_);
  }

 private:
  NodeBase sentinel_{&sentinel_, &sentinel_};
  size_type size_ = 0;
};
}  // namespace dsc
