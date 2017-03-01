CXXFLAGS=-std=c++11

seahash: seahash.o seastate.o
	$(CXX) -o $@ $^

seahash.o: seastate.cpp

seastate.o: seastate.cpp seastate.hh
