

#' Create memory mapped vector
#'
#' The data in these vectors are stored on disk (partially buffered for speed)
#' allowing one to work with more data than fits into available memory. 
#' 
#' @param size the size of the vector
#' @param type the type of the vector. Should be one of the following value: 
#'   \code{"numeric"}, \code{"integer"}, \code{"logical"} or \code{"character"}. 
#'   The types will create vectors corresponding to the corresponding R types. 
#' @param strlen in case of a vector of type \code{"character"} the maximum
#'   length of the strings should also be specified using \code{strlen}.
#'
#' @details
#' The minimum value of \code{strlen} is two. When a value smaller than that is
#' given it is automaticalle set to two. This is because a minimum of two bytes
#' is necessary to also store missing values correctly. 
#' 
#' @return 
#' Returns an object of type \code{lvec}. Elements of this vector are stored on
#' file (partially buffered in memory for speed) allowing one to work with more
#' data than fits into memory. 
#'
#' @examples
#' # create an integer vector of length 100
#' x <- lvec(100, type = "integer")
#' # Get the first 10 values; values are initialised to 0 by default
#' lget(x, 1:10)
#' # Set the first 10 values to 11:20
#' lset(x, 1:10, 11:20)
#' 
#' # set maximum length of the string to 1, strings longer than that get
#' # truncated. However, minimum value of strlen is 2. 
#' x <- lvec(10, type = "character", strlen = 1)
#' lset(x, 1:3, c("a", "foo", NA))
#' lget(x, 1:3)
#'
#' @useDynLib lvec
#' @export
lvec <- function(size, type = c("numeric", "integer", "logical", "character"), strlen = NULL) {
  type <- match.arg(type)
  if (type == "character" && !(is.numeric(strlen) && length(strlen) > 0)) 
    stop("strlen is required when creating a character lvec.")
  lvec <- .Call("new_lvec", size, type, strlen)
  structure(lvec, class="lvec")
}


#' Check if an object is of type lvec
#'
#' @param x the object to check
#'
#' @return 
#' Returns \code{TRUE} is the object is of type \code{\link{lvec}} and
#' \code{FALSE} otherwise.
#'
#' @export
is_lvec <-function(x) {
  inherits(x, "lvec")
}


#' Converts a primitive R-vector to lvec
#'
#' @param x the object to convert. This can be a vector of type
#'   \code{character}, \code{integer}, \code{numeric} or \code{logical}.
#
#' @return 
#' Returns an \code{\link{lvec}} of the same type as \code{x}. When \code{x} is
#' already and \code{lvec}, \code{x} is returned. For character vectors the
#' maximum length of the \code{\link{lvec}} is set to the maximum length found
#' in \code{x}.
#'
#' @examples
#' # convert a character vector to lvec
#' x <- as_lvec(letters)
#' lget(x, 1:26)
#'
#' @useDynLib lvec
#' @export
as_lvec <- function(x) {
  if (is_lvec(x)) {
    x
  } else {
    # strip of class and attributes; otherwise as_lvec will throw error
    # as it only handles basic types
    attributes <- attributes(x)
    attributes(x) <- NULL
    x <- .Call("as_lvec", x)
    x <- structure(x, class="lvec")
    rattr(x) <- attributes
    x
  }
}

#' Convert complete lvec to R vector
#'
#' @param x \code{\link{lvec}} to convert. 
#' 
#' @return
#' Returns an R vector of type integer, numeric, character or logical.
#' 
#' @useDynLib lvec
#' @export
as_rvec <- function(x) {
  if (!is_lvec(x)) {
    x
  } else {
    res <- .Call("as_rvec", x)
    attributes(res) <- rattr(x)
    res
  }
}

#' Get the type of the lvec
#'
#' @param x the \code{\link{lvec}} to get the type from.
#'
#' @return 
#' Returns an character vector of length 1 with one of the following values:
#' \code{"integer"}, \code{"numeric"}, \code{"logical"} or \code{"character"}.
#'
#' @useDynLib lvec
#' @export
lvec_type <- function(x) {
  if (!is_lvec(x)) stop("x should be of type lvec.")
  .Call("get_type", x)
}

