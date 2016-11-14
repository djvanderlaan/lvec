#ifndef vec_h
#define vec_h

namespace ldat {

  class vec {
    public:
      typedef std::size_t vecsize;

      vec() {}
      virtual ~vec() {}

      virtual vec* clone() const = 0;

      virtual vecsize size() const = 0;
      virtual void size(vecsize size) = 0;  

      virtual double get_of_type(vecsize i, double type) const = 0;
      virtual int get_of_type(vecsize i, int type) const = 0;
      virtual std::string get_of_type(vecsize i, const std::string& type) const = 0;

      virtual void visit(lvec_visitor* visitor) = 0;
  };

}

#endif
