
.PHONY: readme install test check build load_all

all: readme build

readme: README.md

README.md: README.Rmd
	R  --vanilla --slave -e "library(knitr); knit('README.Rmd')"
	rm tmp.*.RDS tmp.RDS

inst/include/%.h: src/%.h
	cp $< $@

document:
	R --vanilla --slave -e "devtools::document()"

install:
	R --vanilla --slave -e "devtools::install()"

load_all: 
	R --vanilla --slave -e "devtools::load_all()"

test: load_all
	R --vanilla --slave -e "devtools::test()"

check:
	R --vanilla --slave -e "devtools::check()"

build: 
	R --vanilla --slave -e "devtools::build()"

clean:
	rm -f src/*.o src/*.so
