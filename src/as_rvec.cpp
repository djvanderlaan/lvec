#include "ldat.h"
#include "r_export.h"
#include <cstring>

class as_rvec_visitor : public ldat::lvec_visitor {
  public:
    as_rvec_visitor() : rvec_(0) {
    }

    void visit(ldat::lvec<double>& vec) {
      // TODO: possible optimisation: use memcpy
      Rcpp::NumericVector res(vec.size());
      for (ldat::vec::vecsize i = 0; i < vec.size(); ++i)
        res[i] = vec.get(i);
      rvec_ = PROTECT(Rcpp::wrap(res));
    }

    void visit(ldat::lvec<int>& vec) {
      // TODO: possible optimisation: use memcpy
      Rcpp::IntegerVector res(vec.size());
      for (ldat::vec::vecsize i = 0; i < vec.size(); ++i)
        res[i] = vec.get(i);
      rvec_ = PROTECT(Rcpp::wrap(res));
    }

    void visit(ldat::lvec<cppr::boolean>& vec) {
      Rcpp::LogicalVector res(vec.size());
      for (ldat::vec::vecsize i = 0; i < vec.size(); ++i)
        res[i] = cppr::cast_value<int>(vec.get(i));
      rvec_ = PROTECT(Rcpp::wrap(res));
    }

    void visit(ldat::lvec<std::string>& vec) {
      Rcpp::CharacterVector res(vec.size());
      for (ldat::vec::vecsize i = 0; i < vec.size(); ++i) {
        const std::string val = vec.get(i);
        if (cppr::is_na(val)) res[i] = NA_STRING;
        else res[i] = val;
      }
      rvec_ = PROTECT(Rcpp::wrap(res));
    }

    SEXP rvec() const {
      if (rvec_ == 0) throw Rcpp::exception("Failed to convert.");
      UNPROTECT_PTR(rvec_);
      return rvec_;
    }

  private:
    SEXP rvec_;
};

RcppExport SEXP as_rvec(SEXP rv) {
  BEGIN_RCPP
  as_rvec_visitor visitor{};
  Rcpp::XPtr<ldat::vec> v(rv);
  v->visit(&visitor);
  return visitor.rvec();
  END_RCPP
}

