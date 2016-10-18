context("Sorting")

test_that("numeric vectors are correctly sorted", {
  # I did not use both NA and NaN as order of those seems to be undefined

  a_r <- c(-1.2, 5E10, NA, NA, 4.1)
  a   <- as.lvec(a_r)
  expect_that(as.rvec(sort(a)), equals(sort(a_r, na.last=TRUE)))

  a_r <- c(-1.2, 5E10, NaN, NaN, 4.1)
  a   <- as.lvec(a_r)
  expect_that(as.rvec(sort(a)), equals(sort(a_r, na.last=TRUE)))

  # Test if clone argument works
  b   <- sort(a)
  b_r <- sort(a_r, na.last=TRUE)
  lset(b, 2, 42)
  b_r[2] <- 42
  expect_that(as.rvec(b), equals(b_r))
  a   <- as.lvec(a_r)
  sort(a, clone = FALSE)
  expect_that(as.rvec(a), equals(sort(a_r, na.last=TRUE)))

  # Empty vector
  a_r <- numeric(0)
  a <- as.lvec(a_r)
  expect_that(as.rvec(sort(a)), equals(sort(a_r, na.last=TRUE)))
})

test_that("integer vectors are correctly sorted", {
  a_r <- as.integer(c(-1, 2, 1E9, NA, 1234))
  a   <- as.lvec(a_r)
  expect_that(as.rvec(sort(a)), equals(sort(a_r, na.last=TRUE)))

  # Test if clone argument works
  b   <- sort(a)
  b_r <- sort(a_r, na.last=TRUE)
  lset(b, 2, 42L)
  b_r[2] <- 42L
  expect_that(as.rvec(b), equals(b_r))
  a   <- as.lvec(a_r)
  sort(a, clone = FALSE)
  expect_that(as.rvec(a), equals(sort(a_r, na.last=TRUE)))

  # Empty vector
  a_r <- integer(0)
  a <- as.lvec(a_r)
  expect_that(as.rvec(sort(a)), equals(sort(a_r, na.last=TRUE)))
})

test_that("logical vectors are correctly sorted", {
  a_r <- as.logical(c(TRUE, FALSE, TRUE, NA, NA))
  a   <- as.lvec(a_r)
  expect_that(as.rvec(sort(a)), equals(sort(a_r, na.last=TRUE)))

  # Test if clone argument works
  b   <- sort(a)
  b_r <- sort(a_r, na.last=TRUE)
  lset(b, 2, TRUE)
  b_r[2] <- TRUE
  expect_that(as.rvec(b), equals(b_r))
  a   <- as.lvec(a_r)
  sort(a, clone = FALSE)
  expect_that(as.rvec(a), equals(sort(a_r, na.last=TRUE)))

  # Empty vector
  a_r <- logical(0)
  a <- as.lvec(a_r)
  expect_that(as.rvec(sort(a)), equals(sort(a_r, na.last=TRUE)))
})

test_that("character vectors are correctly sorted", {
  a_r <- c("jan", "pier", NA, "corneel")
  a   <- as.lvec(a_r)
  expect_that(as.rvec(sort(a)), equals(sort(a_r, na.last=TRUE)))

  # Test if clone argument works
  b   <- sort(a)
  b_r <- sort(a_r, na.last=TRUE)
  lset(b, 2, "johan")
  b_r[2] <- "johan"
  expect_that(as.rvec(b), equals(b_r))
  a   <- as.lvec(a_r)
  sort(a, clone = FALSE)
  expect_that(as.rvec(a), equals(sort(a_r, na.last=TRUE)))

  # Empty vector
  a_r <- character(0)
  a <- as.lvec(a_r)
  expect_that(as.rvec(sort(a)), equals(sort(a_r, na.last=TRUE)))
})
