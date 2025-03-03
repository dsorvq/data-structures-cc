#include <algorithm>
#include <memory>
#include <utility>

namespace dsc {
template <class T>
class Vector final {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = T*;
  using const_iterator = const T*;
  using size_type = std::size_t;

  Vector() {}

  Vector(size_type n) : size_{n}, capacity_{n} { data_ = new T[capacity_]{}; }

  Vector(const Vector<T>& other)
      : size_(other.size_), capacity_(other.capacity_) {
    data_ = new T[capacity_];
    std::copy_n(other.data_, size_, data_);
  }

  Vector& operator=(const Vector<T>& other) {
    Vector<T> copy = other;
    copy.Swap(*this);
    return *this;
  }

  Vector(Vector<T>&& other) {
    data_ = std::exchange(other.data_, nullptr);
    size_ = std::exchange(other.size_, 0);
    capacity_ = std::exchange(other.capacity_, 0);
  }

  Vector& operator=(const Vector<T>&& other) {
    Vector<T> copy = std::move(other);
    copy.Swap(*this);
    return *this;
  }

  ~Vector() { delete[] data_; }

  // Element access
  reference At(size_type pos);
  const_reference At(size_type pos) const;
  reference operator[](size_type pos) { return data_[pos]; }
  const_reference operator[](size_type pos) const { return data_[pos]; };
  reference Front();
  const_reference Front() const { return data_[size_ - 1]; }
  reference Back() { return data_[size_ - 1]; }
  const_reference Back() const;

  // Iterators
  iterator begin() { return data_; }
  const_iterator begin() const { return data_; }
  const_iterator cbegin() const { return data_; }
  iterator end() { return data_ + size_; }
  const_iterator end() const { return data_ + size_; }
  const_iterator cend() const { return data_ + size_; }

  // Capacity
  bool Empty() const { return size_ == 0; }
  size_type Size() const { return size_; }
  void Reserve(size_type new_cap);
  size_type Capacity() const { return capacity_; }
  void ShrinkToFit();

  // Modifiers
  void PopBack() { --size_; }
  void Swap(Vector& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

 private:
  T* data_{nullptr};
  size_type size_{0};
  size_type capacity_{0};
};
}  // namespace dsc
