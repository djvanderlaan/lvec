

#' @useDynLib ldat2
#' @export
length.lvec <- function(x) {
  .Call("size", x)
}
