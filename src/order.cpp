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
          if (ldat::is_nan(val_lhs)) return false;
          T val_rhs = vec_.get(rhs-1);
          if (ldat::is_nan(val_rhs)) return true;
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
      for (ldat::vec::vecsize i = 0; i < size; ++i) result->set(i, i+1);
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

    void visit(ldat::lvec<ldat::boolean>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<std::string>& vec) {
      return visit_template(vec);
    }

    ldat::vec* result() {
      return result_;
    }

  private:
    ldat::lvec<double>* result_;
};



RcppExport SEXP order(SEXP rv) {
  BEGIN_RCPP
  order_visitor visitor{};
  Rcpp::XPtr<ldat::vec> v(rv);
  v->visit(&visitor);
  return Rcpp::XPtr<ldat::vec>(visitor.result());
  END_RCPP
}

