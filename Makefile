
.PHONY: readme install_headers

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
