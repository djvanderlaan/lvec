

# TODO: range checkingls

#' @useDynLib ldat2
#' @export
lget <- function(x, index) {
  index <- as.lvec(index)
  if (!is.lvec(x)) stop("x should be of type lvec.")
  res <- .Call("get", x, index)
  structure(res, class="lvec")
}