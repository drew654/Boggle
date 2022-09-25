CXX=g++
IDIR=$(PWD)/include
ODIR=$(PWD)/obj
SDIR=$(PWD)/src
DEPS=$(IDIR)/boggle.h
CXXFLAGS=-std=c++17 -O3 -Wall -Wextra -g -I$(IDIR)

.PHONY: all
all: $(ODIR)/main

$(ODIR)/main: $(ODIR)/main.o $(ODIR)/boggle.o
	$(CXX) $(CXXFLAGS) -o $@ $^
$(ODIR)/main.o: $(SDIR)/main.cpp $(SDIR)/boggle.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(ODIR)/boggle.o: $(SDIR)/boggle.cpp $(IDIR)/boggle.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(ODIR)/*