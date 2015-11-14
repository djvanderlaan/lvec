

#' @useDynLib ldat2
#' @export
lget <- function(x, index = NULL, range = NULL) {
  if (!is.lvec(x)) stop("x should be of type lvec.")

  if (!is.null(index)) {
    if (!is.null(range))
      warning("Both range and index specified. Ignoring range.")
    index <- as.lvec(index)
    res <- .Call("get", x, index)
    structure(res, class="lvec")
  } else if (!is.null(range)) {
    range <- as.numeric(range)
    res <- .Call("get_range", x, range)
    structure(res, class="lvec")
  } else {
    stop("Neither index nor range are specified.")
  }
}

