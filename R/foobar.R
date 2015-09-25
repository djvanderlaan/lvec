

#' @useDynLib ldat
foobar <- function(x) {
  .Call("foobar", package="ldat2", x)
}
