#ifndef lvec_h
#define lvec_h

#include <iostream> //TODO

#include <string>
#include <stdexcept>
#include <cstring>
#include "visitor.h"
#include "vec.h"
#include "string.h"

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
        //return std::string(vec_ + offset);
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
        //std::strncpy(vec_ + offset, str.c_str(), strlen_);
        //vec_[offset + strlen_ - 1] = 0;
      }

    private:
      char* vec_;
      vecsize size_;
      unsigned int strlen_;
  };
}

#endif
