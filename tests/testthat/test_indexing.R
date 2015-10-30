
context("Indexing")

test_that("numeric vectors are correctly indexed", {
  a_r <- c(1.2, 5E10, NA, NaN, 4.1)
  a   <- as.lvec(a_r)

  #expect_that(a[1:4], equalsa(a_r[1:4]))

})


