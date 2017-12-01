#include "../inst/include/cppr.h"
#include "../inst/include/lvec.h"
#include <memory>
#include "r_export.h"

class range_indexing_visitor : public ldat::lvec_visitor {
  public: 
    range_indexing_visitor(ldat::vec::vecsize lower, ldat::vec::vecsize upper) : lower_(lower),
        upper_(upper), result_(0) { } 

    template<typename T>
    void visit_template(ldat::lvec<T>& vec) {
      if (upper_ >= vec.size()) throw std::runtime_error("Index out of range.");
      if (upper_ < lower_) throw std::runtime_error("Range has negative length.");
      ldat::vec::vecsize size = upper_ - lower_ + 1;
      std::unique_ptr<ldat::lvec<T> > result(new ldat::lvec<T>(size, vec));
      ldat::vec::vecsize j = 0;
      for (ldat::vec::vecsize i = lower_; i <= upper_; ++i, ++j) {
        T value = vec.get(i);
        result->set(j, value);
      }
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
    ldat::vec::vecsize lower_;
    ldat::vec::vecsize upper_;
    ldat::vec* result_;
};

RcppExport SEXP get_range(SEXP rv, SEXP rindex) {
  BEGIN_RCPP
  Rcpp::NumericVector index(rindex);
  // check input
  if (index.length() != 2)
    throw Rcpp::exception("Expecting vector of length 2 for range index.");
  if (index.is_na(index[0]))
    throw Rcpp::exception("Missing value for lower bound of range.");
  if (cppr::is_na(index[1]))
    throw Rcpp::exception("Missing value for upper bound of range.");
  // index
  range_indexing_visitor visitor{static_cast<ldat::vec::vecsize>(index[0]-1), 
    static_cast<ldat::vec::vecsize>(index[1]-1)};
  Rcpp::XPtr<ldat::vec> v(rv);
  v->visit(&visitor);
  return Rcpp::XPtr<ldat::vec>(visitor.result(), true);
  END_RCPP
}

