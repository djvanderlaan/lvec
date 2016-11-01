#include "cppr.h"
#include "ldat.h"
#include "lvec.h"


extern "C" {
  SEXP get_size(SEXP rv) {
    CPPRTRY
    ldat::vec* v = sexp_to_vec(rv);
    // TODO: return length as integer??
    cppr::rvec<cppr::integer> result{1};
    result[0] = v->size();
    return result.sexp();
    CPPRCATCH
  }
}


extern "C" {
  SEXP set_size(SEXP rv, SEXP rsize) {
    CPPRTRY
    // TODO: length as integer??
    cppr::rvec<cppr::integer> size{rsize};
    if (size.length() == 0) std::runtime_error("Size is empty");
    ldat::vec* v = sexp_to_vec(rv);
    v->size(size[0]);
    return R_NilValue;
    CPPRCATCH
  }
}

