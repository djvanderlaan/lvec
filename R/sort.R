
#' Sort a lvec
#'
#' @param x lvec to sort
#' @param clone clone x before sorting
#' @param ... unused.
#'
#' @return
#' Sorts \code{x} and returns a sorted copy of \code{x}. When \code{clone} is 
#' \code{FALSE} the input vector is modified.
#'
#' @examples
#' x <- as.lvec(rnorm(10))
#' sort(x)
#'
#' # Effect of clone
#' a <- as.lvec(rnorm(10))
#' b <- sort(a, clone = FALSE)
#' print(a)
#'
#' @useDynLib lvec
#' @export
sort.lvec <- function(x, clone = TRUE, ...) {
  if (!is.lvec(x)) stop("x should be of type lvec.")
  if (clone) x <- clone(x)
  .Call("sort", x)
  x
}

