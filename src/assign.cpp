#include "cppr.h"
#include "ldat.h"
#include "lvec.h"

#include <iostream>

class assign_visitor : public ldat::lvec_visitor {
  public: 
    assign_visitor(ldat::vec& index, ldat::vec& values) : index_(index), values_(values) {
    }

    template<typename T>
    void visit_template(ldat::lvec<T>& vec) {
      for (ldat::vec::vecsize i = 0; i < index_.size(); ++i) {
        int index = index_.get_of_type(i, int()) - 1;
        T value = values_.get_of_type(i, T());
        vec.set(index, value);
      }
    }

    void visit(ldat::lvec<double>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<int>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<std::string>& vec) {
      return visit_template(vec);
    }

  private:
    ldat::vec& index_;
    ldat::vec& values_;
};


// The function below doesn't return anything as the original object (pointed 
// to by rv) is modified. One possibility is to return rv. However, when this
// result is not assigned to a r-value, then the garbage collector will try to
// clean up rv, e.g.
// a <- as.lvec(1:10)
// .Call("assign", a, 1:4, 41:44)
// will result in cleanup of the lvec pointed to by a; using a will result in 
// terrible stuff. 

extern "C" {
  SEXP assign(SEXP rv, SEXP rindex, SEXP rvalues) {
    CPPRTRY
    ldat::vec* index = sexp_to_vec(rindex);
    ldat::vec* values = sexp_to_vec(rvalues);
    assign_visitor visitor{*index, *values};
    ldat::vec* v = sexp_to_vec(rv);
    v->visit(&visitor);
    return R_NilValue;
    CPPRCATCH
  }
}

