#ifndef vec_to_sexp_h
#define vec_to_sexp_h

inline ldat::vec* sexp_to_vec(SEXP rvec) {
  static ldat::vec*(*fun)(SEXP) = NULL;
  if (fun == NULL)
    fun = (ldat::vec*(*)(SEXP)) R_GetCCallable("lvec", "sexp_to_vec");
  return fun(rvec);
}

inline SEXP vec_to_sexp(ldat::vec* vec) {
  static SEXP(*fun)(ldat::vec*) = NULL;
  if (fun == NULL)
    fun = (SEXP(*)(ldat::vec*)) R_GetCCallable("lvec", "vec_to_sexp");
  return fun(vec);
}

#endif
