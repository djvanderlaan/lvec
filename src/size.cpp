#include "../inst/include/lvec.h"
#include "r_export.h"

RcppExport SEXP get_size(SEXP rv) {
  BEGIN_RCPP
  Rcpp::XPtr<ldat::vec> v(rv);
  return Rcpp::wrap(static_cast<double>(v->size()));
  END_RCPP
}

RcppExport SEXP set_size(SEXP rv, SEXP rsize) {
  BEGIN_RCPP
  int size = Rcpp::as<int>(rsize);
  if (size > ldat::max_index) throw Rcpp::exception("Size is too large.");
  Rcpp::XPtr<ldat::vec> v(rv);
  v->size(size);
  return R_NilValue;
  END_RCPP
}

