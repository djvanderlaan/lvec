#ifndef visitor_h
#define visitor_h

#include "vec.h"
#include "doublevec.h"

template<typename T>
class LVec;

class DoubleVec;

class Visitor {
  public:
    virtual void visit(DoubleVec& vec) = 0;

    virtual void visit(LVec<cppr::numeric>& vec) = 0;
};

#endif
