
#' @useDynLib lvec
.onUnload <- function (libpath) {
  library.dynam.unload("lvec", libpath)
}
