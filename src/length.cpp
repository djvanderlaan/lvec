#include "cppr.h"
#include "ldat.h"
#include "lvec.h"


extern "C" {
  SEXP size(SEXP rv) {
    CPPRTRY
    ldat::vec* v = sexp_to_vec(rv);
    // TODO: return length as integer??
    cppr::rvec<cppr::integer> result{1};
    result[0] = v->size();
    return result.sexp();
    CPPRCATCH
  }
}

