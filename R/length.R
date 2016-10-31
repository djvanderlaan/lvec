

#' @useDynLib lvec
#' @export
length.lvec <- function(x) {
  .Call("size", x)
}
