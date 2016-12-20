
#' Get and set the maximum string length of a character lvec
#'
#' @param x a lvec of type character.
#' @param value the new value of the maximum string length.
#'
#' @examples
#' a <- as.lvec('123')
#' strlen(a)  # = 3
#' # Strings are truncated to strlen
#' lset(a, 1, '123456')
#' print(a)  # '123'
#' strln(a) <- 5
#' lset(a, 1, '123456')
#' print(a)  # '12345'
#'
#' @rdname strlen
#' @useDynLib lvec
#' @export
strlen <- function(x) {
  .Call("get_strlen", x)
}

#' @rdname strlen
#' @export
`strlen<-` <- function(x, value) {
  y <- lvec(length(x), type = "character", strlen = value)
  lset(y, TRUE, x)
}

