

#' @useDynLib lvec
#' @export
lvec <- function(size, type = c("numeric", "integer", "logical", "character"), strlen = NULL) {
  type <- match.arg(type)
  if (type == "character" && !(is.numeric(strlen) && length(strlen) > 0)) 
    stop("strlen is required when creating a character lvec.")
  lvec <- .Call("new_lvec", size, type, strlen)
  structure(lvec, class="lvec")
}


#' @export
is_lvec <-function(x) {
  inherits(x, "lvec")
}


#' @useDynLib lvec
#' @export
as_lvec <- function(x) {
  if (is_lvec(x)) {
    x
  } else {
    x <- .Call("as_lvec", x)
    structure(x, class="lvec")
  }
}

#' @useDynLib lvec
#' @export
as_rvec <- function(x) {
  if (!is_lvec(x)) {
    x
  } else {
    .Call("as_rvec", x)
  }
}

#' @useDynLib lvec
#' @export
lvec_type <- function(x) {
  if (!is_lvec(x)) stop("x should be of type lvec.")
  .Call("get_type", x)
}
