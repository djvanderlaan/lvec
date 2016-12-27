
#' Set values in an lvec
#' 
#' @param x \code{\link{lvec}} to set values in
#' @param index a numeric or logical vector with indices at which the values
#'   should be set. 
#' @param values a vector with the new values. When shorter than the length of
#'   the indices the values are recycled.
#'
#' @details
#' Should behave in the same way as assigning and indexing to a regular
#' R-vector. 
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
#' 
#' @useDynLib lvec
#' @export
lset <- function(x, index, values) {
  index <- as_lvec(index)
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
  .Call("assign", x, index, values)
  x
}
