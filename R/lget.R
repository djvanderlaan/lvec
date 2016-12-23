

#' @useDynLib lvec
#' @export
lget <- function(x, index = NULL, range = NULL) {
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

