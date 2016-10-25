#include "memmap.h"
#include "filenamefactory.h"

#include <boost/filesystem.hpp>
#include <fstream>

MemMap::MemMap(const MemMap& mmap) : size_(mmap.size_),
    filename_(FilenameFactory::tempfile()) {
  boost::filesystem::copy_file(mmap.filename_, filename_);
  file_.open(filename_);
}

MemMap::MemMap(std::size_t size, const std::string& filename) : size_(size),
    filename_(filename) {
  if (filename_ == "") filename_ = FilenameFactory::tempfile();
  // TODO: what is file creation or resize fails
  std::ofstream tmp(filename_, std::ios::out | std::ios::binary);
  tmp.close();
  boost::filesystem::resize_file(filename_, size_);
  file_.open(filename_);
}

MemMap::~MemMap() {
  if (file_.is_open()) file_.close();
  // TODO: what if file doesn't exist
  boost::filesystem::remove(filename_);
}

std::size_t MemMap::size() const {
  return size_;
}

void MemMap::size(std::size_t size) {
  if (size == size_) return;
  file_.close();
  boost::filesystem::resize_file(filename_, size);
  // TODO: if resize fails it throw exception; MemMap is then in invalid
  // state: current file should be reopened
  size_ = size;
  file_.open(filename_);
}

const void* MemMap::data() const {
  return file_.data();
}

void* MemMap::data() {
  return file_.data();
}
