#ifndef filenamefactory_h
#define filenamefactory_h

#include <string>
#include <boost/filesystem.hpp>

class FilenameFactory {
  public:
    static void tempdir(const std::string& tempdir) {
      tempdir_ = tempdir;
    }

    static std::string tempdir() {
      return tempdir_;
    }

    static std::string tempfile() {
      for (unsigned int i = 0; i < 20; ++i) {
        boost::filesystem::path path = tempdir();
        boost::filesystem::path temp = boost::filesystem::unique_path();
        path /= temp;
        if (!exists(path)) return path.string();
      }
      throw 1;
    }

  private:
    static std::string tempdir_;
};

#endif
