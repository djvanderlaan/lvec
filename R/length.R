
#' @useDynLib lvec
#' @export
length.lvec <- function(x) {
  .Call("get_size", x)
}

#' @useDynLib lvec
#' @export
`length<-.lvec` <- function(x, value) {
  .Call("set_size", x, value)
  return(x)
}
