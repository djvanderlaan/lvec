#ifndef r_h
#define r_h

#include <R.h>
#include <Rdefines.h>

// Rdefines.h defines a macro length; this clashes with a length function
// defined in some of the boost headers we use; undefine the macro here;
// we dont need it
#undef length

#include <string>
#include <limits>
#include <stdexcept>


// A class implenting RAII to protect and unprotect R-objects
// Use: protSEXP p = function_that_returns_an_sexp()
// and PROTECT and UNPROTECT are not needed any more.
namespace cppr {
  class protSEXP {
    public:
      protSEXP(SEXP p) : p_(PROTECT(p)) { }
      ~protSEXP() { UNPROTECT_PTR(p_); }
      operator SEXP() const { return p_; }
      SEXP get() { return p_;}
    private:
      SEXP p_;
  };
};


namespace cppr {
  class numeric {
    public:
      using value_type = double;
      static constexpr int Sexp_type = REALSXP;

      static value_type* data(SEXP x) { return REAL(x); };
      static int length(SEXP x) { return LENGTH(x); };
      static bool is(SEXP x) { return isReal(x); };
  };

  class integer {
    public:
      using value_type = int;
      static constexpr int Sexp_type = INTSXP;

      static value_type* data(SEXP x) { return INTEGER(x); };
      static int length(SEXP x) { return LENGTH(x); };
      static bool is(SEXP x) { return isInteger(x); };
  };

  class logical {
    public:
      using value_type = int;
      static constexpr int Sexp_type = LGLSXP;

      static value_type* data(SEXP x) { return LOGICAL(x); };
      static int length(SEXP x) { return LENGTH(x); };
      static bool is(SEXP x) { return isLogical(x); };
  };

  class character {
    public:
      static constexpr int Sexp_type = STRSXP;

      static int length(SEXP x) { return LENGTH(x); };
      static bool is(SEXP x) { return isString(x); };
  };
};


namespace cppr {
  // Function that check if an R vector is of a specific type
  template<typename T> inline bool is(SEXP p) { return T::is(p); }
  template<> inline bool is<double>(SEXP p) { return isReal(p); }
  template<> inline bool is<int>(SEXP p) { return isInteger(p);}

  // A bunch of overloaded functions allowing for testing for na/nan's
  inline bool is_na(double x) { return ISNA(x); }
  inline bool is_na(int x) { return x == NA_INTEGER; }
  inline bool is_na(const std::string& x) {
    return x.size() > 1 && x[0] == 0 && x[1] == 123;
  }

  inline bool is_nan(double x) { return ISNAN(x); }
  inline bool is_nan(int x) { return is_na(x); }
  inline bool is_nan(const std::string& x) {
    return is_na(x);
  }

  // A template which returns a NA for the given type. This template needs to be
  // specialized for the different types in R as the representation of NA is
  // type dependent. If we now want a double NA, we can do na<double>() and if
  // we want an integer NA we can do na<int>().
  template<class T> inline T na() { return T();}
  template<> inline double na<double>() { return NA_REAL;}
  template<> inline int na<int>() { return NA_INTEGER;}
  template<> inline std::string na<std::string>() {
    std::string res(2, 'X');
    res[0] = 0;
    res[1] = 123;
    return res;
  }

  template<typename T>
  SEXP cast_sexp(SEXP p) {
    if (!is<T>(p))
      p = coerceVector(p, T::Sexp_type);
    return p;
  }

  template<typename T, typename S>
  bool within_limits(S val) {
    double v = val;
    return v < std::numeric_limits<T>::max() &
      v > std::numeric_limits<T>::min();
  }

  template<typename T, typename S>
  T cast_value(S x) {
    if (is_nan(x)) return na<T>();
    if (!within_limits<T>(x))
      throw std::runtime_error("Overflow when casting between types.");
    return T(x);
  }

};

namespace cppr {

  template<typename T>
  class rvec {
    public:
      rvec(SEXP sexp) : unprotect_(false), sexp_(sexp) {
        if (!is<T>(sexp_)) {
          sexp_ = PROTECT(cast_sexp<T>(sexp_));
          unprotect_ = true;
        }
        data_ = T::data(sexp_);
      }

      rvec(int length) : unprotect_(true) {
        sexp_ = allocVector(T::Sexp_type, length);
        PROTECT(sexp_);
        data_ = T::data(sexp_);
      }

      ~rvec() {
        if (unprotect_) UNPROTECT_PTR(sexp_);
      }

      int length() const {
        return T::length(sexp_);
      }

      typename T::value_type operator[](int i) const {
        return data_[i];
      };

      typename T::value_type& operator[](int i) {
        return data_[i];
      }

      void set(int i, typename T::value_type val) {
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
  class rvec<character> {
    public:
      rvec(SEXP sexp) : unprotect_(false), sexp_(sexp) {
        if (!is<character>(sexp)) {
          sexp_ = PROTECT(cast_sexp<character>(sexp_));
          unprotect_ = true;
        }
      }

      rvec(int length) : unprotect_(true) {
        sexp_ = allocVector(character::Sexp_type, length);
        PROTECT(sexp_);
      }

      ~rvec() {
        if (unprotect_) UNPROTECT_PTR(sexp_);
      }

      int length() const {
        return LENGTH(sexp_);
      }

      std::string operator[](int i) const {
        SEXP str = STRING_ELT(sexp_, i);
        if (str == R_NaString) return na<std::string>();
        return std::string(CHAR(str));
      }

      class element_reference {
        public:
          element_reference(rvec<character>& vec, int el): vec_(vec), el_(el) {};

          element_reference& operator=(const std::string& str) {
            vec_.set(el_, str);
          }

          operator std::string() {
            const rvec<character>& vec = vec_;
            return vec[el_];
          }

        private:
          rvec<character>& vec_;
          int el_;
      };

      element_reference operator[](int i) {
        return element_reference(*this, i);
      }

      void set(int i, const std::string& val) {
        if (is_na(val)) {
          SET_STRING_ELT(sexp_, i, R_NaString);
        } else {
          SET_STRING_ELT(sexp_, i, mkChar(val.c_str()));
        }
      }

      SEXP sexp() const {
        return sexp_;
      }

    private:
      bool unprotect_;
      SEXP sexp_;
  };

};



// Pair of macros that can be used to catch any remaining exceptions and pass
// these on to R. Start and end functions that get called by R by these.
#define CPPRTRY \
  try {
#define CPPRCATCH \
  } catch(const std::string& e) { \
    error(e.c_str()); \
    return R_NilValue; \
  } catch(const std::exception& e) { \
    error(e.what()); \
    return R_NilValue; \
  } catch (...) { \
    error("Uncaught exception."); \
    return R_NilValue; \
  } \

#endif
