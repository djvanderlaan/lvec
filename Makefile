
.PHONY: readme

readme: README.md


README.md: README.Rmd
	R  --vanilla --slave -e "library(knitr); knit('README.Rmd')"
	rm tmp.*.RDS tmp.RDS

