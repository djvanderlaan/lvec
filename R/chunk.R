
#' @rdname chunk
#' @export
chunk <- function(x, ...) {
  UseMethod("chunk")
}


#' Generate a number of index ranges from a vector 
#' 
#' The ranges have a maximum length.
#' 
#' @param x an object for which the index ranges should be calculated. Should 
#'   support the \code{\link{length}} method. For example, an \code{\link{lvec}}
#'   or a regular R vector. 
#' @param chunk_size a numeric vector of length 1 giving the maximum length of
#'   the chunks. 
#' @param ... ignored; used to pass additional arguments to other methods.  
#' 
#' @details 
#' The default chunk size can be changes by setting the option 'chunk_size', 
#' (`options(chunk_size = <new default chunk size>)`).
#' 
#' Implementations of chunk for data frames and regular vectors are provided to
#' make it easier to write code that works on both lvec objects and regular R
#' objects. 
#'   
#' @rdname chunk
#' @export
chunk.lvec <- function(x, chunk_size = 1E6, ...) {
  nchunks <- ceiling(length(x) / chunk_size)
  pos <- seq(1, length(x)+1, length.out = nchunks+1)
  start <- pos[seq_len(length(pos)-1)]
  end   <- pos[seq_len(length(pos)-1) + 1] - 1
  res <- split(cbind(start, end), seq_len(nchunks))
  names(res) <- NULL
  res
}

#' @rdname chunk
#' @export
chunk.default <- function(x, chunk_size = NULL, ...) {
  if (is.null(chunk_size)) chunk_size <- max(1, length(x))
  nchunks <- ceiling(length(x) / chunk_size)
  pos <- seq(1, length(x)+1, length.out = nchunks+1)
  start <- pos[seq_len(length(pos)-1)]
  end   <- pos[seq_len(length(pos)-1) + 1] - 1
  res <- split(cbind(start, end), seq_len(nchunks))
  names(res) <- NULL
  res
}

#' @rdname chunk
#' @export
chunk.data.frame <- function(x, chunk_size = NULL, ...) {
  if (is.null(chunk_size)) chunk_size <- length(x)
  nchunks <- ceiling(nrow(x) / chunk_size)
  pos <- seq(1, nrow(x)+1, length.out = nchunks+1)
  start <- pos[seq_len(length(pos)-1)]
  end   <- pos[seq_len(length(pos)-1) + 1] - 1
  split(cbind(start, end), seq_len(nchunks))
}
