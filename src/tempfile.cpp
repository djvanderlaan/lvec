#include "tempfile.h"
#include "cppr.h"
#include "r_export.h"

#include <exception>


std::string tempdir = ".";

std::string tempfile() {
  char* p = R_tmpnam("lvec", tempdir.c_str());
  std::string res{p};
  free(p);
  return res;
}

extern "C" {
  SEXP set_tempdir(SEXP rd) {
    CPPRTRY
    cppr::rvec<cppr::character> d{rd};
    if (d.length() != 1) 
      std::runtime_error("Path should be a character vector of length 1.");
    tempdir = d[0];
    return R_NilValue;
    CPPRCATCH
  }
}
