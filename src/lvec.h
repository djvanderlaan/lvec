#ifndef lvec_h
#define lvec_h

#include <string>
#include <stdexcept>
#include <cstring>

namespace ldat {

  class dbl_lvec;
  class int_lvec;
  class str_lvec;

  class lvec_visitor {
    public:
      virtual void visit(dbl_lvec& vec) = 0;
      virtual void visit(int_lvec& vec) = 0;
      virtual void visit(str_lvec& vec) = 0;
  };


  class lvec {
    public:
      typedef unsigned int vecsize;

      lvec() {}
      ~lvec() {}

      virtual vecsize size() const = 0;
      virtual double get_double(vecsize i) const = 0;
      virtual int get_int(vecsize i) const = 0;
      virtual bool get_bool(vecsize i) const = 0;

      virtual void visit(lvec_visitor* visitor) = 0;
  };

  class dbl_lvec : public lvec {
    public: 
      dbl_lvec(vecsize size) : lvec(), size_(size) {
        vec_ = new double[size];
      }

      ~dbl_lvec() {
        delete [] vec_;
      }

      vecsize size() const {
        return size_;
      }

      double get_double(vecsize i) const {
        return vec_[i];
      }

      int get_int(vecsize i) const {
        return vec_[i];
      }

      bool get_bool(vecsize i) const {
        return vec_[i] == 0.0;
      }

      void visit(lvec_visitor* visitor) {
        visitor->visit(*this);
      }

      double get(vecsize i) const {
        return vec_[i];
      }

      void set(vecsize i, double value) {
        vec_[i] = value;
      }

      const double* data() const {
        return vec_;
      }

      double* data() {
        return vec_;
      }

    private:
      double* vec_;
      vecsize size_;
  };

  class int_lvec : public lvec {
    public: 
      int_lvec(vecsize size) : lvec(), size_(size) {
        vec_ = new int[size];
      }

      ~int_lvec() {
        delete [] vec_;
      }

      vecsize size() const {
        return size_;
      }

      double get_double(vecsize i) const {
        return vec_[i];
      }

      int get_int(vecsize i) const {
        return vec_[i];
      }

      bool get_bool(vecsize i) const {
        return vec_[i] == 0.0;
      }

      void visit(lvec_visitor* visitor) {
        visitor->visit(*this);
      }

      int get(vecsize i) const {
        return vec_[i];
      }

      void set(vecsize i, int value) {
        vec_[i] = value;
      }

    private:
      int* vec_;
      vecsize size_;
  };


  class str_lvec : public lvec {
    public:
      str_lvec(vecsize size, unsigned int strlen) : lvec(), size_(size), 
          strlen_(strlen) {
        vec_ = new char[size_ * strlen_];
      }

      ~str_lvec() {
        delete [] vec_;
      }

      vecsize size() const {
        return size_;
      }

      double get_double(vecsize i) const {
        throw std::runtime_error("Can't convert a string to double.");
      }

      int get_int(vecsize i) const {
        throw std::runtime_error("Can't convert a string to integer.");
      }

      bool get_bool(vecsize i) const {
        throw std::runtime_error("Can't convert a string to boolean.");
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
