

#' @useDynLib ldat2
#' @export
lvec <- function(size, type = c("numeric", "integer", "logical", "character"), strlen = NULL) {
  type <- match.arg(type)
  if (type == "character" && !(is.numeric(strlen) && length(strlen) > 0)) 
    stop("strlen is required when creating a character lvec.")
  lvec <- .Call("new_lvec", size, type, strlen)
  structure(lvec, class="lvec")
}


#' @export
is.lvec <-function(x) {
  inherits(x, "lvec")
}


#' @useDynLib ldat2
#' @export
as.lvec <- function(x) {
  if (is.lvec(x)) {
    x
  } else {
    x <- .Call("as_lvec", x)
    structure(x, class="lvec")
  }
}

#' @useDynLib ldat2
#' @export
as.rvec <- function(x) {
  if (!is.lvec(x)) {
    x
  } else {
    .Call("as_rvec", x)
  }
}
