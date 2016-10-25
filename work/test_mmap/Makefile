

test: test.cpp memmap.h memmap.cpp filenamefactory.h filenamefactory.cpp
	g++ -o test -std=c++11 test.cpp memmap.cpp filenamefactory.cpp -I ./boost/include -L ./boost/lib -lboost_iostreams -lboost_filesystem -lboost_system --static
