#include "ldat.h"
#include "r_export.h"
#include <cstring>
#include <stdexcept>

RcppExport SEXP as_lvec(SEXP rv) {
  BEGIN_RCPP
  if (cppr::is<cppr::numeric>(rv)) {
    double* v = REAL(rv);
    R_xlen_t l = cppr::numeric::length(rv);
    Rcpp::XPtr<ldat::lvec<double>> res(new ldat::lvec<double>(l));
    std::memcpy(res->data(), v, l * sizeof(double));
    return res;
  } else if (cppr::is<cppr::integer>(rv)) {
    int* v = INTEGER(rv);
    R_xlen_t l = cppr::integer::length(rv);
    Rcpp::XPtr<ldat::lvec<int>> res(new ldat::lvec<int>(l));
    std::memcpy(res->data(), v, l * sizeof(int));
    return res;
  } else if (cppr::is<cppr::logical>(rv)) {
    Rcpp::LogicalVector v(rv);
    R_xlen_t l = cppr::integer::length(rv);
    Rcpp::XPtr<ldat::lvec<cppr::boolean>> res(new ldat::lvec<cppr::boolean>(l));
    for (R_xlen_t i = 0; i < l; ++i) {
      int val = v[i];
      if (v.is_na(val)) res->set(i, cppr::na<cppr::boolean>());
      else res->set(i, val);
    }
    return res;
  } else if (cppr::is<cppr::character>(rv)) {
    Rcpp::CharacterVector v(rv);
    // determine max string length
    int max_len = 0;
    for (R_xlen_t i = 0; i < v.length(); ++i) {
      if (!v.is_na(v[i])) {
        int l = v[i].size();
        if (l > max_len) max_len = l;
      }
    }
    // the minimum string length is always 2 as we need to be able to store
    // missing values which have a length of 2.
    if (max_len < 2) max_len = 2;
    // create lvec
    Rcpp::XPtr<ldat::lvec<std::string>> res(new ldat::lvec<std::string>(v.length(), max_len));
    for (R_xlen_t i = 0; i < v.length(); ++i) {
      if (v.is_na(v[i])) res->set(i, cppr::na<std::string>());
      else res->set(i, Rcpp::as<std::string>(v[i]));
    }
    return res;
  } else {
    throw Rcpp::exception("Unknown type. Unable to transform to lvec.");
  }
  return R_NilValue;
  END_RCPP
}

