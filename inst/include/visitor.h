#ifndef visitor_h
#define visitor_h

#include "cppr.h"
#include "boolean.h"

namespace ldat {

  template<typename T> class lvec;

  class lvec_visitor {
    public:
      virtual void visit(lvec<double>& vec) = 0;
      virtual void visit(lvec<int>& vec) = 0;
      virtual void visit(lvec<boolean>& vec) = 0;
      virtual void visit(lvec<std::string>& vec) = 0;
  };
}

#endif
