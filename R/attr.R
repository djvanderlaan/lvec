
#' Set and get attributes of the original R-vector stored in an lvec
#'
#' @param x and object of type \code{\link{lvec}}
#' @param which a character vector of length one giving the name of the 
#'   attribute.
#' @param value the new value of the attribute
#'
#' @details
#' The attributes of the \code{\link{lvec}} can be set and obtained using the 
#' standard functions \code{\link{attr}} and \code{\link{attributes}}. However 
#' when an lvec is converted to an R-vector using, for example, 
#' \code{\link{as_rvec}}, the attributes of the resulting R-vector are set using 
#' the result of \code{rattr}. This can be used to store vectors such as 
#' factors and dates (POSIXct) in \code{lvec} objects, as these are basically 
#' integer and numeric vectors with a number of additional attributes. 
#'
#' @examples
#' dates <- as_lvec(as.Date("2016-12-05", "2016-12-24"))
#' # When printing and reading the result is converted back to a date object
#' print(dates)
#' as_rvec(dates)
#'
#' # make a factor of an integer lvec 
#' a <- as_lvec(1:3)
#' rattr(a, "class") <- "factor"
#' rattr(a, "levels") <- c("a", "b", "c")
#' print(a)
#'
#' @rdname rattr
#' @export
rattr <- function(x, which) {
  a <- attr(x, "r_attributes")
  if (is.null(a)) a <- list()
  if (missing(which)) a else a[[which]]
}

#' @rdname rattr
#' @export
`rattr<-` <- function(x, which, value) {
  a <- rattr(x)
  if (missing(which)) a <- value else a[[which]] <- value
  attr(x, "r_attributes") <- a
  x
}

