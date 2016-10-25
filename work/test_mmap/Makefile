
CXXFLAGS = -I ./boost/include -std=c++11
LIBS = -L ./boost/lib -lboost_iostreams -lboost_filesystem -lboost_system --static


test: test.o memmap.o filenamefactory.o
	g++ -o test test.o memmap.o filenamefactory.o $(CXXFLAGS) $(LIBS)

test.o: filenamefactory.h memmap.h test.cpp
	g++ -o test.o -c $(CXXFLAGS) test.cpp

memmap.o: memmap.h memmap.cpp filenamefactory.h
	g++ -o memmap.o -c $(CXXFLAGS) memmap.cpp


filenamefactory.o: filenamefactory.h filenamefactory.cpp
	g++ -o filenamefactory.o -c $(CXXFLAGS) filenamefactory.cpp

