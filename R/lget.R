

#' @rdname lget
#' @export
lget <- function(x, ...) {
  UseMethod('lget')
}



#' Read elements from an lvec
#'
#' @param x the \code{\link{lvec}} to read from
#' @param index a logical or numeric vector to index \code{x} with
#' @param range a numeric vector of length 2 specifying a range of elements 
#'   to select. Specify either \code{index} or \code{range}. 
#' @param ... used to pass on additional arguments to other methods.
#'
#' @details
#' Indexing using \code{index} should follow the same rules as indexing a regular
#' R-vector using a logical or numeric index. The range given by \code{range} 
#' includes both end elements. So, a range of \code{c(1,3)} selects the first 
#' three elements. 
#'
#' @return
#' Returns an \code{\link{lvec}} with the selected elements. In order to convert
#' the selection to an R-vector \code{\link{as_rvec}} can e used.
#'
#' @examples
#' a <- as_lvec(letters[1:4])
#' # Select first two elements
#' lget(a, 1:2)
#' lget(a, c(TRUE, TRUE, FALSE, FALSE))
#' lget(a, range = c(1,2))
#'
#' # Logical indices are recycled: select odd elements
#' lget(a, c(TRUE, FALSE))
#' 
#' @useDynLib lvec
#' @rdname lget
#' @export
lget.lvec <- function(x, index = NULL, range = NULL, ...) {
  if (!is_lvec(x)) stop("x should be of type lvec.")

  if (!is.null(index)) {
    if (!is.null(range))
      warning("Both range and index specified. Ignoring range.")
    index <- as_lvec(index)
    res <- .Call("get", x, index)
    attributes(res) <- attributes(x)
    structure(res, class="lvec")
  } else if (!is.null(range)) {
    range <- as.numeric(range)
    res <- .Call("get_range", x, range)
    attributes(res) <- attributes(x)
    structure(res, class="lvec")
  } else {
    stop("Neither index nor range are specified.")
  }
}

#' @rdname lget
#' @export
lget.default <- function(x, index = NULL, range = NULL, ...) {
  if (!is.null(range)) {
    if (!is.null(index))
      warning("Both range and index specified. Ignoring index.")
    index <- seq.int(range[1], range[2], by = 1)
  }
  x[index]
}

#' @rdname lget
#' @export
lget.data.frame <- function(x, index = NULL, range = NULL, ...) {
  if (!is.null(range)) {
    if (!is.null(index))
      warning("Both range and index specified. Ignoring index.")
    index <- seq.int(range[1], range[2], by = 1)
  }
  x[index, , drop = FALSE]
}
