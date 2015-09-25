
#include "r_vec.h"
#include <iostream>

extern "C" {

  SEXP foo(SEXP p) {
    R_vec<R_double> v{p};
    R_vec<R_double> res(1);
    double sum = 0;
    for (int i = 0; i < v.length(); ++i) {
      double val = v[i];
      if (is_nan(val)) {
        std::cout << "na\n";
        sum = na<double>();
        break;
      } 
      sum += val;
    }
    std::cout << na<double>() << "\n";
    std::cout << sum << "\n";
    res.set(0, sum);
    return res.sexp();
  }

  SEXP bar(SEXP p) {
    R_vec<R_string> v{p};
    for (int i = 0; i < v.length(); ++i) {
      R::string str = v[i];
      if (is_na(str)) {
        std::cout << "<NA>" << "\n";
      } else {
        std::cout << str << "\n";
      }
    }
    return R_NilValue;
  }

}

