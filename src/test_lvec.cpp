
#include "cppr.h"
#include "lvec.h"
#include <iostream>


class assign_visitor : public ldat::lvec_visitor {
  public: 
    assign_visitor(ldat::vec& index, ldat::vec& values) : index_(index), values_(values) {
    }

    template<typename T>
    void visit_template(ldat::lvec<T>& vec) {
      for (ldat::vec::vecsize i = 0; i < index_.size(); ++i) {
        int index = index_.get_of_type(i, int());
        T value = values_.get_of_type(i, T());
        vec.set(index, value);
      }
    }

    void visit(ldat::lvec<double>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<int>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<std::string>& vec) {
      return visit_template(vec);
    }

  private:
    ldat::vec& index_;
    ldat::vec& values_;
};

extern "C" {
  SEXP test_lvec2(SEXP rp) {
    CPPRTRY

    const int size = 10;
    ldat::lvec<double> v1(size);
    ldat::lvec<int> v2(size);
    ldat::lvec<double> v3(size);

    for (ldat::vec::vecsize i = 0; i < v1.size(); ++i) {
      v1.set(i, i);
      v2.set(i, size-1-i);
      v3.set(i, i*i);
    }

    assign_visitor visitor(v2, v3);
    v1.visit(&visitor);

    for (ldat::vec::vecsize i = 0; i < v1.size(); ++i) {
      std::cout << v1.get(i) << "\t\t" << v2.get(i) <<
        "\t\t" << v3.get(i) << "\n";
    }

    return R_NilValue;
    CPPRCATCH
  }
}


// ========
// Convert R-vector to lvec

#include <cstring>

extern "C" {
  SEXP r_to_lvec(SEXP rv) {
    CPPRTRY
    if (cppr::is<cppr::numeric>(rv)) {
      double* v = REAL(rv);
      int l = LENGTH(rv);
      ldat::lvec<double> res(l);
      std::memcpy(res.data(), v, l * sizeof(double));
    } else if (cppr::is<cppr::integer>(rv)) {
      throw std::string("Unsupported type.");
    } else if (cppr::is<cppr::logical>(rv)) {
      throw std::string("Unsupported type.");
    } else if (cppr::is<cppr::character>(rv)) {
      throw std::string("Unsupported type.");
    } else {
      throw std::string("Unsupported type.");
    }
    return R_NilValue;
    CPPRCATCH
  }
}

