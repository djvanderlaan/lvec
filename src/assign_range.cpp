#include "cppr.h"
#include "ldat.h"
#include "lvec.h"
#include "r_export.h"

class assign_range_visitor : public ldat::lvec_visitor {
  public:
    assign_range_visitor(ldat::vec::vecsize lower, ldat::vec::vecsize upper, ldat::vec& values) : 
        lower_(lower), upper_(upper), values_(values) {
    }

    template<typename T>
    void visit_template(ldat::lvec<T>& vec) {
      if (upper_ >= vec.size()) throw std::runtime_error("Index out of range.");
      if (upper_ < lower_) throw std::runtime_error("Range has negative length.");
      if (values_.size() == 0)
        throw std::runtime_error("Replacement has length zero.");
      ldat::vec::vecsize j = 0;
      for (ldat::vec::vecsize i = lower_; i <= upper_; ++i, ++j) {
        if (j >= values_.size()) j = 0;
        T value = values_.get_of_type(j, cppr::base_type(T()));
        vec.set(i, value);
      }
    }

    void visit(ldat::lvec<double>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<int>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<cppr::boolean>& vec) {
      return visit_template(vec);
    }

    void visit(ldat::lvec<std::string>& vec) {
      return visit_template(vec);
    }

  private:
    ldat::vec::vecsize lower_;
    ldat::vec::vecsize upper_;
    ldat::vec& values_;
};


// The function below doesn't return anything as the original object (pointed
// to by rv) is modified. One possibility is to return rv. However, when this
// result is not assigned to a r-value, then the garbage collector will try to
// clean up rv, e.g.
// a <- as.lvec(1:10)
// .Call("assign", a, 1:4, 41:44)
// will result in cleanup of the lvec pointed to by a; using a after that will
// result in terrible stuff.

extern "C" {
  SEXP assign_range(SEXP rv, SEXP rindex, SEXP rvalues) {
    CPPRTRY
    cppr::rvec<cppr::numeric> index{rindex};
    ldat::vec* values = sexp_to_vec(rvalues);
    // check index
    if (index.length() != 2)
      throw std::runtime_error("Expecting vector of length 2 for range index.");
    if (cppr::is_na(index[0]))
      throw std::runtime_error("Missing value for lower bound of range.");
    if (cppr::is_na(index[1]))
      throw std::runtime_error("Missing value for upper bound of range.");
    // create and call visitor
    assign_range_visitor visitor{static_cast<ldat::vec::vecsize>(index[0]-1),
      static_cast<ldat::vec::vecsize>(index[1]-1), *values};
    ldat::vec* v = sexp_to_vec(rv);
    v->visit(&visitor);
    return R_NilValue;
    CPPRCATCH
  }
}

