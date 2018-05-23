#include "r_export.h"
#include "tempfile_impl.h"

std::string tempdir = ".";

std::string tempfile_impl() {
  char* p = R_tmpnam("lvec", tempdir.c_str());
  std::string res{p};
  free(p);
  return res;
}

RcppExport SEXP set_tempdir(SEXP rd) {
  BEGIN_RCPP
  tempdir = Rcpp::as<std::string>(rd);
  return R_NilValue;
  END_RCPP
}

