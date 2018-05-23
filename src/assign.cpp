#include "r_export.h"

class assign_visitor : public ldat::lvec_visitor {
  public:
    assign_visitor(ldat::vec& index, ldat::vec& values) : index_(index), values_(values) {
    }

    template<typename T>
    void visit_template_numeric(ldat::lvec<T>& vec) {
      if (index_.size() > 0 && values_.size() == 0)
        throw Rcpp::exception("Replacement has length zero.");
      ldat::vec::vecsize j = 0;
      for (ldat::vec::vecsize i = 0; i < index_.size(); ++i, ++j) {
        double index = index_.get_of_type(i, double());
        if (ldat::is_na(index))
          throw Rcpp::exception("NAs are not allowed in subscripted assignments.");
        // need to floor index to have indices such as 3.1 work correctly
        index = std::floor(index);
        if (index < 1 || index > vec.size())
          throw Rcpp::exception("Index out of range.");
        if (j >= values_.size()) j = 0;
        T value = values_.get_of_type(j, ldat::base_type(T()));
        vec.set(index - 1, value);
      }
    }

    template<typename T>
    void visit_template_logical(ldat::lvec<T>& vec) {
      // check for empty values with non empty index; faster to do this here instead
      // of in the the mainloop; this check is only necessary for empty values
      if (values_.size() == 0) {
        for (ldat::vec::vecsize i = 0; i < index_.size(); ++i) {
          int index = index_.get_of_type(i, int());
          if (index != 0 || ldat::is_na(index))
            throw Rcpp::exception("Replacement has length zero.");
        }
      }
      // index
      ldat::vec::vecsize i_index  = 0;
      ldat::vec::vecsize i_values = 0;
      for (ldat::vec::vecsize i = 0; i < vec.size(); ++i, ++i_index) {
        if (i_index >= index_.size()) i_index = 0;
        int index = index_.get_of_type(i_index, int());
        if (ldat::is_na(index))
          throw Rcpp::exception("NAs are not allowed in subscripted assignments.");
        if (index != 0) {
          if (i_values >= values_.size()) i_values = 0;
          T value = values_.get_of_type(i_values++, ldat::base_type(T()));
          vec.set(i, value);
        }
      }
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

    void visit(ldat::lvec<ldat::boolean>& vec) {
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
// will result in cleanup of the lvec pointed to by a; using a after that will
// result in terrible stuff.

RcppExport SEXP assign(SEXP rv, SEXP rindex, SEXP rvalues) {
  BEGIN_RCPP
  Rcpp::XPtr<ldat::vec> index(rindex);
  Rcpp::XPtr<ldat::vec> values(rvalues);
  assign_visitor visitor{*index, *values};
  Rcpp::XPtr<ldat::vec> v(rv);
  v->visit(&visitor);
  return R_NilValue;
  END_RCPP
}

