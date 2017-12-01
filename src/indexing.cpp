#include "../inst/include/cppr.h"
#include "../inst/include/lvec.h"
#include "r_export.h"
#include <memory>

class indexing_visitor : public ldat::lvec_visitor {
  public:
    indexing_visitor(ldat::vec& index) : index_(index), result_(0) { }

    template<typename T>
    void visit_template_numeric(ldat::lvec<T>& vec) {
      std::unique_ptr<ldat::lvec<T> > result(new ldat::lvec<T>(index_.size(), vec));
      for (ldat::vec::vecsize i = 0; i < index_.size(); ++i) {
        double index = index_.get_of_type(i, double());
        if (cppr::is_na(index)) {
          result->set(i, cppr::na<T>());
        } else {
          // need to floor index to have indices such as 3.1 work correctly
          index = std::floor(index);
          if (index < 1.0 || index > vec.size()) 
            throw Rcpp::exception("Index out of range.");
          T value = vec.get(index - 1);
          result->set(i, value);
        }
      }
      result_ = result.release();
    }

    template<typename T>
    void visit_template_logical(ldat::lvec<T>& vec) {
      // determine size of result
      ldat::vec::vecsize n = 0;
      ldat::vec::vecsize j = 0;
      for (ldat::vec::vecsize i = 0; i < vec.size(); ++i, ++j) {
        if (j >= index_.size()) j = 0;
        int index = index_.get_of_type(j, int());
        if (index != 0 || cppr::is_na(index)) ++n;
      }
      // index
      std::unique_ptr<ldat::lvec<T> > result(new ldat::lvec<T>(n, vec));
      ldat::vec::vecsize result_index = 0;
      for (ldat::vec::vecsize i = 0, j = 0; i < vec.size(); ++i, ++j) {
        if (j >= index_.size()) j = 0;
        int index = index_.get_of_type(j, int());
        if (cppr::is_na(index)) {
	  result->set(result_index++, cppr::na<T>());
        } else if (index != 0) {
          T value = vec.get(i);
          result->set(result_index++, value);
        }
      }
      result_ = result.release();
      return;
    }

    template<typename T>
    void visit_template(ldat::lvec<T>& vec) {
      if (is_logical(index_)) {
        return visit_template_logical(vec);
      } else {
        return visit_template_numeric(vec);
      }
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
    ldat::vec& index_;
    ldat::vec* result_;
};


RcppExport SEXP get(SEXP rv, SEXP rindex) {
  BEGIN_RCPP
  Rcpp::XPtr<ldat::vec> index(rindex);
  indexing_visitor visitor(*index);
  Rcpp::XPtr<ldat::vec> v(rv);
  v->visit(&visitor);
  return Rcpp::XPtr<ldat::vec>(visitor.result(), true);
  END_RCPP
}

