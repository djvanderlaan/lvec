#ifndef r_h
#define r_h

#include <Rcpp.h>

#include <string>
#include <limits>
#include <stdexcept>

namespace cppr{
  constexpr double max_index = 1E15;

  class numeric {
    public:
      using value_type = double;
      static constexpr int Sexp_type = REALSXP;

      static value_type* data(SEXP x) { return REAL(x); };
      static R_xlen_t length(SEXP x) { return LENGTH(x); };
      static bool is(SEXP x) { return Rf_isReal(x); };
  };

  class integer {
    public:
      using value_type = int;
      static constexpr int Sexp_type = INTSXP;

      static value_type* data(SEXP x) { return INTEGER(x); };
      static R_xlen_t length(SEXP x) { return LENGTH(x); };
      static bool is(SEXP x) { return Rf_isInteger(x); };
  };

  class logical {
    public:
      using value_type = int;
      static constexpr int Sexp_type = LGLSXP;

      static value_type* data(SEXP x) { return LOGICAL(x); };
      static R_xlen_t length(SEXP x) { return LENGTH(x); };
      static bool is(SEXP x) { return Rf_isLogical(x); };
  };

  class character {
    public:
      static constexpr int Sexp_type = STRSXP;

      static R_xlen_t length(SEXP x) { return LENGTH(x); };
      static bool is(SEXP x) { return Rf_isString(x); };
  };

  // Function that check if an R vector is of a specific type
  template<typename T> inline bool is(SEXP p) { return T::is(p); }
  template<> inline bool is<double>(SEXP p) { return Rf_isReal(p); }
  template<> inline bool is<int>(SEXP p) { return Rf_isInteger(p);}

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

  template<typename T, typename S>
  bool within_limits(S val) {
    double v = val;
    return v <= std::numeric_limits<T>::max() &&
      v >= std::numeric_limits<T>::lowest();
  }

  template<typename T, typename S>
  T cast_value(S x) {
    if (std::is_same<S, T>::value) return x;
    if (is_nan(x)) return na<T>();
    if (!within_limits<T>(x))
      throw Rcpp::exception("Overflow when casting between types.");
    return T(x);
  }

}

#endif
