

#' @useDynLib lvec
#' @export
lset <- function(x, index, values) {
  index <- as_lvec(index)
  values <- as_lvec(values)
  if (!is_lvec(x)) stop("x should be of type lvec.")
  .Call("assign", x, index, values)
  x
}
