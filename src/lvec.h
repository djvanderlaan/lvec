#ifndef lvec_h
#define lvec_h

#include <string>
#include <stdexcept>
#include <cstring>

namespace ldat {

  class dbl_lvec;
  class int_lvec;
  class str_lvec;

  template<typename T> class gvec;

  class lvec_visitor {
    public:
      virtual void visit(gvec<double>& vec) = 0;
      virtual void visit(gvec<int>& vec) = 0;
      virtual void visit(gvec<std::string>& vec) = 0;
  };


  class lvec {
    public:
      typedef unsigned int vecsize;

      lvec() {}
      ~lvec() {}

      virtual vecsize size() const = 0;
      virtual double get_of_type(vecsize i, double type) const = 0;
      virtual int get_of_type(vecsize i, int type) const = 0;
      virtual std::string get_of_type(vecsize i, const std::string& type) const = 0;

      virtual void visit(lvec_visitor* visitor) = 0;
  };

  template<typename T>
  class gvec : public lvec {
    public: 
      gvec(vecsize size) : lvec(), size_(size) {
        vec_ = new T[size];
      }

      ~gvec() {
        delete [] vec_;
      }

      vecsize size() const {
        return size_;
      }

      double get_of_type(vecsize i, double type) const {
        return vec_[i];
      }

      int get_of_type(vecsize i, int type) const {
        return vec_[i];
      }

      std::string get_of_type(vecsize i, std::string type) const {
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
  class gvec<std::string> : public lvec {
    public:
      gvec(vecsize size, unsigned int strlen) : lvec(), size_(size), 
          strlen_(strlen) {
        vec_ = new char[size_ * strlen_];
      }

      ~gvec() {
        delete [] vec_;
      }

      vecsize size() const {
        return size_;
      }

      double get_of_type(vecsize i, double type) const {
        throw std::runtime_error("Can't convert a string to double.");
      }

      int get_of_type(vecsize i, int type) const {
        throw std::runtime_error("Can't convert a string to integer.");
      }

      std::string get_of_type(vecsize i, std::string type) const {
        return get(i);
      }

      void visit(lvec_visitor* visitor) {
        visitor->visit(*this);
      }

      std::string get(vecsize i) const {
        vecsize offset = i * strlen_;
        return std::string(vec_ + offset);
      }

      void set(vecsize i, const std::string& str) {
        vecsize offset = i * strlen_;
        std::strncpy(vec_ + offset, str.c_str(), strlen_);
        vec_[offset + strlen_ - 1] = 0;
      }

    private:
      char* vec_;
      vecsize size_;
      unsigned int strlen_;
  };


};

#endif
