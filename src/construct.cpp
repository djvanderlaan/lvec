#include "../inst/include/lvec.h"
#include "r_export.h"

RcppExport SEXP new_lvec(SEXP rsize, SEXP rtype, SEXP rstrlen) {
  BEGIN_RCPP
  double size = Rcpp::as<double>(rsize);
  if (Rcpp::NumericVector::is_na(size)) throw Rcpp::exception("Size is not a number.");
  if (size > ldat::max_index) throw Rcpp::exception("Size is too large.");
  std::string type = Rcpp::as<std::string>(rtype);

  ldat::vec* vec = 0;
  if (type == "numeric") {
    vec = new ldat::lvec<double>(size);
  } else if (type == "integer") {
    vec = new ldat::lvec<int>(size);
  } else if (type == "logical") {
    vec = new ldat::lvec<ldat::boolean>(size);
  } else if (type == "character") {
    int strlen = Rcpp::as<int>(rstrlen);
    if (Rcpp::IntegerVector::is_na(strlen)) throw Rcpp::exception("Strlen is not a number.");
    if (strlen < 0) throw Rcpp::exception("Strlen is smaller than zero");
    vec = new ldat::lvec<std::string>(size, strlen);
  }
  return Rcpp::XPtr<ldat::vec>(vec, true);
  END_RCPP
}

