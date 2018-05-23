#include "r_export.h"

class strlen_visitor : public ldat::lvec_visitor {
  public: 

    strlen_visitor() : result_(ldat::na<int>()) {
    }

    void visit(ldat::lvec<double>& vec) {
      throw Rcpp::exception("Vector is not a character vector.");
    }

    void visit(ldat::lvec<int>& vec) {
      throw Rcpp::exception("Vector is not a character vector.");
    }

    void visit(ldat::lvec<ldat::boolean>& vec) {
      throw Rcpp::exception("Vector is not a character vector.");
    }

    void visit(ldat::lvec<std::string>& vec) {
      result_ = vec.strlen();
    }

    int result() const {
      return result_;
    }

  private:
    int result_;
};

RcppExport SEXP get_strlen(SEXP rv) {
  BEGIN_RCPP
  strlen_visitor visitor{};
  Rcpp::XPtr<ldat::vec> v(rv);
  v->visit(&visitor);
  return Rcpp::wrap(visitor.result());
  END_RCPP
}

