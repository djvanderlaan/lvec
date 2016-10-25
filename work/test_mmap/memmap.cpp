#include "memmap.h"
#include "filenamefactory.h"

#include <boost/filesystem.hpp>
#include <exception>
#include <iostream>

MemMap::MemMap(const MemMap& mmap) : size_(mmap.size_),
    filename_(FilenameFactory::tempfile()) {
  boost::filesystem::copy_file(mmap.filename_, filename_);
  file_.open(filename_);
}

MemMap::MemMap(std::size_t size, const std::string& filename) : size_(size),
    filename_(filename) {
  if (filename_ == "") filename_ = FilenameFactory::tempfile();
  boost::iostreams::mapped_file_params params;
  params.path = filename_;
  params.new_file_size = size_;
  params.flags = boost::iostreams::mapped_file::mapmode::readwrite;
  file_.open(params);
}

MemMap::~MemMap() {
  if (file_.is_open()) file_.close();
  // when file doesn't exist remove doesn't give an error and that's ok
  boost::filesystem::remove(filename_);
}

std::size_t MemMap::size() const {
  return size_;
}

void MemMap::size(std::size_t size) {
  if (size == size_) return;
  file_.close();
  boost::system::error_code ec;
  boost::filesystem::resize_file(filename_, size, ec);
  if (!ec) size_ = size;
  file_.open(filename_);
  if (ec) throw std::runtime_error("Failed to resize memory map.");
}

const char* MemMap::data() const {
  return file_.data();
}

char* MemMap::data() {
  return file_.data();
}

