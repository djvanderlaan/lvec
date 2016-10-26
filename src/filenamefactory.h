#ifndef filenamefactory_h
#define filenamefactory_h

#include <string>

class FilenameFactory {
  public:
    static void tempdir(const std::string& tempdir);
    static std::string tempdir();

    static std::string tempfile();

  private:
    static std::string tempdir_;
};

#endif
