
#' Generate a number of index ranges from a vector 
#' 
#' The ranges have a maximum length.
#' 
#' @param x an object for which the index ranges should be calculated. Should 
#'   support the \code{\link{length}} method. For example, an \code{\link{lvec}}
#'   or a regular R vector. 
#' @param chunk_size a numeric vector of length 1 giving the maximum length of
#'   the chunks. 
#'   
#'   
#' @export
chunk <- function(x, chunk_size = 1E6) {
  nchunks <- ceiling(length(x) / chunk_size)
  pos <- seq(1, length(x)+1, length.out = nchunks+1)
  start <- pos[seq_len(length(pos)-1)]
  end   <- pos[seq_len(length(pos)-1) + 1] - 1
  res <- split(cbind(start, end), seq_len(nchunks))
  names(res) <- NULL
  res
}
