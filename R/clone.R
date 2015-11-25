

#' @useDynLib ldat2
#' @export
clone <- function(x) {
  r <- .Call("lclone", x)
  structure(r, class="lvec")
}
