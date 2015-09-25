#ifndef r_string_h
#define r_string_h

#include <string>

namespace R {

  class string : public std::string {
    public:
      string() : std::string(), na_(true) { };
      string(const char* str) : std::string(str), na_(str == 0)  { };

      bool na() const { return na_;};
      void na(bool na) { na_ = na;};

    private:
      bool na_;
  };

};

#endif
