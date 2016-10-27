
context("Construction and initialisation of lvec")

test_that("vectors are correctly initialised", {
  a <- lvec(4, "integer")
  expect_that(as.rvec(a), equals(c(0, 0, 0, 0)))

  a <- lvec(4, "numeric")
  expect_that(as.rvec(a), equals(c(0.0, 0.0, 0.0, 0.0)))

  a <- lvec(4, "logical")
  expect_that(as.rvec(a), equals(c(FALSE, FALSE, FALSE, FALSE)))

  a <- lvec(4, "character", strlen = 4)
  expect_that(as.rvec(a), equals(c("", "", "", "")))
})


