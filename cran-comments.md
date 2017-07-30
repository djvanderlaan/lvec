
This is a small update on the previous release: 

- The inst/include directory of the previous release included old versions of 
  the header files. This has been corrected in this release.
- The package could not be installed on osx oldrelease. This should have been
  fixed with a minor change in c++ code (current version installs correctly
  on maxos oldrel on rhub, while previous version didn't)


## Test environments
* local ubuntu 17.04 install, R 3.4.1
* windows, linux, relaese and devel using `rhub::check_for_cran()`
* macos using rhub
* valgrind and sanitizers using rhub

## R CMD check results

0 errors | 0 warnings | 0 notes


## Reverse dependencies

There are no reverse dependencies.

