#ifndef vec_h
#define vec_h

class Visitor;

class Vec {
  public:
    Vec() {};
    ~Vec() {};

    typedef unsigned int vecsize;

    virtual vecsize size() const = 0;

    virtual Vec* get_range(vecsize from, vecsize to) const = 0;

    virtual void visit(Visitor* visitor) = 0;

  private:
};



#endif
