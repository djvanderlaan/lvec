#ifndef string_h
#define string_h

namespace cppr {
  inline bool is_na(const std::string& x) {
    return x.size() > 1 && x[0] == 0 && x[1] == 123;
  }

  inline bool is_nan(const std::string& x) {
    return is_na(x);
  }

  template<> inline std::string na<std::string>() { 
    std::string res(2, 'X');
    res[0] = 0;
    res[1] = 123;
    return res;
  }
}

#endif
