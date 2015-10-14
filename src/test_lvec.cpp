
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

    template<typename T>
    void visit_template(ldat::gvec<T>& vec) {
      for (ldat::lvec::vecsize i = 0; i < index_.size(); ++i) {
        int index = index_.get_of_type(i, int());
        T value = values_.get_of_type(i, T());
        vec.set(index, value);
      }
    }

    void visit(ldat::gvec<double>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::gvec<int>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::gvec<std::string>& vec) {
      return visit_template(vec);
    }

  private:
    ldat::lvec& index_;
    ldat::lvec& values_;
};

extern "C" {
  SEXP test_lvec2(SEXP rp) {
    RTRY

    const int size = 10;
    ldat::gvec<double> v1(size);
    ldat::gvec<int> v2(size);
    ldat::gvec<double> v3(size);

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
    RTRY
    if (cppr::is<cppr::numeric>(rv)) {
      double* v = REAL(rv);
      int l = LENGTH(rv);
      ldat::gvec<double> res(l);
      std::memcpy(res.data(), v, l * sizeof(double));
    } else if (cppr::is<cppr::integer>(rv)) {
      throw std::string("Unsupported type.");
    } else if (cppr::is<cppr::logical>(rv)) {
      throw std::string("Unsupported type.");
    } else if (cppr::is<cppr::character>(rv)) {
      throw std::string("Unsupported type.");
    } else {
      throw std::string("Unsupported type.");
    }
    return R_NilValue;
    RCATCH
  }
}


