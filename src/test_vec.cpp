#include "cppr.h"

#include <iostream>
#include <cstring>

class lvec {
  public:
    lvec() {};
    ~lvec() {};

    typedef unsigned int vecsize;

    virtual vecsize size() const = 0;

};

class dbl_lvec : public lvec {
  public: 
    dbl_lvec(vecsize size) : lvec(), size_(size) {
      vec_ = new double[size];
    };

    ~dbl_lvec() {
      delete [] vec_;
    };

    vecsize size() const {
      return size_;
    };

    double operator[](vecsize i) const {
      return vec_[i];
    };
    
    double& operator[](vecsize i) {
      return vec_[i];
    }

  private:
    double* vec_;
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

    class element_reference {
      public:
        element_reference(str_lvec& vec, vecsize el): vec_(vec), el_(el) {};
        
        element_reference& operator=(const std::string& str) {
          vec_.set(el_, str);
        }
        
        operator std::string() {
          const str_lvec& vec = vec_;
          return vec[el_];
        }
        
      private:
        str_lvec& vec_;
        vecsize el_;
    };

    std::string operator[](vecsize i) const {
      vecsize offset = i * strlen_;
      return std::string(vec_ + offset);
    }
    
    element_reference operator[](vecsize i) {
      return element_reference(*this, i);
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


extern "C" {

  SEXP test_lvec() {
    dbl_lvec v(100);

    for (dbl_lvec::vecsize i = 0; i < v.size(); ++i) {
      v[i] = i;
    }

    double sum = 0;
    for (dbl_lvec::vecsize i = 0; i < v.size(); ++i) {
      sum += v[i];
    }
    std::cout << sum << std::endl;


    str_lvec w(10, 12);
    for (str_lvec::vecsize i = 0; i < w.size(); ++i) {
      if (i % 2 == 0) {
        w[i] = "foo";
      } else {
        w[i] = "bar";
      }
    }

    int nfoo = 0;
    for (str_lvec::vecsize i = 0; i < w.size(); ++i) {
      std::string val = w[i];
      std::cout << "'" << val.c_str() << "'" ;//<< std::endl;
      std::cout << (val == "bar") << std::endl;
      if (val == "foo") nfoo++;
    }
    std::cout << nfoo << std::endl;

    std::string a = "foo";
    std::cout << (a == "foo") << std::endl;


    return R_NilValue;
  }
}
