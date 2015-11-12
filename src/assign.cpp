#include "cppr.h"
#include "ldat.h"
#include "lvec.h"

#include <iostream>

class assign_visitor : public ldat::lvec_visitor {
  public: 
    assign_visitor(ldat::vec& index, ldat::vec& values) : index_(index), values_(values) {
    }

    template<typename T>
    void visit_template_numeric(ldat::lvec<T>& vec) {
      if (index_.size() > 0 && values_.size() == 0)
        throw std::runtime_error("Replacement has length zero.");
      ldat::vec::vecsize j = 0;
      for (ldat::vec::vecsize i = 0; i < index_.size(); ++i, ++j) {
        int index = index_.get_of_type(i, int());
        if (cppr::is_na(index)) 
          throw std::runtime_error("NAs are not allowed in subscripted assignments.");
        if (index < 1 || index > vec.size()) 
          throw std::runtime_error("Index out of range.");
        if (j >= values_.size()) j = 0;
        T value = values_.get_of_type(j, cppr::base_type(T()));
        vec.set(index - 1, value);
      }
    }

    template<typename T>
    void visit_template_logical(ldat::lvec<T>& vec) {
      // check for empty values with non empty index; faster to do this here instead 
      // of in the the mainloop; this check is only necessary for empty values
      if (values_.size() == 0) {
        for (ldat::vec::vecsize i = 0; i < index_.size(); ++i) {
          int index = index_.get_of_type(i, int());
          if (index != 0 || cppr::is_na(index)) 
            throw std::runtime_error("Replacement has length zero.");
        }
      }
      // index
      ldat::vec::vecsize i_index  = 0;
      ldat::vec::vecsize i_values = 0;
      for (ldat::vec::vecsize i = 0; i < vec.size(); ++i, ++i_index) {
        if (i_index >= index_.size()) i_index = 0;
        int index = index_.get_of_type(i_index, int());
        if (cppr::is_na(index)) 
          throw std::runtime_error("NAs are not allowed in subscripted assignments.");
        if (index != 0) {
          if (i_values >= values_.size()) i_values = 0;
          T value = values_.get_of_type(i_values++, cppr::base_type(T()));
	  vec.set(i, value);
        }
      }
    }

    template<typename T>
    void visit_template(ldat::lvec<T>& vec) {
      if (is_logical(index_)) {
        return visit_template_logical(vec);
      } else {
        return visit_template_numeric(vec);
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
    ldat::vec& index_;
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

// TODO: check for overflow when casting e.g. from double to int
extern "C" {
  SEXP assign(SEXP rv, SEXP rindex, SEXP rvalues) {
    CPPRTRY
    ldat::vec* index = sexp_to_vec(rindex);
    ldat::vec* values = sexp_to_vec(rvalues);
    assign_visitor visitor{*index, *values};
    ldat::vec* v = sexp_to_vec(rv);
    v->visit(&visitor);
    return R_NilValue;
    CPPRCATCH
  }
}

