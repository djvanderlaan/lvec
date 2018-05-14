#ifndef r_h
#define r_h

#include <Rcpp.h>
#include <string>
#include <limits>

namespace ldat{

  constexpr double max_index = 1E15;

  // Function that check if an R vector is of a specific type
  template<typename T> inline bool is(SEXP p) { return T::is(p); }
  template<> inline bool is<double>(SEXP p) { return Rf_isReal(p); }
  template<> inline bool is<int>(SEXP p) { return Rf_isInteger(p);}

  // A bunch of overloaded functions allowing for testing for na/nan's
  inline bool is_na(double x) { return ISNA(x); }
  inline bool is_na(int x) { return x == NA_INTEGER; }
  inline bool is_na(const std::string& x) { return x.size() > 1 && x[0] == 0 && x[1] == 123; }

  inline bool is_nan(double x) { return ISNAN(x); }
  inline bool is_nan(int x) { return is_na(x); }
  inline bool is_nan(const std::string& x) { return is_na(x); }

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

  // Function that returns the base type (the type used by Rcpp) of a type
  // used by lvec. Only for boolean types this is different: lvec stores
  // booleans as bytes; while R/Rcpp uses int's. See boolean.h.
  inline double base_type(double v) { return double();}
  inline int base_type(int v) { return int();}
  inline std::string base_type(const std::string& v) { return std::string();}

  // Check if valiue of type S can be cast to a value of type T without 
  // causing a overflow
  template<typename T, typename S>
  bool within_limits(S val) {
    double v = val;
    return v <= std::numeric_limits<T>::max() &&
      v >= std::numeric_limits<T>::lowest();
  }

  // Cast a value of type S to a value of type T; making sure there is no
  // overflow (this throws) and handling NA's.
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
