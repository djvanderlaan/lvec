

makevars <- function() {
  main_src <- list.files("src", "*.cpp")
  boost_src <- paste0("boost_src/", list.files("src/boost_src", "*.cpp"))
  sources <- paste0(c(main_src, boost_src), collapse=" ")

  lines <- c("CXX_STD=CXX11", 
    "PKG_CPPFLAGS = -Iboost -DBOOST_IOSTREAMS_NO_LIB",
    paste("SOURCES =", sources), "OBJECTS = $(SOURCES:.cpp=.o)")
  writeLines(lines, "src/Makevars")
}

