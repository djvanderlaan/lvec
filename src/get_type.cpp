#include "../inst/include/lvec.h"
#include "r_export.h"

class type_visitor : public ldat::lvec_visitor {
  public: 

    type_visitor() : result_(ldat::na<std::string>()) {
    }

    void visit(ldat::lvec<double>& vec) {
      result_ = "numeric";
    }

    void visit(ldat::lvec<int>& vec) {
      result_ = "integer";
    }

    void visit(ldat::lvec<ldat::boolean>& vec) {
      result_ = "logical";
    }

    void visit(ldat::lvec<std::string>& vec) {
      result_ = "character";
    }

    const std::string& result() const {
      return result_;
    }

  private:
    std::string result_;
};


RcppExport SEXP get_type(SEXP rv) {
  BEGIN_RCPP
  type_visitor visitor;
  Rcpp::XPtr<ldat::vec> v(rv);
  v->visit(&visitor);
  return Rcpp::wrap(visitor.result());
  END_RCPP
}

