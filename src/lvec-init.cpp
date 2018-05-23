#include "r_export.h"
#include "tempfile_impl.h"

#include <R_ext/Rdynload.h>

#define CALLDEF(name, n)  {#name, (DL_FUNC) &name, n}

extern "C" {

  static const R_CallMethodDef r_calldef[] = {
     CALLDEF(as_lvec, 1),
     CALLDEF(as_rvec, 1),
     CALLDEF(assign, 3),
     CALLDEF(assign_range, 3),
     CALLDEF(lclone, 1),
     CALLDEF(new_lvec, 3),
     CALLDEF(get_type, 1),
     CALLDEF(get, 2),
     CALLDEF(get_range, 2),
     CALLDEF(order, 1),
     CALLDEF(get_size, 1),
     CALLDEF(set_size, 2),
     CALLDEF(sort, 1),
     CALLDEF(get_strlen, 1),
     CALLDEF(set_tempdir, 1),
     {NULL, NULL, 0}
  };

  void R_init_lvec(DllInfo *info) {
    R_RegisterCCallable("lvec", "tempfile_impl",  (DL_FUNC) &tempfile_impl);
    R_registerRoutines(info, NULL, r_calldef, NULL, NULL);
    R_useDynamicSymbols(info, static_cast<Rboolean>(FALSE));
  }
}

