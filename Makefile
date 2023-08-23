CXX=g++
IDIR=$(PWD)/include
SDIR=$(PWD)/src
DEPS=$(IDIR)/boggle.h
CXXFLAGS=-pthread -std=c++17 -O3 -Wall -Wextra -g -I$(IDIR)

.PHONY: all
all: boggle clean

boggle: main.o boggle.o
	$(CXX) $(CXXFLAGS) -o $@ $^
main.o: $(SDIR)/main.cpp $(SDIR)/boggle.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

boggle.o: $(SDIR)/boggle.cpp $(IDIR)/boggle.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o