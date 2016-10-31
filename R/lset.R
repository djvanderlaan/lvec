

#' @useDynLib lvec
#' @export
lset <- function(x, index, values) {
  index <- as.lvec(index)
  values <- as.lvec(values)
  if (!is.lvec(x)) stop("x should be of type lvec.")
  .Call("assign", x, index, values)
  x
}
