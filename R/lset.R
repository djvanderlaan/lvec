

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
