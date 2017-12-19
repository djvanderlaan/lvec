#include "../inst/include/lvec.h"
#include "r_export.h"

#include <algorithm>
#include <cstring>

class sort_visitor : public ldat::lvec_visitor {
  public: 

    template<class T>
    class compare {
      public:
        bool operator()(const T& lhs, const T& rhs) {
          if (ldat::is_nan(lhs)) return false;
          if (ldat::is_nan(rhs)) return true;
          return lhs < rhs;
        }
    };

    sort_visitor() {
    }

    template<typename T>
    void visit_template(ldat::lvec<T>& vec) {
      std::sort(vec.begin(), vec.end(), compare<T>());
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

  private:
};


// The function below doesn't return anything as the original object (pointed 
// to by rv) is modified. 
RcppExport SEXP sort(SEXP rv) {
  BEGIN_RCPP
  sort_visitor visitor{};
  Rcpp::XPtr<ldat::vec> v(rv);
  v->visit(&visitor);
  return R_NilValue;
  END_RCPP
}

