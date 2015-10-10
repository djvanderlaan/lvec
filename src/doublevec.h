#ifndef doublevec_h
#define doublevec_h

#include "vec.h"
#include "visitor.h"
#include <cstring>

class DoubleVec : public Vec {
  public: 
    DoubleVec(vecsize size) : Vec(), size_(size) {
      vec_ = new double[size];
    };

    ~DoubleVec() {
      delete [] vec_;
    };

    vecsize size() const {
      return size_;
    };

    DoubleVec* get_range(vecsize from, vecsize to) const {
      // TODO: check range
      DoubleVec* res = new DoubleVec{to-from};
      std::memcpy(res->vec_, vec_ + from, (to-from)*sizeof(double));
      return res;
    };

    void visit(Visitor* visitor) {
      visitor->visit(*this);
    };

    void set(unsigned int i, double val) {
      vec_[i] = val;
    };

    double get(unsigned int i) const {
      return vec_[i];
    };

    double* data() { return vec_;};
    const double* data() const { return vec_;};

  private:
    double* vec_;
    vecsize size_;
};

template<typename T>
class LVec : public Vec {
  public: 
    using value_type = typename T::value_type;

    LVec(vecsize size) : Vec(), size_(size) {
      vec_ = new value_type[size];
    };

    ~LVec() {
      delete [] vec_;
    };

    vecsize size() const {
      return size_;
    };

    LVec<T>* get_range(vecsize from, vecsize to) const {
      // TODO: check range
      LVec<T>* res = new LVec<T>{to-from};
      std::memcpy(res->vec_, vec_ + from, (to-from)*sizeof(value_type));
      return res;
    };

    void visit(Visitor* visitor) {
      visitor->visit(*this);
    };

    void set(unsigned int i, value_type val) {
      vec_[i] = val;
    };

    value_type get(unsigned int i) const {
      return vec_[i];
    };

    value_type* data() { return vec_;};
    const value_type* data() const { return vec_;};

  private:
    value_type* vec_;
    vecsize size_;
};


#endif

