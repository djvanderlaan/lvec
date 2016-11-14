#include "cppr.h"
#include "ldat.h"
#include "lvec.h"


extern "C" {
  SEXP get_size(SEXP rv) {
    CPPRTRY
    ldat::vec* v = sexp_to_vec(rv);
    // TODO: return length as integer??
    cppr::rvec<cppr::numeric> result{1};
    result[0] = v->size();
    return result.sexp();
    CPPRCATCH
  }
}


extern "C" {
  SEXP set_size(SEXP rv, SEXP rsize) {
    CPPRTRY
    cppr::rvec<cppr::numeric> size{rsize};
    if (size.length() == 0) std::runtime_error("Size is empty");
    if (size[0] > cppr::max_index) throw std::runtime_error("Size is too large.");
    ldat::vec* v = sexp_to_vec(rv);
    v->size(size[0]);
    return R_NilValue;
    CPPRCATCH
  }
}

