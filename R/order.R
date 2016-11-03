
#' @rdname order
#' @export
order <- function(x, ...) {
  UseMethod("order")
}

#' @rdname order
#' @export
order.default <- function(x, ...) {
  base::order(x, ...)
}

#' Order a lvec
#'
#' @param x lvec to sort
#' @param ... unused.
#'
#' @return
#' Returns the order of \code{x}. Unlike the default \code{\link{order}} 
#' function in R, the sort used is not stable (e.g. in case there are multiple
#' records with the same value in \code{x}, there relative order after sorting
#' is not defined). 
#'
#' @examples
#' x <- as_lvec(rnorm(10))
#' order(x)
#'
#' @rdname order
#' @useDynLib lvec
#' @export
order.lvec <- function(x, ...) {
  if (!is_lvec(x)) stop("x should be of type lvec.")
  o <- .Call("order", x)
  structure(o, class = "lvec")
}

