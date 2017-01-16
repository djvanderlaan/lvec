#include "tempfile.h"
#include "ldat.h"

#include <R_ext/Rdynload.h>

extern "C" {
  void R_init_lvec(DllInfo *info) {
    R_RegisterCCallable("lvec", "tempfile",  (DL_FUNC) &tempfile);
    R_RegisterCCallable("lvec", "sexp_to_vec",  (DL_FUNC) &sexp_to_vec);
    R_RegisterCCallable("lvec", "vec_to_sexp",  (DL_FUNC) &vec_to_sexp);

  }
}

