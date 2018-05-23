

Update to previous submission, fixing warnings in examples with UBSAN. 

The main change is that the internals of the package have been rewritten to use
Rcpp. This means that packages linking to lvec will have to be rewritten. At the
moment this is only the package 'ldat' which is also maintained by me. I will
submit 'ldat' at the same time with a version restricion on lvec in its
Description file (lvec >= 0.2.0). 


## Test environments
* local ubuntu 17.10 install, R 3.4.4
* local ubuntu 17.10 install, R 3.4.4 with valgrind
* windows, linux, relaese and devel using `rhub::check_for_cran()`
* valgrind and sanitizers using rhub
* R-devel on windows using the win-builder. 

## R CMD check results

On some platforms (e.g. Ubuntu; not Windows) the package gives a note that the
libraries are larger than 6MB. This is probably caused by the fact that the
package links to both BH and Rcpp. The package itself contains only a limited
amount of C++ code. I don't think there is much I can do about this, so I hope
this is not a problem. 

## Reverse dependencies

'ldat' links to lvec. The current version on CRAN gives error. As I am also the
author of 'ldat', I will submit a new version of 'ldat' together with 'lvec'. 

