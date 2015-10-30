#include "ldat.h"
#include "lvec.h"

class indexing_visitor : public ldat::lvec_visitor {
  public: 
    indexing_visitor(ldat::vec& index) : index_(index), result_(0) {
    }

    template<typename T>
    void visit_template(ldat::lvec<T>& vec) {
      auto result = new ldat::lvec<T>(index_.size(), vec);
      for (ldat::vec::vecsize i = 0; i < index_.size(); ++i) {
        int index = index_.get_of_type(i, int());
        T value = vec.get(index - 1);
        result->set(i, value);
      }
      result_ = result;
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

    ldat::vec* result() { 
      return result_;
    }

  private:
    ldat::vec& index_;
    ldat::vec* result_;
};

// TODO: range checking
// TODO: missing values in indexes
extern "C" {
  SEXP get(SEXP rv, SEXP rindex) {
    CPPRTRY
    ldat::vec* index = sexp_to_vec(rindex);
    indexing_visitor visitor{*index};
    ldat::vec* v = sexp_to_vec(rv);
    v->visit(&visitor);
    return vec_to_sexp(visitor.result());
    CPPRCATCH
  }
}

