#include "cppr.h"
#include "ldat.h"
#include "lvec.h"
#include "r_export.h"

#include <algorithm>
#include <cstring>

class order_visitor : public ldat::lvec_visitor {
  public: 

    template<class T>
    class compare {
      public:
        compare(const ldat::lvec<T>& vec) : vec_(vec) {};

        bool operator()(ldat::vec::vecsize lhs, ldat::vec::vecsize rhs) {
          T val_lhs = vec_.get(lhs-1);
          if (cppr::is_nan(val_lhs)) return false;
          T val_rhs = vec_.get(rhs-1);
          if (cppr::is_nan(val_rhs)) return true;
          return val_lhs < val_rhs;
        }

      private:
        const ldat::lvec<T>& vec_;
    };

    order_visitor() : result_(0) {
    }

    template<typename T>
    void visit_template(const ldat::lvec<T>& vec) {
      // initialise result with current order
      ldat::vec::vecsize size = vec.size();
      std::unique_ptr<ldat::lvec<double> > result(new ldat::lvec<double>(size));
      for (int i = 0; i < size; ++i) result->set(i, i+1);
      // we are now going to sort the current order; but when comparing
      // we use the values pointed to by the order vector
      std::sort(result->begin(), result->end(), compare<T>(vec));
      if (result_) delete result_;
      result_ = result.release();
    }

    void visit(ldat::lvec<double>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<int>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<cppr::boolean>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<std::string>& vec) {
      return visit_template(vec);
    }

    ldat::vec* result() {
      return result_;
    }

  private:
    // TODO: int? or double for long vectors?
    ldat::lvec<double>* result_;
};


extern "C" {
  SEXP order(SEXP rv) {
    CPPRTRY
    order_visitor visitor{};
    ldat::vec* v = sexp_to_vec(rv);
    v->visit(&visitor);
    return vec_to_sexp(visitor.result());
    CPPRCATCH
  }
}


