
#' @rdname clone
#' @export
clone <- function(x, ...) {
  UseMethod('clone')
}

#' Clone an lvec object
#'
#' @param x \code{\link{lvec}} object to clone
#' @param ... ignored; used to pass additional arguments to other methods
#'
#' @details
#' \code{\link{lvec}} objects are basically pointers to pieces of memory. 
#' When copying an object only the pointer is copied and when modifying the copied
#' object also the original object is modified. The advantage of this is speed: 
#' these is less copying of the complete vector. In order to obtain a true copy
#' of an \code{lvec} code can be used.
#'
#' @examples
#' a <- as_lvec(1:3)
#' # Copy
#' b <- a
#' # When modifying the copy also the original is modified
#' lset(b, 1, 10)
#' print(a)
#' print(b)
#' # Use clone to make a true copy
#' b <- clone(a)
#' lset(b, 1, 100)
#' print(a)
#' print(b)
#'
#' @useDynLib lvec
#' @rdname clone
#' @export
clone.lvec <- function(x, ...) {
  r <- .Call("lclone", x)
  attributes(r) <- attributes(x)
  structure(r, class="lvec")
}
