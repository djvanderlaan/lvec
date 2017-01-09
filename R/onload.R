
#' @useDynLib lvec
.onLoad <- function(libname, pkgname) {
  path <- tempfile();
  dir.create(path)
  .Call("set_tempdir", PACKAGE="lvec", path)
}

