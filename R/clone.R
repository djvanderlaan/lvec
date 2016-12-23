

#' @useDynLib lvec
#' @export
clone <- function(x) {
  r <- .Call("lclone", x)
  attributes(r) <- attributes(x)
  structure(r, class="lvec")
}
