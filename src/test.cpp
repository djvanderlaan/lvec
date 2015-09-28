
#include "cppr.h"
#include <iostream>

extern "C" {

  SEXP foo(SEXP p) {
    cppr::rvec<cppr::integer> v{p};
    cppr::rvec<cppr::numeric> res(1);
    double sum = 0;
    for (int i = 0; i < v.length(); ++i) {
      double val = cppr::cast_value<double>(v[i]);
      if (cppr::is_nan(val)) {
        sum = cppr::na<double>();
        break;
      } 
      sum += val;
    }
    //std::cout << na<double>() << "\n";
    //std::cout << sum << "\n";
    res.set(0, sum);
    return res.sexp();
  }

  SEXP bar(SEXP p) {
    cppr::rvec<cppr::character> v{p};
    for (int i = 0; i < v.length(); ++i) {
      cppr::rstring str = v[i];
      if (is_na(str)) {
        std::cout << "<NA>" << "\n";
      } else {
        std::cout << str << "\n";
      }
    }
    return R_NilValue;
  }
  
  SEXP logical(SEXP p) {
    cppr::rvec<cppr::logical> v{p};
    cppr::rvec<cppr::integer> res(1);
    int sum = 0;
    for (int i = 0; i < v.length(); ++i) {
      int val = v[i];
      std::cout << val << "\n";
      if (cppr::is_nan(val)) {
        sum = cppr::na<int>();
        break;
      } 
      sum += val;
    }
    res.set(0, sum);
//    return res.sexp();
  }

}

