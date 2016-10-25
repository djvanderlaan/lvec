#include "filenamefactory.h"
#include <boost/filesystem.hpp>

std::string FilenameFactory::tempdir_ = "";

void FilenameFactory::tempdir(const std::string& tempdir) {
  tempdir_ = tempdir;
}

std::string FilenameFactory::tempdir() {
  return tempdir_;
}

std::string FilenameFactory::tempfile() {
  for (unsigned int i = 0; i < 20; ++i) {
    boost::filesystem::path path = tempdir();
    boost::filesystem::path temp = boost::filesystem::unique_path();
    path /= temp;
    if (!exists(path)) return path.string();
  }
  throw 1;
}

