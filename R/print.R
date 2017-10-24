
#' Print an lvec
#'
#' @param x \code{\link{lvec}} to print.
#' @param ... unused
#'
#' @return
#' Returns \code{x} invisibly.
#'
#' @export
print.lvec <- function(x, ...) {
  l <- length(x)
  t <- lvec_type(x)
  cat(t, " lvec of length ", format(l, scientific = FALSE, big.mark=","), 
    ":\n", sep="")
  if (l < 20) {
    print(as_rvec(x))
  } else {
    nrec <- 20
    # Get the first n and last n records and format
    index <- c(seq(1,nrec), seq.int(l-nrec+1, l))
    sample <- as_rvec(lget(x, index = index))
    sample <- format(sample)
    # format indices
    index <- format(paste0(" [", index, "] "), justify = "right")
    width_index <- max(nchar(index))
    # Width in which to display
    width  <- getOption('width')
    if (is.null(width)) width <- 80
    width_vector <- width - width_index
    # Determine the number of records per line
    ncol <- floor(width_vector / (max(nchar(sample))+1))
    # Determine the number of rows
    nrow   <- ceiling(nrec / ncol)
    # Format the records from the head of the vector
    sample1 <- sample[1:nrec]
    sample1 <- sapply( 
        split(sample1, rep(1:nrow, each = ncol, length.out = nrec)),
        paste, collapse = " "
      )
    index1 <- index[1:nrec]
    index1 <- index1[seq(1, length(index1), by=ncol)]
    lines1 <- paste0(index1, sample1)
    # Format the records from the tail of the vector
    sample2 <- sample[nrec + 1:nrec]
    sample2 <- sapply( 
        split(sample2, rep(1:nrow, each = ncol, length.out = nrec)),
        paste, collapse = " "
      )
    index2 <- index[nrec + 1:nrec]
    index2 <- index2[seq(1, length(index2), by=ncol)]
    lines2 <- paste0(index2, sample2)
    # Print
    cat(paste0(lines1, collapse="\n"), "\n")
    cat(format("...", justify="right", width = width_index - 2), "\n")
    cat(paste0(lines2, collapse="\n"), "\n")
  }
  invisible(x)
}
