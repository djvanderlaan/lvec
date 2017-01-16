#ifndef tempfile_h
#define tempfile_h

#include <R_ext/Rdynload.h>
#include <R.h>
#include <Rinternals.h>
#include <string>

inline std::string tempfile() {
  static std::string(*fun)() = NULL;
  if (fun == NULL)
    fun = (std::string(*)()) R_GetCCallable("lvec", "tempfile");
  return fun();
}

#endif

