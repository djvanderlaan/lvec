
.PHONY: readme install test check build load_all document

all: readme build

readme: README.md

README.md: README.Rmd
	R  --vanilla --slave -e "library(knitr); knit('README.Rmd')"
	rm tmp.*.RDS tmp.RDS

document:
	R --vanilla --slave -e "devtools::document()"

install: document
	R --vanilla --slave -e "devtools::install()"

load_all: document
	R --vanilla --slave -e "devtools::load_all()"

test: load_all
	R --vanilla --slave -e "devtools::test()"

check: document
	R --vanilla --slave -e "devtools::check()"

build: document
	R --vanilla --slave -e "devtools::build()"

clean:
	rm -f src/*.o src/*.so
