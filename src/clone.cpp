#include "r_export.h"

RcppExport SEXP lclone(SEXP rv) {
  BEGIN_RCPP
  Rcpp::XPtr<ldat::vec> v(rv);
  return Rcpp::XPtr<ldat::vec>(v->clone(), true);
  END_RCPP
}

