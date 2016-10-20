#ifndef lvec_h
#define lvec_h

#include <iostream> //TODO

#include <string>
#include <stdexcept>
#include <cstring>
#include <memory>
#include "boolean.h"
#include "visitor.h"
#include "iterator.h"
#include "val_ref.h"
#include "vec.h"
#include "cppr.h"

namespace ldat {

  template<typename T>
  class lvec : public vec {
    public:

      lvec(vecsize size) : vec(), size_(size) {
        vec_ = new T[size];
      }

      lvec(vecsize size, const lvec<T>& templ) : vec(), size_(size) {
        vec_ = new T[size];
      }

      ~lvec() {
        delete [] vec_;
      }

      vec* clone() const {
        std::unique_ptr<lvec<T> > result(new lvec<T>(size_));
        std::memcpy(result->vec_, vec_, size_ * sizeof(T));
        return result.release();
      }

      vecsize size() const {
        return size_;
      }

      double get_of_type(vecsize i, double type) const {
        return cppr::cast_value<double>(vec_[i]);
      }

      int get_of_type(vecsize i, int type) const {
        return cppr::cast_value<int>(vec_[i]);
      }

      std::string get_of_type(vecsize i, const std::string& type) const {
        throw std::runtime_error("Can't convert numeric to string.");
      }

      void visit(lvec_visitor* visitor) {
        visitor->visit(*this);
      }

      T get(vecsize i) const {
        return vec_[i];
      }

      void set(vecsize i, T value) {
        vec_[i] = value;
      }

      const T* data() const {
        return vec_;
      }

      T* data() {
        return vec_;
      }

      typedef lvec_iterator<T> iterator;
      iterator begin() { return iterator(*this, 0); }
      iterator end() { return iterator(*this, size_); }

    private:
      T* vec_;
      vecsize size_;
  };

  template<>
  class lvec<std::string> : public vec {
    public:
      lvec(vecsize size, unsigned int strlen) : vec(), size_(size),
          strlen_(strlen+1) {
        vec_ = new char[size_ * strlen_];
      }

      lvec(vecsize size, const lvec<std::string>& templ) : vec(), size_(size),
          strlen_(templ.strlen_) {
        vec_ = new char[size_ * strlen_];
      }

      ~lvec() {
        delete [] vec_;
      }

      vec* clone() const {
        std::unique_ptr<lvec<std::string> > result(new lvec<std::string>(size_, *this));
        std::memcpy(result->vec_, vec_, strlen_ * size_);
        return result.release();
      }

      vecsize size() const {
        return size_;
      }

      unsigned int strlen() const {
        return strlen_;
      }

      double get_of_type(vecsize i, double type) const {
        throw std::runtime_error("Can't convert a string to double.");
      }

      int get_of_type(vecsize i, int type) const {
        throw std::runtime_error("Can't convert a string to integer.");
      }

      std::string get_of_type(vecsize i, const std::string& type) const {
        return get(i);
      }

      void visit(lvec_visitor* visitor) {
        visitor->visit(*this);
      }

      std::string get(vecsize i) const {
        vecsize offset = i * strlen_;
        return std::string(vec_ + offset, vec_ + (offset + strlen_));
      }

      void set(vecsize i, const std::string& str) {
        vecsize offset = i * strlen_;
        unsigned int n = str.size();
        if (n > (strlen_ - 1)) n = strlen_ - 1;
        vecsize k = offset;
        for (unsigned j = 0; j < n; ++j, ++k) {
          vec_[k] = str[j];
        }
        vec_[k] = 0;
      }

      typedef lvec_iterator<std::string> iterator;
      iterator begin() { return iterator(*this, 0); }
      iterator end() { return iterator(*this, size_); }


    private:
      char* vec_;
      vecsize size_;
      unsigned int strlen_;
  };
}

namespace ldat {

  inline bool is_logical(const ldat::vec& vec) {
    auto p = dynamic_cast<const ldat::lvec<cppr::boolean>*>(&vec);
    return p != 0;
  }

}

#endif
