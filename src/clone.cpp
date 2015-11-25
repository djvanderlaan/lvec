#include "ldat.h"
#include "lvec.h"

extern "C" {
  SEXP lclone(SEXP rv) {
    CPPRTRY
    ldat::vec* v = sexp_to_vec(rv);
    ldat::vec* result = v->clone();
    return vec_to_sexp(result);
    CPPRCATCH
  }
}

