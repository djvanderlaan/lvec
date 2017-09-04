

#' Read and write lvec object to file
#' 
#' @param x \code{\link{lvec}} object to save
#' @param filename name of the file(s) to save the lvec to. See details. 
#' @param overwrite overwrite existing files or abort when files would be 
#'   overwritten. 
#' @param compress a logical specifying if the data should be compressed. 
#'   (see \code{\link{saveRDS}}).
#'   
#' @details 
#' The \code{\link{lvec}} is written in chunks to a number of RDS files using
#' \code{\link{saveRDS}}. When filename contains the extension 'RDS' 
#' (capitalisation may differ), this extension is stripped from the filename. 
#' After that the lvec is written in blocks (or chunks) to files having 
#' names \code{<filename>.00001.RDS}, \code{<filename>.00002.RDS} etc. Some 
#' additional data (data type, the number of blocks, the size of the lvec, etc)
#' is written to the file \code{<filename>.RDS}.
#' 
#' The size of the chunks can be controlled by the option `chunk_size` (see
#' \code{\link{chunk}}). 
#' 
#' @return 
#' \code{lsave} does not return anything. \code{lload} returns an 
#' \code{\link{lvec}}.
#' 
#' @rdname lsave
#' @export
lsave <- function(x, filename, overwrite = TRUE, compress = FALSE) {
  path     <- dirname(filename)
  filename <- basename(filename)
  # if filename ends in .RDS remove extension
  filename <- gsub("\\.[Rr][Dd][Ss]$", "", filename)
  # check if file exists and remove existing files
  existing_files <- list.files(path = path, pattern = "test\\.[0-9.]*RDS")
  if (!overwrite && length(existing_files) > 0) 
    stop("Refusing to overwrite existing files.")
  file.remove(file.path(path, existing_files))
  # create chunks
  chunks <- chunk(x)
  # create meta
  meta <- list(
    type = lvec_type(x),
    length = length(x),
    strlen = if (lvec_type(x) == "character") strlen(x) else NULL,
    nchunks = length(chunks),
    chunks = chunks,
    filename = sprintf("%s.%05d.RDS", filename, seq_along(chunks))
  )
  # write each chunk
  for (i in seq_along(chunks)) {
    d <- as_rvec(lget(x, range = chunks[[i]]))
    f <- file.path(path, meta$filename[i])
    saveRDS(d, f, compress = compress)
  }
  # write meta
  f <- file.path(path, sprintf("%s.RDS", filename))
  saveRDS(meta, f)
}

#' @export
#' @rdname lsave
lload <- function(filename) {
  path     <- dirname(filename)
  filename <- basename(filename)
  # if filename does exist try adding RDS to the filename
  if (!file.exists(file.path(path, filename))) {
    files <- list.files(path = path, pattern = paste0("^", filename, "\\.[Rr][Dd][Ss]$"))
    if (length(files) == 0) stop("'", filename, "' does not exist.")
    if (length(files) > 1) 
      warning("Multiple candidates for '", filename, "' using first.")
    filename <- files[1]
  }
  # read meta
  meta     <- readRDS(file.path(path, filename))
  # initialise result
  res <- lvec(meta$length, meta$type, strlen = meta$strlen)
  # read chunks
  for (i in seq_along(meta$chunks)) {
    d <- readRDS(file.path(path, meta$filename[i]))
    index <- seq(meta$chunks[[i]][1], meta$chunks[[i]][2], by = 1.0)
    lset(res, index, d)
  }
  res
}

