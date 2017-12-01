
.PHONY: readme install_headers install test check build load_all

all: readme install_headers

readme: README.md

README.md: README.Rmd
	R  --vanilla --slave -e "library(knitr); knit('README.Rmd')"
	rm tmp.*.RDS tmp.RDS



HEADERS = boolean.h cppr.h iterator.h lvec_interface.h lvec.h memmap.h \
	tempfile.h val_ref.h vec_to_sexp.h vec.h visitor.h

install_headers: $(addprefix inst/include/, $(HEADERS))

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

build: install_headers
	R --vanilla --slave -e "devtools::build()"

