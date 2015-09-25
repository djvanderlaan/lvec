#ifndef r_vec_h
#define r_vec_h

#include "r.h"

template<typename T>
class R_vec {
  public:
    R_vec(SEXP sexp) : unprotect_(false), sexp_(sexp) {
      if (!is<T>(sexp_)) {
        sexp_ = PROTECT(r_cast<T>(sexp_));
        unprotect_ = true;
      }
      data_ = T::data(sexp_);
    }

    R_vec(int length) : unprotect_(true) {
      sexp_ = allocVector(T::Sexp_type, length);
      PROTECT(sexp_);
      data_ = T::data(sexp_);
    }

    ~R_vec() {
      if (unprotect_) UNPROTECT_PTR(sexp_);
    }

    int length() const {
      return T::length(sexp_);
    }

    typename T::value_type operator[](int i) const {
      return data_[i];
    };

    void set(int i, double val) {
      data_[i] = val;
    }

    SEXP sexp() const {
      return sexp_;
    }

  private:
    bool unprotect_;
    SEXP sexp_;
    typename T::value_type* data_;
};


template<>
class R_vec<R_string> {
  public:
    R_vec(SEXP sexp) : unprotect_(false), sexp_(sexp) {
      if (!is<R_string>(sexp)) {
        sexp_ = PROTECT(r_cast<R_string>(sexp_));
        unprotect_ = true;
      }
    }

    R_vec(int length) : unprotect_(true) {
      sexp_ = allocVector(R_string::Sexp_type, length);
      PROTECT(sexp_);
    }

    ~R_vec() {
      if (unprotect_) UNPROTECT_PTR(sexp_);
    }

    int length() const {
      return LENGTH(sexp_);
    }

    R::string operator[](int i) const {
      SEXP str = STRING_ELT(sexp_, i);
      if (str == R_NaString) return R::string();
      return R::string(CHAR(str));
    }

    void set(int i, const R::string& val) {
      SET_STRING_ELT(sexp_, i, mkChar(val.c_str()));
    }

    SEXP sexp() const {
      return sexp_;
    }

  private:
    bool unprotect_;
    SEXP sexp_;
};

#endif
