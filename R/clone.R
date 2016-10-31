

#' @useDynLib lvec
#' @export
clone <- function(x) {
  r <- .Call("lclone", x)
  structure(r, class="lvec")
}
