
#include "cppr.h"
#include "lvec.h"
#include <iostream>

// Pair of macros that can be used to catch any remaining exceptions and pass
// these on to R. Start and end functions that get called by R by these.
#define RTRY \
    try {
#define RCATCH \
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


class assign_visitor : public ldat::lvec_visitor {
  public: 
    assign_visitor(ldat::lvec& index, ldat::lvec& values) : index_(index), values_(values) {
    }

    void visit(ldat::dbl_lvec& vec) {
     // TODO check sizes
     for (ldat::lvec::vecsize i = 0; i < index_.size(); ++i) {
       int index = index_.get_int(i);
       double value = values_.get_double(i);
       vec.set(index, value);
     }
    }

    void visit(ldat::int_lvec& vec) {
     for (ldat::lvec::vecsize i = 0; i < index_.size(); ++i) {
       int index = index_.get_int(i);
       int value = values_.get_int(i);
       vec.set(index, value);
     }
    }

    void visit(ldat::str_lvec& vec) {
    }

  private:
    ldat::lvec& index_;
    ldat::lvec& values_;
};

extern "C" {
  SEXP test_lvec2(SEXP rp) {
    RTRY

    const int size = 10;
    ldat::dbl_lvec v1(size);
    ldat::int_lvec v2(size);
    ldat::dbl_lvec v3(size);

    for (ldat::lvec::vecsize i = 0; i < v1.size(); ++i) {
      v1.set(i, i);
      v2.set(i, size-1-i);
      v3.set(i, i*i);
    }

    assign_visitor visitor(v2, v3);
    v1.visit(&visitor);

    for (ldat::lvec::vecsize i = 0; i < v1.size(); ++i) {
      std::cout << v1.get(i) << "\t\t" << v2.get(i) <<
        "\t\t" << v3.get(i) << "\n";
    }

    return R_NilValue;
    RCATCH
  }
}


// ========
// Convert R-vector to lvec

#include <cstring>

extern "C" {
  SEXP r_to_lvec(SEXP rv) {
    double* v = REAL(rv);
    int l = LENGTH(rv);
    ldat::dbl_lvec res(l);
    std::memcpy(res.data(), v, l * sizeof(double));

    /*for (ldat::lvec::vecsize i = 0; i < res.size(); ++i) {
      std::cout << res.get(i) << "\n";
    }*/

    return R_NilValue;
  }
}


