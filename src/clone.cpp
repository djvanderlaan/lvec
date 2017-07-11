#include "ldat.h"
#include "lvec.h"
#include "r_export.h"

extern "C" {
  SEXP lclone(SEXP rv) {
    CPPRTRY
    ldat::vec* v = sexp_to_vec(rv);
    ldat::vec* result = v->clone();
    return vec_to_sexp(result);
    CPPRCATCH
  }
}

