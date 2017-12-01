#include "../inst/include/cppr.h"
#include "../inst/include/lvec.h"

extern "C" {
  SEXP as_lvec(SEXP rv);
  SEXP as_rvec(SEXP rv);
  SEXP assign(SEXP rv, SEXP rindex, SEXP rvalues);
  SEXP assign_range(SEXP rv, SEXP rindex, SEXP rvalues);
  SEXP lclone(SEXP rv);
  SEXP new_lvec(SEXP rsize, SEXP rtype, SEXP rstrlen);
  SEXP get_type(SEXP rv);
  SEXP get(SEXP rv, SEXP rindex);
  SEXP get_range(SEXP rv, SEXP rindex);
  SEXP order(SEXP rv);
  SEXP get_size(SEXP rv);
  SEXP set_size(SEXP rv, SEXP rsize);
  SEXP sort(SEXP rv);
  SEXP get_strlen(SEXP rv);
  SEXP set_tempdir(SEXP rd);
}

