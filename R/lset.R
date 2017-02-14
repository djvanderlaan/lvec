
#' Set values in an lvec
#' 
#' @param x \code{\link{lvec}} to set values in
#' @param index a numeric or logical vector with indices at which the values
#'   should be set. 
#' @param values a vector with the new values. When shorter than the length of
#'   the indices the values are recycled.
#' @param range a numeric vector of length 2 specifying a range of elements 
#'   to select. Specify either \code{index} or \code{range}. 
#'
#' @details
#' Should behave in the same way as assigning and indexing to a regular
#' R-vector. The range given by \code{range} includes both end elements. So, a 
#' range of \code{c(1,3)} selects the first three elements. 
#'
#' When \code{range} is given, and \code{values} is not given it is assumed 
#' \code{index} contains the values. Therefore, one can do
#' \code{lset(x, range = c(1,4), NA)}, to set the first four elements of 
#' \code{x} to missing. 
#'
#' @examples
#' a <- as_lvec(1:10)
#' # set second element to 20
#' lset(a, 2, 20)
#' print(a)
#' # set odd elements to 20
#' lset(a, c(TRUE, FALSE), 20)
#' print(a)
#' # values are recycled
#' lset(a, 1:4, 100:101)
#' print(a)
#' # range index; set first 3 elements to NA
#' lset(a, range = c(1,3), NA)
#' print(a)
#' 
#' @useDynLib lvec
#' @export
lset <- function(x, index = NULL, values, range = NULL) {
  # When range index is used; assume that second argument are the values
  if (!is.null(range) && missing(values) && !is.null(index)) {
    values <- index
    index  <- NULL
  }
  values <- as_lvec(values)
  if (!is_lvec(x)) stop("x should be of type lvec.")
  # Some additional checks for factors
  has_r_class <- function(x, class) {
    c <- rattr(x, "class")
    !is.null(c) && (class %in% c)
  }
  if (has_r_class(x, "factor")) {
    levels <- rattr(x, "levels")
    if (!has_r_class(values, "factor"))
      stop("values is not a factor vector")
    if (!isTRUE(all.equal(levels, rattr(values, "levels"))))
      stop("Levels of x and values to not match")
  }
  # Do the assignment
  if (!is.null(index)) {
    if (!is.null(range))
      warning("Both range and index specified. Ignoring range.")
    index <- as_lvec(index)
    .Call("assign", x, index, values)
  } else if (!is.null(range)) {
    .Call("assign_range", x, range, values)
  } else {
    stop("Neither index nor range are specified.")
  }
  x
}

