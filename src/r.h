#ifndef r_h
#define r_h

#include <R.h>
#include <Rdefines.h>

#include "r_string.h"

// A class implenting RAII to protect and unprotect R-objects
// Use: protSEXP p = function_that_returns_an_sexp()
// and PROTECT and UNPROTECT are needed any mode.
class protSEXP {
  public:
    protSEXP(SEXP p) : p_(PROTECT(p)) { }
    ~protSEXP() { UNPROTECT_PTR(p_); }
    operator SEXP() const { return p_; }
    SEXP get() { return p_;}
  private:
    SEXP p_;
};


class R_double {
  public: 
    using value_type = double;
    static constexpr int Sexp_type = REALSXP;

    static value_type* data(SEXP x) { return REAL(x); };
    static int length(SEXP x) { return LENGTH(x); };
    static bool is(SEXP x) { return isReal(x); };
};

class R_integer {
  public: 
    using value_type = int;
    static constexpr int Sexp_type = INTSXP;

    static value_type* data(SEXP x) { return INTEGER(x); };
    static int length(SEXP x) { return LENGTH(x); };
    static bool is(SEXP x) { return isInteger(x); };
};

class R_string {
  public: 
    static constexpr int Sexp_type = STRSXP;

    static int length(SEXP x) { return LENGTH(x); };
    static bool is(SEXP x) { return isString(x); };
};


template<typename T>
bool is(SEXP p) {
  return T::is(p);
}

template<>
bool is<double>(SEXP p) {
  return isReal(p);
}

template<>
bool is<int>(SEXP p) {
  return isInteger(p);
}

// A bunch of overloaded functions allowing for testing for na/nan's
inline bool is_na(double x) { return ISNA(x); }
inline bool is_na(int x) { return x == NA_INTEGER; }
inline bool is_na(R::string x) { return x.na(); }
inline bool is_nan(double x) { return ISNAN(x); }
inline bool is_nan(int x) { return is_na(x); }
inline bool is_nan(R::string x) { return is_na(x); }

// A template which returns a NA for the given type. This template needs to be
// specialized for the different types in R as the representation of NA is
// type dependent. If we now want a double NA, we can do na<double>() and if we
// want an integer NA we can do na<int>().
template<class T> inline T na() { return T();}
template<> inline double na<double>() { return NA_REAL;}
template<> inline int na<int>() { return NA_INTEGER;}



template<typename T>
SEXP r_cast(SEXP p) {
  if (!is<T>(p)) 
    p = coerceVector(p, T::Sexp_type);
  return p;
}

#endif
