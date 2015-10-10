
#include "cppr.h"
#include "doublevec.h"
#include <iostream>

class TestVisitor : public Visitor {
  public:
    void visit(DoubleVec& vec) {
      std::cout << "Visit double\n";
    };

    void visit(LVec<cppr::numeric>& vec) {
      std::cout << "Visit LVec<cppr::numeric>\n";
    };
};

inline Vec* sexp_to_vec(SEXP rvec) {
  if(!R_ExternalPtrAddr(rvec)) return 0;
  void* p = R_ExternalPtrAddr(rvec);
  return reinterpret_cast<Vec*>(p);
}


extern "C" {
  SEXP test_visitor(SEXP rvec) {
    Vec* vec = sexp_to_vec(rvec);
    TestVisitor test;
    vec->visit(&test);
    return R_NilValue;
  }
}


class SetVisitor : public Visitor {
  public:
    SetVisitor(SEXP indices, SEXP values) : Visitor(), indices_(indices),
        values_(values) {
    };

    ~SetVisitor() {
    };

    void visit(DoubleVec& vec) {
      cppr::rvec<cppr::integer> indices{indices_};
      cppr::rvec<cppr::numeric> values{values_};
      for (int i = 0; i < indices.length(); ++i) {
        vec.set(indices[i], values[i]);
      }
    }

    void visit(LVec<cppr::numeric>& vec) {
      std::cout << "Visit LVec<cppr::numeric>\n";
    }

  private:
    SEXP indices_;
    SEXP values_;
};

extern "C" {
  SEXP test_visitor2(SEXP rvec, SEXP indices, SEXP values) {
    Vec* vec = sexp_to_vec(rvec);
    SetVisitor test(indices, values);
    vec->visit(&test);
    return R_NilValue;
  }
}

