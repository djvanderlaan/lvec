
#' Get and set the length of an lvec
#'
#' @param x the \code{\link{lvec}}
#' @param value the new length of the \code{link{lvec}}
#'
#' @return
#' The length of the \code{\link{lvec}}.
#'
#' @rdname order
#' @useDynLib lvec
#' @export
length.lvec <- function(x) {
  .Call("get_size", x)
}

#' @rdname order
#' @useDynLib lvec
#' @export
`length<-.lvec` <- function(x, value) {
  .Call("set_size", x, value)
  return(x)
}

